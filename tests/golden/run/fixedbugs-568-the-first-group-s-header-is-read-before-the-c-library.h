/* Defect 568, 2026-10-10: a first group's header that must come before every header of the C library. */
#if defined(INT8_MAX) || defined(HUGE_VAL) || defined(EOF)
#error "a header of the C library was read before this one"
#endif
#define HERO_CASE_SWITCH 1
#include <stdint.h>
static inline int64_t answer(void) { return HERO_CASE_SWITCH * 42; }
