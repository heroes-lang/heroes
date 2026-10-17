/* Defect 2116: record constants whose header value is no constant, each
   read twice by a program: a counter's call, a call returning a record, and
   a compound literal array whose address outlives the accessor's frame. */
#include <stdint.h>
typedef struct { int32_t x; int32_t y; } pt;
static inline int32_t calls_next(void) { static int32_t n; return ++n; }
static inline pt calls_make(void) { pt p = { 5, 6 }; return p; }
#define R_CNT { calls_next(), 2 }
#define R_MADE calls_make()
typedef struct { const char *s; const int32_t *p; } held;
#define R_DEAD { "a", (int32_t[]){ 5 } }
