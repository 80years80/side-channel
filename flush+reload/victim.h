#ifndef __VICTIM_H__
#define __VICTIM_H__

#pragma once

#include "rdtsc.h"

/*
 * DO NOT MODIFY THIS CODE
 */
void operate(int i) {

  // calculate bit position
  register int byte_idx = i/8;
  register int bit_idx = i&7;

  // get bit_value
  register int value = random_bytes[byte_idx];
  int bit_value = (value >> bit_idx) & 1;

  // get encoded value
  register int encoded_value = l1_cache.blocks[bit_value*31].u8val;

  // write value
  encoded_bytes[byte_idx] |= ((encoded_value) << bit_idx);

  rdtsc();
}

#endif
