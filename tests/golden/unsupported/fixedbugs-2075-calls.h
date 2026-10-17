/* Record constants whose leaf is a call: one that divides INT_MIN by -1,
   which C leaves undefined, and runs only when the accessor is called. */
#include <limits.h>
struct pair { int x; int y; };
static inline int calls_divmin(void) { volatile int a = INT_MIN; int b = -1; return a / b; }
#define C_PAIR { calls_divmin(), 2 }
