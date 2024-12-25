// version 0.2

/*
  arena r = alloc_arena(sizeof(int) * 3);

  int *test = alloc_ptr(&r, sizeof(int));
  *test = 1999;
  char *testc = alloc_ptr(&r, sizeof(char));
  *testc = 101;
  float *testf = alloc_ptr(&r, sizeof(float));
  *testf = 0.5;

  printf("%d\n", *test);
  printf("%c\n", *testc);
  printf("%f\n", *testf);

  reset_arena(&r); // it is now reusable

  free_arena(&r);
*/

#ifndef V_EXIT
#include <stdlib.h>
#define V_EXIT(x) exit(x)
#endif // V_EXIT

#ifndef V_CUSTOM_ALLOC
#include <stdlib.h>
#define V_MALLOC malloc
#define V_REALLOC realloc
#define V_FREE free
#endif // V_CUSTOM_ALLOC

#ifndef V_FPRINTF
#include <stdio.h>
#define V_FPRINTF fprintf
#endif // V_FPRINTF

typedef struct arena arena;

arena alloc_arena(size_t size);
void *alloc_ptr(arena *a, size_t size);
void reset_arena(arena* a);
void free_arena(arena *a);

// #define C_ARENA
#ifdef C_ARENA

struct arena {
  size_t current_offset;
  void *base_pointer;
  size_t size;
} ;

arena alloc_arena(size_t size) {
  arena r;
  r.base_pointer = V_MALLOC(size);
  if (r.base_pointer == NULL) {
    V_FPRINTF(stderr, "buy more ram :)");
    V_EXIT(1);
  }

  r.current_offset = 0;
  r.size = size;
  return r;
}

void *alloc_ptr(arena *a, size_t size) {
  if (a->base_pointer == NULL) {
    V_FPRINTF(stderr, "base pointer is null either it was not initialized or it has been freed :)\n");
    V_EXIT(1);
  }

  if (size < a->size - a->current_offset) {
    void *out = (a->base_pointer + a->current_offset);
    a->current_offset += size;
    return out;
  }
  V_FPRINTF(stderr, "ERROR: tried to allocate more than the arena had %zu > %zu\n", size , a->size - a->current_offset);
  V_EXIT(1);
}

void reset_arena(arena* a) {
  V_FREE(a->base_pointer);
  a->base_pointer = V_MALLOC(a->size);
  a->current_offset = 0;
  *(int*)(a->base_pointer) = 0;
}

void free_arena(arena *a) {
  V_FREE(a->base_pointer);
  a->base_pointer = NULL;
  a->current_offset = 0;
  a->size = 0;
}
#endif // C_ARENA
