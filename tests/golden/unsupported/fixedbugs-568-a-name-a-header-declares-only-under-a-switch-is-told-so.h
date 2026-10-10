/* Defect 568, 2026-10-10: a function declared only under a switch, as glibc's sched.h does. */
#include <stdint.h>
#ifdef _GNU_SOURCE
static inline int32_t hidden_cpu(void) { return 0; }
#endif
