/* Defect 584, 2026-10-10: fortification off, so the program's call of
   `sprintf` names the C library's own declaration, which this Mac's SDK
   marks deprecated, rather than the fortified macro.

   Since 2026-11-10 (panel 243's R1, defect 2421) a binding declares no
   parameter past `sprintf`'s `...`, so the call is this header's door,
   `print_number`, a function of a new name with a fixed prototype whose
   literal format clang reads against `n`. The call now stands in a header's
   own code, where clang says a deprecation: panel 208's R1 quiets it over
   the unit after the groups' close and widened nothing into their region.
   So the door quiets its own call, a header's choice about its own C. */
#define _FORTIFY_SOURCE 0
#include <stdio.h>
#include <stdlib.h>

static inline int print_number(char *buf, long long n) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    int written = sprintf(buf, "%lld", n);
#pragma clang diagnostic pop
    return written;
}
