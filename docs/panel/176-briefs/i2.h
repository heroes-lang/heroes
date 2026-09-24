#include <stdlib.h>
#include <stdint.h>
typedef struct hh hh;
static const char *kept;
static __attribute__((noinline)) int keep(const char *s) { kept = s; return 7; }
static __attribute__((noinline)) hh *h_open(void) { return (hh *)malloc(8); }
static __attribute__((noinline)) void h_close(hh *x) { free(x); }
static __attribute__((noinline)) void h_close2(hh *x) { free(x); }
static __attribute__((noinline)) hh *h_peek(hh *x) { return x; }
static __attribute__((noinline)) char *make_text(void) { char *p = malloc(4); p[0] = 'h'; p[1] = 'i'; p[2] = 0; return p; }
static __attribute__((noinline)) void my_free(void *p) { free(p); }
static __attribute__((noinline)) void other_free(void *p) { (void)p; }
static __attribute__((noinline)) int fill(const void *b, uint64_t n) { (void)b; return (int)n; }
static __attribute__((noinline)) void h_open_out(hh **out) { *out = (hh *)malloc(8); }
