/* Defect 2109: header values C leaves undefined, computed in an initialiser
   clang folds, so no check of the accessor's own code sees them. */
#include <limits.h>
enum { E_SHIFT = 1 << 31 };
static const int S_DIV = INT_MIN / -1;
struct pair { int x; int y; };
#define R_BRACE { INT_MIN / -1, 2 }
enum { EA = 1 << 31, EB = EA + 1 };
/* clang warns of TB's overflow where the header is compiled, and a warning
   names the build's own unit file: kept quiet, the shape unchanged. */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winteger-overflow"
enum T { TA = 2147483647, TB = TA + 1, TBIG = 0x100000000LL };
#pragma clang diagnostic pop
enum { IA = 1 << 31, IB, IC };
static const int folded_k = INT_MIN / -1;
#define M_OBJ (folded_k)
static const struct pair R_OBJ = { INT_MIN / -1, 3 };
#define M_ROBJ (R_OBJ)
#define K_QCH ';' + EA
struct holder { int a[2]; int b; };
#define R_FILL { { 1 << 31 }, 2 }
struct ae { enum { AE_NEG = -1, AE0, AE1 } k; int y; };
#define R_AE { INT_MIN / -1, 2 }
/* AE_NEG makes the enumeration `int` on every platform: with no negative
   enumerator clang gives it `unsigned int` on Darwin and Linux and `int`
   for the MSVC target, and the case read two answers (defect 2125). */
