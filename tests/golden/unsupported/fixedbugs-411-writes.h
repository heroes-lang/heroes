/* Beside defect 411's cases: one C function per way a header spells a pointer
   C may write through, each writing the first byte it is handed. A program
   that declares one of them `cstr` is refused at the declaration, so none of
   these bodies runs; they are what a lend of the program's own bytes would
   meet if it were not. */
#include <stdint.h>

typedef unsigned char *hero_bytes;

static inline int32_t w_uchar(unsigned char *md) { md[0] = 'Z'; return 1; }
static inline int32_t w_schar(signed char *p) { p[0] = 'Z'; return 1; }
static inline int32_t w_u8(uint8_t *p) { p[0] = 'Z'; return 1; }
static inline int32_t w_typedef(hero_bytes p) { p[0] = 'Z'; return 1; }
static inline int32_t w_array(unsigned char md[32]) { md[0] = 'Z'; return 1; }
static inline int32_t w_restrict(unsigned char *restrict p) { p[0] = 'Z'; return 1; }
static inline int32_t w_atomic(unsigned char *_Atomic p) { p[0] = 'Z'; return 1; }
static inline int32_t w_char(char *p) { p[0] = 'Z'; return 1; }
static inline int32_t w_const_pointer(char *const p) { p[0] = 'Z'; return 1; }
static inline int32_t w_void(void *p) { ((unsigned char *)p)[0] = 'Z'; return 1; }

/* Three parameters where a binding declares two: the first is written, the
   two the binding names are read. */
static inline int32_t w_shifted(char *x, const char *a, const char *b) { x[0] = a[0]; return b[0]; }
