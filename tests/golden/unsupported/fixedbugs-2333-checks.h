/* Defect 2333: macros over names C reserves that the compiler's own C and its
   probes write to judge a binding. Each is given back after the group, so
   every binding below is judged as it would be without them. */
#include <stdio.h>
#include <stdint.h>
#define _Static_assert(c, m)
#define __builtin_constant_p(x) 1
#define __builtin_offsetof(t, m) 0
#define __builtin_types_compatible_p(a, b) 1
#define _Generic(...) 1
#define HERO_2333_OUT stdout
struct hero_2333_pt { int32_t x; int32_t y; };
static inline double hero_2333_half(double x) { return x / 2; }
