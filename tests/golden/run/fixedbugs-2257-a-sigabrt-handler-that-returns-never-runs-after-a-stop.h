/* Defect 2257: the program's own C for its case, a handler for SIGABRT that
 * says so and returns, which the program hands to C's `signal` from Heroes
 * as an `extern` function; no Heroes function is a handler (panel 247's R1). */
#include <signal.h>
#include <stdio.h>

static void return_on_abort(int sig) {
    (void)sig;
    fputs("C code ran inside the stop, and the handler returns\n", stderr);
}
