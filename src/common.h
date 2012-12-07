#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <assert.h>

#include <openssl/bn.h>
#include "lib/bunny24.h"
#include "lib/bunny24_prng.h"
#include "lib/A5_1.h"
#include "lib/rsa.h"
#include "lib/sponge_hash.h"

#define MSG_SIZE_MAX 2048
#define CONNECTION_STRING "Hello!"
#define CLOSE_CONNECTION_STRING "Bye"
#define DECRYPTED_STRING "MESSAGE RECEIVED AND DECRYPTED!"
#define CORRUPTED_STRING "CORRUPTED MESSAGE RECEIVED!"
#define OK_STRING "OK"
#define R_SIZE 32
#define R_SIZE_3 33
#define K_SIZE 8
#define K_SIZE_3 9
#define SEED_SIZE 12  /* Must be multiple of 3 */
#define RSA_LENGTH 65
#define HASH_LENGTH 20
#define NM_LENGTH 65
#define MAX_LOOPS 1024

void print_buff(uint8_t *, size_t);
int open_channel(const char *);
void close_channel(int);
ssize_t read_msg(int, uint8_t **);
ssize_t write_msg(int, const uint8_t *, uint16_t);

int read_string(int, const char *);
int write_OK(int);
int write_BYE(int);

void cipher_suite_table(char suite_id, uint8_t *symm_cipher,
                        uint8_t *hash, uint8_t* asymm_cipher);

void decrypt(uint8_t cipher_id, uint8_t *c, int c_len, uint8_t *k);
void encrypt(uint8_t cipher_id, uint8_t *c, int c_len, uint8_t *k);

#endif
