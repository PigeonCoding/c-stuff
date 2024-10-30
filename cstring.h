#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  string test_string = alloc_string();

  push_string(&test_string, "Hello World!");

  printf("%s\n", get_string_view(&test_string));

  free_string(&test_string);

  return 0;
*/


#define type_size sizeof(char)

#ifndef V_MALLOC
#define V_MALLOC malloc
#endif

typedef struct {
  void *base_pointer;
  size_t size;
  size_t length;
} string;


string alloc_string();
void prealloc_string(string *s, size_t num);
void *get_data_pointer(string *a, size_t size);
void push_char_string(string *s, char c);
void push_string_whitespace(string *s, char *c);
void push_string(string *s, char *c);
char *get_string_view(string *s);
void free_string(string *s);

#ifdef C_STRING
string alloc_string() {
  string s;
  s.base_pointer = V_MALLOC(type_size * 2);
  if (s.base_pointer == NULL) {
    printf("buy more ram :)");
    exit(1);
  }

  s.size = type_size * 2;
  s.length = 0;
  return s;
}

void prealloc_string(string *s, size_t num) {
  s->base_pointer = realloc(s->base_pointer, type_size * num * 2);
  s->size = type_size * num * 2;
}

void *get_data_pointer(string *a, size_t size) {
  if (a->base_pointer == NULL) {
    fprintf(stderr, "base pointer is null either it was not initialized or it "
                    "has been freed:)\n ");
    exit(1);
  }

  if (size < a->size - (a->length - 1) * type_size) {
    void *out = (a->base_pointer + (a->length - 1) * type_size);
    return out;
  }
  printf("ERROR: tried to allocate more than the arena had %zu > %zu\n", size,
         a->size - (a->length - 1) * type_size);
  exit(1);
}

void push_char_string(string *s, char c) {
  // push_vector(&s->data, (void *)&c);
  s->length++;

  if (s->length * type_size < s->size) {
    prealloc_string(s, s->length * 2 + 1);
  }

  void *g = get_data_pointer(s, type_size);
  memcpy(g, (void*)(&c), type_size);
}

void push_string_whitespace(string *s, char *c) {
  char *h;
  for (h = (char *)c; *h; h++) {
    push_char_string(s, *h);
  }
  push_char_string(s, ' ');
}

void push_string(string *s, char *c) {
  char *h;
  for (h = (char *)c; *h; h++) {
    push_char_string(s, *h);
  }
}

char *get_string_view(string *s) {
  *(char *)(s->base_pointer + s->length) = '\0';
  return (char *)s->base_pointer;
}

void free_string(string *s) {
  free(s->base_pointer);
  s->base_pointer = NULL;
  s->size = 0;
  s->length = 0;
}
#endif // C_STRING