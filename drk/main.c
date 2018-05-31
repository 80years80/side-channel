#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <sys/mman.h>


#include "drk-test.h"
#include "drk.h"

/*
 * DO NOT MODIFY THIS CODE
 */

volatile size_t __counter, __value;
void warmup() {
  for(__counter=0; __counter < 0x4000000; ++__counter) {
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


int main() {
  printf("Warming up\n");
  warmup();

  uint64_t unmapped_write = test_unmapped_write();
  uint64_t non_writable_write = test_non_writable_write();
  uint64_t unmapped_exec = test_unmapped_exec();
  uint64_t non_exec_exec = test_non_exec_exec();
  uint64_t exec_exec = test_exec_exec();
  printf("Test\n");
  printf("Writing on an unmapped area (slow): ");
  printf("Timing %lu cycles\n", unmapped_write);
  printf("Writing on a non-writable area (fast): ");
  printf("Timing %lu cycles\n", non_writable_write);
  printf("\n\n");
  printf("Executing on an unmapped area (slow): ");
  printf("Timing %lu cycles\n", unmapped_exec);
  printf("Executing on a non-executable area (slow): ");
  printf("Timing %lu cycles\n", non_exec_exec);
  printf("Executing on an executable area (fast): ");
  printf("Timing %lu cycles\n", exec_exec);

  printf("\n\nYour DrK attack starts!\n");

  kaddr_t kernel_addrs = your_answer_kernel_addrs();
  kaddr_t kernel_exec_addrs = your_answer_kernel_executables(kernel_addrs);
  printf("Kernel addr start: %p ends: %p\n", \
          (void*)kernel_addrs.base, (void*)kernel_addrs.bound);
  printf("Kernel executable addr start: %p ends: %p\n", \
          (void*)kernel_exec_addrs.base, (void*)kernel_exec_addrs.bound);
}
