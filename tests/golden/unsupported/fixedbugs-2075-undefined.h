/* Defect 2075: header constants whose values C leaves undefined and clang
   folds with no warning at all. */
#include <limits.h>
#define K_DIV (INT_MIN / -1)
#define K_FLT ((int)(1e20))
#define K_REM (INT_MIN % -1)
