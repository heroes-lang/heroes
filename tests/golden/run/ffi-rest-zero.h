/* Panel 194 R1, lane b13-bs, 2026-10-06: a construction of a group's record
   that ends with `rest: zero` makes every field sharing no byte with one it
   names zero, and a union none of whose members is named zero in all its
   bytes (spec § 13). Each helper counts, through the program's own lend, the
   bytes of the members it names that are not zero, never padding, of which
   nothing is promised. `dirty` fills the stack below `main` with 0xAA first,
   so the frame that builds the record holds no zero it did not write. */
#include <stdint.h>
#include <stddef.h>

static inline void dirty(void) { volatile unsigned char b[4096]; for (int k = 0; k < 4096; k++) b[k] = 0xAA; }
static inline int64_t nonzero(const void *p, size_t n) { const unsigned char *b = p; int64_t k = 0; for (size_t i = 0; i < n; i++) k += b[i] != 0; return k; }

/* two long arrays around a number */
struct big { char name[256]; int32_t n; char tail[100]; };
static inline int64_t big_arrays(struct big *b) { return nonzero(b->name, sizeof b->name) + nonzero(b->tail, sizeof b->tail); }

/* an anonymous union whose first member is narrower than its second */
typedef struct { int32_t kind; union { char c; double d; }; int32_t x; } SA;
static inline int64_t sa_union(SA *s) { return nonzero(&s->d, sizeof s->d); }
static inline int64_t sa_x(SA *s) { return nonzero(&s->x, sizeof s->x); }

/* a union type with the same two members */
typedef union { char c; double d; } UD;
static inline int64_t ud_all(UD *u) { return nonzero(u, sizeof *u); }

/* a group record holding another */
struct outer { int32_t k; SA inner; };
static inline int64_t outer_union(struct outer *o) { return nonzero(&o->inner.d, sizeof o->inner.d) + nonzero(&o->inner.x, sizeof o->inner.x); }

/* a named union a `partial` record leaves to C */
typedef struct { int32_t kind; union { char c; double d; } u; } SP;
static inline int64_t sp_union(SP *s) { return nonzero(&s->u, sizeof s->u); }
