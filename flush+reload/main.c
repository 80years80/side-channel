#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "cache.h"

/*
 * DO NOT MODIFY THIS CODE
 */

#define RAND_SIZE (2)
#define RAND_SIZE_IN_BITS (RAND_SIZE * 8)

// cache block scratchpad
volatile l1_entire_blocks l1_cache;

char random_bytes[RAND_SIZE*2];
char answer[RAND_SIZE*2];
char encoded_bytes[RAND_SIZE*2];

#include "victim.h"
#include "flush-reload.h"

volatile size_t __counter, __value;
void warmup() {
  for(__counter=0; __counter < 0x1000000; ++__counter) {
    __value += __counter;
  };
}

void print_in_hex(char *ptr, int len) {
  if(ptr == NULL) printf("NULL DATA\n");
  else {
    if(len == 0) {
      len = strlen(ptr);
    }
    for(size_t i=0; i<len; ++i) {
      printf("%02x", ptr[i]&0xff);
    }
    printf("\n");
  }
}

int get_random() {
  FILE *fp = fopen("/dev/urandom", "rb");
  if(fp == NULL) {
    return -1;
  }
  random_bytes[0] = '\0';
  while(strlen(random_bytes) != RAND_SIZE) {
    fread(random_bytes, RAND_SIZE, 1, fp);
    random_bytes[RAND_SIZE] = '\0';
  }
  fclose(fp);
}


void prepare() {
  memset(encoded_bytes, 0, RAND_SIZE+1);
  l1_cache.blocks[0].u8val = 1;
  l1_cache.blocks[31].u8val = 0;
}

int main() {
  if(get_random() == -1) {
    printf("Reading random failed\n");
    exit(-1);
  }
  printf("Random in hex: ");
  print_in_hex(random_bytes, RAND_SIZE);

  printf("Warming up\n");
  warmup();

  printf("Flush+Reload attack starts!\n");

  prepare();

  for(int i=0; i<RAND_SIZE_IN_BITS; ++i) {
    flush(&l1_cache);
    operate(i);
    reload(&l1_cache);
  }

  your_answer(answer);
  printf("Answer in hex: ");
  print_in_hex(answer, RAND_SIZE);
  if(memcmp(answer, random_bytes, RAND_SIZE) == 0) {
    printf("Correct!\n");
  }
  else {
    printf("Incorrect!\n");
  }
}
