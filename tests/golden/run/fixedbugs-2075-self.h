/* Macros that name themselves: each expands once, as C expands them. */
enum { SELF_W = 2147483646 };
#define SELF_W (SELF_W + 1)
enum { SELF_V = 5 };
#define SELF_V (SELF_V * 2)
