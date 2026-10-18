/* Defect 2128's header: one value the region refuses at its accessor, and
   two only the probe sees, which clang folds with no warning. */
#define K_SHIFT (1 << 40)
#define K_DIV ((-2147483647 - 1) / -1)
#define K_FLT ((int)1e20)
