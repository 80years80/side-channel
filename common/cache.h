#ifndef __CACHE_H__
#define __CACHE_H__

#pragma once

#include <stdint.h>

#define BLOCK_SIZE  (64)
#define FLUSH_BLOCK_SIZE (64)
#define L1_SIZE     (32*1024)
#define L2_SIZE     (512*1024)
#define L3_SIZE     (8192*1024)


#define L1_BLOCKS     (L1_SIZE/BLOCK_SIZE)
#define L2_BLOCKS     (L2_SIZE/BLOCK_SIZE)
#define L3_BLOCKS     (L3_SIZE/BLOCK_SIZE)

#define L3_ALIGN      (8192*1024)

typedef union __attribute__((packed, aligned(64))) {
  uint8_t   u8val;
  uint16_t  u16val;
  uint32_t  u32val;
  uint64_t  u64val;
  char*     cptrval;
  int*      iptrval;
  long*     lptrval;
  void*     vptrval;
  char block[BLOCK_SIZE];
} cache_block;

typedef struct __attribute__((packed, aligned(L1_SIZE))) {
  cache_block blocks[L1_BLOCKS];
} l1_entire_blocks;

typedef struct __attribute__((packed, aligned(L2_SIZE))) {
  cache_block blocks[L2_BLOCKS];
} l2_entire_blocks;

typedef struct __attribute__((packed, aligned(L3_ALIGN))) {
  cache_block blocks[L3_BLOCKS];
} l3_entire_blocks;

#define barrier() __asm__ __volatile__("": : :"memory")

static inline void __attribute__((__always_inline__))
clflush(void *ptr)
{
    barrier();

    __asm __volatile("clflush (%0);"
                     :
                     : "r"(ptr)
                     :);
    barrier();
}

static inline void __attribute__((__always_inline__))
mfence()
{
    barrier();

    __asm __volatile("mfence");
    barrier();
}

static inline void __attribute__((__always_inline__))
lfence()
{
    barrier();

    __asm __volatile("lfence");
    barrier();
}



#endif
