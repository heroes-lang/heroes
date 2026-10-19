/* Defect 2137's header (panel 225's compiler-engineer, `ctr`): a bound
   function behind an object-like macro whose every expansion counts, and a
   constant whose value reads the count; `K_DIV` is undefined everywhere. */
static inline int narrow_impl(int x) { return x; }
#define narrow (*(__COUNTER__, &narrow_impl))
#define K_CTR (__COUNTER__ < 3 ? (-2147483647 - 1) / -1 : 0)
#define K_DIV ((-2147483647 - 1) / -1)
