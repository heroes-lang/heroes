/* Defect 588, 2026-10-10: a function its header marks `unavailable`, beside one it does not. */
#include <stdint.h>
__attribute__((unavailable("gone for good"))) static inline int64_t gone(int64_t x) { return x; }
static inline int64_t kept(int64_t x) { return x; }
