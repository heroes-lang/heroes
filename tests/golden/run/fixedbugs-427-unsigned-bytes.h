/* Beside defect 427's run case: one C function per way a header spells a
   pointer to `const` bytes of a sign other than `char`'s, each returning the
   first byte. */
#include <stdint.h>

typedef const unsigned char *hero_const_bytes;
typedef const unsigned char hero_const_byte;

static inline int32_t r_uchar(const unsigned char *p) { return p[0]; }
static inline int32_t r_schar(const signed char *p) { return p[0]; }
static inline int32_t r_u8(const uint8_t *p) { return p[0]; }
static inline int32_t r_typedef(hero_const_bytes p) { return p[0]; }
static inline int32_t r_byte_typedef(hero_const_byte *p) { return p[0]; }
static inline int32_t r_array(const unsigned char p[4]) { return p[0]; }
static inline int32_t r_volatile(const volatile unsigned char *p) { return p[0]; }
static inline int32_t r_atomic(const unsigned char *_Atomic p) { return p[0]; }
