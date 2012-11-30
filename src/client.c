#include "common.h"

int main(int argc, char ** argv) {
  int sc_fifo_fd, cs_fifo_fd;
  FILE *fp;
  uint8_t rsa_n, rsa_d, rsa_e, rsa_server_n, rsa_server_e;
  ssize_t msg_size;
  char * buff;

  /* Mandatory arguments */
  if (!argv[1] || !argv[2] || !argv[3] || !argv[4]) {
    fprintf(stderr,"client [server->client fifo] [client->server fifo] [password file] [username]\n");
    exit(1);
  }

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

  /* GET public rsa key of S, (s_puk,n), from "client_folder/server_rsa_public_key.txt" */
  if ((fp = fopen("client_folder/server_rsa_public_key.txt", "r")) == NULL) {
    fprintf(stderr, "Error while getting server RSA public key...\n");
    goto next;
  }
  fscanf(fp, "%c", &rsa_server_n);
  fscanf(fp, "%c", &rsa_server_e);
  fclose(fp);

  /* CREATE a random number r */
  /* ... */
  /* ENCRYPT r using (s_puk,n) -> c = r^s_puk mod n */
  /* ... */
  /* WRITE c to S */
  if ((write_msg(cs_fifo_fd, buff, msg_size)) < 0) {
    fprintf(stderr, "Error while sending C to the server...\n");
    goto next;
  }

  /* READ r' from C */
  /* ... */
  /* CHECK if r = r' */
  /* ... */

  /* Client authentication */
  /* SEND client_name to S */
  /* ... */
  /* GET private rsa key of C, (s_prk,n) from "client_folder/client_rsa_private_key.txt" */
  /* ... */
  /* READ c from S */
  /* ... */
  /* DECRYPT c using (c_prk,n) -> r' = c^c_prk mod n */
  /* ... */
  /* WRITE r' to S */
  /* ... */
  /* GET private rsa key of C, c_prk from "client_folder/client_rsa_private_key.txt" */
  /* ... */

  /* Negotiation of the cipher suite */
  /* ... */

  /* Negotiation of the private key */
  /* ... */

  /* Encrypt communication */
  /* ... */

  /* Disconnection */
  /* ... */


  next:
    /* Close connection with the server */
    fprintf(stderr,"Closing connection...\n");

    /* Expect BYE */
    if (read_string(sc_fifo_fd,CLOSE_CONNECTION_STRING) < 0) {
        fprintf(stderr,"Communication error\n");
        goto next;
    }

    close_channel(cs_fifo_fd);
    close_channel(sc_fifo_fd);
    exit(0);
}
