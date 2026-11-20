/* Defect 2522's row 3 (the author's ruling C of 2026-11-20): the program's own
 * C for its cases. A handler installed by a route past the names the runtime
 * defines or rewrites calls the program's function through DEEP_FRAMES frames
 * of its own, more than the thunk's walk reads, so only the route's entry can
 * tell the function is running inside the signal: `sigaction` asked of `dlsym`
 * with the C library's own handle or with `RTLD_NEXT`, `sigset`, and `signal`
 * followed by `siginterrupt`, which re-installs the handler (on Darwin the
 * last two are libsystem_c's and install inside the shared cache). And a
 * correct program whose handler jumps out of itself by a `siglongjmp` asked of
 * `dlsym`, then calls the function, the signal over, from far deeper. POSIX's
 * signals and `dlfcn.h`, which a platform without them lacks: `unistd.h` is
 * asked first, so such a platform is told the header is missing and skips the
 * case (`absence.hero`). `sigset`, `siginterrupt` and `RTLD_NEXT` are glibc's
 * only under `_GNU_SOURCE`, which glibc reads at its first header, so it is
 * defined here first, before any (a unit reads none before its groups). */
#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <unistd.h>
#include <dlfcn.h>
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>

#define DEEP_FRAMES 24

#if defined(__APPLE__)
#define PAST_C_LIBRARY "/usr/lib/libSystem.B.dylib"
#else
#define PAST_C_LIBRARY "libc.so.6"
#endif

typedef int (*past_action_fn)(int, const struct sigaction *, struct sigaction *);
typedef void (*past_jump_fn)(sigjmp_buf, int);

static int64_t (*past_called)(int64_t) = NULL;
static volatile int64_t past_got = 0;
static past_jump_fn past_jump = NULL;
static sigjmp_buf past_env;

__attribute__((noinline, disable_tail_calls)) static int64_t past_down(int left, int sig) {
    if (left == 0) return past_called((int64_t)sig);
    int64_t below = past_down(left - 1, sig);
    __asm__ volatile("" ::: "memory");
    return below;
}

static void past_handler(int sig) { past_got = past_down(DEEP_FRAMES, sig); }

static void past_jumping_handler(int sig) { past_jump(past_env, sig); }

static void past_action(struct sigaction *sa, void (*handler)(int)) {
    memset(sa, 0, sizeof *sa);
    sa->sa_handler = handler;
    sigemptyset(&sa->sa_mask);
}

/* SIGUSR1's handler installed by `how`: 1 the `sigaction` `dlsym` finds with
 * the C library's own handle, 2 the one it finds next after this program, 3
 * `sigset`, 4 `signal` and then `siginterrupt`. 0, or -1 where one was not
 * found or the system refused. `sigset` and `siginterrupt` are installers a
 * library calls still, which glibc marks deprecated; that warning is held off
 * here alone. */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
static inline int32_t past_install(int32_t how, int64_t (*called)(int64_t)) {
    struct sigaction sa;
    past_called = called;
    past_action(&sa, past_handler);
    if (how == 1) {
        void *library = dlopen(PAST_C_LIBRARY, RTLD_NOW);
        past_action_fn install = library != NULL ? (past_action_fn)dlsym(library, "sigaction") : NULL;
        return install != NULL && install(SIGUSR1, &sa, NULL) == 0 ? 0 : -1;
    }
    if (how == 2) {
        past_action_fn install = (past_action_fn)dlsym(RTLD_NEXT, "sigaction");
        return install != NULL && install(SIGUSR1, &sa, NULL) == 0 ? 0 : -1;
    }
    if (how == 3) return sigset(SIGUSR1, past_handler) != SIG_ERR ? 0 : -1;
    if (how == 4) {
        if (signal(SIGUSR1, past_handler) == SIG_ERR) return -1;
        return siginterrupt(SIGUSR1, 1) == 0 ? 0 : -1;
    }
    return -1;
}
#pragma clang diagnostic pop

/* SIGUSR1 raised once; the handler's call of the function is what is judged. */
static inline int32_t past_raise(void) { return raise(SIGUSR1); }

/* SIGUSR1's handler that jumps out of itself by the `siglongjmp` `dlsym` finds
 * with the C library's own handle: 0, or -1. */
static inline int32_t past_jump_install(void) {
    struct sigaction sa;
    void *library = dlopen(PAST_C_LIBRARY, RTLD_NOW);
    past_jump = library != NULL ? (past_jump_fn)dlsym(library, "siglongjmp") : NULL;
    if (past_jump == NULL) return -1;
    past_action(&sa, past_jumping_handler);
    return sigaction(SIGUSR1, &sa, NULL);
}

/* 1 once the handler jumped out, 0 if it returned. */
static inline int32_t past_escape(void) {
    if (sigsetjmp(past_env, 1) == 0) {
        raise(SIGUSR1);
        return 0;
    }
    return 1;
}

/* The function called under 256 KiB of this frame, deeper than any frame the
 * handler had (as `fixedbugs-2522-a-jump-out-of-a-handler-through-a-copied-buffer.h`). */
__attribute__((noinline)) static int64_t past_deep(int64_t (*cb)(int64_t)) {
    char room[1 << 18];
    memset(room, 1, sizeof room);
    __asm__ volatile("" : : "r"(room) : "memory");
    return cb(7) + room[0] - room[sizeof room - 1];
}

static inline int64_t past_call(int64_t (*cb)(int64_t)) { return past_deep(cb); }
