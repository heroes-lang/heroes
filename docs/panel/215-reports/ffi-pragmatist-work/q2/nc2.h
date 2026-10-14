/* ffi-pragmatist, panel 215: more types C or a platform says must not be copied, each held in a
   struct of the program's own where the type itself has no portable field to name. */
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
struct condbox { pthread_cond_t cv; int tag; };
struct atombox { atomic_int n; int tag; };
static inline int cond_init(struct condbox *b) { return pthread_cond_init(&b->cv, NULL); }
static inline int cond_signal(struct condbox *b) { return pthread_cond_signal(&b->cv); }
static inline int cond_destroy(struct condbox *b) { return pthread_cond_destroy(&b->cv); }
static inline int atom_add(struct atombox *b) { return atomic_fetch_add(&b->n, 1) + 1; }
