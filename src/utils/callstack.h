#ifndef CALLSTACK_H_
#define CALLSTACK_H_

#include <stddef.h>

#include "ast/grammar_types.h"

typedef struct CallStack {
  Function **data;
  size_t size;
  size_t capacity;
} CallStack;

void      callstack_init(CallStack *cs);
void      callstack_free(CallStack *cs);
int       callstack_push(CallStack *cs, Function *fn);
Function *callstack_pop(CallStack *cs);
Function *callstack_peek(const CallStack *cs);
size_t    callstack_size(const CallStack *cs);
int       callstack_is_empty(const CallStack *cs);

#endif // CALLSTACK_H_
