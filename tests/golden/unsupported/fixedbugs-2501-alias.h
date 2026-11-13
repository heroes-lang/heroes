/* Defect 2501, lane b34-fsl, 2026-11-13: a bound name this header makes a
   macro for a function nothing on the link line defines. */
int base2501(void);
#define top2501 base2501
