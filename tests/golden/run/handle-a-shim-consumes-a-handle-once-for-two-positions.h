/* The shim panel 176 asks for over `SSL_set_bio(s, b, b)`: one C line that
 * takes the handle once, so the binding has one consuming position. */
#include <stdint.h>
typedef struct bio { int64_t refs; } bio;
typedef struct ssl { bio *r; bio *w; } ssl;
static inline __attribute__((noinline)) bio *bio_new(void) { static bio one; one.refs = 1; return &one; }
static inline __attribute__((noinline)) void bio_free(bio *b) { b->refs--; }
static inline __attribute__((noinline)) int64_t bio_refs(bio *b) { return b->refs; }
static inline __attribute__((noinline)) ssl *ssl_new(void) { static ssl one; one.r = 0; one.w = 0; return &one; }
static inline __attribute__((noinline)) void ssl_set_bio(ssl *s, bio *r, bio *w) { s->r = r; s->w = w; }
static inline __attribute__((noinline)) void ssl_free(ssl *s) { if (s->r) bio_free(s->r); if (s->w && s->w != s->r) bio_free(s->w); s->r = 0; s->w = 0; }
static inline __attribute__((noinline)) void ssl_set_one_bio(ssl *s, bio *b) { ssl_set_bio(s, b, b); }
