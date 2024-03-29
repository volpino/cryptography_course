#include "common.h"

int open_fifo(const char * pathname) {
  /* Recreate the FIFO in pathname */
  unlink(pathname);
  if (mkfifo(pathname,0600) < 0) {
    perror("mkfifo()");
    exit(1);
  }

  /* Open the channel */
  return (open_channel(pathname));
}

int wait_connection(int channel_fd) {
  ssize_t msg_size;
  u_int8_t * buff;

  /* Wait a connection from the client */
  if (((msg_size = read_msg(channel_fd,&buff)) < 0) ||
      (msg_size != strlen(CONNECTION_STRING) ||
      strncasecmp((const char *)CONNECTION_STRING,(const char *)buff,msg_size))) {
    /* Protocol error */
    return (-1);
  }

  return (0);
}

int main(int argc, char ** argv) {
  int sc_fifo_fd, cs_fifo_fd;
  int done;
  int i, loops, c_hex_len, c_len;
  char client_nm[NM_LENGTH], client_nm_tmp[NM_LENGTH], cipher_tmp, client_cipher_suite;
  uint8_t rsa_tmp[RSA_LENGTH], rsa_tmp2[RSA_LENGTH];
  ssize_t msg_size;
  uint8_t *buff;
  unsigned int tmp;
  uint8_t c[MSG_SIZE_MAX];
  uint8_t r[R_SIZE_3];
  uint8_t seed[SEED_SIZE];
  uint8_t iv[3];
  uint8_t k[K_SIZE_3];
  uint8_t g[HASH_LENGTH], g1[HASH_LENGTH];
  uint8_t sym_id, hash_id, public_id;
  int k_len;
  FILE *fp;

  BIGNUM *bn_n, *bn_d, *bn_client_e, *bn_client_n, *bn_r, *bn_r1;

  bn_n = BN_new();
  bn_d = BN_new();
  bn_client_e = BN_new();
  bn_client_n = BN_new();
  bn_r = BN_new();
  bn_r1 = BN_new();

  /* Mandatory arguments */
  if (!argv[1] || !argv[2]) {
    fprintf(stderr,"server [server->client fifo] [client->server fifo]\n");
    exit(1);
  }

  /* Main loop */
  loops = 0;
  do {
    if (loops % MAX_LOOPS == 0) {
      fp = fopen("/dev/random", "r");
      for (i=0; i<SEED_SIZE; i++) {
        fscanf(fp, "%c", &(seed[i]));
      }
      fclose(fp);
      iv[0] = 0;
      iv[1] = 0;
      iv[2] = 0;
    }

    loops++;

    /* Open the server->client fifo */
    fprintf(stderr,"Opening server->client fifo...\n");
    sc_fifo_fd = open_fifo(argv[1]);

    /* Open the client->server fifo */
    fprintf(stderr,"Opening client->server fifo...\n");
    cs_fifo_fd = open_fifo(argv[2]);

    /* Wait for a connection */
    fprintf(stderr,"Waiting connection...\n");
    if (wait_connection(cs_fifo_fd) < 0) {
      fprintf(stderr,"Communication error...\n");
      goto next;
    }

    /* Write OK */
    write_OK(sc_fifo_fd);

    /* Server authentication */

    /* GET private rsa key of S, (s_prk,n) from "server_folder/server64_rsa_private_key.txt" */
    if ((fp = fopen("server_folder/server_rsa64_private_key.txt", "r")) == NULL) {
      fprintf(stderr, "Error while getting server RSA private key...\n");
      goto next;
    }
    fscanf(fp, "%129[^,],%129[^,]", rsa_tmp, rsa_tmp2);
    BN_hex2bn(&bn_n, (const char *) rsa_tmp);
    BN_hex2bn(&bn_d, (const char *) rsa_tmp2);
    fclose(fp);

    /* READ c from C */
    if ((msg_size = read_msg(cs_fifo_fd,&buff)) < 0) {
      fprintf(stderr, "Error while getting C from the client...\n");
      goto next;
    }

    /* Assert c not in (0, 1) ? No, useless */

    /* DECRYPT c using (s_prk,n) -> r' = c^s_prk mod n */
    buff[msg_size] = '\0';
    BN_hex2bn(&bn_r, (const char *) buff);
    rsa_decrypt(bn_r, bn_d, bn_n);
    buff = (uint8_t *) BN_bn2hex(bn_r);

    /* SEND r' to C */
    if ((write_msg(sc_fifo_fd, buff, strlen((char *) buff))) < 0) {
      fprintf(stderr, "Error while sending C to the client...\n");
      goto next;
    }
    OPENSSL_free(buff);

    /* Client authentication */
    /* READ client name nm of C */
    if ((msg_size = read_msg(cs_fifo_fd,&buff)) < 0) {
      fprintf(stderr, "Error while getting the client name...\n");
      goto next;
    }
    buff[msg_size] = '\0';
    strncpy((char *)client_nm, (const char *) buff, NM_LENGTH);

    /* GET the public rsa keys of the possible clients associated to each name,
     * (names[],c_puk[],n[]) from "client_folder/clients_rsa64_public_keys.txt"
     */
    /* EXTRACT from (names[],c_puk[],n[]) the pair (c_puk[i],n[i]) where names[i] = nm */
    if ((fp = fopen("server_folder/clients_rsa64_public_keys.txt", "r")) == NULL) {
      fprintf(stderr, "Error while getting clients RSA public keys...\n");
      goto next;
    }
    done = 0;
    while (!feof(fp)) {
      fscanf(fp, "%129s %129s %129s", client_nm_tmp, rsa_tmp, rsa_tmp2);
      if (strcmp(client_nm_tmp, client_nm) == 0) {
        done = 1;
        break;
      }
    }
    if (done == 0) {
      fprintf(stderr, "Error: unrecognized client\n");
      goto next;
    }
    fclose(fp);

    BN_hex2bn(&bn_client_n, (const char *) rsa_tmp);
    BN_hex2bn(&bn_client_e, (const char *) rsa_tmp2);

    /* CREATE a pseudo-random message r */
    do {
      bunny24_prng(seed, SEED_SIZE, iv, r, R_SIZE_3);
      iv[0] = r[R_SIZE_3 - 3];
      iv[1] = r[R_SIZE_3 - 2];
      iv[2] = r[R_SIZE_3 - 1];

      BN_bin2bn(r, R_SIZE, bn_r);
    } while (BN_is_zero(bn_r) || BN_is_one(bn_r));

    /* ENCRYPT r using c_puk[i] -> r' = r^c_puk[i] mod n[i] */
    rsa_encrypt(bn_r, bn_client_e, bn_client_n);

    /* WRITE c to C */
    buff = (uint8_t *) BN_bn2hex(bn_r);
    if ((write_msg(sc_fifo_fd, buff, strlen((char *) buff))) < 0) {
      fprintf(stderr, "Error while sending C to the client...\n");
      goto next;
    }
    OPENSSL_free(buff);

    /* READ r' from C */
    if ((msg_size = read_msg(cs_fifo_fd,&buff)) < 0) {
      fprintf(stderr, "error while getting r' from the client...\n");
      goto next;
    }

    /* CHECK that r = r' */
    BN_bin2bn(r, R_SIZE, bn_r);

    buff[msg_size] = '\0';
    BN_hex2bn(&bn_r1, (const char *) buff);
    if (BN_cmp(bn_r, bn_r1) != 0) {
      fprintf(stderr, "Error, r and r' mismatch!\n");
      goto next;
    }

    /* Negotiation of the cipher suite */
    /* READ list from C */
    if ((msg_size = read_msg(cs_fifo_fd,&buff)) < 0) {
      fprintf(stderr, "error while getting the list from the client...\n");
      goto next;
    }
    /* get c_sym_id, c_hash_id, c_public_id from buff[0] */
    client_cipher_suite = buff[0];
    cipher_suite_table(client_cipher_suite, &sym_id, &hash_id, &public_id);

    /* Search for the cipher suite in the file */
    if ((fp = fopen("server_folder/server_cipher_suite_list.txt", "r")) == NULL) {
      fprintf(stderr, "Error while getting the cipher list...\n");
      goto next;
    }
    done = 0;
    while (!feof(fp)) {
      fscanf(fp, "%c", &cipher_tmp);
      if (client_cipher_suite == cipher_tmp) {
        done = 1;
        break;
      }
    }
    if (done == 0) {
      fprintf(stderr, "Error: unrecognized client\n");
      goto next;
    }
    fclose(fp);

    /* Negotiation of the private key */

    /* CREATE a pseudo-random key */
    do {
      bunny24_prng(seed, SEED_SIZE, iv, k, K_SIZE_3);
      iv[0] = k[K_SIZE_3 - 3];
      iv[1] = k[K_SIZE_3 - 2];
      iv[2] = k[K_SIZE_3 - 1];

      if (sym_id == 1) {
        k_len = 3;
      }
      else {
        k_len = K_SIZE;
      }
      BN_bin2bn(k, k_len, bn_r);
    } while (BN_is_zero(bn_r) || BN_is_one(bn_r));

    /* If we're using RSA512 read the correct key (we have the 64bit one) */
    if (public_id == 6) {
      if ((fp = fopen("server_folder/clients_rsa512_public_keys.txt", "r")) == NULL) {
        fprintf(stderr, "Error while getting clients RSA public keys...\n");
        goto next;
      }
      done = 0;
      while (!feof(fp)) {
        fscanf(fp, "%129s %129s %129s", client_nm_tmp, rsa_tmp, rsa_tmp2);
        if (strcmp(client_nm_tmp, client_nm) == 0) {
          done = 1;
          break;
        }
      }
      if (done == 0) {
        fprintf(stderr, "Error: unrecognized client\n");
        goto next;
      }
      fclose(fp);

      BN_hex2bn(&bn_client_n, (const char *) rsa_tmp);
      BN_hex2bn(&bn_client_e, (const char *) rsa_tmp2);
    }

    /* ENCRYPT key */
    rsa_encrypt(bn_r, bn_client_e, bn_client_n);

    /* WRITE k to C */
    buff = (uint8_t *) BN_bn2hex(bn_r);
    if ((write_msg(sc_fifo_fd, buff, strlen((char *) buff))) < 0) {
      fprintf(stderr, "Error while sending C to the client...\n");
      goto next;
    }
    OPENSSL_free(buff);

    /* Encrypt communication */
    if ((msg_size = read_msg(cs_fifo_fd,&buff)) < 0) {
      fprintf(stderr, "Error while reading message from the client...\n");
      goto next;
    }

    c_hex_len = msg_size - HASH_LENGTH * 2;
    if (c_hex_len <= 0) {
      fprintf(stderr, "Error, malformed message...\n");
      goto next;
    }

    c_len = c_hex_len / 2;
    for (i=0; i<msg_size; i+=2) {
      if (i < c_hex_len) {
        sscanf((char *) (buff+i), "%02x", &tmp);
        c[i/2] = (uint8_t) tmp;
      }
      else {
        sscanf((char *) (buff+i), "%02x", &tmp);
        g[(i - c_hex_len) / 2] = (uint8_t) tmp;
      }
    }

    /* Decrypt C */
    decrypt(sym_id, c, c_len, k);

    /* COMPUTE G' = H(M) */
    sponge_hash(c, c_len, g1);

    c[c_len] = '\0';

    /* CHECK G' = G */
    done = 1;
    for (i=0; i<HASH_LENGTH; i++) {
      if (g[i] != g1[i]) {
        done = 0;
      }
    }

    /* If the check fails print error message */
    if (done == 0) {
      if ((write_msg(sc_fifo_fd, (uint8_t *) CORRUPTED_STRING, strlen(CORRUPTED_STRING))) < 0) {
        fprintf(stderr, "Error while writing to the client...\n");
        goto next;
      }
    }

    /* PUT M' on a file */
    if ((fp = fopen("server_folder/received_messages.txt", "a+")) == NULL) {
        fprintf(stderr, "Error while saving message...\n");
        fclose(fp);
        goto next;
    }
    fprintf(fp, "%s", c);
    fclose(fp);

    /* WRITE ok message to C */
    if ((write_msg(sc_fifo_fd, (uint8_t *) DECRYPTED_STRING, strlen(DECRYPTED_STRING))) < 0) {
      fprintf(stderr, "Error while writing C to the client...\n");
      goto next;
    }

  /* Disconnection */
  next:
    /* Close current connection */
    fprintf(stderr,"Closing connection...\n");
    write_BYE(sc_fifo_fd);
    close_channel(sc_fifo_fd);
    close_channel(cs_fifo_fd);
  } while(1);

  close_channel(sc_fifo_fd);
  close_channel(cs_fifo_fd);

  BN_free(bn_n);
  BN_free(bn_d);
  BN_free(bn_client_n);
  BN_free(bn_client_e);
  BN_free(bn_r);
  BN_free(bn_r1);

  exit(0);
}
