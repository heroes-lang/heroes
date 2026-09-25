#include <stdint.h>
typedef struct ob { int64_t refs; } ob;
static inline __attribute__((noinline)) ob *ob_new(void) { static ob one; one.refs = 1; return &one; }
static inline __attribute__((noinline)) void ob_put(ob *o) { o->refs--; }
static inline __attribute__((noinline)) uint8_t ob_narrow(ob *o) { o->refs--; return 0; }
static inline __attribute__((noinline)) int32_t ob_wide(ob *o) { o->refs--; return 0; }
static inline __attribute__((noinline)) uint64_t ob_huge(ob *o) { o->refs--; return 0; }
static inline __attribute__((noinline)) uint8_t ob_octal(ob *o) { o->refs--; return 0; }
static inline __attribute__((noinline)) int8_t ob_signed(ob *o) { o->refs--; return 0; }
static inline __attribute__((noinline)) uint8_t ob_hex(ob *o) { o->refs--; return 255; }
static inline __attribute__((noinline)) int8_t ob_edge(ob *o) { o->refs--; return 127; }
