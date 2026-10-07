#ifndef FIXEDBUGS_413_KEEPERS_H
#define FIXEDBUGS_413_KEEPERS_H

/* Defect 413's shape on a header of the program's own (lane b13-land-addr,
   2026-10-07): a library that knows a record by its address, as zlib knows
   its stream (`deflateStateCheck` compares `strm->state->strm` with the
   pointer it is handed) and libuv its handles. `keep_init` records the
   address it is handed; every later call refuses, -2, a record that is not
   the one it initialised. Every platform has this header, so the shape runs
   on every platform where zlib's and libuv's cases run only where they are. */
#include <stdint.h>

struct keeper { void *self; int64_t uses; };
struct wrap { int64_t before; struct keeper k; };
struct pair { struct keeper k[2]; int64_t after; };

static inline int32_t keep_init(struct keeper *k) { k->self = k; k->uses = 0; return 0; }
static inline int32_t keep_use(struct keeper *k) { if (k->self != (void *)k) return -2; k->uses += 1; return 0; }

#endif
