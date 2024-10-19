#include "cvector.h"

typedef struct
{
  size_t h;
  size_t w;
  const char *hh;
} b;

int main(void)
{
  vector vec = alloc_vector(sizeof(b));
  // prealloc_vector(&vec, 4);

  b jj = {
      .h = 1,
      .w = 2,
      .hh = "hello"};

  push_vector(&vec, &jj);

  for (size_t i = 0; i < vec.length; i++)
  {
    b *g = get_from_vec(&vec, i);
    printf("%s\n", g->hh);
  }

  free_vector(&vec);

  return 0;
}