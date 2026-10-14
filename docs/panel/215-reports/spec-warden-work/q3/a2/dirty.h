/* the A2 header, unchanged, plus a way to see every byte of the object */
#include <stddef.h>
#include <stdint.h>
#include "union.h"
static inline void dirty(void) { volatile unsigned char b[4096]; for (int k = 0; k < 4096; k++) b[k] = 0xAA; }
static inline int64_t tg_nonzero(const struct tagged *t) { const unsigned char *b = (const unsigned char *)t; int64_t k = 0; for (size_t i = 0; i < sizeof *t; i++) k += b[i] != 0; return k; }
static inline int64_t tg_via_nonzero(const struct tagged *t) { const unsigned char *b = (const unsigned char *)&t->via; int64_t k = 0; for (size_t i = 0; i < sizeof t->via; i++) k += b[i] != 0; return k; }
static inline int64_t tg_size(void) { return (int64_t)sizeof(struct tagged); }
