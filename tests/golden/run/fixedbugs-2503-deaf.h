/* Defect 2503's third generation: SIGTERM caught by a handler that only notes
   it, so the signal its runner passes on ends nothing and the runner's grace
   runs to its end, where SIGKILL ends it. `sigaction` is POSIX's; a Windows
   runner sends no signal a child hears, and the case is skipped there by the
   harness's list (`tests/harness/platform_skips.hero`, `windows()`), since
   the box's clang refuses `struct sigaction` below as an incomplete type
   before `unistd.h`, which the case also binds, is asked (run 2026-11-13). */
#include <signal.h>
#include <stdint.h>
#include <string.h>

static volatile sig_atomic_t case2503_heard_flag = 0;

static void case2503_on_term(int sig) {
    (void)sig;
    case2503_heard_flag = 1;
}

/* SIGTERM noted and survived from now on: 0, or -1 where it was refused. */
static inline int32_t case2503_deaf(void) {
    struct sigaction quiet;
    memset(&quiet, 0, sizeof quiet);
    quiet.sa_handler = case2503_on_term;
    sigemptyset(&quiet.sa_mask);
    return (int32_t)sigaction(SIGTERM, &quiet, NULL);
}

/* 1 once SIGTERM has come, 0 before. */
static inline int32_t case2503_heard(void) {
    return (int32_t)case2503_heard_flag;
}
