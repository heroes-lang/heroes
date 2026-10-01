/* Beside the cases of defect 145, lane emit, 2026-10-01: every way a header
   can name a struct, one of each, so each case can name one wrongly. Beside
   its case rather than a system header, so no platform lacks it. */
#include <stdint.h>

/* a typedef of an anonymous struct: the shape of regex_t, div_t, ldiv_t */
typedef struct { int32_t a; int32_t b; } anon_s;

/* a typedef of an anonymous union */
typedef union { int32_t i; float f; } anon_u;

/* a typedef of a tagged struct: two names for one type */
typedef struct tagged_s { int32_t a; } tagged_t;

/* a typedef of a struct the header keeps opaque, as sqlite3.h does */
typedef struct opaque_s opaque_t;

/* a typedef of void, as curl.h writes typedef void CURL; */
typedef void void_t;

/* a struct tag with no typedef */
struct only_tag { int32_t a; };

static inline int32_t use_anon_s(anon_s *p) { return p->a; }
static inline int32_t use_anon_u(anon_u *p) { return p->i; }
static inline int32_t use_tagged(tagged_t *p) { return p->a; }
static inline anon_s give_anon_s(void) { anon_s v = {1, 2}; return v; }
