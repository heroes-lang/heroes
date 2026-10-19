/* Defect 2145's header (panel 225's compiler-engineer): a constant the
   region refuses at its own accessor whose value counts, and one after it
   whose value reads the count. In the program's own unit K_BAD's accessor
   expands it three times before K_CTR's, so K_CTR reads 5 and is 0. */
#define K_BAD (__COUNTER__ * 0 + (1 << 40))
#define K_CTR (__COUNTER__ < 3 ? (-2147483647 - 1) / -1 : 0)
