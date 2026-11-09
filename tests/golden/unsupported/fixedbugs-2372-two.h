/* Defect 2372, lane b26-cli, 2026-11-09: two `static inline` bodies, each
   calling a name nothing on the link line defines. */
int base2372_one(void);
int base2372_two(void);
static inline int top2372_one(void) { return base2372_one() + 1; }
static inline int top2372_two(void) { return base2372_two() + 2; }
