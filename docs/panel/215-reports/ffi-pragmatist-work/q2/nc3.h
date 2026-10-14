/* ffi-pragmatist, panel 215: C11 <threads.h>'s mtx_t, held in a struct of the program's own. */
#include <threads.h>
struct mtxbox { mtx_t m; int tag; };
static inline int mtx_box_init(struct mtxbox *b) { return mtx_init(&b->m, mtx_plain); }
static inline int mtx_box_lock(struct mtxbox *b) { return mtx_lock(&b->m); }
static inline int mtx_box_trylock(struct mtxbox *b) { return mtx_trylock(&b->m); }
static inline int mtx_box_unlock(struct mtxbox *b) { return mtx_unlock(&b->m); }
