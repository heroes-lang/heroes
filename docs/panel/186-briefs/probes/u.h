#include <stdint.h>
typedef union { int32_t i; uint32_t n; } W2;
typedef union { int32_t i; uint32_t n; float f; } W3;
typedef union { int32_t i; } W1;
typedef struct { int32_t a; int32_t b; } P;
typedef union { P p; int32_t i; } WP;
typedef union { int32_t i; P p; } WI;
typedef struct { int32_t kind; union { int32_t i; float f; }; int32_t x; } SA;
typedef struct { int32_t kind; W2 u; } SN;
static inline W2 make_w2(void) { W2 w; w.i = 7; return w; }
static inline W3 make_w3(void) { W3 w; w.i = 8; return w; }
static inline W1 make_w1(void) { W1 w; w.i = 9; return w; }
static inline WP make_wp(void) { WP w; w.i = 10; return w; }
static inline WI make_wi(void) { WI w; w.i = 11; return w; }
static inline SA make_sa(void) { SA s; s.kind = 1; s.i = 12; s.x = 3; return s; }
static inline SN make_sn(void) { SN s; s.kind = 2; s.u.i = 13; return s; }
