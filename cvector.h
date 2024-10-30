#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  vector vec = alloc_vector(sizeof(int));
  prealloc_vector(&vec, 4); // allocates (4 * vec.type_size) bits

  int h1 = 111;
  int h2 = 112;
  int h3 = 113;
  int h4 = 114;

  push_vector(&vec, &h1);
  push_vector(&vec, &h2);
  push_vector(&vec, &h3);
  push_vector(&vec, &h4);

  // pop_vector(&vec, 0);

  for (size_t i = 0; i < vec.length; i++)
  {
    int *g = (int *)get_from_vec(&vec, i);
    printf("%zu: %d\n", i, *g);
  }

  reset_vector(&vec);
  printf("%zu\n", vec.length);

  free_vector(&vec);
}
*/

#ifndef V_MALLOC
#define V_MALLOC malloc
#endif

typedef struct {
  void *base_pointer;
  size_t size;
  size_t type_size;
  size_t length;
} vector;


void *get_data_pointer(vector *a, size_t size);
vector alloc_vector(size_t type_siz);
void prealloc_vector(vector *vec, size_t num);
void push_vector(vector *vec, void *v);
void *get_from_vec(vector *vec, size_t index);
void reset_vector(vector *vec);
void free_vector(vector *vec);

#ifdef C_VECTOR
void *get_data_pointer(vector *a, size_t size) {
  if (a->base_pointer == NULL) {
    fprintf(stderr, "base pointer is null either it was not initialized or it "
                    "has been freed:)\n ");
    exit(1);
  }

  if (size < a->size - (a->length - 1) * a->type_size) {
    void *out = (a->base_pointer + (a->length - 1) * a->type_size);
    return out;
  }
  printf("ERROR: tried to allocate more than the arena had %zu > %zu\n", size,
         a->size - (a->length - 1) * a->type_size);
  exit(1);
}

vector alloc_vector(size_t type_siz) {
  vector vec;

  vec.base_pointer = V_MALLOC(type_siz * 2);
  if (vec.base_pointer == NULL) {
    printf("buy more ram :)");
    exit(1);
  }

  vec.size = type_siz * 2;
  vec.length = 0;
  vec.type_size = type_siz;

  return vec;
}

void prealloc_vector(vector *vec, size_t num) {
  vec->base_pointer = realloc(vec->base_pointer, vec->type_size * num * 2);
  vec->size = vec->type_size * num * 2;
}

void push_vector(vector *vec, void *v) {
  vec->length++;

  if (vec->length * vec->type_size < vec->size) {
    prealloc_vector(vec, vec->length * 2 + 1);
  }

  void *g = get_data_pointer(vec, vec->type_size);
  memcpy(g, v, vec->type_size);
}

void *get_from_vec(vector *vec, size_t index) {
  if (index >= vec->length) {
    fprintf(stderr, "ERROR: index out of range\n");
    exit(1);
  }
  return vec->base_pointer + vec->type_size * index;
}

void reset_vector(vector *vec) {
  *(int *)(vec->base_pointer) = 0;
  vec->length = 0;
}

void free_vector(vector *vec) {
  free(vec->base_pointer);
  vec->base_pointer = NULL;
  vec->size = 0;
  vec->length = 0;
  vec->type_size = 0;
}

// TODO: make it somewhat working (it works half of the time)
// void pop_vector(vector *vec, size_t index) {
//   fprintf(stderr, "pop not yet implemented\n");
//   exit(1);
  // printf("%zu and %zu\n", vec->length, index + 1);
  // if (index + 1 > vec->length) {
  //   fprintf(stderr, "ERROR: index out of range\n");
  //   exit(1);
  // }
  // size_t size = vec->length - index;
  // memcpy(vec->base_pointer + (index)*vec->type_size,       // dest
  //        vec->base_pointer + (index + 1) * vec->type_size,  // src
  //        size                                                 // size
  // );
  // vec->length -= 1;
// }
#endif // C_VECTOR