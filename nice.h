#pragma once

#ifndef V_MEMCPY
#include <string.h>
#define V_MEMCPY memcpy
#endif

#ifndef V_FPRINTF
#include <stdio.h>
#define V_FPRINTF fprintf
#endif

#ifdef V_FPRINTF
#define P_INFO 1
#define P_WARN 2
#define P_ERR 3
#define eprintf(type, ...)                                                     \
  switch (type) {                                                              \
  default:                                                                     \
    V_FPRINTF(stdout, __VA_ARGS__);                                            \
    break;                                                                     \
  case P_INFO:                                                                 \
    V_FPRINTF(stdout, "[INFO]: ");                                             \
    V_FPRINTF(stdout, __VA_ARGS__);                                            \
    break;                                                                     \
  case P_WARN:                                                                 \
    V_FPRINTF(stdout, "[WARNING]: ");                                          \
    V_FPRINTF(stdout, __VA_ARGS__);                                            \
    break;                                                                     \
  case P_ERR:                                                                  \
    V_FPRINTF(stderr, "[ERROR]: ");                                            \
    V_FPRINTF(stderr, __VA_ARGS__);                                            \
    break;                                                                     \
  }

#define eprintfn(...)                                                          \
  eprintf(__VA_ARGS__);                                                        \
  V_FPRINTF(stdout, "\n");
#endif // V_FPRINTF

#define array_length(a) sizeof(a) / sizeof(a[0])
#define array_nsize(a, b) array_length(a) + array_length(b)
#ifdef V_MEMCPY
// concatonate 2 array of same type into a new array of same type
#define concat_array(a, b, c)                                                  \
  test(sizeof(a[0]) == sizeof(b[0]) & sizeof(a[0]) == sizeof(c[0]) &           \
       (array_length(c)) >= array_nsize(a, b));                                \
  V_MEMCPY(&c[0], &a[0], sizeof(a));                                           \
  V_MEMCPY(&c[(array_length(a))], &b[0], sizeof(b));
#endif // NO_STR
#define push_to_array(array, element)                                          \
  for (size_t zzzzz = 0; zzzzz < array_length(array) - 1; zzzzz++) {           \
    array[array_length(array) - zzzzz - 1] =                                   \
        array[array_length(array) - zzzzz - 2];                                \
  }                                                                            \
  array[0] = element;

#define aforeach_val(type, name, array, i)                                     \
  for (unsigned long i = 0; i < array_length(array); i++) {                    \
    type name = array[i];
#define aforeach_ref(type, name, array, i)                                     \
  for (unsigned long i = 0; i < array_length(array); i++) {                    \
    type *name = &array[i];
#ifndef end_foreach
#define end_foreach }
#endif // end_foreach
