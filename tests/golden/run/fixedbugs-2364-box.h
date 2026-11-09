typedef union Un {
    int i;
#ifdef FIXEDBUGS_2364_WIDE
    long long wide;
#endif
} Un;
typedef struct Box { Un u; int t; } Box;
static inline Box box_make(int v) { Box b; b.u.i = v; b.t = 1000; return b; }
