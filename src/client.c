#include "common.h"

int main(int argc, char ** argv) {
  int sc_fifo_fd, cs_fifo_fd;
  int i;
  FILE *fp;
  char rsa_tmp[RSA_LENGTH];
  ssize_t msg_size;
  uint8_t * buff;
  char * client_name;
  uint8_t seed[SEED_SIZE];
  uint8_t bin_r[R_SIZE];
  uint8_t bin_k[K_SIZE];
  unsigned char message[ENCRYPTED_MSG_MAX + HASH_LENGTH];
  char message_hex[(ENCRYPTED_MSG_MAX + HASH_LENGTH) * 2];
  uint8_t symm_cipher, hash, asymm_cipher;
  char ciphersuite;

  BIGNUM
    *rsa_n, *rsa_d, /* my private key */
    *rsa_server_n, *rsa_server_e, /* server pub key */
    *r,   /* random number */
    *rc,  /* encrypted r */
    *k;   /* symmetric key */

  rsa_n = BN_new();
  rsa_d = BN_new();
  rsa_server_n = BN_new();
  rsa_server_e = BN_new();
  r = BN_new();
  rc = BN_new();
  rsa_n = BN_new();
  rsa_d = BN_new();
  rsa_server_n = BN_new();
  rsa_server_e = BN_new();
  k = BN_new();

  /* Mandatory arguments */
  if (!argv[1] || !argv[2] || !argv[3] || !argv[4]) {
    fprintf(stderr, "%s [server->client fifo] [client->server fifo] [password file] [username]\n", argv[0]);
    exit(1);
  }
  client_name = argv[4];

  /* Create connection with the server */
  fprintf(stderr,"Create connection...\n");
  sc_fifo_fd = open_channel(argv[1]);
  cs_fifo_fd = open_channel(argv[2]);

  write_msg(cs_fifo_fd,(const u_int8_t *)CONNECTION_STRING,strlen(CONNECTION_STRING));

  /* Read OK */
  if (read_string(sc_fifo_fd,OK_STRING) < 0) {
    fprintf(stderr,"Communication error\n");
    goto next;
  }

  /* Server authentication */
  /* GET public rsa key of S, (s_puk,n), from "client_folder/server_rsa64_public_key.txt" */
  if ((fp = fopen("client_folder/server_rsa64_public_key.txt", "r")) == NULL) {
    fprintf(stderr, "Error while getting server RSA public key...\n");
    goto next;
  }
  fscanf(fp, "%64[^,]", rsa_tmp);
  BN_hex2bn(&rsa_server_n, rsa_tmp);
  fscanf(fp, ",");
  fscanf(fp, "%64[^,]", rsa_tmp);
  BN_hex2bn(&rsa_server_e, rsa_tmp);
  fclose(fp);

  /* CREATE a random number r */
  fp = fopen("/dev/random", "r");
  for (i=0; i<SEED_SIZE; i++) {
    fscanf(fp, "%c", &(seed[i]));
  }
  fclose(fp);
  bunny24_prng(seed, SEED_SIZE, NULL, bin_r, R_SIZE);

  BN_bin2bn(bin_r, R_SIZE, r);

  /* ENCRYPT r using (s_puk,n) -> c = r^s_puk mod n */
  BN_copy(rc, r);
  rsa_encrypt(rc, rsa_server_e, rsa_server_n);

  /* WRITE c to S */
  buff = (uint8_t *) BN_bn2hex(rc);
  msg_size = strlen((char*)buff);
  if ((write_msg(cs_fifo_fd, buff, msg_size)) < 0) {
    fprintf(stderr, "Error while sending C to the server...\n");
    goto next;
  }
  OPENSSL_free(buff);

  /* READ r' from C */
  if ((msg_size = read_msg(sc_fifo_fd, &buff)) < 0) {
    fprintf(stderr, "Error while getting response to my challange from server...\n");
    goto next;
  }
  buff[msg_size] = '\0';
  BN_hex2bn(&rc, (char*)buff);   /* overwrite rc to store r' */

  /* CHECK if r = r' */
  if (BN_cmp(r, rc) != 0) {
    fprintf(stderr, "Server authentication failed, closing.\n");
    goto next;
  }

  /* Client authentication */
  /* SEND client_name to S */
  msg_size = strlen(client_name);
  if ((write_msg(cs_fifo_fd, (uint8_t*)client_name, msg_size)) < 0) {
    fprintf(stderr, "Error while sending name to the server...\n");
    goto next;
  }

  /* GET private rsa key of C, (s_prk,n) from "client_folder/client_rsa64_private_key.txt" */
  if ((fp = fopen("client_folder/client_rsa64_private_key.txt", "r")) == NULL) {
      fprintf(stderr, "Error while getting my private key...\n");
      goto next;
  }
  fscanf(fp, "%64[^,]\n", rsa_tmp);
  BN_hex2bn(&rsa_n, rsa_tmp);
  fscanf(fp, ",");
  fscanf(fp, "%64[^,]\n", rsa_tmp);
  BN_hex2bn(&rsa_d, rsa_tmp);
  fclose(fp);

  /* READ c from S */
  if ((msg_size = read_msg(sc_fifo_fd, &buff)) < 0) {
    fprintf(stderr, "Error while getting challenge from server...\n");
    goto next;
  }
  buff[msg_size] = '\0';
  BN_hex2bn(&rc, (char*)buff);

  /* DECRYPT c using (c_prk,n) -> r' = c^c_prk mod n */
  rsa_decrypt(rc, rsa_d, rsa_n);

  /* WRITE r' to S */
  buff = (uint8_t*) BN_bn2hex(rc);
  msg_size = strlen((char*)buff);
  if ((write_msg(cs_fifo_fd, buff, msg_size)) < 0) {
    fprintf(stderr, "Error while sending response to the server's chellange...\n");
    goto next;
  }
  OPENSSL_free(buff);

  /* Negotiation of the cipher suite */
  /* GET my cipher suite from file */
  if ((fp = fopen("client_folder/client_cipher_suite.txt", "r")) == NULL) {
    fprintf(stderr, "Error while reading my cipher suite...\n");
  }
  fscanf(fp, "%c", &ciphersuite);
  fclose(fp);
  cipher_suite_table(ciphersuite, &symm_cipher, &hash, &asymm_cipher);

  /* SEND my cipher suite to server */
  if ((write_msg(cs_fifo_fd, (uint8_t*)(&ciphersuite), 1)) < 0) {
    fprintf(stderr, "Error while sending siphersuite to the server...\n");
    goto next;
  }

  /* Negotiation of the private key */
  /* READ h from server */
  if ((msg_size = read_msg(sc_fifo_fd, &buff)) < 0) {
    fprintf(stderr, "Error while getting symmetric key from server...\n");
    goto next;
  }
  buff[msg_size] = '\0';
  BN_hex2bn(&k, (char*)buff);   /* overwrite rc to store r' */

  /* GET my private key from proper file (if not already loaded) */
  if (asymm_cipher == 6) { /* 512 bits - rsa */
    if ((fp = fopen("client_folder/client_rsa512_private_key.txt", "r")) == NULL) {
      fprintf(stderr, "Error while getting my private key...\n");
      goto next;
    }
    fscanf(fp, "%64s\n", rsa_tmp);
    BN_hex2bn(&rsa_n, rsa_tmp);
    fscanf(fp, "%64s\n", rsa_tmp);
    BN_hex2bn(&rsa_d, rsa_tmp);
    fclose(fp);
  }

  /* compute k from h and my private key */
  rsa_decrypt(k, rsa_d, rsa_n);
  assert(BN_num_bytes(k) <= K_SIZE);
  BN_bn2bin(k, bin_k);

  /* Encrypt communication */
  /* GET message from file */
  if ((fp = fopen("client_folder/client_message.txt", "r")) == NULL) {
    fprintf(stderr, "Error while getting my private key...\n");
    goto next;
  }
  fgets((char *) message, ENCRYPTED_MSG_MAX, fp);
  fclose(fp);

  msg_size = strlen((char *) message);

  /* hash the message */
  sponge_hash((uint8_t*)message, msg_size, (uint8_t*)(message + msg_size));

  /* encrypt the message */
  encrypt(symm_cipher, (uint8_t *)message, msg_size, bin_k);

  for (i=0; i<msg_size+HASH_LENGTH; i++) {
    sprintf(message_hex+(2*i), "%02x", message[i]);
  }

  /* send message + hash to server*/
  if ((write_msg(cs_fifo_fd, (uint8_t*)message_hex,(msg_size + HASH_LENGTH)*2)) < 0) {
    fprintf(stderr, "Error while sending encrypted message to the server...\n");
    goto next;
  }

  /* Disconnection */
  if (read_string(sc_fifo_fd, DECRYPTED_STRING) < 0) {
    fprintf(stderr,"Error reading server confirmation!\n");
  }

 next:
  /* Close connection with the server */
  fprintf(stderr,"Closing connection...\n");

  /* Expect BYE */
  if (read_string(sc_fifo_fd, CLOSE_CONNECTION_STRING) < 0) {
    fprintf(stderr,"Communication error\n");
    goto next;
  }

  close_channel(cs_fifo_fd);
  close_channel(sc_fifo_fd);

  BN_free(r);
  BN_free(rc);
  BN_free(rsa_n);
  BN_free(rsa_d);
  BN_free(rsa_server_n);
  BN_free(rsa_server_e);
  BN_free(k);

  exit(0);
}
