/* Defect 2372, lane b26-cli, 2026-11-09: a `static inline` body calling a
   name the header declares and nothing on the link line defines. */
int base2372_helper(void);
static inline int top2372(void) { return base2372_helper() + 1; }
