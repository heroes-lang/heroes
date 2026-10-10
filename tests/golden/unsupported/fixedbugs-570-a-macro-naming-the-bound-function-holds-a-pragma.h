/* Defect 570, 2026-10-10: a macro naming the bound function that holds a pragma. */
#include <stdlib.h>
static inline int my_abs(int x) { return x < 0 ? -x : x; }
#define my_abs _Pragma("clang diagnostic ignored \"-Wsign-conversion\"") my_abs
