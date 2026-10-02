/* Panel 186 R7, lane land186, 2026-10-02: a record over a C union is built
   naming exactly one member of each union, an anonymous struct's fields
   counting as one, and every field outside them. The shapes a construction
   is judged on, so no case leans on a system header. */
#include <stdint.h>

/* the blind readings' struct: an anonymous union between two fields */
typedef struct { int32_t kind; union { int32_t i; float f; }; int32_t x; } SA;
static inline SA make_sa(void) { SA s; s.kind = 1; s.i = 12; s.x = 3; return s; }
static inline float sa_f(SA s) { return s.f; }
static inline int32_t sa_x(SA s) { return s.x; }

/* an anonymous union whose other member is an anonymous struct */
typedef struct { int32_t kind; union { int32_t i; struct { int16_t lo; int16_t hi; }; }; } SB;
static inline int32_t sb_i(SB s) { return s.i; }
static inline int32_t sb_kind(SB s) { return s.kind; }

/* a union type */
typedef union { int32_t i; float f; } UT;
static inline int32_t ut_i(UT u) { return u.i; }

/* structs with no union at all */
typedef struct { int32_t x; int32_t y; } PT;
typedef struct { int32_t x; int32_t y; int32_t z; } PT3;
static inline int32_t pt_y(PT p) { return p.y; }
static inline int32_t pt3_z(PT3 p) { return p.z; }
