/* Beside defect 151's cases, lane land186, 2026-10-02 (panel 186): a struct
   holding an anonymous union, the shapes beside it, and a named union a
   header macro reaches, so no case leans on a system header. */
#include <stdint.h>

/* the defect's own struct */
typedef struct { int32_t kind; union { int32_t i; float f; }; int32_t x; } SA;

/* three members, for one left out between two the record names */
typedef struct { int32_t a; int32_t b; int32_t c; } S3;

/* an anonymous union inside an anonymous struct */
typedef struct { int32_t kind; struct { union { int32_t i; float f; }; int32_t y; }; int32_t x; } DEEP;

/* a named union a header macro reaches, as libc's `sa_handler` is */
typedef struct { int32_t kind; union { uint8_t small; uint64_t big; } u; } NUW;
#define nu_small u.small
#define nu_big u.big

/* Compared (panel 186 R3): an integer arm narrower than its union, the
   critic's `SB`; a float arm as wide as its union, which compares -0.0 equal
   to 0.0; a record arm a macro reaches, which compares its fields and never
   its padding; an array arm short of its union's width by the union's own
   alignment; and a union type compared by an arm narrower than it. */
typedef struct { int32_t kind; union { int8_t b; int64_t q; }; } SB;
typedef struct { int32_t kind; union { int32_t i; float f; }; } SZ;
typedef struct { int8_t a; int32_t b; } Inner;
typedef struct { int32_t kind; union { Inner s; int64_t q; } u; } SP;
#define sp_s u.s
typedef struct { int32_t kind; union { int8_t c[5]; int32_t i; }; } SPAD;
typedef union { int8_t b; int64_t q; } UB;

/* The union's own width, which no formula over its members knows: an
   `aligned` union wider than its one member. */
typedef struct { int32_t kind; union __attribute__((aligned(16))) { int64_t q; }; } AL;
