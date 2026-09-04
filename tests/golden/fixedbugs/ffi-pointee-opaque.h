/* Beside the case, so no leg skips it for a missing package: an opaque type
   of the kind every real library hands out (`sqlite3`, `FILE` on some libcs,
   `SDL_Window`), declared and never defined here. */
#include <stdint.h>
struct opaque;
static inline int32_t fill(struct opaque *out) { (void)out; return 0; }
