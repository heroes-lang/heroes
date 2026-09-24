/* A C function that takes a handle — one pointer — and frees it. The program
 * beside this declares its parameter with `@`, which hands C the ADDRESS of
 * the program's cell, and `void *` takes that in silence. */
#include <stdlib.h>
static inline __attribute__((noinline)) void *make(void) { return malloc(8); }
static inline __attribute__((noinline)) void eat(void *p) { free(p); }
