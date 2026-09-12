# M-isolated-threads — the isolation, or nothing *(closed 2026-09-06)*


**OPEN 2026-09-03, re-scoped by `docs/panel/111` on 2026-09-05.** design.md Part
7.13 — isolated per-thread heaps, copying at the boundaries, OS threads, **no
scheduler**. Its width is settled and unopposed: **data parallelism only**, which
is the design's own words at `:2599` (*"the first and probably only rung Heroes
needs"*); the mailbox stays deferred.

**Panel 030 R6's rider is answered, and the answer is yes.** The C11 backend can
express the model: a Heroes function emits exactly the C `pthread_create` wants
and runs on another thread at exit 0 (`docs/measurements/017`). Stack switching
and a CPS transform are what green threads and coroutines need, and Part 7.13
refuses both by name. cfront's fate does not reach this architecture, and the
deferral was never a bet.

**What the sitting found instead is that the deferred half was the deliverable.**
The proposal that opened this milestone — threads now, isolation later — was
**refused on two vetoes**, each with a running program: a shared `str` across 32
threads is `heap-use-after-free` or double-free in **9 of 10** ASan runs; `a == b`
on nested arrays is exit 139 with an empty stderr; copy-on-write double-frees; and
the stack guard is main-thread-only, so a worker overflow is exit 132 in silence.
Four classes, each from one line of ordinary Heroes. **So this milestone delivers
Part 7.13's isolation or it delivers nothing** — the route both compiling seats
accept, and the only one with a precedent: Erlang is the single surveyed language
that needed no type-system change, and per-process heaps are what it paid.

**The refcount half of design.md's own v1 invariant (`:2618-2624`) closed at step
3**, 2026-09-06, the way the document promised it would: one edit, because the
boundary was narrow and never inlined. It cost a measured **+2.0%** on the
compiler's own test suite, five alternating runs per arm from a cleared cache,
which is the sitting's own +1.7% reproduced on a different workload, and CLAUDE.md §12 is what lets it land — robustness outranks
speed, and this closes the class panel 111 measured at nine ASan runs in ten.

**The scratch the runtime keeps between calls closed at step 4**, the same day.
Ten objects are per-thread — `array.c`'s comparison scratch, `dir.c`'s listing,
and `run.c`'s argv buffer, which is on §1.0's closure list — and the buffer
neither counter weighs is given back by the thread that made it, through a key
whose destructor `parts/alloc.c` owns. `f64.c`'s cached C locale stops racing by
compare-and-exchange. **Cost +0.20%, inside the noise.** That is panel 111's
class 2 closed, and class 1 went at step 3.

**What is left is one line, and the sweep that says so is an instrument rather
than a paragraph.** `tests/harness/suite_runtime.hero`'s rule 3 takes the list
from the tree on every run and fails in both directions: **32** objects in
`runtime/` survive between calls — 15 `_Thread_local`, 3 `_Atomic`, **14 still
shared**, six of them inside function bodies. Ten of the fourteen are
M-thread-stacks' or are safe by where they are called, and every one carries its
reason in the file. **The one that matters is `cow.c`'s `if (refcount == 1)`**, a
test and then a mutate that no memory order can close — panel 111's class 3, the
last reason `parts/thread.c`'s guard cannot come down, and now its own item in
`docs/work/SCHEDULED.md (retired 2026-09-12)`.

**And the instrument has already earned itself.** Rule 3 landed at step 3 and
caught its own author three times in the three hours after: ten allow-list lines
that stopped matching when their objects went per-thread, four newly shared
objects introduced by the repair itself, and a step-3 unit test pinning a fact
step 4 changed. None would have survived a re-reading, because the reasoning was
right each time and the list was short.

*******************************************************************************
**OPEN: 0**

*******************************************************************************
