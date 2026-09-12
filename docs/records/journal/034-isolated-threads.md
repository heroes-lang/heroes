# 034 — M-isolated-threads: the door was in the checker

Closed 2026-09-06. Tag `m-isolated-threads`, chain row 35. Opened 2026-09-03,
re-scoped by `docs/panel/111` on 2026-09-05.

## Goal

design.md Part 7.13's model: isolated per-thread heaps, copying at the
boundaries, OS threads, **no scheduler**. Panel 111 refused the plan that opened
this milestone on two vetoes and set the scope with one line — *it delivers Part
7.13's isolation, or it delivers nothing* — after measuring **four classes of
memory corruption, each from one line of ordinary Heroes**: a shared `str` across
32 threads as `heap-use-after-free` in 9 ASan runs of 10; `a == b` on nested
arrays as exit 139 with an empty stderr; copy-on-write double-freeing; and a
worker's stack overflow as exit 132 in silence.

Three of the four are answered here. Class 1 closed at step 3, class 2 at step 4,
and class 3 turned out not to be where the corruption came from. Class 4 is
`M-thread-stacks`' by name (panel 107), and the author moved it to row 36 on the
day this closed, *because* this closed: until a Heroes function could run on
another thread, nobody could meet that defect.

## What surprised

**The sitting convened on copy-on-write could not make copy-on-write fail.** That
is the milestone's finding and it is worth stating before anything else. Panel
113 was called on `cow.c`'s `if (refcount == 1)` — a test followed by a mutate
that step 3's atomic counter made *whole* without making *single*. Two compiling
seats, independently, in separate checkouts, with `parts/thread.c`'s guard patched
down in their own copies, ran 8 threads and 240,000 mutations of nested `[[str]]`
and `{str: i64}` under ThreadSanitizer and got **exit 0 with zero warnings**. One
of them proved the instrument live in the same session with a hand-made race:
`data race … hero_array_push_owned cow.c:80`, exit 134, length 246,821 instead of
400,000. The negative is therefore admissible.

**The reason is the grammar rather than luck.** A callback parameter arrives
BORROWED, and the emitted body increfs before it can store —
`t1 = h0_a; hero_array_incref(t1); h1_ys = t1;` — so `unshare` never sees the
window. The two ways round that are closed by the language: `@` inside a function
type is `error[expected_type]`, and writing to a callback parameter is
`error[not_mutable]`.

**The door was one line in the checker, and it had been open the whole time.**
`selfhost/check/ffi.hero`'s `crosses_the_boundary` is the single function that
decides what a C header may spell. It refuses `.array | .fixed | .map |
.fallible` and answered `.function_ty => return true` **without recursing into
that function type's own parameters and result**. So a `[T]` or a `{K: V}`
crossed the FFI boundary inside a callback signature, in both directions, at
**exit 0 with an empty stderr**, and the bar was one forward declaration with no
runtime include. `spec:224` had already refused it — *"a callback is a parameter,
never a result; its parameters follow the same rule"* — so this was the compiler
disagreeing with the specification, not a design question, and it cost no spec
token to settle. Filed as defect 014 and repaired the same day.

**Panel 111 measured the corruption correctly and attributed it to the wrong
noun.** Nobody noticed for a day. What found it was a sitting convened on the
wrong question, which is an argument for convening sittings rather than for
convening the right ones.

**The atomic refcount cost +2.0% and the answer was to pay it.** Five alternating
runs per arm from a cleared cache: control 36.30/36.09/35.75, atomic
36.51/35.82/36.03 — and the ranges do not overlap. It reproduces panel 111's own
+1.7%, measured there over 20M incref pairs and here over 583 compiler tests, two
instruments and one number. CLAUDE.md §12 is what lets it land: robustness
outranks speed. Everything else in the milestone cost **+0.20%**, inside the
noise.

