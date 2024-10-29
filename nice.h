#include <stdlib.h>

#define test(f) if (f < 0) {                     \
          fprintf(stderr, "an error has occured\n");\
          exit(1);                                  \
        }

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

