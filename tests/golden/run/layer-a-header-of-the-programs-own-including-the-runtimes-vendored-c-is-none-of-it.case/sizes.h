#ifndef SIZES_H
#define SIZES_H
#include <stdint.h>
/* Includes the runtime's vendored Monocypher header for its declarations
   only; binds nothing of the layer and calls nothing of it. */
#include "vendor/monocypher/monocypher.h"
static inline int64_t sizes_answer(void) { return (int64_t)sizeof(crypto_aead_ctx); }
#endif
