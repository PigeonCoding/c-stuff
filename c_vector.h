// #pragma once

// version 0.5

/*
  vector vec = alloc_vector_sz(int); // == alloc_vector(sizeof(int))
  prealloc_vector(&vec, 4); // allocates (4 * vec.type_size) in arena

  int h1 = 111;
  int h2 = 112;

  // copies the memory of the variable to the arena with V_MEMCPY
  push_vector_by_ref(vec, h1);
  push_vector_by_ref(vec, h2);

  `// assigns the value directly not using V_MEMCPY
  push_vector_by_val(vec, 113, int);
  push_vector_by_val(vec, 114, int);

  for (size_t i = 0; i < vec.length; i++) {
    int *g = (int *)get_from_vec(&vec, i);
    printf("value: %i\n", *g);
  }

  printf("--------------\n");
  pop_element_from_vec(&vec, 1);

  vforeach_val(int, el, vec, i)
    printf("value: %i\n", el);
  end_foreach

  printf("--------------\n");

  reset_vector(&vec);
  printf("new size: %zu\n", vec.length);

  free_vector(&vec);
}
*/

#ifndef V_ALLOC
#include <stdlib.h>
#define V_MALLOC malloc
#define V_REALLOC realloc
#define V_FREE free
#endif

#ifndef V_EXIT
#include <stdlib.h>
#define V_EXIT(x) exit(x)
#endif

#ifndef V_MEMCPY
#include <string.h>
#define V_MEMCPY memcpy
#endif

#ifndef V_FPRINTF
#include <stdio.h>
#define V_FPRINTF fprintf
#endif

typedef struct vector vector;

vector alloc_vector(size_t type_siz);
#ifndef alloc_vector_sz
#define alloc_vector_sz(sz) alloc_vector(sizeof(sz))
#endif
void prealloc_vector(vector *vec, size_t num);
void *get_from_vec(vector *vec, size_t index);
void reset_vector(vector *vec);
void free_vector(vector *vec);
void pop_element_from_vec(vector *vec, size_t index);
#ifndef push_vector_by_val
#define push_vector_by_val(vec, v, type) push_vector_by_val_def(vec, v, type)
#endif
#ifndef push_vector_by_ref
#define push_vector_by_ref(vec, v) push_vector_by_ref_def(vec, v)
#endif
#ifndef vforeach_ref
#define vforeach_ref(type, name, vector, i)                                    \
  vforeach_ref_def(type, name, vector, i)
#endif
#ifndef vforeach_val
#define vforeach_val(type, name, vector, i)                                    \
  vforeach_val_def(type, name, vector, i)
#endif
#ifndef end_foreach
#define end_foreach end_foreach_ref
#endif

// #define C_VECTOR_IMPLEMENTATION
#ifdef C_VECTOR_IMPLEMENTATION

struct vector {
  void *base_pointer;
  size_t size;
  size_t type_size;
  size_t length;
};

void *internal_get_data_pointer(vector *a, size_t size);

#define vforeach_ref_def(type, name, vector, i)                                \
  for (unsigned long i = 0; i < vector.length; i++) {                          \
    type *name = (type *)get_from_vec(&vector, i);
#define vforeach_val_def(type, name, vector, i)                                \
  for (unsigned long i = 0; i < vector.length; i++) {                          \
    type name = *(type *)get_from_vec(&vector, i);
#define end_foreach_ref }

#define push_vector_by_val_def(vec, v, type)                                   \
  {                                                                            \
    vec.length++;                                                              \
    if (vec.length * vec.type_size < vec.size) {                               \
      prealloc_vector(&vec, vec.length * 2 + 1);                               \
    }                                                                          \
    type *g = (type *)internal_get_data_pointer(&vec, vec.type_size);          \
    *g = v;                                                                    \
  }

#define push_vector_by_ref_def(vec, v)                                         \
  {                                                                            \
    vec.length++;                                                              \
    if (vec.length * vec.type_size < vec.size) {                               \
      prealloc_vector(&vec, vec.length * 2 + 1);                               \
    }                                                                          \
    void *g = internal_get_data_pointer(&vec, vec.type_size);                  \
    V_MEMCPY(g, &v, vec.type_size);                                            \
  }

void *internal_get_data_pointer(vector *a, size_t size) {
  if (a->base_pointer == NULL) {
    V_FPRINTF(stderr,
              "base pointer is null either it was not initialized or it "
              "has been freed:)\n ");
    V_EXIT(1);
  }

  if (size < a->size - (a->length - 1) * a->type_size) {
    void *out = (a->base_pointer + (a->length - 1) * a->type_size);
    return out;
  }
  V_FPRINTF(stderr,
            "ERROR: tried to allocate more than the arena had %zu > %zu\n",
            size, a->size - (a->length - 1) * a->type_size);
  V_EXIT(1);
}

vector alloc_vector(size_t type_siz) {
  vector vec;

  vec.base_pointer = V_MALLOC(type_siz * 2);
  if (vec.base_pointer == NULL) {
    V_FPRINTF(stderr, "buy more ram :)");
    V_EXIT(1);
  }

  vec.size = type_siz * 2;
  vec.length = 0;
  vec.type_size = type_siz;

  return vec;
}

void prealloc_vector(vector *vec, size_t num) {
  vec->base_pointer =
      V_REALLOC(vec->base_pointer, vec->type_size * (vec->size + num * 2));
  vec->size = vec->size + num * 2;
}

void *get_from_vec(vector *vec, size_t index) {
  if (index >= vec->length) {
    V_FPRINTF(stderr, "ERROR: index out of range\n");
    V_EXIT(1);
  }
  return vec->base_pointer + vec->type_size * index;
}

void reset_vector(vector *vec) {
  V_FREE(vec->base_pointer);
  vec->base_pointer = V_MALLOC(vec->type_size * 2);
  vec->size = vec->type_size * 2;
  vec->length = 0;
}

void free_vector(vector *vec) {
  V_FREE(vec->base_pointer);
  vec->base_pointer = NULL;
  vec->size = 0;
  vec->length = 0;
  vec->type_size = 0;
}

void pop_element_from_vec(vector *vec, size_t index) {
  if (index + 1 > vec->length) {
    V_FPRINTF(stderr, "[ERROR]: index out of range %zu > %zu\n", index,
              vec->length - 1);
    V_EXIT(1);
  }

  if (index == vec->length - 1) {
    vec->length -= 1;
    return;
  }

  // TODO: use memcpy to directly shift memory (was buggy for some reason)
  size_t size = vec->length - index - 1;
  for (size_t i = 0; i < size; i++) {
    V_MEMCPY(get_from_vec(vec, index + i), get_from_vec(vec, index + i + 1),
             vec->type_size);
  }
  vec->length -= 1;
}
#endif // C_VECTOR_IMPLEMENTATION
