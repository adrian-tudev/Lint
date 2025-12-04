#ifndef VECTOR_H_
#define VECTOR_H_

#include <stddef.h>

typedef struct Vector {
  void **data;
  size_t size;
  size_t capacity;
} Vector;

void  vec_init(Vector *v);
void  vec_free(Vector *v);
int   vec_push(Vector *v, void *value);
void *vec_get(const Vector *v, size_t index);
int   vec_set(Vector *v, size_t index, void *value);
int   vec_reserve(Vector *v, size_t new_capacity);

#endif // VECTOR_H_
