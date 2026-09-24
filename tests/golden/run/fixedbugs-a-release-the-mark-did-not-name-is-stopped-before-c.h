/* Two families of handle over one C type, the shape of `popen` and `fopen`
 * over `FILE *`: a `p_open` handle is owed `p_close`, and `h_close` is the
 * wrong way to end it. `h_close` writes to stdout so the case can show that
 * C never ran. */
#include <stdint.h>
#include <stdio.h>
typedef struct hh { int64_t n; } hh;
static inline __attribute__((noinline)) hh *p_open(void) { static hh one; one.n = 1; return &one; }
static inline __attribute__((noinline)) void p_close(hh *x) { (void)x; }
static inline __attribute__((noinline)) void h_close(hh *x) { (void)x; printf("C ran\n"); }
