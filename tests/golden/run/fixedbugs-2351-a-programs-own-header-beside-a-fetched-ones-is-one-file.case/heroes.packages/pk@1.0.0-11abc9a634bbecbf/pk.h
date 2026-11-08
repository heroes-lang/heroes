/* The package `pk`'s own header (defect 2351). */
#include <stdint.h>
#define PK_LIMIT ((1LL << 40) + 3)
static inline int64_t pk_twice(int64_t x) { return x * 2; }
