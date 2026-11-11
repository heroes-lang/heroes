/* Defect 2467: a function, and a macro of its name that takes one fewer argument. */
int f2467(int a, int b);
#define f2467(a) f2467(a, 0)
