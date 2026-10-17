/* A header whose own code ends the process before main, as a constructor may. */
#include <stdlib.h>
__attribute__((constructor)) static void ender_starts(void) { _Exit(3); }
#define E_VALUE (1 << 3)
