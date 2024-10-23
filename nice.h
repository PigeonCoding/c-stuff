#include <stdlib.h>

#define test(f) if (f < 0) {                     \
          fprintf(stderr, "an error has occured\n");\
          exit(1);                                  \
        }


