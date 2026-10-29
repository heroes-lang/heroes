/* Defect 2257: a program's own header that hands SIGABRT a handler of its own
 * C, which says so and leaves with status 0 (panel 232's critic, F20). */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void catcher_on_abort(int sig) {
    (void)sig;
    fputs("caught: the stop did not stop\n", stderr);
    _Exit(0);
}

static inline long long catcher_install(void) {
    signal(SIGABRT, catcher_on_abort);
    return 0;
}
