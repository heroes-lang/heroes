/* Panel 178, compiler-engineer: the padding of a C record through the shapes the
   emitter would write, into memory the optimiser cannot see (the caller is another
   translation unit and pre-fills the destination with 0xAA). */
#include <string.h>
#include "q.h"
/* CONTROL: every member stored, no initialiser, then copied out. */
void shape_control(struct q *out, int8_t c, int32_t i) { struct q t; t.c = c; t.i = i; *out = t; }
/* today's emitter: `t = (T){...}; h0 = t;` and then the value leaves by copy. */
void shape_literal(struct q *out, int8_t c, int32_t i) { struct q t; struct q h0; t = (struct q){.c = c, .i = i}; h0 = t; *out = h0; }
/* R1 by designated initialiser with the field omitted: `(T){.i = i}`. */
void shape_omitted(struct q *out, int32_t i) { struct q t; t = (struct q){.i = i}; *out = t; }
/* R0: a zero of the whole record, `(T){0}`, then a member store through the cell. */
void shape_zero_then_store(struct q *out, int8_t c, int32_t i) { struct q h0; h0 = (struct q){0}; h0.c = c; h0.i = i; *out = h0; }
/* memset, then member stores, then copied out. */
void shape_memset(struct q *out, int8_t c, int32_t i) { struct q t; memset(&t, 0, sizeof t); t.c = c; t.i = i; *out = t; }
/* memset straight into the destination, the only shape that writes the bytes where C reads them. */
void shape_memset_in_place(struct q *out, int8_t c, int32_t i) { memset(out, 0, sizeof *out); out->c = c; out->i = i; }
