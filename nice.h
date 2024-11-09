#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define test(f) if ((f) < 0 || (f) == 0) {                  \
    eprintf(P_ERR, "an error has occured\n");               \
    exit(1);                                                \
  }                                                         \

#define P_INFO 1
#define P_WARN 2
#define P_ERR 3
#define eprintf(type, ...)                   \
  switch (type) {                            \
  default:                                   \
    printf(__VA_ARGS__);                     \
    break;                                   \
  case P_INFO:                               \
    printf("[INFO]: ");                      \
    printf(__VA_ARGS__);                     \
    break;                                   \
  case P_WARN:                               \
    printf("[WARNING]: ");                   \
    printf(__VA_ARGS__) ;                    \
    break;                                   \
  case P_ERR:                                \
    fprintf(stderr, "[ERROR]: ");            \
    fprintf(stderr, __VA_ARGS__);            \
    break;                                   \
  }                                          \
  

// concatonate 2 array of same type into a new array of same type
#define array_nsize(a, b) sizeof(a) / sizeof(a[0]) + sizeof(b) / sizeof(b[0])
#define concat_array(a, b, c) test(sizeof(a[0]) == sizeof(b[0]) & sizeof(a[0]) == sizeof(c[0]) & (sizeof(c) / sizeof(c[0])) >= array_nsize(a, b)); memcpy(&c[0], &a[0], sizeof(a)); memcpy(&c[(sizeof(a) / sizeof(a[0]))], &b[0], sizeof(b));


