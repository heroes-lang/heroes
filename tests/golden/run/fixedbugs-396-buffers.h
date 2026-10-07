#ifndef FIXEDBUGS_396_BUFFERS_H
#define FIXEDBUGS_396_BUFFERS_H

/* Defect 396's shapes on a header of the program's own (lane b13-land-buf,
   2026-10-07; panel 196's R4): C functions that fill a buffer whose extent no
   argument carries, or one a sibling carries, each writing exactly what its
   name says, so a twin of every OpenSSL and POSIX case runs on all three
   platforms. */
#include <stdint.h>
#include <string.h>

struct pair { int32_t a; int32_t b; };
struct cell { unsigned char m; unsigned char rest[7]; int64_t after; };

#define DIGEST_LEN 32

/* `SHA256_Final`'s shape: 32 bytes through `unsigned char *`, no count. */
static inline int digest32(unsigned char *md) { for (int i = 0; i < 32; i++) md[i] = (unsigned char)(i * 7 + 1); return 1; }
static inline int digest_const(unsigned char *md) { for (int i = 0; i < DIGEST_LEN; i++) md[i] = (unsigned char)(255 - i); return 1; }
/* `read`'s: `n` bytes through `void *`, the count beside it. */
static inline int64_t fill_void(void *buf, uint64_t n) { memset(buf, 0x41, (size_t)n); return (int64_t)n; }
/* One byte more than it is told: an extent past a page, overrun by one. */
static inline int64_t fill_one_more(void *buf, uint64_t n) { memset(buf, 0x41, (size_t)n + 1); return (int64_t)n; }
/* `fgets`'s shape with the bytes a canary would hold: `n` of 0xA5. */
static inline void fill_a5(char *s, int32_t n) { memset(s, 0xA5, (size_t)n); }
/* `getsockopt`'s shape: the count read through a pointer, `*n` bytes. */
static inline int64_t fill_cell(void *buf, int32_t *n) { memset(buf, 0x43, (size_t)*n); return (int64_t)*n; }
/* Four `int32_t`s, two records, three doubles. */
static inline void fill_ints4(int32_t *p) { for (int k = 0; k < 4; k++) p[k] = -(k + 1); }
static inline void fill_pairs2(struct pair *p) { p[0].a = 1; p[0].b = 2; p[1].a = 3; p[1].b = 4; }
static inline void fill_reals3(double *p) { p[0] = 0.5; p[1] = 1.5; p[2] = 2.5; }
/* Reads what it is handed and writes it back doubled: the copy in. */
static inline void double_bytes(unsigned char *p) { for (int k = 0; k < 4; k++) p[k] = (unsigned char)(p[k] * 2); }
/* Calls back into the program, which lends again, then writes its own 16. */
static inline void fill_after(unsigned char *md, void (*cb)(void)) { cb(); memset(md, 0x42, 16); }
/* A seeded 32 bytes, slow on purpose, for two threads at once. */
static inline void stamp32(unsigned char *md, uint64_t seed) {
    for (int round = 0; round < 64; round++) {
        for (int i = 0; i < 32; i++) md[i] = (unsigned char)((seed * 31u + (uint64_t)i * 17u + (uint64_t)round) & 0xff);
    }
}
/* The defect's own field shape, kept inside its record: 16 bytes through a
   one-byte cell, over the seven beside it and the field after them. */
static inline int digest_cell(unsigned char *md) { memset(md, 0x41, 16); return 1; }

#endif
