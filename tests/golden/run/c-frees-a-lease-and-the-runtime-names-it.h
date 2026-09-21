/* The C side of defect 070's filed reproducer: a function that frees what it
 * is handed. The lease's bytes sit past a leading header, so this `free` is of
 * an interior pointer and the allocator kills the process inside the call. */
#include <stdlib.h>
#include <stdint.h>
static inline void eat(const char *s) { free((void *)(uintptr_t)s); }
