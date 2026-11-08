/* Defect 2333: a group's header defining, as macros, the three overflow
   builtins the compiler's own C writes for every `i64` sum, difference and
   product. Each is given back after the group, so the arithmetic aborts. */
#define HERO_2333_ONE 1
#define __builtin_add_overflow(a, b, r) ((*(r) = (a) + (b)), 0)
#define __builtin_sub_overflow(a, b, r) ((*(r) = (a) - (b)), 0)
#define __builtin_mul_overflow(a, b, r) ((*(r) = (a) * (b)), 0)
