/* Panel 215's R1 (issue 1334): a group record whose C type IS a union, and
   a struct holding a named union, each bound naming only the narrow arm. C
   writes the wide arm; the record is lent to C again, which reads it back.
   Zeroing what the declared arm does not hold would zero the arm C wrote:
   the sitting's first build of R1 did, `0.0 1432778632` at exit 0. */
#include <stdint.h>

typedef union { char c; double d; } UN;
typedef union v { int32_t fd; void *h; } V;
struct nu { int32_t kind; V via; };

static inline void un_set(UN *p) { p->d = 2.5; }
static inline double un_get(const UN *p) { return p->d; }
static inline void nu_set(struct nu *p) { p->via.h = (void *)(uintptr_t)0x1122334455667788ull; }
static inline int64_t nu_get(const struct nu *p) { return (int64_t)(uintptr_t)p->via.h; }
