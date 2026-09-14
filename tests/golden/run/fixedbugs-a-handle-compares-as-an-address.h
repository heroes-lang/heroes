/* Two distinct non-null handles and a null one, from a type this header names
 * and never sizes — so the case can ask the three questions that tell address
 * comparison apart from a constant answer.
 *
 * The header is shipped beside the case rather than borrowed from the system:
 * the defect was found against `sqlite3.h`, and a case that needs a library
 * installed fires on one platform of three. */
#include <stdint.h>

typedef struct Chunk Chunk;

struct Chunk {
    int64_t tag;
};

/* A pool with static storage, so the two addresses differ, are stable across
 * runs, and are never freed — the golden compares handles and must not depend
 * on what an allocator happens to hand back.
 *
 * `static inline` and not `static`: this header reaches a second translation
 * unit that calls none of them, and C11 exempts an unused inline where it would
 * warn on an unused static. The `warnings` suite holds this directory to zero. */
static inline Chunk *chunk_at(int64_t i) {
    static struct Chunk pool[2] = { { 10 }, { 20 } };
    return &pool[i];
}

static inline Chunk *chunk_none(void) { return 0; }

static inline int64_t chunk_tag(Chunk *c) { return c->tag; }
