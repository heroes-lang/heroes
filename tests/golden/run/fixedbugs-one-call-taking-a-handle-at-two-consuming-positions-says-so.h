/* OpenSSL's `SSL_set_bio(s, b, b)` shape: when both BIOs are the same, the
 * library takes ONE reference, and `SSL_free` gives it back once. The call
 * prints, so the golden proves the program is stopped BEFORE C runs. */
#include <stdint.h>
#include <stdio.h>
typedef struct bio { int64_t refs; } bio;
typedef struct ssl { bio *r; bio *w; } ssl;
static inline __attribute__((noinline)) bio *bio_new(void) { static bio one; one.refs = 1; return &one; }
static inline __attribute__((noinline)) void bio_free(bio *b) { b->refs--; }
static inline __attribute__((noinline)) ssl *ssl_new(void) { static ssl one; one.r = 0; one.w = 0; return &one; }
static inline __attribute__((noinline)) void ssl_set_bio(ssl *s, bio *r, bio *w) { puts("C ran"); s->r = r; s->w = w; }
static inline __attribute__((noinline)) void ssl_free(ssl *s) { if (s->r) bio_free(s->r); if (s->w && s->w != s->r) bio_free(s->w); s->r = 0; s->w = 0; }
