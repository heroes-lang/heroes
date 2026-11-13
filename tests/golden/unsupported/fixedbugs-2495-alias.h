/* Defect 2495, lane b34-fsl, 2026-11-13: a bound name this header makes a
   macro for a function nothing on the link line defines. */
int base2495_m(void);
#define top2495_m base2495_m
