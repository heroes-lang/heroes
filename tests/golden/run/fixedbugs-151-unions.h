/* Beside defect 150's and 151's run cases, lane land186, 2026-10-02 (panel
   186): the unions the header's layout now reads, each with a maker, so no
   case leans on a system header. */
#include <stdint.h>
#include <string.h>

/* a union read through two of its members (defect 150) */
typedef union { int32_t i; uint32_t n; } W;
static inline W make_w(void) { W w; w.i = 7; return w; }

/* a struct holding an anonymous union, bound by one arm (defect 151) */
typedef struct { int32_t kind; union { int32_t i; float f; }; int32_t x; } SA;
static inline SA make_sa(int32_t i) { SA s; memset(&s, 0, sizeof s); s.kind = 1; s.i = i; s.x = 3; return s; }

/* members with no bytes, a GNU empty struct and a zero-length array */
struct hero_empty { };
typedef struct { struct hero_empty e; int32_t z[0]; int32_t x; } ZE;
static inline ZE make_ze(void) { ZE v; memset(&v, 0, sizeof v); v.x = 4; return v; }

/* a union holding an anonymous struct, bound by its wide arm */
typedef struct { int32_t kind; union { struct { int32_t a; int32_t b; }; int64_t q; }; } UAS;
static inline UAS make_uas(void) { UAS v; memset(&v, 0, sizeof v); v.kind = 2; v.a = 4; v.b = 5; return v; }
