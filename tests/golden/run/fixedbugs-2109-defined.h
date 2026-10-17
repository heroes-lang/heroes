/* Header values C defines, computed in initialisers clang folds: each
   must build and print C's value once their initialisers are asked. */
#include <limits.h>
#include <stddef.h>
static int g_var = 3;
enum { E_A = 5, E_B = E_A * 2, E_NEXT };
struct pt { int x; int y; };
enum { E_SIZE = sizeof(struct pt), E_OFF = offsetof(struct pt, y) };
enum { E_30 = 1 << 30, E_FDIV = (int)(1000.0 / 60) };
static const int S_FROME = E_B + 1;
static const unsigned int S_WRAP = 0u - 1;
static const int S_NEGOK = -2147483647 - 1;
static const double S_FOK = 1000.0 / 60;
static const int S_BCPV = __builtin_constant_p(g_var) ? (INT_MIN / -1) : 5;
#define R_OK { E_B, 2 }
static const struct pt R_OBJ_OK = { E_30, -1 };
#define M_ROBJ_OK (R_OBJ_OK)