**Nothing in the language was needed for threads, and the sitting that asked was
convened on a silence that was a ruling.** Panel 114 refused conditional
compilation with three vetoes, **none of them the token budget** — every shape
fitted the 266-token headroom. What the author had asked for turned out to exist
already: a program ships its own header with the `#ifdef` inside, found by the
ergonomist seat reading the spec cold and measured on two platforms the same
hour. And even that was not needed: `runtime/parts/spawn.c` put the arm where
every other platform arm in this project already lives, and
`selfhost/cli/io.hero:22` had stated that doctrine since August.

**And the constraint made the ten example programs clearer rather than poorer.**
Only an `i64` crosses a thread boundary, so not one of the ten has a shared
structure to protect, a lock, or a question about who wrote last. `matmul`
allocates thousands of arrays inside a thread and drops them there; `wordbands`
builds maps and concatenates strings; `nqueens` recurses with growing arrays. In
a shared-memory model each of those wants care. Here none does — not because they
were made safe, but because the question does not exist.

## What broke and why

**Rule 3 caught its own author four times in three hours**, and that is the
milestone's second finding. `tests/harness/suite_runtime.hero` gained a sweep at
step 3 that takes the list of shared mutable state from the tree on every run and
fails in **both** directions. It then found: ten allow-list lines that stopped
matching when their objects went `_Thread_local`; **four newly shared objects
introduced by the very hook written to fix the others** — a key, a slot and two
once-guards; a step-3 unit test pinning a fact step 4 changed; and at step 6 the
spawn table and its mutex, which are shared on purpose and are the first entries
on that list not waiting for a repair. None of the four would have survived a
re-reading: the reasoning was right every time and the list it reasoned over was
short.

