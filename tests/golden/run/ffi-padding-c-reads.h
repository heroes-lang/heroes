/* Panel 215's R1 (issue 1334): records with padding, each lent to a C
   function that counts the padding bytes it reads that are not zero. The
   stack is filled with 0xAA first, so a byte nothing wrote reads nonzero
   wherever it survives: through a by-value call on Linux x86-64 at -O0
   (the sitting measured `true 0 7` there before the repair), and in a
   record C builds by members alone and returns by value, on every
   platform. Each count is C's own offsetof and sizeof. */
#include <stddef.h>
#include <stdint.h>

struct cd { char c; double d; };   /* padding inside, after c */
struct tp { double d; char c; };   /* padding at the tail */
struct ocd { struct cd inner; };   /* padding inside a member */

__attribute__((noinline)) static void pad_dirty(void) {
    volatile unsigned char buf[4096];
    for (int i = 0; i < 4096; i++) buf[i] = 0xAA;
}
static inline void dirty(void) { pad_dirty(); }

static inline int32_t nonzero(const unsigned char *b, size_t from, size_t to) {
    int32_t n = 0;
    for (size_t i = from; i < to; i++) n += b[i] != 0;
    return n;
}
static inline int32_t cd_pad(const struct cd *p) { return nonzero((const unsigned char *)p, sizeof p->c, offsetof(struct cd, d)); }
static inline int32_t tp_pad(const struct tp *p) { return nonzero((const unsigned char *)p, offsetof(struct tp, c) + 1, sizeof *p); }
static inline int32_t ocd_pad(const struct ocd *p) { return cd_pad(&p->inner); }
static inline int32_t cds_pad(const struct cd *p) { return cd_pad(&p[0]) + cd_pad(&p[1]); }

/* Built member by member, its padding never written: what the stack held. */
__attribute__((noinline)) static struct cd cd_made(void) {
    struct cd r;
    r.c = 1;
    r.d = 2.0;
    return r;
}
