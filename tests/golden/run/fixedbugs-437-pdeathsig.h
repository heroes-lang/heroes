/* Defect 437's reading of PR_GET_PDEATHSIG, through a prototype of the
   case's own: glibc declares `int prctl(int option, ...)`, and an `@` lend
   past a `...` is refused, since nothing holds what C writes there to the
   width the program lends (defect 2106). Here the pointer crosses at the
   type PR_GET_PDEATHSIG writes, `int *`, which the binding is held to. */
#include <stdint.h>
#include <sys/prctl.h>

static inline int32_t case437_pdeathsig(int32_t *signal) {
    return (int32_t)prctl(PR_GET_PDEATHSIG, signal);
}
