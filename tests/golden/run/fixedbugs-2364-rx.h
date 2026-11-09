typedef struct Rx {
    int a;
#ifdef FIXEDBUGS_2364_WIDE
    int extra;
#endif
    int b;
} Rx;
static inline Rx rx_make(int v) { Rx x = { .a = v, .b = 2 }; return x; }
static inline int rx_sum(Rx *r) { return r->a * 100 + r->b; }
