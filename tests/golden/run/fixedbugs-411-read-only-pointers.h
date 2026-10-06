/* Beside defect 411's run case: one C function per way a header spells a
   pointer to bytes it only reads, each returning the first byte. Every one
   must take a `cstr`, however the program lends it. The `const unsigned
   char *` family is held by the pointee check's own test in
   `selfhost/cli/pointee.hero`, since clang warns on it today. */
#include <stdint.h>

typedef const char *hero_text;

static inline int32_t r_char(const char *p) { return p[0]; }
static inline int32_t r_void(const void *p) { return ((const unsigned char *)p)[0]; }
static inline int32_t r_typedef(hero_text p) { return p[0]; }
static inline int32_t r_const_pointer(const char *const p) { return p[0]; }
static inline int32_t r_volatile(const volatile char *p) { return p[0]; }
static inline int32_t r_array(const char p[4]) { return p[0]; }
static inline int32_t r_atomic(const char *_Atomic p) { return p[0]; }
