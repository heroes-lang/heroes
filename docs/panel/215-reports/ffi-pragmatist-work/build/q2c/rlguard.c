#include "rl.h"
/* the lock unit's guarded form: ask Windows' locks only where winnt.h is already in */
#if defined(_WINNT_)
_Static_assert(0, "winnt.h was brought by the program's header");
#endif
int main(void) { return 0; }
