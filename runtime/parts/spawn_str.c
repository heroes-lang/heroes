/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/spawn_str.c — a thread of one `str` in and one `str` back, each COPIED
 * (decision 2498, the author's `2a`; feature 2521; panel 113's R5, *"the
 * scalars and `str`"*, ratified 2026-09-06; design.md Part 7.13).
 *
 * WHAT CROSSES IS BYTES, NEVER A COUNT. A `str` is a counted block, and its
 * count is atomic since M-isolated-threads, but copy-on-write's test and its
 * write are two steps no atomic makes one (`parts/cow.c` names the line): two
 * threads holding one block can both read themselves its only holder and both
 * write, which panel 111 built to a double free. Shared by its count, a
 * spawner's `str` handed to a worker drew no ThreadSanitizer report on this
 * Mac (2026-11-14), the count being atomic; the race is the protocol's, which
 * a copy leaves nothing to race on. So the
 * argument's bytes are copied on the spawning thread into a block of the
 * runtime's, made a `str` on the worker and released there; the body's answer
 * is copied on the worker into a second block, released there, and made a
 * `str` again on the thread that joins. Each `str` is born and dies on one
 * thread, Part 7.13's *"isolated per-thread heaps, copying at the
 * boundaries"*.
 *
 * THE COPIES ARE THE RUNTIME'S OWN BLOCKS, weighed by neither counter
 * (`hero_malloc_raw`, as a large value's room is): what the leak gate counts
 * is the program's values, and it tells one still live at exit as *"a missing
 * decref — this is a compiler bug"*. Counted as a `str`'s bytes, the answer
 * of a thread the program finished and never joined was told that way,
 * measured on this Mac on 2026-11-14, blaming the compiler for the program's
 * join. So a thread never joined drops its answer with the process, as an
 * `i64` thread's answer is dropped, and its slot is kept, so the drop is
 * bounded by the table's 256 (`parts/spawn.c`).
 *
 * BUILT ON THE `i64` RUNG, so it has no platform arm of its own: the way in
 * crosses `hero_spawn_start` as the runtime's number for its own block, and
 * the answer crosses back as the slot's result, two numbers no program sees
 * (panel 246's ffi-pragmatist built the way in, 20 of 20 runs right on this
 * Mac and in Linux arm64, plain and under `--sanitize`).
 *
 * A THREAD IS JOINED BY THE DOOR OF ITS KIND (`parts/spawn.c`'s
 * `HERO_SPAWN_STR`): `hero_thread_join` on a thread started here would hand
 * the program an address as a number, and `hero_thread_join_str` on one
 * `hero_thread_spawn` started would read a number as an address; each is a
 * named panic before any wait. */

typedef HeroStr (*HeroThreadBodyStr)(HeroStr);

/* One `str`'s bytes between two threads, and the body on the way in. */
typedef struct {
    HeroThreadBodyStr body;
    int64_t len;
    char bytes[];
} HeroSpawnStrBox;

/* `s`'s bytes and their NUL copied into a block of the runtime's (+1). */
static HeroSpawnStrBox *hero_spawn_str_box(HeroThreadBodyStr body, HeroStr s) {
    int64_t len = hero_str_len(s);
    HeroSpawnStrBox *box = hero_malloc_raw(sizeof *box + (size_t)len + 1);
    box->body = body;
    box->len = len;
    memcpy(box->bytes, s.ptr, (size_t)len);
    box->bytes[len] = '\0';
    return box;
}

/* The worker's side: the argument made a `str` here, lent to the body, which
 * gives back a `str` this side owns (the convention of every Heroes function:
 * a `str` parameter borrowed, a `str` result handed over), both released here
 * once the answer is copied. */
static int64_t hero_spawn_str_run(int64_t number) {
    HeroSpawnStrBox *in = (HeroSpawnStrBox *)(uintptr_t)number;
    HeroThreadBodyStr body = in->body;
    HeroStr arg = hero_str_from_bytes(in->bytes, in->len);
    hero_free_raw(in);
    HeroStr answer = body(arg);
    hero_str_decref(arg);
    HeroSpawnStrBox *out = hero_spawn_str_box(NULL, answer);
    hero_str_decref(answer);
    return (int64_t)(uintptr_t)out;
}

/* `hero_spawn_start` refuses a missing function, and sees only the trampoline
 * below it here, so the program's own is asked first. */
int64_t hero_thread_spawn_str(HeroThreadBodyStr body, HeroStr arg) {
    if (body == NULL) hero_panic("a thread was started with no function to run");
    HeroSpawnStrBox *in = hero_spawn_str_box(body, arg);
    int64_t why = 0;
    int64_t at = hero_spawn_start(hero_spawn_str_run, (int64_t)(uintptr_t)in, 0, HERO_SPAWN_STR, &why);

    if (at < 0) {
        hero_free_raw(in);
        hero_panic("the operating system refused to start a thread");
    }
    return at;
}

HeroStr hero_thread_join_str(int64_t handle) {
    HeroSpawnStrBox *out = (HeroSpawnStrBox *)(uintptr_t)hero_spawn_join(handle, HERO_SPAWN_STR);
    HeroStr answer = hero_str_from_bytes(out->bytes, out->len);
    hero_free_raw(out);
    return answer;
}
