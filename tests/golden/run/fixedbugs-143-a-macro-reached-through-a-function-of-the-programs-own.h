/* Beside defect 143's run case, lane ffi-macro, 2026-10-02: a function-like
   macro and the function of the program's own that `ffi_macro_name` names
   as its repair. The function's prototype is the author's, written from the
   macro's documentation, and clang holds the declaration to it. */
#include <stdint.h>

#define SQUARE(x) ((x) * (x))

static inline int32_t hero_SQUARE(int32_t x) { return SQUARE(x); }
