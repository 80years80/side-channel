#ifndef __DRK_H__
#define __DRK_H__

#pragma once

#include "rtm.h"
#include "rdtsc.h"

/*
 * DO NOT MODIFY THIS CODE
 */
typedef struct {
  uint64_t base;
  uint64_t bound;
} kaddr_t;

kaddr_t your_answer_kernel_addrs();
kaddr_t your_answer_kernel_executables(kaddr_t);

#endif
