#pragma once
#include "arena.h"
#include <stdio.h>
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

  pop_vector(&vec, 2);

  for (size_t i = 0; i < vec.length; i++)
  {
    int *g = (int *)get_from_vec(&vec, i);
    printf("%d\n", *g);
  }

  free_vector(&vec);
}
*/

typedef struct {
  arena arena;
  size_t length;
  size_t type_size;
} vector;

vector alloc_vector(size_t type_siz) {
  vector vec;

  vec.arena = alloc_arena(type_siz * 2);
  vec.length = 0;
  vec.type_size = type_siz;

  return vec;
}

void prealloc_vector(vector *vec, size_t num) {
  // arena temp_a = alloc_arena(vec->type_size * num);

  // memcpy(temp_a.base_pointer, vec->arena.base_pointer,
  //        vec->length * vec->type_size);

  // vec->arena.base_pointer = temp_a.base_pointer;
  // vec->arena.size = temp_a.size;
  vec->arena.base_pointer =
      realloc(vec->arena.base_pointer, vec->type_size * num * 2 + 1);
  vec->arena.size = vec->type_size * num * 2 + 1;
}

void push_vector(vector *vec, void *v) {
  vec->length++;

  if (vec->length * vec->type_size < vec->arena.size) {
    prealloc_vector(vec, vec->length * 2);
  }

  void *g = get_data_pointer(&vec->arena, vec->type_size);
  memcpy(g, v, vec->type_size);
}

void *get_from_vec(vector *vec, size_t index) {
  if (index > vec->length) {
    fprintf(stderr, "ERROR: index out of range\n");
    exit(1);
  }
  return vec->arena.base_pointer + vec->type_size * index;
}

void free_vector(vector *vec) {
  free_arena(&vec->arena);
  vec->length = 0;
  vec->type_size = 0;
}

// TODO: make it somewhat working (it works half of the time)
void pop_vector(vector *vec, size_t index) {
  // printf("%zu and %zu\n", vec->length, index + 1);
  if (index + 1 > vec->length) {
    fprintf(stderr, "ERROR: index out of range\n");
    exit(1);
  }
  size_t size = vec->length - index;
  printf("%zu\n", size);
  memcpy(vec->arena.base_pointer + (index)*vec->type_size,       // dest
         vec->arena.base_pointer + (index + 1) * vec->type_size,  // src
         size                                                       // size
  );
  vec->length -= 1;
}
