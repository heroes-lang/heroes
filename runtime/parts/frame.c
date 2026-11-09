/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/frame.c — the room of a large value, off its function's frame (defect
 * 2278; panel 233, provisional, author ratification pending).
 *
 * A value of a program's own type larger than the emitter's threshold
 * (`selfhost/emit/large_value.hero`, 4,096 bytes as it counts them) is not a
 * local of its function's C frame: the prologue takes its room here, the
 * function's one exit gives it back, and the body names it through a pointer.
 * So no frame holds a record of records megabytes deep, which an 8 MiB stack
 * could not (the other face of defect 2253's frame), and clang's SROA, which
 * splits only what a frame holds, never sees such a value whole (panel 233
 * measured 314 billion instructions and 12.3 GB at `-O2` for a 4 MiB record,
 * 0.44 billion and 12 MB with this room).
 *
 * THE ORDER IS THE STACK'S. A function gives back what it took before it
 * returns, the last taken first, so the room is a stack of its own on each
 * thread: a bump pointer in chunks of a mebibyte or of the value, whichever is
 * larger, the newest chunk on top and one emptied chunk kept for the next. A
 * give that is not of the newest take is the compiler's error, never the
 * program's, and stops loudly rather than lose a value. The chunks come from
 * the one `malloc` neither counter weighs and go back through its mate
 * (`hero_malloc_raw`, which stops at `out of memory` rather than return NULL,
 * and `hero_free_raw`, both in `parts/alloc.c` where Part 7.13's single point
 * puts every such call), so the leak gate counts what the program owns and not
 * this room. */

typedef struct HeroFrameChunk {
    struct HeroFrameChunk *below;
    size_t size; /* bytes after the header */
    size_t used;
} HeroFrameChunk;

#define HERO_FRAME_CHUNK ((size_t)1 << 20)
#define HERO_FRAME_HEADER ((sizeof(HeroFrameChunk) + 15) & ~(size_t)15)

/* THE ROOM IS BOUNDED ONCE FOR THE WHOLE PROCESS, AND RUNNING OUT OF IT ABORTS
 * AS RUNNING OUT OF STACK DOES (spec § 9: *running out of stack aborts*).
 * Without a bound a recursion holding a large value would take the heap until
 * the system stopped the process, which on Linux is the OOM killer's SIGKILL
 * with nothing said. One gibibyte for every thread together, counted
 * atomically, a number and not a question asked of the machine, so a program
 * aborts at the same depth everywhere: panel 233's 128 MiB shapes held at most
 * 616 MB in it. Until the critic's second pass of that sitting it was a
 * gibibyte a THREAD, and three runaway threads held 3.29 GB before the first
 * bound fired. */
#define HERO_FRAME_ROOM ((size_t)1 << 30)

static _Thread_local HeroFrameChunk *hero_frame_top = NULL;
static _Thread_local HeroFrameChunk *hero_frame_spare = NULL;
static _Atomic size_t hero_frame_held = 0;

/* THE MESSAGE OPENS AS THE STACK GUARD'S DOES AND SAYS WHICH ROOM RAN OUT:
 * `panic: stack exhausted in <module.function>`, the function whose prologue
 * asked, named as the guard names one where the platform's guard names any
 * (`parts/stack.c`, POSIX by `hero_stack_symbol` on the return address, a
 * generic instance's `static` copy included since defect 2371, Windows the
 * first sentence alone), then that the room large values take off the stack
 * is full, since the stack itself may be nearly empty. One thread tells it, through
 * the guard's own flag (`hero_stack_tell_first`, defect 2287): a second that
 * runs out while the first is telling, of its room or of its stack, waits for
 * the process to end, so two messages never interleave. */
static _Noreturn void hero_frame_exhausted(void *caller) {
    fflush(stdout);
#if !defined(_WIN32)
    hero_stack_tell_first();
    const char *who = caller != NULL ? hero_stack_symbol((uintptr_t)caller) : NULL;
    hero_stack_say("panic: stack exhausted");
    if (hero_stack_is_heroes(who)) {
        hero_stack_say(" in ");
        hero_stack_say_heroes_name(who);
    }
    hero_stack_say(": the room large values take off the stack is full, 1 GiB for the whole program\n");
    hero_abort();
#else
    (void)caller;
    hero_panic("stack exhausted: the room large values take off the stack is full, 1 GiB for the whole program");
#endif
}

void *hero_frame_take(size_t bytes) {
    if (bytes > HERO_FRAME_ROOM) hero_frame_exhausted(__builtin_return_address(0));
    size_t need = (bytes + 15) & ~(size_t)15;
    size_t before = atomic_fetch_add(&hero_frame_held, need);
    if (before > HERO_FRAME_ROOM - need) {
        atomic_fetch_sub(&hero_frame_held, need);
        hero_frame_exhausted(__builtin_return_address(0));
    }
    HeroFrameChunk *top = hero_frame_top;
    if (top == NULL || top->size - top->used < need) {
        HeroFrameChunk *fresh = NULL;
        if (hero_frame_spare != NULL && hero_frame_spare->size >= need) {
            fresh = hero_frame_spare;
            hero_frame_spare = NULL;
        } else {
            size_t size = need > HERO_FRAME_CHUNK ? need : HERO_FRAME_CHUNK;
            fresh = (HeroFrameChunk *)hero_malloc_raw(HERO_FRAME_HEADER + size);
            fresh->size = size;
        }
        fresh->below = top;
        fresh->used = 0;
        hero_frame_top = fresh;
        top = fresh;
    }
    char *at = (char *)top + HERO_FRAME_HEADER + top->used;
    top->used += need;
    return at;
}

void hero_frame_give(void *p) {
    HeroFrameChunk *top = hero_frame_top;
    char *base = top == NULL ? NULL : (char *)top + HERO_FRAME_HEADER;
    if (top == NULL || (char *)p < base || (char *)p >= base + top->used) {
        hero_panic("internal error: a large value's room was given back out of order (compiler bug)");
    }
    atomic_fetch_sub(&hero_frame_held, top->used - (size_t)((char *)p - base));
    top->used = (size_t)((char *)p - base);
    if (top->used > 0) return;
    hero_frame_top = top->below;
    if (hero_frame_spare == NULL || hero_frame_spare->size < top->size) {
        hero_free_raw(hero_frame_spare);
        hero_frame_spare = top;
    } else {
        hero_free_raw(top);
    }
}

/* The same, for `__attribute__((__cleanup__))`, which hands the address of
 * the pointer and runs after a `return` has copied its value: a function that
 * returns a group's record out of this room by C's convention gives its
 * regions back through here (`selfhost/emit/large_value.hero`'s
 * `taken_to_exit`, defect 2316). Given back before the `return`, the chunk
 * the value stood in was freed above whenever a spare at least as large was
 * already kept, and the return's copy read freed memory. */
void hero_frame_give_at(void *slot) {
    hero_frame_give(*(void **)slot);
}

/* A spawned thread's room goes back with the thread (`parts/spawn.c`), as its
 * lend regions do: every function it ran has returned, so only the kept chunk
 * is left. */
static void hero_frame_thread_leave(void) {
    while (hero_frame_top != NULL) {
        HeroFrameChunk *below = hero_frame_top->below;
        hero_free_raw(hero_frame_top);
        hero_frame_top = below;
    }
    hero_free_raw(hero_frame_spare);
    hero_frame_spare = NULL;
}
