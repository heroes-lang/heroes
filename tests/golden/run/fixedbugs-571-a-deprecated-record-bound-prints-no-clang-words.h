/* Defect 571, 2026-10-10: a deprecated struct, bound as a record. */
#include <stdint.h>
struct __attribute__((deprecated)) old_pair { int64_t a; };
static inline int64_t one(void) { return 1; }
