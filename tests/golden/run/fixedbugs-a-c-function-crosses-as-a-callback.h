/* Defect 069's C side: a library that takes a buffer AND the function that
 * disposes of it, which is how every real C library expresses retention —
 * sqlite3_bind_blob's fifth argument, curl_easy_setopt's write callbacks.
 *
 * The counter is what makes the case an assertion rather than a smoke test: a
 * callback that is never called leaves it at zero, and a callback whose address
 * was a garbage temporary does not reach here at all. */
#include <stdint.h>
#include <stdlib.h>

typedef void (*blob_free_t)(void *);

static int64_t g_frees = 0;
static int64_t g_first = 0;

static inline void *blob_make(void) {
    unsigned char *b = (unsigned char *)malloc(4);
    b[0] = 7;
    b[1] = 8;
    b[2] = 9;
    b[3] = 10;
    return b;
}

/* Reads the bytes, then hands them to the disposer the caller named. */
static inline void blob_take(const void *p, int64_t n, blob_free_t d) {
    const unsigned char *b = (const unsigned char *)p;
    g_first = n > 0 ? (int64_t)b[0] : -1;
    d((void *)(uintptr_t)p);
}

static inline void counting_free(void *p) {
    g_frees += 1;
    free(p);
}

static inline int64_t blob_first(void) { return g_first; }
static inline int64_t blob_frees(void) { return g_frees; }
