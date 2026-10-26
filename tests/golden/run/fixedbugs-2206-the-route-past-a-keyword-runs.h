/* A library's two symbols, spelled with keywords of Heroes... */
static inline int match(int x) { return x + 1; }
enum { test = 7 };

/* ...and the program's own second names for them, the route `expected_name`
   names (defect 2206): a caller for the function, a macro for the constant. */
static inline int kw_match(int x) { return match(x); }
#define kw_test test
