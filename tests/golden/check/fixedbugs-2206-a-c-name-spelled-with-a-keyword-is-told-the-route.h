/* Defect 2206's case: a C function and a C constant spelled with keywords of
   Heroes, as a library's header can spell them. */
static inline int match(int x) { return x + 1; }
enum { test = 7 };
