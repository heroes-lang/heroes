/* Defect 401, lane b13-zero401, 2026-10-06: a binding called `zero` beside
   the words `rest: zero` changes nothing they mean. `dirty` fills the stack
   below `main` with 0xAA first, so the frame that builds the record holds no
   zero it did not write. */
#include <stdint.h>

static inline void dirty(void) { volatile unsigned char b[4096]; for (int k = 0; k < 4096; k++) b[k] = 0xAA; }

struct pair { int32_t a; int32_t b; int32_t zero; };
