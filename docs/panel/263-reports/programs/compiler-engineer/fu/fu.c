/* fu.c, panel 263's compiler-engineer: does a forced unwind (panel 258's R2, lane unwinder) cross route (b)'s switch?
 * guard() saves a point and calls middle(), whose cleanup slot lies ABOVE the switch on C's stack; middle() calls the
 * thunk, which (SWITCH=1) runs body() on an mmap'd stack through hero_on_stack, an assembly trampoline whose CFI
 * describes the switch (CFI=1) or does not (CFI=0); body() holds two cleanup slots BELOW the switch and calls
 * lib_jump(), which runs _Unwind_ForcedUnwind with a stop function that lands at guard()'s frame: by address (STOP=0,
 * panel 258's ffi-pragmatist's `_Unwind_GetCFA(ctx) > target`) or by the frame itself (STOP=1, CFA == guard's CFA).
 * Prints how many of the three slots were released, the stop calls, and whether the walk ended with no stop. Build:
 * clang -O0 -g -fexceptions -DSWITCH=s -DCFI=c -DSTOP=t fu.c */
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#endif
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unwind.h>
#include <ucontext.h>
#if !defined(CO)
#define CO 0
#endif

#if defined(__APPLE__)
#define SYM(x) "_" #x
#define TYPE(x) ""
#define SIZE(x) ""
#else
#define SYM(x) #x
#define TYPE(x) ".type " #x ", %function\n"
#define SIZE(x) ".size " #x ", .-" #x "\n"
#endif

#if CFI
/* CFA = *(x29) + 16, x29 saved at *(x29), x30 at *(x29) + 8: x29 names record B on the new stack, whose first word is
 * record A's address on C's stack. DW_CFA_def_cfa_expression (0x0f) and DW_CFA_expression (0x10), DW_OP_breg29
 * (0x8d), DW_OP_deref (0x06), DW_OP_plus_uconst (0x23). */
#define SWITCHED_CFI ".cfi_escape 0x0f, 0x05, 0x8d, 0x00, 0x06, 0x23, 0x10\n" \
                     ".cfi_escape 0x10, 0x1d, 0x03, 0x8d, 0x00, 0x06\n" \
                     ".cfi_escape 0x10, 0x1e, 0x05, 0x8d, 0x00, 0x06, 0x23, 0x08\n"
#define START_CFI ".cfi_startproc\n"
#define END_CFI ".cfi_endproc\n"
#define FRAME_CFI ".cfi_def_cfa_offset 16\n.cfi_offset w30, -8\n.cfi_offset w29, -16\n"
#else
#define SWITCHED_CFI ""
#define START_CFI ""
#define END_CFI ""
#define FRAME_CFI ""
#endif

/* hero_on_stack(top, fn, arg): fn(arg) on the stack whose highest address is top. */
void hero_on_stack(void *top, void (*fn)(void *), void *arg);
__asm__(".text\n.p2align 2\n.globl " SYM(hero_on_stack) "\n" TYPE(hero_on_stack) SYM(hero_on_stack) ":\n"
        START_CFI
        "stp x29, x30, [sp, #-16]!\n" FRAME_CFI
        "mov x29, sp\n"
        "mov x9, sp\n"
        "sub x0, x0, #32\n"
        "str x9, [x0, #16]\n"
        "stp x29, x30, [x0]\n"
        "mov sp, x0\n"
        "mov x29, sp\n" SWITCHED_CFI
        "mov x0, x2\n"
        "blr x1\n"
        "ldr x9, [sp, #16]\n"
        "mov sp, x9\n"
        "ldp x29, x30, [sp], #16\n"
        "ret\n" END_CFI SIZE(hero_on_stack));

static long released, stops, ended;
static jmp_buf env;
static uintptr_t target_cfa, target_fp;
static void rel(char **p) { if (*p != NULL) { free(*p); *p = NULL; released += 1; } }

static _Unwind_Reason_Code stop_fn(int v, _Unwind_Action a, _Unwind_Exception_Class c, struct _Unwind_Exception *e,
                                   struct _Unwind_Context *ctx, void *arg) {
    (void)v; (void)c; (void)e; (void)arg;
    stops += 1;
    uintptr_t cfa = _Unwind_GetCFA(ctx);
    if ((a & _UA_END_OF_STACK) != 0) { ended += 1; longjmp(env, 1); }
    if (STOP == 0 && cfa > target_fp) longjmp(env, 1);
    if (STOP == 1 && cfa == target_cfa) longjmp(env, 1);
    return _URC_NO_REASON;
}

__attribute__((noinline)) static void lib_jump(void) {
    static struct _Unwind_Exception exc;
    memset(&exc, 0, sizeof exc);
    exc.exception_class = 0x4845524f32363300ULL;
    _Unwind_ForcedUnwind(&exc, stop_fn, NULL);
    ended += 100; /* returned: no frame stopped it */
    longjmp(env, 1);
}

__attribute__((noinline)) static void body(void *arg) {
    (void)arg;
    __attribute__((cleanup(rel))) char *a = malloc(10);
    __attribute__((cleanup(rel))) char *b = malloc(10);
    a[0] = 1; b[0] = 2;
    lib_jump();
}

static char *stack_top;
__attribute__((noinline)) static void thunk(void) {
    if (SWITCH) hero_on_stack(stack_top, body, NULL);
    else body(NULL);
}

__attribute__((noinline)) static void middle(void) {
    __attribute__((cleanup(rel))) char *m = malloc(10);
    m[0] = 3;
    thunk();
}

__attribute__((noinline)) static int guard(void) {
    target_fp = (uintptr_t)__builtin_frame_address(0);
    target_cfa = target_fp + 16;
    if (setjmp(env)) return 1;
    middle();
    return 0;
}

static int landed;
static void on_co(void) { landed = guard(); }
static unsigned char co_stack[(size_t)256 << 10];
static ucontext_t co_home, co_co;
int main(void) {
    size_t size = (size_t)1 << 20;
    char *map = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (map == MAP_FAILED) return 2;
    stack_top = map + size;
    if (CO) {
        /* C's own stack a coroutine's in the image's data, below the mmap'd one */
        getcontext(&co_co);
        co_co.uc_stack.ss_sp = co_stack;
        co_co.uc_stack.ss_size = sizeof co_stack;
        co_co.uc_link = &co_home;
        makecontext(&co_co, on_co, 0);
        swapcontext(&co_home, &co_co);
    } else {
        landed = guard();
    }
    printf("co %d switch %d cfi %d stop %s: landed %d, %ld of 3 slots released (2 below the switch, 1 above), %ld stop calls, ended %ld; the runtime's stack %s C's\n",
           CO, SWITCH, CFI, STOP ? "by frame" : "by address", landed, released, stops, ended,
           (uintptr_t)map > target_fp ? "above" : "below");
    return 0;
}
