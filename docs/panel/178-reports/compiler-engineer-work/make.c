#include <string.h>
#include "q.h"
/* today's emitter shape: a local assigned from a compound literal, returned by value */
struct q make_literal(int8_t c, int32_t i) { struct q t; t = (struct q){.c = c, .i = i}; return t; }
/* the proposal's robust shape: memset, then member stores, returned by value */
struct q make_memset(int8_t c, int32_t i) { struct q t; memset(&t, 0, sizeof t); t.c = c; t.i = i; return t; }
