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
