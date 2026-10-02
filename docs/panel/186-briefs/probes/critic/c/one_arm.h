#include <stdint.h>
typedef struct { int32_t kind; union { int8_t b; int64_t q; }; } SB;
typedef union { int8_t b; int64_t q; } UB;
static inline SB make_a(void) { SB s; s.kind = 1; s.q = 0x100; return s; }
static inline SB make_b(void) { SB s; s.kind = 1; s.q = 0x200; return s; }
static inline UB make_ua(void) { UB u; u.q = 0x100; return u; }
static inline UB make_ub(void) { UB u; u.q = 0x200; return u; }
