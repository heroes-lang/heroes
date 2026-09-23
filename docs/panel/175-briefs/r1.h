/* Panel 175, item 1 and defect 076: C that makes a pointer and frees it. Every
 * function is noinline, because at -O2 clang deletes a static inline malloc and
 * two frees whole and the program exits 0 (measured 2026-09-23). */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
typedef struct box box;
static __attribute__((noinline)) void *make(void) { return malloc(16); }
static __attribute__((noinline)) void release(void *p) { free(p); }
static __attribute__((noinline)) void fill(char **out) { char *s = strdup("x"); *out = s; free(s); }
static __attribute__((noinline)) box *box_open(void) { return (box *)malloc(8); }
static __attribute__((noinline)) void box_close(box *b) { free(b); }
static __attribute__((noinline)) void fill_out(const char **out) { *out = (const char *)malloc(8); }
static __attribute__((noinline)) void free_out(const char *p) { free((void *)(uintptr_t)p); }
