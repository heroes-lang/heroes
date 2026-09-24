/* The C side of M-agreed-retention's first item: a pointer C makes and frees.
 * noinline, because at -O2 clang deletes a static inline malloc and two frees
 * whole and the program exits 0 (measured 2026-09-23). */
#include <stdlib.h>
static __attribute__((noinline)) void *make(void) { return malloc(16); }
static __attribute__((noinline)) void release(void *p) { free(p); }
