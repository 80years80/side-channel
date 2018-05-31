#ifndef __DRK_TEST_H__
#define __DRK_TEST_H__

#pragma once
#include <sys/mman.h>
#include <stdint.h>

#include "rdtsc.h"
#include "rtm.h"

/*
 * DO NOT MODIFY THIS CODE
 */
void *map_non_writable_address() {
  void *ptr = mmap(0, 0x1000, PROT_READ, \
                  MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE, -1, 0);
  return ptr;
}

void *map_executable_address() {
  void *ptr = mmap(0, 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC, \
                  MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE, -1, 0);
  char *c_ptr = (char*)ptr;

  // fill int $0x3, which is a debugger trap that breaks TSX
  c_ptr[0] = 0xcc;

  return ptr;
}

void execute_address(void* ptr) {
  ((void(*)())ptr)();
}

#define PROBE_TIMES 1000
uint64_t probe_an_address_via_write(void *ptr) {

  uint64_t min_time = (uint64_t)(-1);
  char *c_ptr = (char*)ptr;

  // run for PROBE_TIMES times for better accuracy
  for(int i=0; i<PROBE_TIMES; ++i) {
    uint64_t start, end;
    start = rdtsc();
    if(_xbegin() == _XBEGIN_STARTED) {
      // write access
      c_ptr[0] = ~c_ptr[0];
    }
    else {
      // abort handler
      end = rdtsc();

      // get minimum timing
      uint64_t diff = end - start;
      if(diff < min_time) {
        min_time = diff;
      }
    }
  }
  return min_time;
}

uint64_t probe_an_address_via_exec(void *ptr) {

  uint64_t min_time = (uint64_t)(-1);

  // run for PROBE_TIMES times for better accuracy
  for(int i=0; i<PROBE_TIMES; ++i) {
    uint64_t start, end;
    start = rdtsc();
    if(_xbegin() == _XBEGIN_STARTED) {
      // execute access
      execute_address(ptr);
    }
    else {
      // abort handler
      end = rdtsc();

      // get minimum timing
      uint64_t diff = end - start;
      if(diff < min_time) {
        min_time = diff;
      }
    }
  }
  return min_time;
}



uint64_t test_unmapped_write() {
  void *unmapped_address = (void*)0;
  return probe_an_address_via_write(unmapped_address);
}

uint64_t test_non_writable_write() {
  void *non_writable_address = map_non_writable_address();
  if(non_writable_address == NULL || non_writable_address == (void*)-1) {
    // Error
    return 0;
  }

  return probe_an_address_via_write(non_writable_address);
}

uint64_t test_unmapped_exec() {
  void *unmapped_address = (void*)0;
  return probe_an_address_via_exec(unmapped_address);
}

uint64_t test_non_exec_exec() {
  void *non_writable_address = map_non_writable_address();
  if(non_writable_address == NULL || non_writable_address == (void*)-1) {
    // Error
    return 0;
  }

  return probe_an_address_via_exec(non_writable_address);
}

uint64_t test_exec_exec() {
  void *executable_address = map_executable_address();
  if(executable_address == NULL || executable_address == (void*)-1) {
    // Error
    return 0;
  }

  return probe_an_address_via_exec(executable_address);
}





#endif
