/* Defect 2109: header values C leaves undefined, computed in an initialiser
   clang folds, so no check of the accessor's own code sees them. */
#include <limits.h>
enum { E_SHIFT = 1 << 31 };
static const int S_DIV = INT_MIN / -1;
struct pair { int x; int y; };
#define R_BRACE { INT_MIN / -1, 2 }
enum { EA = 1 << 31, EB = EA + 1 };
enum T { TA = 2147483647, TB = TA + 1, TBIG = 0x100000000LL };
enum { IA = 1 << 31, IB, IC };
static const int folded_k = INT_MIN / -1;
#define M_OBJ (folded_k)
static const struct pair R_OBJ = { INT_MIN / -1, 3 };
#define M_ROBJ (R_OBJ)
