#pragma once
#include "cvector.h"

/*
  string test_string = alloc_string();

  push_string(&test_string, "Hello World!");

  printf("%s\n", get_string(&test_string));

  free_string(&test_string);

  return 0;
*/

typedef struct {
  vector data;
  size_t *length;
} string;

string alloc_string() {
  string s;
  s.data = alloc_vector(sizeof(char));
  s.length = &s.data.length;
  return s;
}

void push_char_string(string *s, char c) {
  push_vector(&s->data, (void *)&c);
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

char *get_string(string *s) {
  *(char *)(s->data.arena.base_pointer + *s->length) = '\0';
  return (char *)s->data.arena.base_pointer;
}

void free_string(string *s) {
  free_vector(&s->data);
}