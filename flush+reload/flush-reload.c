#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "cache.h"
#include "rdtsc.h"

// You can define any global variables here to use for sharing it
// between flush and reload, and also share with your_answer
// functions.

// Example
static int temp_values[512];
static int temp_size = 0;

void flush(l1_entire_blocks *l1_cache) {
  // TODO: flush cache lines using clflush(&l1_cache->blocks[xxx])
}

void reload(l1_entire_blocks *l1_cache) {
  // TODO: 1. get a timestamp using rdtsc
  // TODO: 2. access cache line
  //        e.g., volatile int i = l1_cache->blocks[i].u8val;
  // TODO: 3. get a timestamp using rdtsc
  // TODO: 4. get time diff
  // TODO: 5. determine the value of the secret
  uint64_t start, end, diff1, diff2;
  volatile int v;
}

void your_answer(char* answer) {
  // TODO: collect all bits and write two bytes.

  // we only use 16 bits, 2 bytes. So mark the third byte as zero
  answer[0] = answer[1] = answer[2] = '\0';

}
