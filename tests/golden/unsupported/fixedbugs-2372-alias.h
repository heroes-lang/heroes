/* Defect 2372, lane b26-cli, 2026-11-09: a bound name the header makes a
   macro for a name nothing on the link line defines. */
int base2372_alias(void);
#define top2372_alias base2372_alias
