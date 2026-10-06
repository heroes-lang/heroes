/* Panel 194, compiler-engineer: a C function that counts in ints, not bytes. */
struct held { unsigned char buf[16]; long after; };
static inline void fill_ints(int *a, unsigned long n) { for (unsigned long k = 0; k < n; k++) a[k] = 0x41414141; }
