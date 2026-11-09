/* Defect 2372, lane b26-cli, 2026-11-09: the bound `static inline` body
   reaches another, which calls the name nothing on the link line defines. */
int base2372_deep(void);
static inline int mid2372(void) { return base2372_deep() * 2; }
static inline int top2372_mid(void) { return mid2372() + 1; }
