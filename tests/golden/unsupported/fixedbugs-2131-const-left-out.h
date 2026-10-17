/* Defect 2131: a `const` member a `partial` record leaves out. */
struct cm { const int a; int b; };
static inline struct cm cm_make(void) { struct cm v = { 1, 2 }; return v; }
