/* Panel 173, ffi-pragmatist: what siginfo_t says about the ORIGIN of SIGABRT
 * and SIGTRAP on this platform. Not Heroes: a plain C probe, so the answer is
 * the kernel's and the libc's and nothing of ours is in the way. */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

static void say(const char *s) { write(2, s, strlen(s)); }
static void num(long v) {
    char b[32]; int n = snprintf(b, sizeof b, "%ld", v); write(2, b, (size_t)n);
}
static void h(int sig, siginfo_t *si, void *ctx) {
    (void)ctx;
    say("probe: signal="); num(sig);
    say(" si_code="); num(si->si_code);
    say(" si_pid="); num((long)si->si_pid);
    say(" getpid="); num((long)getpid());
    say(" si_addr="); num((long)(uintptr_t)si->si_addr);
    say("\n");
    struct sigaction d; memset(&d, 0, sizeof d); d.sa_handler = SIG_DFL;
    sigaction(sig, &d, NULL);
    raise(sig);
}
int main(int argc, char **argv) {
    int which = argc > 1 ? atoi(argv[1]) : 0;
    struct sigaction sa; memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = h; sa.sa_flags = SA_SIGINFO;
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGTRAP, &sa, NULL);
    char *b = malloc(64);
    fprintf(stderr, "which %d\n", which);
    if (which == 0) free(b + 16);            /* allocator: interior free */
    if (which == 1) abort();                 /* libc abort(), as every runtime panic */
    if (which == 2) __builtin_trap();        /* compiler trap instruction */
    if (which == 3) { free(b); free(b); }    /* allocator: double free */
    if (which == 4) { char *big = malloc(100000); free(big + 16); } /* large interior */
    if (which == 5) assert(0 && "library assert");  /* a library's own assert */
    fprintf(stderr, "after\n");
    return 0;
}
