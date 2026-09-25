typedef struct h { int open; } h;
static inline __attribute__((noinline)) h *h_open(void) { static h one; one.open = 1; return &one; }
static inline __attribute__((noinline)) void h_close2(h *p) { p->open = 0; }
