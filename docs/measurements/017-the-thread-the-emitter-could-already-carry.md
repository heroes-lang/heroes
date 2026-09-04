# 017 — The thread the emitter could already carry, and the counter that cannot

Date: 2026-09-05 · M-isolated-threads step 1 ·
**the answer to panel 030 R6's cfront rider, which has been owed since 2026-08-03.**

## What was owed

Panel 030 R6, adopted from the historian and quoted here from
`docs/panel/030-the-build-order-revised.md:182-188`:

> **before M8c** the record states whether the C11 backend can express the
> intended concurrency model at all (stack switching in the runtime, or a
> CPS/state-machine transform). cfront — the direct ancestor of this
> architecture — was abandoned in 1993 after a failed attempt to add exception
> support, having frozen around forms that could not carry non-local control
> flow. If the answer is "not yet known", the deferral is a bet and is logged
> as one.

`docs/ROADMAP.md` § M-isolated-threads repeats it. Nobody had answered it, and
the milestone opened 2026-09-03 with it still open.

**The answer is yes, and it is not an argument.** A Heroes function, emitted,
was handed to `pthread_create` and ran.

## Provenance

| what | value |
|---|---|
| compiler | `heroes-from-seed`, built from `seed/heroes.c` at `320654e3` with `clang -I runtime seed/heroes.c runtime/runtime.c` |
| machine | the author's Mac, arm64, Apple clang 21.0.0 |
| instruments | `heroes check`, `heroes build --emit-c`, `clang -std=gnu11 -Wall -Werror`, `clang -fsanitize=thread`, `hero_runtime_check_leaks()` |
| programs | four probes, all in the session scratch, all reproduced below in full |

## 1. The library binds, links and runs

```
extern "pthread.h" link "pthread"
    function pthread_self() -> ptr
    function pthread_equal(a: ptr, b: ptr) -> i32

function main()
    me = pthread_self()
    print("same thread: " + pthread_equal(a: me, b: pthread_self()).to_str())
```

`heroes run` → `same thread: 1`, **exit 0**. So nothing about pthreads, the
header, the link flag or the ABI is in the way.

## 2. The one thing the language refuses, and it has two doors

Both were run; the `SCHEDULED.md` item that named this hole described only one
of them, and named it at the wrong door.

| the callback declared as | where it is refused | the message |
|---|---|---|
| `start: ptr` | **the call site**, not the declaration | `error[type_mismatch]: expected `ptr`, found `(function(ptr) -> ptr)`` |
| `start: (function(ptr) -> ptr)` | the declaration | `error[ffi_type]: `(function(ptr) -> ptr)` cannot cross the FFI boundary, and it is an `extern`'s parameter` |

Both exit 1. The item had said the refusal comes from
`selfhost/check/ffi.hero`'s `crosses_the_boundary` *"in every extern
position"*; that is true of the second door only. Under the first, the
**declaration is accepted** and ordinary type checking refuses the argument.
A brief that named one door would have sent a sitting to look at half the
problem.

## 3. The emitted types are already identical — no trampoline, no cast

`heroes build --emit-c` on

```
function worker(arg: ptr) -> ptr
    print("hello from the other side")
    return arg
```

emits

```c
void * h_main_worker(void * h0_arg);
```

which is `void *(*)(void *)`, which is exactly `pthread_create`'s third
parameter. The emitted C was compiled with a hand-written `main` that passes
`h_main_worker` **straight** to `pthread_create` — no cast, no wrapper — under
`-std=gnu11 -Wall -Werror`:

```
pthread_create said 0
hello from the other side
joined, the thread returned 0x0
exit 0
```

`-Werror` is the load-bearing half: clang compared the two function types and
had nothing to say. So the trampoline the scheduling item assumed would be
owed — *"the emitter owes one generated trampoline per spawned function
type"* — is **not owed for this shape**. What is missing is a permission, not
a mechanism: the compiler refuses to declare a thing it already knows how to
print. `selfhost/emit/ctype.hero:379` says so in its own comment, written long
before this was measured — *"a Heroes function value and a C callback are the
same eight bytes"*.

