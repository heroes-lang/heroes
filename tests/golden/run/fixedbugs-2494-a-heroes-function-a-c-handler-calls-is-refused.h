/* Defect 2494: the program's own C for its case. Its handler is C, and it
 * calls the Heroes function it was handed for a result it needs before it
 * returns, so the call could not wait for the signal's end; the handler
 * keeps a frame record on every target, as one compiled with frame pointers
 * does. */
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

static int64_t (*called_answer)(int64_t) = NULL;
static volatile sig_atomic_t called_got = 0;

static void called_handler(int sig) {
    (void)__builtin_frame_address(0);
    called_got = (sig_atomic_t)called_answer((int64_t)sig);
}

static inline int32_t called_install(int32_t sig, int64_t (*answer)(int64_t)) {
    struct sigaction sa;
    called_answer = answer;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = called_handler;
    sigemptyset(&sa.sa_mask);
    return sigaction(sig, &sa, NULL);
}

static inline int32_t called_raise(int32_t sig) { return raise(sig); }

static inline int64_t called_seen(void) { return (int64_t)called_got; }
