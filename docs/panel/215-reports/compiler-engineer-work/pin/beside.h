/* Compiler-engineer, panel 215: a struct holding the pinned one by value. */
#include "mx.h"
struct pair { struct box first; long k; };
static inline int pair_lock(struct pair *p) { return box_lock(&p->first); }
