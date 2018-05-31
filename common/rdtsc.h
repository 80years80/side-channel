#ifndef __RDTSC_H_CUSTOM__
#define __RDTSC_H_CUSTOM__

#pragma once

#include <stdint.h>

#define barrier() __asm__ __volatile__("": : :"memory")

static inline uint64_t __attribute__((__always_inline__))
rdtsc(void)
{
    barrier();

    register uint64_t tsc;
    __asm __volatile("rdtscp; shl $32, %%rdx; or %%rdx, %%rax"
                     : "=a" (tsc)
                     : : "%rbx", "%rcx", "%rdx");
    barrier();
    return tsc;
}

static inline uint64_t __attribute__((__always_inline__))
get_minimal_overhead(void)
{
  volatile uint64_t v = 0;
  volatile uint64_t g = 0;
  volatile uint64_t overhead = (uint64_t)(-1);
  uint64_t start, end, value;
  for(int i=0; i<0x10000000; ++i) {
    v += i;
    g += i;
    start = rdtsc();
    end = rdtsc();
    value = end - start;
    if(value < overhead && value != 0) {
      overhead = value;
    }
  }
  return (v - g + overhead);
}

static inline uint64_t __attribute__((__always_inline__))
rdtsc_diff(uint64_t start, uint64_t end)
{
  return (end - start);
}


#endif
