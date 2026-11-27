/* unwind-catch.h: a binding's own save point around a callback, the shape
 * libpng's error model gives one (`png_jmpbuf` saved, the read run, `png_error`
 * jumping back), for the cases of panel 258's R2: C jumps over the frames of
 * the function the program handed it, and the values those frames held are
 * released when the call holding the save point returns. */
#include <setjmp.h>
#include <stdint.h>
#include <string.h>

typedef int64_t (*uw_cb_t)(int64_t);

/* One save point a call, as each libpng read has its own `png_struct`, so a
 * callback may call the guard again inside it. */
static jmp_buf uw_envs[8];
static int uw_depth;
static const char *uw_said = "";
static char uw_heard_text[256];

/* The jump, called through the pointer a library keeps, as libpng's
 * `png_longjmp` calls `png_ptr->longjmp_fn`: clang cannot read that pointer's
 * callee, so a call into the helpers below is one that may unwind. Set where
 * the save point is made, as `png_jmpbuf` sets it, so it names the C
 * library's `longjmp` as the running program resolves it. */
static __typeof__(&longjmp) volatile uw_jump;

/* What the catch read of the jump's message, the program's to copy. */
static inline const char *uw_heard(void) { return uw_heard_text; }

/* Calls `cb(n)` under a save point: its answer where it returns, -1 where C
 * jumped over it, the jump's message read at the catch, after the landing and
 * before this call returns, as a binding's error function hands libpng's on. */
static inline int64_t uw_guard(uw_cb_t cb, int64_t n) {
    uw_jump = longjmp;
    int at = uw_depth++;
    if (setjmp(uw_envs[at]) == 0) {
        volatile int64_t answer = cb(n);
        uw_depth = at;
        return answer;
    }
    uw_depth = at;
    size_t len = strlen(uw_said);
    if (len >= sizeof uw_heard_text) len = sizeof uw_heard_text - 1;
    memcpy(uw_heard_text, uw_said, len);
    uw_heard_text[len] = 0;
    return -1;
}

/* Jumps to the save point carrying `msg`, which C keeps and reads at the
 * catch: a lend whose call never returns, as `png_error`'s message is. */
static inline void uw_fail(const char *msg) {
    uw_said = msg;
    uw_jump(uw_envs[uw_depth - 1], 1);
}

/* Jumps to the save point carrying nothing. */
static inline void uw_throw(void) {
    uw_said = "";
    uw_jump(uw_envs[uw_depth - 1], 1);
}

/* The same jump made by the C library's `longjmp` itself, which glibc's
 * `<setjmp.h>` declares `__THROWNL`, a function that cannot unwind, so a
 * caller of this helper is compiled with no landing pad at the call. */
static inline void uw_throw_direct(void) {
    uw_said = "";
    longjmp(uw_envs[uw_depth - 1], 1);
}

/* `uw_guard` whose frame moves its stack pointer after the save, as a catch
 * that `alloca`s a buffer before it calls on does: no frame of the chain then
 * stands at the stack pointer the jump installs, the same answer a walk gives
 * that stops at a frame built without unwind information, before the target. */
static inline int64_t uw_guard_moved(uw_cb_t cb, int64_t n) {
    uw_jump = longjmp;
    int at = uw_depth++;
    if (setjmp(uw_envs[at]) == 0) {
        volatile char *pad = (volatile char *)__builtin_alloca(256);
        pad[0] = 1;
        volatile int64_t answer = cb(n);
        uw_depth = at;
        return answer + pad[0] - 1;
    }
    uw_depth = at;
    return -1;
}
