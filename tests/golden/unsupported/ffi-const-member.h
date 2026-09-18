/* Beside the case, so no leg skips it for a missing package, and because no
   header this project binds has one: measured 2026-09-18 by a clang JSON AST
   walk over time.h, stdio.h, sqlite3.h, curl/curl.h, dirent.h and pwd.h —
   ZERO const non-pointer struct members. The shape is real in C and absent
   from this corpus, which is why it needed a case written for it rather than
   one found. */
#include <stdint.h>

typedef struct {
    const int32_t id;
    int32_t n;
} Locked;

static inline int32_t locked_id(Locked l) { return l.id; }
