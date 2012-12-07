#include "common.h"
#include <assert.h>

void print_buff(uint8_t * buff, size_t buff_size) {
  int i = 0, j;

  while (i < buff_size) {
    j = 0;
    while (((j + i) < buff_size) && (j < 16)) {
      fprintf(stderr,"%02X ",buff[i+j]);
      j++;
    }

    while (j < 16) {
      fprintf(stderr,"-- ");
      j++;
    }
    fprintf(stderr," *** ");

    j = 0;
    while (((j + i) < buff_size) && (j < 16)) {
      fprintf(stderr,"%c",isprint(buff[i+j]) ? buff[i+j] : '.');
      j++;
    }

    while (j < 16) {
      fputc('.',stderr);
      j++;
    }

    i += j;
    fputc('\n',stderr);
  }
}

int write_OK(int channel_fd) {
  return (write_msg(channel_fd,(const uint8_t *)OK_STRING,strlen(OK_STRING)));
}

int write_BYE(int channel_fd) {
  return (write_msg(channel_fd,(const uint8_t *)CLOSE_CONNECTION_STRING,strlen(CLOSE_CONNECTION_STRING)));
}

int open_channel(const char * pathname) {
  int fd;

  /* Open the FIFO in pathname */
  if ((fd = open(pathname,O_RDWR)) < 0) {
    perror("open()");
    exit(1);
  }

  return (fd);
}

void close_channel(int channel_fd) {
  /* Close the file descriptor */
  close(channel_fd);
}

ssize_t read_msg(int channel_fd, uint8_t ** msg) {
  ssize_t read_len;
  uint16_t msg_size;
  static uint8_t msg_buff[MSG_SIZE_MAX + 1];
  /* +1 in case someone wants to append '\0' */

  /* The first 16 bits represents the message size */
  if (read(channel_fd,(void *)&msg_size,sizeof(msg_size)) < 0) {
    perror("read()");
    exit(1);
  }

  fprintf(stderr,"Reading size:\n");
  print_buff((uint8_t *)(&msg_size),sizeof(msg_size));

  /* Verify that the size of the message is no more than MSG_SIZE_MAX bytes */
  if (msg_size >= MSG_SIZE_MAX) {
    fprintf(stderr,"Message too large\n");
    return (-1);
  }

  /* Read msg_size bytes from the channel */
  if ((read_len = read(channel_fd,(void *)msg_buff,msg_size)) < 0) {
    perror("read()");
    exit(1);
  }

  /* Assign the buffer to msg */
  (*msg) = msg_buff;

  fprintf(stderr,"Reading message:\n");
  print_buff((uint8_t *)(*msg),msg_size);

  return (read_len);
}

ssize_t write_msg(int channel_fd, const uint8_t * msg, uint16_t msg_size) {
  /* Send the size */
  if (write(channel_fd,(const void *)&msg_size,sizeof(msg_size)) < 0) {
    perror("write()");
    exit(1);
  }

  fprintf(stderr,"Writing size:\n");
  print_buff((uint8_t *)&msg_size,sizeof(msg_size));

  /* Send the message */
  if (write(channel_fd,(const void *)msg,msg_size) < 0) {
    perror("write()");
    exit(1);
  }

  fprintf(stderr,"Writing message:\n");
  print_buff((uint8_t *)msg,msg_size);

  return (msg_size);
}

int read_string(int channel_fd, const char * str) {
  size_t read_len;
  uint8_t * msg;

  read_len = read_msg(channel_fd,&msg);
  if ((read_len != strlen(str)) ||
      memcmp(msg,str,strlen(str)))
    return (-1);

  return (0);
}

void cipher_suite_table(char suite_id, uint8_t *symm_cipher,
                        uint8_t *hash, uint8_t* asymm_cipher) {
  switch (suite_id) {
    case 'A':
      *symm_cipher = 1;
      *hash = 4;
      *asymm_cipher = 5;
      break;
    case 'B':
      *symm_cipher = 1;
      *hash = 4;
      *asymm_cipher = 6;
      break;
    case 'C':
      *symm_cipher = 2;
      *hash = 4;
      *asymm_cipher = 5;
      break;
    case 'D':
      *symm_cipher = 2;
      *hash = 4;
      *asymm_cipher = 6;
      break;
    case 'E':
      *symm_cipher = 3;
      *hash = 4;
      *asymm_cipher = 5;
      break;
    case 'F':
      *symm_cipher = 3;
      *hash = 4;
      *asymm_cipher = 6;
      break;
    default:
      assert(0);
  }
}

void decrypt(uint8_t cipher_id, uint8_t *c, int c_len, uint8_t *k) {
  uint8_t iv[] = {0x42, 0x42, 0x42};
  uint8_t *out = (uint8_t *) malloc(c_len * sizeof(uint8_t));
  int i;

  if (cipher_id == 1) {
    bunny24_decrypt_cbc(c, c_len, k, iv);
  }
  else if (cipher_id == 2 || cipher_id == 3) {
    if (cipher_id == 2) {
      all5(k, out, c_len*8);
    }
    else {
      maj5(k, out, c_len*8);
    }
    for (i=0; i<c_len; i++) {
      c[i] ^= out[i];
    }
  }

  free(out);
}


void encrypt(uint8_t cipher_id, uint8_t *c, int c_len, uint8_t *k) {
  uint8_t iv[] = {0x42, 0x42, 0x42};
  uint8_t *out = (uint8_t *) malloc(c_len * sizeof(uint8_t));
  int i;

  if (cipher_id == 1) {
    bunny24_encrypt_cbc(c, c_len, k, iv);
  }
  else if (cipher_id == 2 || cipher_id == 3) {
    if (cipher_id == 2) {
      all5(k, out, c_len*8);
    }
    else {
      maj5(k, out, c_len*8);
    }
    for (i=0; i<c_len; i++) {
      c[i] ^= out[i];
    }
  }

  free(out);
}
