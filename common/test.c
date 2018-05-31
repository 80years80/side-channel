#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "rdtsc.h"

#include "cache.h"

volatile uint64_t value;

void warmup() {
  for(size_t i=0; i<0x10000000; ++i) {
    value += 1;
  }
}
uint64_t times[100];
uint64_t idx = 0;
void do_benchmark(int loop) {
  uint64_t start, end;
  start = rdtsc();
  for(int i=0; i<loop; ++i);
  end = rdtsc();
  times[loop] = rdtsc_diff(start, end);
}

uint64_t get_threshold(l1_entire_blocks* l1_cache) {
  uint64_t cached = (uint64_t)-1, uncached = (uint64_t)-1;
  uint64_t threshold;
  register uint64_t start, end, diff;
  // cached access
  for(int i=0; i<1000; ++i) {
    start = rdtsc();
    value = l1_cache->blocks[0].u64val;
    end = rdtsc();
    diff = end - start;
    if(diff < cached) {
      cached = diff;
    }
  }

  for(register int i=0; i<3; ++i) {
    for(register int j=0; j<5; ++j) {
      clflush(&l1_cache->blocks[18+j]);
    }
    mfence();
    rdtsc();
    start = rdtsc();
    value = l1_cache->blocks[20].u64val;
    end = rdtsc();
    diff = end - start;
    if(diff < uncached) {
      uncached = diff;
    }
  }
  printf("Cached %ld Uncached %ld\n", cached, uncached);
  threshold = (uncached + cached) / 2;

  threshold = cached * 2;
  printf("Threshold %ld\n", threshold);
  return threshold;
}

void flush_all(l1_entire_blocks* l1_cache) {
  char *p = (char*)l1_cache;
  for(int j=0; j<10; ++j) {
  for(int i=0; i<L1_BLOCKS; i += FLUSH_BLOCK_SIZE) {
    clflush(&p[i]);
  }
  }
  mfence();
}

uint64_t diffs[512];

volatile uint64_t vv;
int reload_each( l1_entire_blocks *l1_cache,
                  int position, uint64_t threshold)
{
  register uint64_t start, end, diff;
  start = rdtsc();
  value = l1_cache->blocks[position].u64val;
  end = rdtsc();
  diff = end - start;
  diffs[position] = diff;
  //printf("diff %ld\n", diff);
  // tell if value is read -> cached!
  // -> return true if time is less than threshold
  if(diff < threshold) {
    return 1;
  }
  else {
    return 0;
  }
}

#define NUM_READ 10
int read_values[NUM_READ];
int victim_read(l1_entire_blocks *l1_cache) {
  for(int i=0; i<NUM_READ; ++i) {
    read_values[i] = rand()%(NUM_READ*3);
  }
  for(int i=0; i<NUM_READ; ++i) {
    value = l1_cache->blocks[read_values[i]].u64val;
  }
}

void reload_all( l1_entire_blocks *l1_cache, uint64_t threshold, uint64_t load)
{
  int blocks[L1_BLOCKS];
  int idx = 0;
  for(int i=0; i<load; ++i) {
    if(reload_each(l1_cache, i, threshold) == 1) {
      printf("Reloaded at %d\n", i);
      blocks[++idx] = i;
    }
  }
  for(int i=0; i<idx; ++i) {
    printf("Block position %d has read by victim\n", i);
  }
}

int main() {

  srand((unsigned int)time(0));
  warmup();
  /*
  for(int i=0; i<20; ++i) {
    do_benchmark(i);
  }
  for(int i=0; i<20; ++i) {
    printf("Idx %d: %ld\n", i, times[i]);
  }
  */

  printf("\n\n");
  printf("cache_block size %ld\n", sizeof(cache_block));
  printf("l1 size %ld\n", sizeof(l1_entire_blocks));
  printf("l2 size %ld\n", sizeof(l2_entire_blocks));
  printf("l3 size %ld\n", sizeof(l3_entire_blocks));

  l1_entire_blocks l1_cache;
  printf("L1_blocks address %p\n", &l1_cache);

  uint64_t threshold = get_threshold(&l1_cache);
  printf("Threshold: %d\n", threshold);

  flush_all(&l1_cache);

  victim_read(&l1_cache);

  reload_all(&l1_cache, threshold, NUM_READ*3);

  for(register int i=0; i<NUM_READ*3; ++i) {
    printf("Pos %d diff %ld\n", i, diffs[i]);
  }

  printf("Victim read: ");
  for(register int i=0; i<NUM_READ; ++i) {
    printf("%d", read_values[i]);
    if(i != NUM_READ - 1) {
      printf(", ");
    }
  }
  printf("\n");
}
