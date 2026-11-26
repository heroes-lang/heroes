/* Lane p261-beside, 2026-11-26: the case's own header declares zlib's
 * `zlibVersion` by pasting its name, so clang places the name in its own
 * buffer, `<scratch space>`, and this file is the one that expanded it; then
 * the platform's header declares it again. */
#define HERO_PASTED(stem, tail) const char *stem##tail(void);
HERO_PASTED(zlib, Version)
#include <zlib.h>
