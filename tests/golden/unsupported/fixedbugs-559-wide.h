/* Defect 559, lane b18-ffi, 2026-10-10: a macro another module's header
   reads, and `twice`, the name this module binds. */
#define WIDE 1
static inline long long twice(long long x) { return 2 * x; }
