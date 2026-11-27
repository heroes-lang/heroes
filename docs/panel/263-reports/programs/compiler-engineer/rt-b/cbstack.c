/* parts/cbstack.c: panel 263's compiler-engineer's prototype of route (b), unlanded. The thunk C is handed asks its
 * question on C's stack, then calls `hero_callback_run`, which runs the function on a stack of this runtime's own: 8
 * MiB `mmap`ed with a page of no access below, one per callback entered from a stack not this runtime's and not yet
 * returned, from a per-thread pool of 16; an entry already on one of them runs where it is. While the function runs,
 * the thread's bounds the stack guard reads (`hero_stack_lo/hi`, `parts/stack.c`) name that stack, so its run-out is
 * told by the guard. The switch is `hero_on_stack`, an AArch64 trampoline whose CFI names the frame it came from (a
 * forced unwind crosses it); no other architecture is written. */
#if !defined(__aarch64__)
#error "panel 263's route (b) prototype is AArch64 only"
#endif
#if defined(__APPLE__)
#define HERO_CB_SYM(x) "_" #x
#define HERO_CB_TYPE(x) ""
#define HERO_CB_SIZE(x) ""
#else
#define HERO_CB_SYM(x) #x
#define HERO_CB_TYPE(x) ".type " #x ", %function\n"
#define HERO_CB_SIZE(x) ".size " #x ", .-" #x "\n"
#endif
void hero_on_stack(void *top, void (*fn)(void *), void *arg);
__asm__(".text\n.p2align 2\n.globl " HERO_CB_SYM(hero_on_stack) "\n" HERO_CB_TYPE(hero_on_stack) HERO_CB_SYM(hero_on_stack) ":\n"
        ".cfi_startproc\n"
        "stp x29, x30, [sp, #-16]!\n.cfi_def_cfa_offset 16\n.cfi_offset w30, -8\n.cfi_offset w29, -16\n"
        "mov x29, sp\n"
        "mov x9, sp\n"
        "sub x0, x0, #32\n"
        "str x9, [x0, #16]\n"
        "stp x29, x30, [x0]\n"
        "mov sp, x0\n"
        "mov x29, sp\n"
        ".cfi_escape 0x0f, 0x05, 0x8d, 0x00, 0x06, 0x23, 0x10\n"
        ".cfi_escape 0x10, 0x1d, 0x03, 0x8d, 0x00, 0x06\n"
        ".cfi_escape 0x10, 0x1e, 0x05, 0x8d, 0x00, 0x06, 0x23, 0x08\n"
        "mov x0, x2\n"
        "blr x1\n"
        "ldr x9, [sp, #16]\n"
        "mov sp, x9\n"
        "ldp x29, x30, [sp], #16\n"
        "ret\n"
        ".cfi_endproc\n" HERO_CB_SIZE(hero_on_stack));

#define HERO_CB_STACKS 16
#if !defined(HERO_CB_STACK_SIZE)
#define HERO_CB_STACK_SIZE ((size_t)8 << 20)
#endif
typedef struct {
    char *lo;
    char *hi;
    bool busy;
} HeroCbStack;
static _Thread_local HeroCbStack hero_cb_stacks[HERO_CB_STACKS];

static _Noreturn void hero_cb_refuse(const char *why) {
    const char *const said[] = {"panic: ", why};
    hero_signal_stop(said, sizeof said / sizeof said[0]);
}

void hero_callback_run(void (*fn)(void *), void *arg) {
    uintptr_t sp = (uintptr_t)__builtin_frame_address(0);
    int free_at = -1;
    for (int i = 0; i < HERO_CB_STACKS; i += 1) {
        HeroCbStack *s = &hero_cb_stacks[i];
        if (s->busy && sp >= (uintptr_t)s->lo && sp < (uintptr_t)s->hi) {
            fn(arg); /* nested: already on this runtime's stack */
            return;
        }
        if (!s->busy && free_at < 0) free_at = i;
    }
    if (free_at < 0) hero_cb_refuse("more than 16 callbacks entered from C's stacks and not returned on this thread");
    HeroCbStack *s = &hero_cb_stacks[free_at];
    if (s->lo == NULL) {
        size_t page = (size_t)sysconf(_SC_PAGESIZE);
        char *map = mmap(NULL, HERO_CB_STACK_SIZE + page, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (map == MAP_FAILED) hero_cb_refuse("a callback's stack could not be mapped");
        if (mprotect(map, page, PROT_NONE) != 0) hero_cb_refuse("a callback's stack's guard could not be placed");
        s->lo = map + page;
        s->hi = map + page + HERO_CB_STACK_SIZE;
    }
    s->busy = true;
#if defined(HERO_STACK_GUARD_YIELDS_TO_ASAN)
    uintptr_t was_lo = hero_stack_walk_lo, was_hi = hero_stack_walk_hi;
    hero_stack_walk_lo = (uintptr_t)s->lo;
    hero_stack_walk_hi = (uintptr_t)s->hi;
#else
    uintptr_t was_lo = hero_stack_lo, was_hi = hero_stack_hi;
    hero_stack_lo = (uintptr_t)s->lo;
    hero_stack_hi = (uintptr_t)s->hi;
#endif
    hero_on_stack(s->hi, fn, arg);
#if defined(HERO_STACK_GUARD_YIELDS_TO_ASAN)
    hero_stack_walk_lo = was_lo;
    hero_stack_walk_hi = was_hi;
#else
    hero_stack_lo = was_lo;
    hero_stack_hi = was_hi;
#endif
    s->busy = false;
}
