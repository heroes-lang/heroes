/* Panel 215, the spec-warden's union shapes, each beside A2's `struct tagged`. */
#include <stddef.h>
#include <stdint.h>
static inline void dirty(void) { volatile unsigned char b[4096]; for (int k = 0; k < 4096; k++) b[k] = 0xAA; }
static inline int64_t nz(const void *p, size_t n) { const unsigned char *b = p; int64_t k = 0; for (size_t i = 0; i < n; i++) k += b[i] != 0; return k; }

/* 1: an anonymous C11 union, its members the struct's own */
struct s1 { int kind; union { int fd; void *handle; }; };
static inline int64_t s1_nz(const struct s1 *t) { return nz(t, sizeof *t); }
static inline int s1_fd(const struct s1 *t) { return t->fd; }

/* 3: a named member of a TAGGED union */
struct s3 { int kind; union v3 { int fd; void *handle; } via; };
static inline int64_t s3_nz(const struct s3 *t) { return nz(t, sizeof *t); }
static inline int s3_fd(const struct s3 *t) { return t->via.fd; }

/* 4: a typedef'd union, alone and as a named member */
typedef union { int fd; void *handle; } VU;
struct s4 { int kind; VU via; };
static inline int64_t s4_nz(const struct s4 *t) { return nz(t, sizeof *t); }
static inline int s4_fd(const struct s4 *t) { return t->via.fd; }
static inline int64_t vu_nz(const VU *u) { return nz(u, sizeof *u); }

/* 5: an anonymous struct inside an anonymous union */
struct s5 { int kind; union { struct { int16_t lo; int16_t hi; }; int32_t i; }; };
static inline int64_t s5_nz(const struct s5 *t) { return nz(t, sizeof *t); }
static inline int32_t s5_i(const struct s5 *t) { return t->i; }

/* 6, beside 5: a NAMED struct inside an anonymous union */
struct s6 { int kind; union { struct { int16_t lo; int16_t hi; } parts; int32_t i; }; };
static inline int64_t s6_nz(const struct s6 *t) { return nz(t, sizeof *t); }
static inline int16_t s6_hi(const struct s6 *t) { return t->parts.hi; }
