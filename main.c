#include "cstring.h"
#include <stdio.h>

int main(void) {
  string test_string = alloc_string();

  push_string(&test_string, "Hello World!");
  // push_string(&test_string, "Hello World!2 ");
  // push_string(&test_string, "Hello World!3 \n");

  
  printf("%s\n", get_string(&test_string));

  // push_string(&test_string, "Hello World!4");

  // printf("%s\n", get_string(&test_string));

  // printf("%zu and %zu\n", *test_string.length, test_string.data.length);

  free_string(&test_string);

  return 0;
}