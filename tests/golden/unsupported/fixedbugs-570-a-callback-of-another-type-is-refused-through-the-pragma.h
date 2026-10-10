/* Defect 570, 2026-10-10: a callback of another type behind an ignored pragma. */
#pragma clang diagnostic ignored "-Wincompatible-function-pointer-types"
static inline int run_cb(void (*f)(void)) { f(); return 1; }
