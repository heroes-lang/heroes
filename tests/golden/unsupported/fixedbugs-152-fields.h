/* Beside the field cases of defect 152's lane, 2026-10-02: every way a header
   names a struct or a union, one of each, so each case can name a field the
   type lacks. Beside its cases rather than a system header, so no platform
   lacks it. */
#include <stdint.h>

/* a typedef of an anonymous struct */
typedef struct { int32_t a; int32_t b; } anon_s;

/* a typedef of a tagged struct: two names for one type */
typedef struct tagged_s { int32_t a; int32_t b; } tagged_t;

/* a struct tag with no typedef */
struct only_tag { int32_t a; int32_t b; };

/* a typedef that repeats its struct's tag, raylib's `Color` shape */
typedef struct named_s { int32_t a; int32_t b; } named_s;

/* a typedef of an anonymous union */
typedef union { int32_t i; uint32_t n; } anon_u;
