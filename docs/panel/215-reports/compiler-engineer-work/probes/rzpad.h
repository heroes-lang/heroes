/* Compiler-engineer, panel 215: records built with `rest: zero`, read WHOLE
   by C, padding included: write() hands every byte to the kernel (MSan checks
   a syscall's buffer) and `all` counts the nonzero ones. */
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
static inline int64_t all(const void *p, size_t n) { const unsigned char *b = p; int64_t k = 0; for (size_t i = 0; i < n; i++) k += b[i] != 0; return k; }
/* 4 bytes of padding after kind, then an anonymous union, a narrow arm named */
typedef struct { int32_t kind; union { char c; double d; }; } PU;
static inline int64_t pu_all(const PU *s) { (void)!write(1, s, sizeof *s); return all(s, sizeof *s); }
/* a partial record: the union is C's, never named */
typedef struct { int32_t kind; union { char c; double d; } u; int16_t t; } PP;
static inline int64_t pp_all(const PP *s) { (void)!write(1, s, sizeof *s); return all(s, sizeof *s); }
