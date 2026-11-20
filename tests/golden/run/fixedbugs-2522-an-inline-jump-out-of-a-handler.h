/* Defect 2522's row 2 (the author's ruling B of 2026-11-20): the program's own
 * C for its case. A correct program: its handler jumps OUT of itself by a jump
 * no function of the C library makes, so this runtime sees no event for it,
 * and only then, the signal over, it calls the program's function from a frame
 * far deeper than the handler's was.
 *
 * On x86-64 the jump is the inline `__builtin_setjmp` and `__builtin_longjmp`
 * pair, the row's own shape. clang refuses `__builtin_longjmp` on arm64 (*not
 * supported for the current target*, Linux arm64 at panel 251), so there the
 * same jump is written by hand, as the pair compiles: the callee-saved
 * registers, the frame record, the return address and the stack pointer saved
 * by one function and put back by another, neither of them the C library's.
 * POSIX's signals, which a platform without them lacks: `unistd.h` is asked
 * first, so such a platform is told the header is missing and skips the case
 * (`absence.hero`). */
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>

#if defined(__x86_64__)
typedef void *inline_buf[5];
#define INLINE_SAVE(b) __builtin_setjmp(b)
#define INLINE_JUMP(b) __builtin_longjmp(b, 1)
#elif defined(__aarch64__)
/* x19 to x28, the frame record x29 and x30, the stack pointer, d8 to d15. */
typedef uint64_t inline_buf[21];
__attribute__((naked, noinline, returns_twice)) static int inline_save(inline_buf b) {
    __asm__ volatile("stp x19, x20, [x0, #0]\n\t"
                     "stp x21, x22, [x0, #16]\n\t"
                     "stp x23, x24, [x0, #32]\n\t"
                     "stp x25, x26, [x0, #48]\n\t"
                     "stp x27, x28, [x0, #64]\n\t"
                     "stp x29, x30, [x0, #80]\n\t"
                     "mov x9, sp\n\t"
                     "str x9, [x0, #96]\n\t"
                     "stp d8, d9, [x0, #104]\n\t"
                     "stp d10, d11, [x0, #120]\n\t"
                     "stp d12, d13, [x0, #136]\n\t"
                     "stp d14, d15, [x0, #152]\n\t"
                     "mov w0, #0\n\t"
                     "ret");
}
__attribute__((naked, noinline, noreturn)) static void inline_jump(inline_buf b) {
    __asm__ volatile("ldp x19, x20, [x0, #0]\n\t"
                     "ldp x21, x22, [x0, #16]\n\t"
                     "ldp x23, x24, [x0, #32]\n\t"
                     "ldp x25, x26, [x0, #48]\n\t"
                     "ldp x27, x28, [x0, #64]\n\t"
                     "ldp x29, x30, [x0, #80]\n\t"
                     "ldr x9, [x0, #96]\n\t"
                     "mov sp, x9\n\t"
                     "ldp d8, d9, [x0, #104]\n\t"
                     "ldp d10, d11, [x0, #120]\n\t"
                     "ldp d12, d13, [x0, #136]\n\t"
                     "ldp d14, d15, [x0, #152]\n\t"
                     "mov w0, #1\n\t"
                     "ret");
}
#define INLINE_SAVE(b) inline_save(b)
#define INLINE_JUMP(b) inline_jump(b)
#else
#error this case names no jump of its own for this machine
#endif

static inline_buf inline_env;

static void inline_handler(int sig) {
    (void)sig;
    INLINE_JUMP(inline_env);
}

static inline int32_t inline_install(int32_t sig) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = inline_handler;
    sa.sa_flags = SA_NODEFER;
    sigemptyset(&sa.sa_mask);
    return sigaction(sig, &sa, NULL);
}

/* 1 once the handler jumped out, 0 if it returned. */
__attribute__((noinline)) static int32_t inline_escape(int32_t sig) {
    if (INLINE_SAVE(inline_env) == 0) {
        raise(sig);
        return 0;
    }
    return 1;
}

/* The function called under 256 KiB of this frame, deeper than any frame the
 * handler had on any platform the case runs on, the room's address handed to
 * an empty `asm` so the compiler keeps all of it (as
 * `fixedbugs-2522-a-jump-out-of-a-handler-through-a-copied-buffer.h`). */
__attribute__((noinline)) static int64_t inline_deep(int64_t (*cb)(int64_t)) {
    char room[1 << 18];
    memset(room, 1, sizeof room);
    __asm__ volatile("" : : "r"(room) : "memory");
    return cb(7) + room[0] - room[sizeof room - 1];
}

static inline int64_t inline_call(int64_t (*cb)(int64_t)) { return inline_deep(cb); }
