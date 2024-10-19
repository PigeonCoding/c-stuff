#pragma once
#include "cvector.h"
#include <stdio.h>

/*
  string test_string = alloc_string();

  push_string(&test_string, "Hello World!");

  char str[test_string.length] = {};
  get_string(&test_string, str);
  printf("%s\n", str);

  free_string(&test_string);

  return 0;
*/

typedef struct {
  vector data;
  size_t length;
} string;

string alloc_string() {
  string s;
  s.data = alloc_vector(sizeof(char));
  s.length = 0;
  return s;
}

void push_char_string(string *s, char c) {
  push_vector(&s->data, (void *)&c);
  s->length += 1;
}

void push_string(string *s, char *c) {
  char *h;
  for (h = (char *)c; *h; h++) {
    push_char_string(s, *h);
  }
}

char *get_string(string *s) { return (char *)s->data.arena.base_pointer; }

void free_string(string *s) {
  free_vector(&s->data);
  s->length = 0;
}