**The count the plan rested on was wrong, and the sweep is what said so.** The
refused plan named two shared objects and panel 111 named eighteen, because both
counted **file scope**. Measured on the first run: **28 objects, five of them
inside function bodies** — `f64.c`'s cached C locale and four `static int done =
0;` once-guards. Four are safe because of where they are called; the locale is
not, and it is reached by any program that prints a number with a point. It was
repaired with a compare-and-exchange rather than a `_Thread_local`, which would
have traded one leaked locale per race for one per thread.

**Three measurements were spoiled by the measurer**, all the same way, and the
rule that now forbids it is in CLAUDE.md §15 with the author's words. *"Look at
the machine's load first"* was obeyed — 1.89, nothing heavy — and it looks at an
**instant**. The load that ruined the runs did not exist at that instant: this
session made it afterwards, by building, by running three suites, and by syncing
the Windows box while its own comparison was in flight. One arm read **937.21 s
of wall against 33.68 s of CPU** where the other arm's comparable run was 36 s.
The full net later read **5312 s against 725 s** with the CPU unchanged, on a
machine reporting 27 million memory decompressions. Both are discarded, and both
discards are written down with their reason. The corpus suite timed alone
afterwards is 262.69 s for 54 programs with 160 s of CPU in 263 s of wall, so the
ten new programs cost about **44 seconds**.

**The tree was edited under a suite that was reading it.** `runtime/` changed
while the net was running over that `runtime/`, so some checks used the old
objects and some the new. That run was killed and redone, and the rule now binds
the tree as well as the clock.

**The seed went stale and a judge found it in passing.** Step 5 changed four
files under `selfhost/` and `seed/heroes.c` stayed where it was — 733,032 lines
against 733,838, differing at char 108818. `.github/workflows/ci.yml:604` checks
exactly that on the Linux leg of every push, so `main` was red until it was
regenerated. It was reported by panel 114's compiler seat while pricing something
else, as a failed search rather than a conclusion — *"the snapshot has no `.git`,
so I cannot say whether that is a stale seed in the snapshot or in `main`"* — and
the coordinator confirmed it in one command. The fixpoint closes again: the
compiler built from the new seed emits the new seed, byte for byte.

**Two claims in a panel brief were false and a third was worse.** The brief told
five judges that CI compares the seed *on every leg* — it is one, `if: runner.os
== 'Linux'`. It told them panel 049's return condition had opened the door, when
condition 6 unblocks the **word** `windows` and not the feature; corrected by the
judge who wrote that veto. And it called *"Heroes has no conditional
compilation"* a gap, when it is the artifact of three rulings —
`DESIGN-LOG.md:407`, `:455`, `:485` with panel 097. CLAUDE.md §1 says to grep for
the ruling behind a silence **before** convening on it, and that is the rule this
sitting broke while obeying every other one.

**A golden went in the wrong drawer and the harness explained why.** Defect 014's
case was written under `tests/golden/fixedbugs/`, which is for wrong FFI bindings
that **clang** rejects at build time — and `--emit-c` never calls clang, so every
case there emits. This one is refused by the checker before the emitter, so it
belongs in `tests/golden/check/` with an `.expected` written by hand after
reading the output, because CLAUDE.md §9 forbids regenerating a golden there.

**And a test was wrong rather than the program.** `firsthit/` first looked for a
square ending in 89, which happens four times per hundred, so every band found
one, the `NOWHERE` sentinel was never returned, and the test asserting otherwise
failed correctly. The repair was to the **program**: a sentinel nothing
exercises is a sentinel nothing checks (CLAUDE.md §9). It now looks for a
multiple of 97 and 89, and five of eight bands find nothing.

## What landed, and what carried forward

**Three of panel 111's four corruption classes are answered.** The reference
count of `str`, `[T]` and `{K: V}` is one `_Atomic int64_t` behind one typedef,
with relaxed increments and acquire-release decrements asked for by name in three
files and nowhere else. Ten runtime scratch objects are `_Thread_local`, and the
one buffer no counter weighs is given back by the thread that made it, through a
`pthread_key_create` destructor on POSIX and `FlsAlloc` on Windows — the pointer
in the key rather than in a thread-local, because panel 111 measured that Darwin
tears thread-local storage down before a destructor runs. `f64.c`'s cached locale
stops racing by compare-and-exchange.

**A Heroes function runs on a thread**, through `hero_thread_spawn` in
`hero_os.h`, the door `read_file`, `args` and `exit` already use — so it costs
**zero spec tokens**, and because that header ships with the compiler there is no
machine that can lack it. The handle is an `int64_t` table index and never a
`pthread_t`, which has three spellings and no portable one, and joining twice or
joining a handle nobody was given are named panics rather than undefined
behaviour. The bound is on threads **alive at once**, so a slot returns on join.

**The isolation is the type rule and not a promise.** The body is
`(function(i64) -> i64)`, bound through `hero_os.h`, so a `[T]` inside that
signature is `error[ffi_type]` on the author's line — measured on macOS and on
Debian. That is defect 014's repair paying for the sitting that came after it.

**Ten example programs**, thirty-nine test blocks, each teaching a different
thing: the model, uneven bands, the cost rule, a reduction that is a minimum, a
seed instead of a stream, a two-dimensional band, a thread that builds big and
returns small, splitting the output instead of the input, work as a search tree,
and strings and maps on a thread that is not the main one. **All ten run on all
three platforms with byte-identical output, all thirty-nine tests pass on all
three, and zero are skipped anywhere** — which is the author's own acceptance
criterion, measured on the string the harness skips on.

**What carries forward.** `cow.c`'s `if (refcount == 1)` stays open with a
written return condition: a program that corrupts memory through those four sites
**with every reference counted**. Option A, a compare-and-exchange to a busy
sentinel, was measured cheap (0.37 s against 0.36 s over 40M stores) and found
unsound as scoped, because the sentinel must be held across the caller's
mutation and a panic inside `drop` would strand a block busy forever. Option B,
unconditional copying, is vetoed by both compiling seats at 24.50 s against
0.36 s — O(n²), not a percentage. `parts/thread.c`'s guard stays up, and it is
what keeps any thread but the program's own out of there.

**The chain entry.** Row 35, closed 2026-09-06, tag `m-isolated-threads`. Panel
113 and panel 114 are its sittings; defect 014 is its defect; `M-thread-stacks`
moved to row 36 the same day, by author instruction, because this milestone made
that defect reachable.
