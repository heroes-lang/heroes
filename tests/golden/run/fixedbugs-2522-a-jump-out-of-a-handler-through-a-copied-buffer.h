/* Defect 2522's row (panel 251's R1, *it owes words of its own*): the
 * program's own C for its case. A correct program: it saves a buffer, copies
 * it with `memcpy`, raises a signal whose handler jumps OUT of itself through
 * the copy, and only then, the signal over, calls the program's function from
 * a frame far deeper than the handler's was. The copy is a buffer the runtime
 * never saw filled, so it cannot tell this jump from one that lands inside the
 * handler, and keeps the handler as running. POSIX's, which a platform without
 * POSIX's signals lacks: such a platform is told the header is missing and
 * skips the case (`absence.hero`). */
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>

static sigjmp_buf copied_env;
static sigjmp_buf copied_copy;

static void copied_handler(int sig) {
    (void)sig;
    siglongjmp(copied_copy, 1);
}

static inline int32_t copied_install(int32_t sig) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = copied_handler;
    sigemptyset(&sa.sa_mask);
    return sigaction(sig, &sa, NULL);
}

/* 1 once the handler jumped out through the copy, 0 if it returned. */
static inline int32_t copied_escape(int32_t sig) {
    if (sigsetjmp(copied_env, 1) == 0) {
        memcpy(copied_copy, copied_env, sizeof copied_env);
        raise(sig);
        return 0;
    }
    return 1;
}

/* The function called under 256 KiB of this frame, deeper than any frame the
 * handler had on any platform the case runs on. */
__attribute__((noinline)) static int64_t copied_deep(int64_t (*cb)(int64_t)) {
    volatile char room[1 << 18];
    room[0] = 1;
    room[sizeof room - 1] = 1;
    return cb(7) + room[0] - room[sizeof room - 1];
}

static inline int64_t copied_call(int64_t (*cb)(int64_t)) { return copied_deep(cb); }
