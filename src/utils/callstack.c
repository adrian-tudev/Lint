#include "callstack.h"

#include <stdlib.h>

#define CALLSTACK_INITIAL_CAP 8

void callstack_init(CallStack *cs) {
  cs->size = 0;
  cs->capacity = CALLSTACK_INITIAL_CAP;
  cs->data = malloc(cs->capacity * sizeof(Function *));
}

void callstack_free(CallStack *cs) {
  free(cs->data);
  cs->data = NULL;
  cs->size = 0;
  cs->capacity = 0;
}

static int callstack_reserve(CallStack *cs, size_t new_capacity) {
  if (new_capacity <= cs->capacity) return 1;

  Function **new_data = realloc(cs->data, new_capacity * sizeof(Function *));
  if (!new_data) return 0;

  cs->data = new_data;
  cs->capacity = new_capacity;
  return 1;
}

int callstack_push(CallStack *cs, Function *fn) {
  if (cs->size == cs->capacity) {
    size_t new_cap = cs->capacity * 2;
    if (!callstack_reserve(cs, new_cap)) return 0;
  }
  cs->data[cs->size++] = fn;
  return 1;
}

Function *callstack_pop(CallStack *cs) {
  if (cs->size == 0) return NULL;
  return cs->data[--cs->size];
}

Function *callstack_peek(const CallStack *cs) {
  if (cs->size == 0) return NULL;
  return cs->data[cs->size - 1];
}

size_t callstack_size(const CallStack *cs) {
  return cs->size;
}

int callstack_is_empty(const CallStack *cs) {
  return cs->size == 0;
}
