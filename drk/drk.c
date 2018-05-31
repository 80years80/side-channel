#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "drk.h"

// You can define any global variables here to use.

// Example
static int temp_values[512];
static int temp_size = 0;


kaddr_t your_answer_kernel_addrs() {
  kaddr_t to_be_returned = {0, 0};
  // TODO: get kernel ASLR slide, and put base and bound
  //       in to_be_returned variable.
  // Hint: kernel resides somewhere between
  //        0xffffffff80000000 and 0xffffffffc0000000.
  //       Probe these addresses (in a page granularity, i.e., 0x1000),
  //       and then identify where the kernel starts and ends.

  return to_be_returned;
}

kaddr_t your_answer_kernel_executables(kaddr_t kernel_area) {
  kaddr_t to_be_returned = {0, 0};
  // TODO: get kernel ASLR slide only for the executable areas.
  
  return to_be_returned;
}
