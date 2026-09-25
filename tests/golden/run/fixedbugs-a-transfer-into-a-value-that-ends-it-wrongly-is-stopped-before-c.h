/* OpenSSL's `BIO_new_fp(stream, BIO_CLOSE)` shape: the wrapper keeps the
 * stream and ends it with `fclose` when the wrapper is freed. Over a `popen`
 * stream that is the wrong call, and the child is never waited for. */
#include <stdint.h>
typedef struct st { int64_t open; } st;
typedef struct wr { st *inner; } wr;
static inline __attribute__((noinline)) st *st_popen(void) { static st one; one.open = 1; return &one; }
static inline __attribute__((noinline)) void st_pclose(st *s) { s->open = 0; }
static inline __attribute__((noinline)) void st_fclose(st *s) { s->open = 0; }
static inline __attribute__((noinline)) wr *wr_new(st *s) { static wr one; one.inner = s; return &one; }
static inline __attribute__((noinline)) void wr_free(wr *w) { st_fclose(w->inner); w->inner = 0; }
