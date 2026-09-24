/* A C library that IGNORES a signal and then raises it: before `main` it sets
 * SIGABRT and SIGILL to be ignored, and the raises below then return and the
 * program goes on, on every platform, Windows' `signal` included.
 *
 * NOT SIGTRAP, although the runtime takes it too and puts it back the same way:
 * under Rosetta, the Linux x86-64 leg on an Apple machine, a C program with no
 * Heroes in it that ignores SIGTRAP and raises it dies at 133, where the same
 * program exits 0 on Linux arm64 and Darwin (measured 2026-09-24). A case that
 * raised it would be red on that leg for the emulator's reason, not the
 * runtime's. */
#include <signal.h>
__attribute__((constructor)) static void ignore_them(void) {
    signal(SIGABRT, SIG_IGN);
    signal(SIGILL, SIG_IGN);
}
static __attribute__((noinline)) int raise_ignored(void) {
    raise(SIGABRT);
    raise(SIGILL);
    return 1;
}
