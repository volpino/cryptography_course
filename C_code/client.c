#include "common.h"

int main(int argc, char ** argv)
{
	int channel_fd;

	/* Three arguments are mandatory */
	if( !argv[1] || !argv[2] || !argv[3] ) {
		fprintf(stderr,"client [fifo channel] [password file] [username]\n");
		exit(1);
	}

	/* Create connection with the server */
	fprintf(stderr,"Create connection...\n");
	channel_fd = open_channel(argv[1]);
	write_msg(channel_fd,(const u_int8_t *)CONNECTION_STRING,strlen(CONNECTION_STRING));

	/* Read OK */
	if( read_string(channel_fd,OK_STRING) < 0 ) {
		fprintf(stderr,"Communication error\n");
		goto next;
	}

	/* Server authentication */
  // GET public rsa key of S, (s_puk,n), from "client_folder/server_rsa_public_key.txt"
  /* ... */
  // CREATE a random number r
  /* ... */
  // ENCRYPT r using (s_puk,n) -> c = r^s_puk mod n
  /* ... */
  // WRITE c to S
  /* ... */
  // READ r' from C
  /* ... */
  // CHECK if r = r'
  /* ... */

	/* Client authentication */
  // SEND client_name to S
  /* ... */
  // GET private rsa key of C, (s_prk,n) from "client_folder/client_rsa_private_key.txt"
  /* ... */
  // READ c from S
  /* ... */
  // DECRYPT c using (c_prk,n) -> r' = c^c_prk mod n
  /* ... */
  // WRITE r' to S
  /* ... */
  // GET private rsa key of C, c_prk from "client_folder/client_rsa_private_key.txt"
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
	if( read_string(channel_fd,CLOSE_CONNECTION_STRING) < 0 ) {
		fprintf(stderr,"Communication error\n");
		goto next;
	}

	close_channel(channel_fd);
	exit(0);
}
