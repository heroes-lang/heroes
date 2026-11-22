/* Panel 255, the ffi-pragmatist: route T's two residues on the dangerous side,
 * each a jump that lands INSIDE the handler, after which the handler calls the
 * program back while it still runs (so the call must be refused).
 *   partial: the handler copies every byte of its own save into a buffer
 *            filled earlier at a returned frame, all but bytes [104,112).
 *   unseen:  the handler refills that buffer by a save the runtime never sees
 *            (libsystem_platform's `__setjmp` sequence, arm64, as a save made
 *            inside the shared cache would be), then jumps to it.
 * In both, a later save point stands between the first fill and the handler,
 * as any program's would. */
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>

static int64_t (*rs_cb)(int64_t) = 0;
static volatile int64_t rs_got = 0;
static jmp_buf rs_env;
static int rs_how = 0;

#if defined(__aarch64__)
__attribute__((naked)) static int rs_unseen_setjmp(void *b) {
    __asm__ volatile("mov x12, sp\n\t"
                     "mrs x16, TPIDRRO_EL0\n\t"
                     "ldr x16, [x16, #0x38]\n\t"
                     "eor x10, x29, x16\n\t"
                     "eor x11, x30, x16\n\t"
                     "eor x12, x12, x16\n\t"
                     "stp x19, x20, [x0]\n\t"
                     "stp x21, x22, [x0, #0x10]\n\t"
                     "stp x23, x24, [x0, #0x20]\n\t"
                     "stp x25, x26, [x0, #0x30]\n\t"
                     "stp x27, x28, [x0, #0x40]\n\t"
                     "stp x10, x11, [x0, #0x50]\n\t"
                     "str x12, [x0, #0x60]\n\t"
                     "stp d8, d9, [x0, #0x70]\n\t"
                     "stp d10, d11, [x0, #0x80]\n\t"
                     "stp d12, d13, [x0, #0x90]\n\t"
                     "stp d14, d15, [x0, #0xa0]\n\t"
                     "mov w0, #0\n\t"
                     "ret");
}
#endif

__attribute__((noinline)) static int rs_prime(void) { return setjmp(rs_env); }
__attribute__((noinline)) static void rs_fail(void) { longjmp(rs_env, 1); }

static void rs_handler(int sig) {
    jmp_buf here;
    if (rs_how == 1) {
        if (setjmp(here) == 0) {
            memcpy(rs_env, here, 104);
            memcpy((unsigned char *)rs_env + 112, (unsigned char *)here + 112, sizeof(jmp_buf) - 112);
            rs_fail();
        }
    } else {
        if (rs_unseen_setjmp(rs_env) == 0) rs_fail();
    }
    rs_got = rs_cb((int64_t)sig);
}

static inline int32_t rs_install(int32_t how, int64_t (*cb)(int64_t)) {
    struct sigaction sa;
    jmp_buf later;
    rs_cb = cb;
    rs_how = how;
    (void)rs_prime();
    (void)setjmp(later);
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = rs_handler;
    sigemptyset(&sa.sa_mask);
    return sigaction(SIGUSR1, &sa, 0);
}
static inline int32_t rs_raise(void) { return raise(SIGUSR1); }
static inline int64_t rs_seen(void) { return rs_got; }
