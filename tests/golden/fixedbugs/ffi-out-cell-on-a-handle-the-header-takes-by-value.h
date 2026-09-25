/* A C function that takes a handle — one pointer — and frees it. The program
 * beside this declares its parameter with `@`, which hands C the ADDRESS of
 * the program's cell, and `void *` takes that in silence. `drop` is the
 * releaser the acquiring mark names, so the binding's own words are true and
 * the one diagnostic left is the header's. */
#include <stdlib.h>
static inline __attribute__((noinline)) void *make(void) { return malloc(8); }
static inline __attribute__((noinline)) void eat(void *p) { free(p); }
static inline __attribute__((noinline)) void drop(void *p) { free(p); }
