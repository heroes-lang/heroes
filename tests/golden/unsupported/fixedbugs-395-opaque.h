#ifndef FIXEDBUGS_395_OPAQUE_H
#define FIXEDBUGS_395_OPAQUE_H

/* A count in a type the header keeps opaque (lane b13-unit, 2026-10-06): C
   counts `n` of `struct opaque`, whose size only C's own source knows, spelled
   three ways: plainly, through a typedef of the struct, and through a typedef
   of the pointer. */
#include <stdint.h>

struct opaque;
typedef struct opaque OP;
typedef struct opaque *LPOPAQUE;
struct held { unsigned char buf[16]; int64_t after; };

static inline int64_t count_plain(struct opaque *o, uint64_t n) { (void)o; return (int64_t)n; }
static inline int64_t count_named(OP *o, uint64_t n) { (void)o; return (int64_t)n; }
static inline int64_t count_hidden(LPOPAQUE o, uint64_t n) { (void)o; return (int64_t)n; }

#endif
