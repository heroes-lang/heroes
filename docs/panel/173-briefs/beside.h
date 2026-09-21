/* The C side of panel 172's three shapes beside the filed reproducer, written
 * down from `docs/panel/172-reports/completeness-critic.md` § 4 so the programs
 * beside this file build. Every function frees a lease by a route that has no
 * pointer parameter to mark at the freeing call. */
#include <stdlib.h>
#include <stdint.h>

static const char *stashed;
static inline void stash(const char *s) { stashed = s; }
static inline void later_free(void) { free((void *)(uintptr_t)stashed); }

static inline void eat(const char *s) { free((void *)(uintptr_t)s); }
static inline void take_cb(const char *s, void (*cb)(const char *)) { cb(s); }

static inline void fill_out(const char **out) { *out = (const char *)malloc(8); }
static inline void free_out(const char *p) { free((void *)(uintptr_t)p); }
