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

ssize_t forward_msg(int from_fd, int to_fd) {
  ssize_t read_len;
  uint8_t *buff;

  if ((read_len = read_msg(from_fd, &buff)) < 0)
    return -1;

  if (write_msg(to_fd, buff, read_len) < 0)
    return -1;

  return read_len;
}

ssize_t forward_msg_read(int from_fd, int to_fd, uint8_t **buff) {
  ssize_t read_len;

  if ((read_len = read_msg(from_fd, buff)) < 0)
    return -1;

  if (write_msg(to_fd, *buff, read_len) < 0)
    return -1;

  return read_len;
}

ssize_t forward_string(int from_fd, int to_fd, const char* str) {
  if (read_string(from_fd, str) < 0)
    return -1;

  return (write_msg(to_fd, (const uint8_t *)str, strlen(str)));;
}

int main(int argc, char ** argv) {
  int ec_fifo, ce_fifo, es_fifo, se_fifo;
  FILE* fp;
  ssize_t msg_size;
  uint8_t *buff;
  char client_nm[NM_LENGTH], client_nm_tmp[NM_LENGTH];
  uint8_t rsa_tmp[RSA_LENGTH], rsa_tmp2[RSA_LENGTH];
  BIGNUM /**bn_n, *bn_d,*/ *bn_client_e, *bn_client_n, *bn_r;
  char client_cipher_suite;
  uint8_t sym_id, hash_id, public_id;
  uint8_t k[K_SIZE] = {0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1};
  int i, done;
  int k_len, c_hex_len, c_len;
  uint8_t g[HASH_LENGTH], g1[HASH_LENGTH];
  uint8_t c[MSG_SIZE_MAX];
  unsigned int tmp;

  assert(K_SIZE == 8);

  bn_client_e = BN_new();
  bn_client_n = BN_new();
  bn_r = BN_new();


  /* Mandatory arguments */
  if (!argv[1] || !argv[2] || !argv[3] || !argv[4] ||
      !argv[1] || !argv[2] || !argv[3] || !argv[4]) {
    fprintf(stderr, "%s [server->eve fifo] [eve->server fifo] [eve->client fifo] [client->eve fifo]\n", argv[0]);
    exit(1);
  }

  se_fifo = open_channel(argv[1]);
  es_fifo = open_channel(argv[2]);
  ec_fifo = open_fifo(argv[3]);
  ce_fifo = open_fifo(argv[4]);

  /* wait real client to connect */
  fprintf(stderr,"(E) Waiting connection...\n");
  if (wait_connection(ce_fifo) < 0) {
    fprintf(stderr,"(E) Communication error...\n");
    goto next;
  }
  /* connect to server */
  write_msg(es_fifo, (const u_int8_t *)CONNECTION_STRING, strlen(CONNECTION_STRING));

  forward_string(se_fifo, ec_fifo, OK_STRING);

  /* Server authentication */
  /* client challange */
  forward_msg(ce_fifo, es_fifo);
  /* server response to challange */
  forward_msg(se_fifo, ec_fifo);

  /* Client authentication */
  /* Client name */
  msg_size = forward_msg_read(ce_fifo, es_fifo, &buff);

  /*  vvvv  copy-paste from server  vvvv  */
  buff[msg_size] = '\0';
  strncpy((char *)client_nm, (const char *) buff, NM_LENGTH);
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

  /*  ^^^^  copy-paste from server  ^^^^  */
  /* now I know client name and pub key */

  /* server challange */
  forward_msg(se_fifo, ec_fifo);
  /* client response */
  forward_msg(ce_fifo, es_fifo);


  /* Negotiation of the cipher suite */
  /* cipher suite c -> s */
  forward_msg_read(ce_fifo, es_fifo, &buff);

  /*  vvvv  copy-paste from server  vvvv  */
  client_cipher_suite = buff[0];
  cipher_suite_table(client_cipher_suite, &sym_id, &hash_id, &public_id);
  /*  ^^^^  copy-paste from server  ^^^^  */


  /* Negotiation of the private key */
  /* k already set as an arbitrary key */

  /*  vvvv  copy-paste from server  vvvv  */

  if (sym_id == 1) {
    k_len = 3;
  }
  else {
    k_len = K_SIZE;
  }
  BN_bin2bn(k, k_len, bn_r);

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

  /* WRITE encrypted k to C */
  buff = (uint8_t *) BN_bn2hex(bn_r);
  if ((write_msg(ec_fifo, buff, strlen((char *) buff))) < 0) {
    fprintf(stderr, "Error while sending C to the client...\n");
    goto next;
  }
  OPENSSL_free(buff);

  /* Encrypted communication */
  if ((msg_size = read_msg(ce_fifo,&buff)) < 0) {
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
    if ((write_msg(ec_fifo, (uint8_t *) CORRUPTED_STRING, strlen(CORRUPTED_STRING))) < 0) {
      fprintf(stderr, "Error while writing to the client...\n");
      goto next;
    }
  }

  /* PUT M' on a file */
  if ((fp = fopen("eve_folder/received_messages.txt", "a+")) == NULL) {
    fprintf(stderr, "Error while saving message...\n");
    fclose(fp);
    goto next;
  }
  fprintf(fp, "%s", c);
  fflush(stdout);
  fprintf(stdout, "MESSAGGIO SEGRETO >>>%s<<< FINE MESSAGGIO SEGRETO\n", c);
  fflush(stdout);
  fclose(fp);

  /* WRITE ok message to C */
  if ((write_msg(ec_fifo, (uint8_t *) DECRYPTED_STRING, strlen(DECRYPTED_STRING))) < 0) {
    fprintf(stderr, "Error while writing C to the client...\n");
    goto next;
  }

  /*  ^^^^  copy-paste from server  ^^^^  */



 next:

  /*close_channels ...!*/

  BN_free(bn_client_n);
  BN_free(bn_client_e);
  BN_free(bn_r);

  exit(0);
}
