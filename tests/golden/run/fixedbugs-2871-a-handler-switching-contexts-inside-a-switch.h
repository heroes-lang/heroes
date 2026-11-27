/* Defect 2871 (panel 262's ffi-pragmatist, `gtc`): a signal handler of C's
 * that switches contexts, delivered while the code it interrupts is itself
 * switching. A switch loads the target's signal mask before its registers
 * (Darwin's `setcontext` and glibc's `swapcontext` alike), so a signal
 * pending and blocked is delivered inside the switch, on the stack being
 * left, whenever the target's mask lets it through: here the signal is made
 * pending on purpose, so the handler runs there every time, where a timer's
 * scheduler meets it once in some hundreds of runs. Neither `swapcontext` nor
 * `setcontext` is async-signal-safe, and in a green-thread scheduler the
 * switch the handler makes saves into the very context the interrupted one
 * was about to load. No function of the program runs in a signal; it is
 * called after, on the thread's own stack.
 * Portable: Darwin (`_XOPEN_SOURCE`) and glibc (`_GNU_SOURCE`).
 * sw_run shape 0: the interrupted switch is a `swapcontext` to a coroutine,
 *     and the handler `swapcontext`s saving into that coroutine's context, as
 *     a scheduler whose current thread is already the new one does: MUST BE
 *     REFUSED.
 * sw_run shape 1: the same, the interrupted switch a `setcontext`: MUST BE
 *     REFUSED.
 * sw_run shape 2: the interrupted switch is a `swapcontext`, and the handler
 *     `setcontext`s to the context that switch saves into: MUST BE REFUSED.
 * sw_run shape 3: the interrupted switch is a `swapcontext`, and the handler
 *     switches nothing, it counts: correct, MUST RUN.
 * sw_run shape 4: the signal interrupts no switch, and the handler swaps to a
 *     coroutine that swaps back to it: correct, MUST RUN.
 * sw_run shape 5: the interrupted switch is a `swapcontext`, and the handler
 *     swaps to another coroutine and back, naming no context of the
 *     interrupted switch, which the C library finishes when the handler
 *     returns: MUST RUN.
 * sw_run shape 6: shape 0 with the handler run on an alternate signal stack
 *     the program gives (`SA_ONSTACK`): MUST BE REFUSED. */
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>

typedef int64_t (*sw_cb)(int64_t);
#define SW_STACK ((size_t)256 << 10)
static ucontext_t sw_home, sw_co, sw_inner, sw_inner_co, sw_inner_home;
static volatile int sw_shape, sw_counted, sw_co_ran, sw_inner_ran;
static ucontext_t *volatile sw_back;

/* The coroutine the interrupted switch was entering: it notes it ran and
 * goes home. */
static void sw_co_body(void) {
    sw_co_ran = 1;
    setcontext(&sw_home);
}

/* The coroutine the handler's own switch enters: it notes it ran and goes
 * back to the handler, through the context the handler saved into. */
static void sw_inner_body(void) {
    sw_inner_ran = 1;
    swapcontext(&sw_inner_co, sw_back);
}

static void sw_handler(int sig) {
    (void)sig;
    if (sw_shape == 3) {
        sw_counted += 1;
    } else if (sw_shape == 2) {
        setcontext(&sw_home);
    } else if (sw_shape == 0 || sw_shape == 1 || sw_shape == 6) {
        sw_back = &sw_co;
        swapcontext(&sw_co, &sw_inner_co);
    } else {
        sw_back = &sw_inner;
        swapcontext(&sw_inner, &sw_inner_co);
    }
}

static int sw_made(ucontext_t *uc, void (*body)(void), ucontext_t *link) {
    char *stack = malloc(SW_STACK);
    if (stack == NULL || getcontext(uc) != 0) return -1;
    uc->uc_stack.ss_sp = stack;
    uc->uc_stack.ss_size = SW_STACK;
    uc->uc_link = link;
    sigdelset(&uc->uc_sigmask, SIGUSR1);
    makecontext(uc, body, 0);
    return 0;
}

static inline int64_t sw_run(int32_t shape, sw_cb cb) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = sw_handler;
    sigemptyset(&sa.sa_mask);
    if (shape == 6) {
        stack_t alt;
        alt.ss_sp = malloc(SW_STACK);
        alt.ss_size = SW_STACK;
        alt.ss_flags = 0;
        if (alt.ss_sp == NULL || sigaltstack(&alt, NULL) != 0) return -3;
        sa.sa_flags = SA_ONSTACK;
    }
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -1;
    sw_shape = shape;
    if (sw_made(&sw_co, sw_co_body, NULL) != 0 || sw_made(&sw_inner_co, sw_inner_body, &sw_inner_home) != 0) return -2;
    sigset_t usr1;
    sigemptyset(&usr1);
    sigaddset(&usr1, SIGUSR1);
    if (shape == 4) {
        raise(SIGUSR1);
        return cb(sw_inner_ran);
    }
    volatile int back = 0;
    sigprocmask(SIG_BLOCK, &usr1, NULL);
    raise(SIGUSR1);
    if (shape == 1) {
        getcontext(&sw_home);
        if (back == 0) {
            back = 1;
            setcontext(&sw_co);
        }
    } else {
        swapcontext(&sw_home, &sw_co);
    }
    sigprocmask(SIG_UNBLOCK, &usr1, NULL);
    return cb(sw_co_ran + sw_counted);
}
