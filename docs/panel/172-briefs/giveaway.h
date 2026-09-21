#include <stdlib.h>
#include <stdint.h>
static inline void eat(const char *s) { free((void *)(uintptr_t)s); }
static inline void eat_lent(const char *s) { free((void *)(uintptr_t)s); }
static inline void look(const char *s) { (void)s; }
static const char *keeper;
static inline void stash(const char *s) { keeper = s; }
