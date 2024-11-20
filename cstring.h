#pragma once

/*
  string test_string = alloc_string();

  push_string(&test_string, "Hello World!");

  printf("%s\n", get_string_c(&test_string));

  free_string(&test_string);
*/

#define type_size sizeof(char)

#ifndef V_ALLOC
#include <stdlib.h>
#define V_MALLOC malloc
#define V_REALLOC realloc
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

typedef struct {
  void *base_pointer;
  size_t size;
  size_t length;
} string;

string alloc_string();
void prealloc_string(string *s, size_t num);
void *get_string_data_pointer(string *a, size_t size);
void push_char_string(string *s, char c);
void push_string_whitespace(string *s, char *c);
void push_string(string *s, char *c);
char *get_string_c(string *s);
short int compare_str(const char *s1, const char *s2, size_t size, short int check_sz);
char *get_char(string *s, size_t index);
void read_file(string *s, const char *filename);
void free_string(string *s);
#define sforeach_ref(name, str, i) sforeach_ref_def(name, str, i)
#define sforeach_val(name, str, i) sforeach_val_def(name, str, i)
#ifndef end_foreach
#define end_foreach end_foreach_def
#endif // end_foreach

// #define C_STRING
#ifdef C_STRING

short int compare_str(const char *s1, const char *s2, size_t size,
                      short int check_sz) {

  if ((size > strlen(s1) || size > strlen(s2)) && check_sz) {
    return -1;
  }

  for (size_t i = 0; i < size; i++) {
    if (s1[i] != s2[i]) {
      return 1;
    }
  }
  return 0;
}

#define sforeach_ref_def(name, str, i)                                         \
  for (unsigned long i = 0; i < str.length; i++) {                             \
    char *name = get_char(&str, i);
#define sforeach_val_def(name, str, i)                                         \
  for (unsigned long i = 0; i < str.length; i++) {                             \
    char name = *get_char(&str, i);
#define end_foreach_def }

string alloc_string() {
  string s;
  s.base_pointer = V_MALLOC(type_size * 2);
  if (s.base_pointer == NULL) {
    V_FPRINTF(stderr, "buy more ram :)");
    V_EXIT(1);
  }

  s.size = type_size * 2;
  s.length = 0;
  return s;
}

void prealloc_string(string *s, size_t num) {
  s->base_pointer = V_REALLOC(s->base_pointer, type_size * num * 2);
  s->size = type_size * num * 2;
}

void *get_string_data_pointer(string *a, size_t size) {
  if (a->base_pointer == NULL) {
    V_FPRINTF(stderr,
              "base pointer is null either it was not initialized or it "
              "has been freed:)\n ");
    V_EXIT(1);
  }

  if (size < a->size - (a->length - 1) * type_size) {
    void *out = (a->base_pointer + (a->length - 1) * type_size);
    return out;
  }
  V_FPRINTF(stderr,
            "ERROR: tried to allocate more than the arena had %zu > %zu\n",
            size, a->size - (a->length - 1) * type_size);
  V_EXIT(1);
}

char *get_char(string *s, size_t index) {
  return (char *)(s->base_pointer + index * type_size);
}

void read_file(string *s, const char *filename) {
  FILE *fptr;
  fptr = fopen(filename, "r");

  char content[1024 * 1024];
  while (fgets(content, 100, fptr)) {
    push_string(s, content);
  }
  fclose(fptr);
}

void push_char_string(string *s, char c) {
  s->length++;

  if (s->length * type_size < s->size) {
    prealloc_string(s, s->length + 1);
  }

  void *g = get_string_data_pointer(s, type_size);
  V_MEMCPY(g, (void *)(&c), type_size);
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

char *get_string_c(string *s) {
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