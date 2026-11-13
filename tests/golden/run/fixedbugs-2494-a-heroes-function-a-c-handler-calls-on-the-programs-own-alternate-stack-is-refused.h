/* Defect 2494: the program's own C for its case. Its handler is C and runs
 * on an alternate signal stack this header makes, which no walk of frame
 * records can climb out of; it calls the Heroes function it was handed for a
 * result. The stack is a static block, so nothing is left to give back. */
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

static int64_t (*aside_answer)(int64_t) = NULL;
static volatile sig_atomic_t aside_got = 0;
static _Alignas(16) char aside_stack[1 << 17];

static void aside_handler(int sig) { aside_got = (sig_atomic_t)aside_answer((int64_t)sig); }

static inline int32_t aside_install(int32_t sig, int64_t (*answer)(int64_t)) {
    stack_t ss;
    struct sigaction sa;
    memset(&ss, 0, sizeof ss);
    ss.ss_sp = aside_stack;
    ss.ss_size = sizeof aside_stack;
    if (sigaltstack(&ss, NULL) != 0) return -1;
    aside_answer = answer;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = aside_handler;
    sa.sa_flags = SA_ONSTACK;
    sigemptyset(&sa.sa_mask);
    return sigaction(sig, &sa, NULL);
}

static inline int32_t aside_raise(int32_t sig) { return raise(sig); }

static inline int64_t aside_seen(void) { return (int64_t)aside_got; }
