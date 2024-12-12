#pragma once

/*
  arena r = alloc_arena(sizeof(int) * 3);

  int *test = get_space_arena(&r, sizeof(int));
  *test = 1999;
  char *testc = get_space_arena(&r, sizeof(char));
  *testc = 101;
  float *testf = get_space_arena(&r, sizeof(float));
  *testf = 0.5;

  printf("%d\n", *test);
  printf("%c\n", *testc);
  printf("%f\n", *testf);

  reset_arena(&r); // it is now reusable

  free_arena(&r);
*/
#ifndef NO_STDLIB

#ifndef V_EXIT
#include <stdlib.h>
#define V_EXIT(x) exit(x)
#endif // V_EXIT

#ifndef V_ALLOC
#include <stdlib.h>
#define V_MALLOC malloc
#define V_REALLOC realloc
#endif // V_ALLOC

#endif // NO_STDLIB

#ifndef V_FPRINTF
#include <stdio.h>
#define V_FPRINTF fprintf
#endif

typedef struct {
  size_t current_offset;
  void *base_pointer;
  size_t size;
} arena;

arena alloc_arena(size_t size);
void *get_data_pointer(arena *a, size_t size);
void reset_arena(arena* a);
void free_arena(arena *a);

#ifdef C_ARENA
arena alloc_arena(size_t size) {
  arena r;
  r.base_pointer = malloc(size);
  if (r.base_pointer == NULL) {
    V_FPRINTF(stderr, "buy more ram :)");
    exit(1);
  }

  r.current_offset = 0;
  r.size = size;
  return r;
}

void *get_data_pointer(arena *a, size_t size) {
  if (a->base_pointer == NULL) {
    V_FPRINTF(stderr, "base pointer is null either it was not initialized or it has been freed :)\n");
    exit(1);
  }

  if (size < a->size - a->current_offset) {
    void *out = (a->base_pointer + a->current_offset);
    a->current_offset += size;
    return out;
  }
  V_FPRINTF(stderr, "ERROR: tried to allocate more than the arena had %zu > %zu\n", size , a->size - a->current_offset);
  exit(1);
}

void reset_arena(arena* a) {
  a->current_offset = 0;
  *(int*)(a->base_pointer) = 0;
}

void free_arena(arena *a) {
  free(a->base_pointer);
  a->base_pointer = NULL;
  a->current_offset = 0;
  a->size = 0;
}
#endif // C_ARENA