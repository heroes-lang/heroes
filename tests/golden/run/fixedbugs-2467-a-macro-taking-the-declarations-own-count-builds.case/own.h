/* Defect 2467: a function, and a macro of its name that takes the same two arguments. */
static inline int f2467(int a, int b) { return a + b; }
#define f2467(a, b) f2467(a, b)
