/* ffi-pragmatist, panel 215: the atomic box and stdio, with no POSIX header (the Windows box). */
#include <stdatomic.h>
#include <stdio.h>
struct atombox { atomic_int n; int tag; };
static inline int atom_add(struct atombox *b) { return atomic_fetch_add(&b->n, 1) + 1; }
