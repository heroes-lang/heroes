/* fixedbugs 2230: a parameter and two fields at the header's own width. */
struct pair { int a; int b; };
static inline int twice(int x) { return 2 * x; }
