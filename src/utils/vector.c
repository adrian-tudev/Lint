#include "vector.h"

#include <stdlib.h>

#define VEC_INITIAL_CAP 8

void vec_init(Vector *v) {
  v->size = 0;
  v->capacity = VEC_INITIAL_CAP;
  v->data = malloc(v->capacity * sizeof(void *));
}

void vec_free(Vector *v) {
  free(v->data);
  v->data = NULL;
  v->size = 0;
  v->capacity = 0;
}

int vec_reserve(Vector *v, size_t new_capacity) {
  if (new_capacity <= v->capacity) return 1;

  void **new_data = realloc(v->data, new_capacity * sizeof(void *));
  if (!new_data) return 0;

  v->data = new_data;
  v->capacity = new_capacity;
  return 1;
}

int vec_push(Vector *v, void *value) {
  if (v->size == v->capacity) {
    size_t new_cap = v->capacity * 2;
    if (!vec_reserve(v, new_cap)) return 0;
  }
  v->data[v->size++] = value;
  return 1;
}

int vec_merge(Vector* v, Vector* u) {
  for (size_t i = 0; i < u->size; i++) {
    if (!vec_push(v, vec_get(u, i))) return 0;
  }
  return 1;
}

void *vec_get(const Vector *v, size_t index) {
  if (index >= v->size) return NULL;
  return v->data[index];
}

int vec_set(Vector *v, size_t index, void *value) {
  if (index >= v->size) return 0;
  v->data[index] = value;
  return 1;
}
