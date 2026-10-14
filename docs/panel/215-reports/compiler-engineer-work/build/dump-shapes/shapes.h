#include <stdint.h>
struct cd { char c; double d; };
struct ocd { struct cd inner; };
struct arr { struct cd a[2]; int32_t n; };
struct arr2 { struct cd m[2][3]; char t; };
struct nu { int32_t kind; union { int32_t fd; void *h; } via; int32_t tail; };
struct au { int32_t kind; union { int32_t fd; void *h; }; char z; };
struct bf { char a; unsigned b : 3; unsigned : 2; unsigned c : 4; double d; };
struct as { char k; struct { char x; int64_t y; }; };
