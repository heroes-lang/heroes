#ifndef FIXEDBUGS_092_COUNTED_BY_H
#define FIXEDBUGS_092_COUNTED_BY_H

/* Where `counted_by` stands on a record lent whole (lane b13-unit,
   2026-10-06): the declarations of the `check/` case beside it. */
#include <stdint.h>
#include <string.h>

struct one { int32_t a; };
struct opaque;

static inline int64_t by_value_count(void *buf, uint64_t n) { memset(buf, 0, (size_t)n); return (int64_t)n; }
static inline int64_t cell_count(void *buf, uint32_t *n) { memset(buf, 0, (size_t)*n); return (int64_t)*n; }
static inline int64_t not_lent(struct one buf, uint64_t n) { return (int64_t)buf.a + (int64_t)n; }
static inline int64_t a_handle(struct opaque **h, uint64_t n) { (void)h; return (int64_t)n; }
static inline int64_t a_float(void *buf, double *n) { (void)buf; return (int64_t)*n; }
static inline int64_t no_sibling(void *buf, uint64_t n) { (void)buf; return (int64_t)n; }

#endif
