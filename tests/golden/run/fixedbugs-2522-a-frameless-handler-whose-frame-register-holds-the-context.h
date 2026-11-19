/* Defect 2522 (panel 251's R1): the program's own C for its case, panel 249's
 * `kmiss`. A handler of C's keeps no frame record: when it calls the
 * program's function, its frame register holds the signal's context, an
 * address in the signal frame above the delivery, as clang may allocate the
 * register under `-fomit-frame-pointer` (x29 on aarch64, rbp on x86-64, both
 * measured by panel 249 holding live data across a call). Written in assembly
 * so the register's content is exact. The thunk's walk then reads the
 * context's words as frame records and finds no delivery. On Darwin x18 is
 * the platform's register and is left out of the clobbers (panel 251's
 * ffi-pragmatist's twin), elsewhere it is a scratch register the call may
 * change. */
/* POSIX's, which a platform without POSIX's signals lacks: such a platform
 * is told the header is missing and skips the case (`absence.hero`). */
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>

static int64_t (*frameless_cb)(int64_t) = NULL;
static volatile int64_t frameless_got = 0;

static void frameless_handler(int sig, siginfo_t *info, void *ctx) {
    (void)info;
    int64_t r;
#if defined(__aarch64__)
    register int64_t x0 __asm__("x0") = sig;
    __asm__ volatile("stp x29, x30, [sp, #-16]!\n\tmov x29, %[c]\n\tblr %[f]\n\tldp x29, x30, [sp], #16"
                     : "+r"(x0)
                     : [c] "r"(ctx), [f] "r"(frameless_cb)
                     : "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "x10", "x11", "x12", "x13", "x14",
                       "x15", "x16", "x17",
#if !defined(__APPLE__)
                       "x18",
#endif
                       "x30", "memory", "cc");
    r = x0;
#else
    __asm__ volatile("push %%rbp\n\tsub $8, %%rsp\n\tmov %[c], %%rbp\n\tcall *%[f]\n\tadd $8, %%rsp\n\tpop %%rbp"
                     : "=a"(r)
                     : "D"((int64_t)sig), [c] "r"(ctx), [f] "r"(frameless_cb)
                     : "rcx", "rdx", "rsi", "r8", "r9", "r10", "r11", "memory", "cc");
#endif
    frameless_got = r;
}

static inline int32_t frameless_install(int32_t sig, int64_t (*cb)(int64_t)) {
    struct sigaction sa;
    frameless_cb = cb;
    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = frameless_handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    return sigaction(sig, &sa, NULL);
}

static inline int32_t frameless_raise(int32_t sig) { return raise(sig); }

static inline int64_t frameless_seen(void) { return frameless_got; }
