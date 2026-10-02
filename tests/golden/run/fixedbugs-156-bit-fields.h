/* Beside defect 156's run case, lane land186, 2026-10-02 (panel 186 R5): the
   records a struct with bit-fields still binds. */
#include <stdint.h>

typedef struct { int32_t kind; uint32_t flag : 1; uint32_t rest : 31; } BF;
static inline BF make_bf(void) { BF b = {6, 1, 5}; return b; }

/* an unnamed bit-field and a zero-width one: padding C names nothing */
typedef struct { int32_t a; uint32_t : 3; int32_t b; } UNNAMED;
static inline UNNAMED make_unnamed(void) { UNNAMED u = {1, 2}; return u; }
typedef struct { int32_t a; uint32_t : 0; int32_t b; } ZEROW;
static inline ZEROW make_zerow(void) { ZEROW z = {3, 4}; return z; }
