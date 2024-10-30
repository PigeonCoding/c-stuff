#pragma once
#include <stdlib.h>
#include <stdio.h>

#define test(f) if (f < 0 || f == 0) {                  \
          dprintf(P_ERR, "an error has occured\n");     \
          exit(1);                                      \
        }                                               \

#define P_INFO 1
#define P_WARN 2
#define P_ERR 3
#define dprintf(type, ...)                   \
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
  

float shit_sqrt(float in, float div) {
  float s = in / div;
  while (1) {
    if ((s) * (s) < in) {
      break;
    } else {
      s = s * div;
    }
  }
  return s;
}