**This answers the rider.** The model Part 7.13 names is OS threads, and it
refuses green threads and coroutines by name (`design.md:2566-2573`). Stack
switching and a CPS transform are what those two need. An OS thread is a C
function call, and the C emitter carries it today.

## 4. What the model actually costs, which is the runtime and not the backend

Four threads, each running a Heroes function that allocates — 2000 string
concatenations pushed onto an array — joined, then `hero_runtime_check_leaks()`:

```
run 1: panic: 2586 heap blocks still live at exit (a missing decref) — this is a compiler bug
run 2: panic: 1836 heap blocks still live at exit
run 3: panic: 3141 heap blocks still live at exit
```

Three identical runs, three different numbers, **exit 134** each time. There is
no leak: `hero_live_blocks` is a plain `int64_t` that four threads increment and
decrement with no synchronisation, so the count is torn. The message accuses the
compiler of a bug it does not have, which is the §12 failure this project cares
about most — a wrong answer stated confidently.

ThreadSanitizer names it exactly:

```
SUMMARY: ThreadSanitizer: data race alloc.c:113 in hero_alloc_block
Location is global 'hero_live_blocks' at 0x000100c58080

  Write of size 8 by thread T2:
    #0 hero_alloc_block   alloc.c:113
    #1 hero_array_new     array.c:55
    #2 h_main_busy        main.hero:4

  Previous write of size 8 by thread T1:
    #0 hero_release_block alloc.c:118
    #1 hero_array_decref  array.c:84
    #2 h_main_busy        main.hero:8
```

Two things in that trace are worth as much as the race.

**The stack ends on a `.hero` line.** `main.hero:4` and `main.hero:8` are the
author's own lines, not the generated C's, because CLAUDE.md §7's `#line`
discipline reaches all the way into a sanitizer's report. Nothing was done to
make that true for threads; it simply is.

**Every frame is inside `runtime/parts/alloc.c`.** design.md §4.20 has argued
since day zero that the allocator must be a single point *"because Part 7.13's
per-thread heaps need one place to change, and a second allocation site
discovered later is a redesign"*. That single point became true and got an
instrument the same day as this measurement (M-isolated-threads step 0,
`23de5b5b`), and the race the model has to fix turns out to live entirely in the
file the instrument now guards. The invariant paid inside one session.

And one thing that is **not** broken, which is a decision holding up under
measurement: TSan reports no race in the drop queue. `hero_drop_arrays`,
`hero_drop_maps` and `hero_drop_running` are `_Thread_local` already
(`runtime/parts/drop.c:59-61`) because panel 070's ffi-pragmatist required it
when they were written.

## What this leaves for the sitting

Not *can it be done* — that is answered. The open questions are narrower and
each has a number attached now:

1. **The permission.** Which door opens: a function value admitted at an
   `extern` parameter, or a conversion to `ptr`. Both are the language, so
   CLAUDE.md §4 makes it a sitting.
2. **The counters.** `hero_live_blocks` and `hero_live_scratch`
   (`runtime/parts/alloc.c:61, :64`) are the whole measured race. Thread-local
   sums, an atomic, or a per-thread heap that makes the question disappear —
   design.md's invariant 1 says whichever it is, it is one edit in one file, and
   that is now checked by `tests/harness/suite_runtime.hero` rather than hoped.
3. **`hero_eq_queue`** (`runtime/parts/array.c:194-197`), which TSan did not
   reach because the probe compared nothing, and which its own comment already
   says must become `_Thread_local`.
4. **The stack guard**, which is `M-thread-stacks` and panel 107's, not this
   one: `hero_stack_lo/hi` are the main thread's, so an overflow on any other
   thread is exit 132 with an empty stderr.

## Reproducing it

Every probe is five to fifteen lines and none needs the repository. The four
`.hero` sources are quoted above in full; the C probes are the compiler's own
`--emit-c` output with `main` replaced by the block shown, compiled with
`clang -std=gnu11 -Wall -Werror -I runtime <probe>.c runtime/runtime.c`, and
`-fsanitize=thread -g` for the report in § 4.
