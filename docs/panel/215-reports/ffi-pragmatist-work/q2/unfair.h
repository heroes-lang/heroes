/* ffi-pragmatist, panel 215: Darwin's os_unfair_lock, whose header says a bytewise copy of an
   UNLOCKED one is a new lock (os/lock.h:59-63), held in a struct of the program's own. */
#include <os/lock.h>
struct ufbox { os_unfair_lock l; int tag; };
static inline void uf_init(struct ufbox *b) { b->l = OS_UNFAIR_LOCK_INIT; }
static inline void uf_lock(struct ufbox *b) { os_unfair_lock_lock(&b->l); }
static inline int uf_trylock(struct ufbox *b) { return os_unfair_lock_trylock(&b->l) ? 1 : 0; }
static inline void uf_unlock(struct ufbox *b) { os_unfair_lock_unlock(&b->l); }
