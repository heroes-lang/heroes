/* Critic, panel 215 pass 2: a value probe, no sanitiser. dirty() fills the
   stack with 0xAA; pad_nonzero() counts the padding bytes of what it is lent
   that are not zero. */
#ifndef DV_H
#define DV_H
struct cd { char c; double d; };
__attribute__((noinline)) static void dv_dirty(void) {
    volatile unsigned char buf[4096];
    for (int i = 0; i < 4096; i++) buf[i] = 0xAA;
}
static inline void dirty(void) { dv_dirty(); }
static inline int pad_nonzero(const struct cd *p) {
    const unsigned char *b = (const unsigned char *)p;
    int n = 0;
    for (int i = 1; i < 8; i++) n += b[i] != 0;
    return n;
}
#endif
