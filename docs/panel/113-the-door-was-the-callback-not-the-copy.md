# Panel 113 — the door was the callback, not the copy

**Convened** 2026-09-06, M-isolated-threads step 5, on the last of panel 111's
four measured corruption classes: copy-on-write's `if (refcount == 1)`, a test
followed by a mutate that step 3's atomic counter made *whole* without making
*single*. Soundness lane — no surface, no diagnostic, no spec token — **plus the
historian**, seated against the lane's normal shape for one reason: CLAUDE.md §1's
newest rule says a recommendation is a claim about the option SET, and the
convener's set was the convener's.

**Frozen at `36363aee`**, clean tree, one snapshot with a built compiler copied
per judge (panels 087 and 088's lesson).

## The proposal, verbatim as it went to the seats

1. **THE SITE.** `runtime/parts/cow.c:44`, `:78`, `:83` and
   `runtime/parts/map-write.c:127` decide whether to mutate in place by reading
   `refcount == 1`. Measured today: four sites, and the fifth grep hit is a
   comment.
2. **WHAT STEP 3 DID AND DID NOT DO.** The counter is `_Atomic` since
   M-isolated-threads step 3, so the read is whole. The pair is not atomic, so
   two threads can both read 1 and both mutate.
3. **THE PRIOR QUESTION, AND THE BRIEF DID NOT ASSERT A PROTOCOL WAS NEEDED.**
   Can a Heroes value be reachable from two threads at all, today, writing only
   Heroes? Measured for the brief: there are no mutable globals (`spec:47`); a
   top-level `constant XS: [i64]` is emitted as a FUNCTION that rebuilds the
   array on every call, with no cached global (`--emit-c` on a three-line
   program); and `selfhost/check/ffi.hero`'s `crosses_the_boundary` refuses
   `.array | .fixed | .map | .fallible`. If nothing can be shared, class 3 is not
   repaired but PROVEN unreachable and pinned with a test.
4. **OPTIONS.** (A) compare-and-exchange from 1 to a busy sentinel; (B)
   unconditional copy in `unshare`; (C) prove it unreachable and pin it;
   (D) leave `parts/thread.c`'s guard up permanently and refuse the milestone.
5. **AND ATTACK THE LIST, NOT THE OPTIONS.** Each seat was told the list is the
   convener's and asked what would have to be true for an option nobody listed to
   exist, and to go and look there before answering.

**Extended mid-sitting, 2026-09-06**, after the author ruled that the RUNTIME
carries thread creation: measured that day, `pthread.h` is absent on Windows
under clang targeting MSVC and C11's `<threads.h>` is absent from the macOS SDK
while present on Windows and glibc, so the intersection is empty and no `.hero`
program can bind threads portably. The extension is not a second sitting because
it is the same question one level up: **if the runtime creates the thread, the
runtime decides what the new thread receives**, which is the enforcement
mechanism option C was missing.

## The verdicts

| seat | verdict | measured | condition |
|---|---|---|---|
| compiler-engineer | **veto of A and B**; approves closing the channel plus C | the channel is real and it is not on the brief's list; B is quadratic | a program that corrupts through the four sites *with every reference counted* |
| historian | **objects to the SET** (advisory, no veto) | the two closest ancestors ship a mechanism that is none of A–D | a shipping runtime with refcounted COW, real threads and none of {CAS, isolation, owner tag}, still sound |
| ffi-pragmatist | **veto of B**; objects pending A; approves the runtime thread arm under one constraint | built the door and a live positive control; wrote and ran the spawn arm | the resolution carries closing the door **and** A |

## What the sitting found instead, and it is a defect

**`crosses_the_boundary` (`selfhost/check/ffi.hero:45-57`) is the single function
that decides what a C header may spell. It refuses containers at `:53` and
answers `.function_ty => return true` at `:52` — without recursing into that
function type's own parameters and result.** So a `[T]` or a `{K: V}` crosses the
FFI boundary inside a callback signature, in both directions, and the bar is one
forward declaration:

```c
struct HeroArrayHeader;
void hold(struct HeroArrayHeader *(*mk)(void), void (*run)(struct HeroArrayHeader *));
```

`heroes build … --emit-c` is **exit 0 with an empty stderr**, emitting
`HeroArrayHeader * h_t_make(void);` and `void h_t_bump(HeroArrayHeader * h0_xs);`.
Built by the compiler seat and **reproduced independently by the coordinator the
same hour**.

**The spec already refuses it**, `spec:224`: *"A callback is a **parameter**,
never a result; **its parameters follow the same rule**"* — and the same rule is
`crosses_the_boundary`, which refuses `.array`. CLAUDE.md §12: spec beats
compiler, the compiler has the bug. Filed as **defect 014** in
`docs/work/DEFECTS.md`, with its reproducer, the same day.

**And the repair costs nothing that exists.** Measured over the whole tree: the
function types inside an `extern` group are **seven, in golden cases only**, and
every one of them carries scalars — `ptr`, `i32`, `i64`, `u64`, `()`. No
`examples/` program binds a callback at all.

## What did NOT reproduce, which is the finding worth reading twice

The compiler seat patched `parts/thread.c`'s guard down **in its own copy** — the
milestone simulated — and could not corrupt anything:

| program | threads | mutations | instrument | result |
|---|---|---|---|---|
| `[i64]`, `ys @ xs; ys[0] @ 9` | 2 | 40,000 | ASan + UBSan | exit 0, 3/3 |
| `[[str]]` nested, per-step unshare | 8 | 240,000 | ASan + UBSan | exit 0 |
| `[[str]]` nested | 8 | 240,000 | **TSan** | exit 0, **0 warnings** |
| `{str: i64}` through `hero_map_unshare` | 8 | 240,000 | **TSan** | exit 0, **0 warnings** |

**The reason is mechanical and it was instrumented, not reasoned**: C's own
reference holds the count at 1, and the emitted body increfs *before* it mutates,
so `unshare` never sees the window — `C: refcount as handed over = 1`,
`C: refcount after 8 threads = 1`. The one crash the seat did produce lands in
**`hero_array_incref`, not in `hero_array_unshare`**: it needed C to release a
reference it was still lending, which is a C lifetime bug of panel 053's family
and no protocol at `cow.c:44` touches it.

**Reported as a failed search naming what was searched for** (CLAUDE.md §1), not
as an impossibility: across `hero_array_set`, `hero_array_push_owned`'s in-place
append, `hero_map_set`, nested per-step unshare, `[str]` element drops, 2 and 8
threads, ASan and TSan. Not tried: `sort` through a function pointer, the `eq` and
`hash` descriptor walks, the drop-list drain under contention.

**With the guard UP the same program is** `panic: … ran on a thread this program
did not start`, **exit 134**. So today's safety is the guard, not unreachability,
and that distinction is what the sitting exists to settle.

## Option B is not a percentage, it is a complexity change

Measured by the compiler seat, `/usr/bin/time -p`, the patch applied to all four
sites and reverted, three runs per arm:

```
baseline, n=40000    real 0.00   0.00   0.00     (below the tool's resolution)
option B, n=40000    real 7.27   7.32   7.30
option B, n=20000    real 2.08
```

Twice the data, **3.5 times the time**. `xs[i] @ v` in a loop becomes O(n²), and
removing `cow.c:78`/`:83` makes `p @ p.push(v)` O(n) per push. The compiler builds
arrays by push everywhere. The seat did not run `heroes test selfhost/main.hero`
— the snapshot's `build/` was 288K, i.e. cold, which is the twenty-minute arm and
panel 087's watchdog says no — and said so rather than reporting a number it did
not take.

## The precedent, and it is one-directional again

The historian's survey, built from three lineages reached by search rather than
recall — the FBIP/reuse literature, the C++ implicit-sharing lineage, and dynamic
object models with a shared/duplicate idiom — with the gaps it did not close
named rather than hidden (kdb+/q, Pony, `immer` transients, Julia, Mojo, Vale).

**No surveyed system shipped a bare `count == 1` reachable from two threads and
kept it.** C++ deprecated `shared_ptr::unique()` in C++17 and removed it in C++20
(P0521R0, Lavavej, 2016-11-11: *"`use_count() == 1` does not imply that accesses
through a previously destroyed `shared_ptr` have in any sense completed"*). Swift
documents the case as undefined behaviour in `isKnownUniquelyReferenced`'s own
comment, after SR-6543 (2017-12-06), which Apple called Swift's bug. CPython
turned every `Py_REFCNT(x) == 1` into a defect class the day the GIL went
(gh-140061, 2025-10-13); NumPy fixed one (PR #29685, 2025-09-05).

**And the sharper half, which bears on option C directly**: Rust HAS isolation —
`Arc::make_mut(this: &mut Self)`, statically checked — and still shipped a
soundness hole, issue #51780 (2018-06-25), from a *relaxed* load that did not
synchronise with `drop`'s release write. **Isolation of the slot is necessary and
not sufficient**; the load must synchronise with the other thread's
release-decrement.

## The option the brief did not list

Two seats named one, and they are **different mechanisms with the same letter**,
so this record spells them out rather than numbering them.

**The owner tag** (historian). The uniqueness predicate answers **false** for any
value that has escaped its owning thread. Lean 4 ships it —

```c
static inline bool lean_is_exclusive(lean_object * o) {
    if (LEAN_LIKELY(lean_is_st(o))) { return lean_internal_get_rc(o) == 1; }
    else { return false; }
}
```

— with `lean_mark_mt(o)` deep-marking the reachable graph on escape, and the
paper's own justification is a measurement: *"the additional test is much cheaper
than memory fences on modern hardware."* CPython 3.13t's
`_PyObject_IsUniquelyReferenced` is the same family, requiring owner-thread **and**
`ob_ref_local == 1` **and** `ob_ref_shared == 0`, over biased reference counting
(Choi, Shull, Torrellas, PACT '18). It is not A — no CAS on the hot path, one tag
load. It is not B — unescaped values still mutate in place. It is not C — it
proves nothing statically; it marks the values that ARE shared instead of proving
none is. These two systems are the closest ancestors on the table: refcounted,
C-emitting, admitting threads they did not start.

**Closing the channel** (compiler-engineer). Recurse `crosses_the_boundary`
through `.function_ty` into its parameters and result. **~7 lines in one file**,
`selfhost/check/ffi.hero` (420 lines), with a termination precedent on the same
interned table at `selfhost/check/table.hero:278-285`; **zero** lines in
`selfhost/emit/container.hero`, in the lowering, in the descriptors, in the
ownership pass and in the runtime. Erased in the frontend before the IR sees it,
which is design.md §1.7's own test for sugar against core. After it, option C is
true rather than hopeful, and its test is real.

## The positive control, which is what makes the negative result admissible

The ffi seat did what the compiler seat could not: it proved the instrument
alive before trusting its silence. `exp/control2.c` races `hero_array_push_owned`
by hand and ThreadSanitizer names the line:

```
WARNING: ThreadSanitizer: data race
  Write of size 8 ... #1 hero_array_push_owned cow.c:80
len 246821            (should be 400000)              exit 134, 3 races
```

Then, through the real door — 4 foreign threads plus main, 100,000 concurrent
mutating touches of one shared header — **0 races**, exit 0, three runs, for a
`[i64]`, for a `{str: [i64]}`, and for one `str` over 250,000 operations.

**And the reason is the grammar, not luck.** A callback parameter arrives
*borrowed*, and the emitted body increfs before it can become a slot:
`t1 = h0_a; hero_array_incref(t1); h1_ys = t1;` — the count is at least 2 before
any store. The two ways to skip that incref are closed by the language: `@`
inside a function type is `error[expected_type]`, so a `HeroArrayHeader **` slot
cannot cross, and writing to a callback parameter is
`error[not_mutable]: 'a' is a parameter without '@'`.

## Resolution — provisional, author ratification pending

CLAUDE.md §4: the most robust and complete resolution, never the cheapest and
never a compromise, with the conservative one recorded so the author can take it.

**R1 — The sitting's own question is refused as posed, and that is the finding.**
The brief asked which protocol repairs `cow.c`'s test-and-mutate. Two compiling
seats, independently, in separate checkouts, with a live positive control, could
not race those four sites through the only route that exists. The reachable
defect is the **door**, not the copy. Panel 111 measured the corruption correctly
and attributed it to the wrong noun; this sitting does not overturn its
measurements, it re-aims them.

**R2 — Defect 014 is repaired first, and it is eight lines.**
`crosses_the_boundary` recurses through `.function_ty` into its parameters and
its result. It edits nothing that exists: enumerated **twice, by two seats, from
the tree**, there are **7 callback declarations inside `extern` groups and 0 of
them name a container**. It makes the compiler agree with `spec:224`, which
already says a callback's parameters follow the same rule — so this is CLAUDE.md
§12's *spec beats compiler*, not a design change, and it needs no spec token. It
is erased in the frontend before the IR sees it, which is design.md §1.7's own
test for sugar against core. Owed with it: a golden under `tests/golden/check/`
with its `#~` annotation (CLAUDE.md §9), naming the container and the callback.

**R3 — Option B does not land, on two vetoes, and the number is not a
percentage.** 400,000 in-place stores under B cost **24.50 s** against **0.36 s**
for **40 million** stores today: a workload a hundred times smaller taking
seventy times longer. `xs[i] @ v` in a loop becomes O(n²) and `p @ p.push(v)`
becomes O(n) per push. The compiler is written in Heroes and stores into arrays
everywhere. CLAUDE.md §15 is past the point where this is a trade.

**R4 — Option A is deferred with a return condition, and the reason is soundness
rather than cost.** The seat that proposed pricing it measured it **cheap** —
0.37 s against 0.36 s over 40M stores, an uncontended CAS on arm64 costing
nothing over the atomic load already there — and then found it **unsound as
scoped**: the sentinel has to be held across the *caller's* mutation, because
`hero_array_set` does `drop(place); memcpy(...)` **after** `unshare` returns. So
`unshare` and `set` become one critical section, and a panic inside `drop` leaves
a block BUSY forever. A resolution that took A as briefed would have shipped a
deadlock in the name of safety. **Return condition**: a program that corrupts
memory through `cow.c:44`/`:78`/`:83`/`map-write.c:127` with **every reference
counted** — that is, without C releasing a reference it still lends. Produce it
and A lands, with the critical section widened to cover the caller.

**R5 — Thread creation is the runtime's, and the entry point is a CHECKED
ISOLATION BOUNDARY rather than a portable spelling.** This is the historian's,
it is the sitting's most important adoption, and it is the half the author's
ruling did not yet contain. Rust's `thread::spawn` requires `Send + 'static`, and
the practical effect of that signature is exactly that **the non-atomic
refcounted type cannot cross**; Ruby's `Ractor.new` calls `Proc#isolate` at
creation and raises `Ractor::IsolationError`; Erlang copies; Web Workers refuse
functions with `DataCloneError` and *transfer* an `ArrayBuffer`, which then
becomes unusable in the sender. The one surveyed system that refuses nothing at
that entry is Go, **and Go has no non-atomic refcount to protect**. In the seat's
own words: without a rule at the entry, *"the runtime creates the thread" buys
portability and buys nothing for safety*. What follows:
- the handle is an **`int64_t` table index**, never a `pthread_t` — which is an
  opaque pointer on Darwin, an `unsigned long` on glibc and a `HANDLE` on
  Windows, and one index is the same spelling on all three. Double join is then
  a loud panic instead of undefined behaviour;
- the body signature admits only what is measured safe to cross. Today that is
  the scalars and **`str`** — 250,000 concurrent operations on one `HeroStr`,
  0 races, because a literal short-circuits on a negative count and a `str` is
  immutable, so no COW path touches it;
- **`[T]` and `{K: V}` do not cross until a DEEP copy exists.**
  `hero_copy_array` is `hero_array_incref` plus a pointer store
  (`array.c:294-297`) and `hero_copy_str` is the same shape (`desc.c:86-89`), so
  a boundary copy built from today's descriptors would hand the child the
  parent's element headers — precisely the sharing this sitting exists to
  prevent. design.md Part 7.13 already says the boundary copy is *"unconditional
  and real"* and that the two copy regimes are two code paths; this is that
  sentence becoming code.

**R6 — Attachment is priced now rather than discovered later.** Every runtime in
the survey that owns thread creation had to build an attach API afterwards, for
the case of a C library calling back on a thread the runtime did not make: D's
`thread_attachThis` (which does **not** run thread-local constructors, and says
so), Go's `needm`/`dropm` (heavy enough that Go later bound the extra M to the C
thread through a `pthread_key_create` destructor), CPython's `PyGILState_Ensure`
(still required on the **free-threaded** build, despite the name), and JNI's
`AttachCurrentThread`. **Heroes already has the refusal half** —
`hero_thread_guard` and `hero_thread_claim`, panel 111 R9 — and what is owed is
the admission half. It is named here so that the milestone is not surprised by
it, and it is NOT in this milestone's scope: the guard's narrow form stays, and
a foreign thread goes on stopping by name.

**R7 — Boehm's argument is recorded because it outlives the portability one.**
Hans-J. Boehm, *"Threads Cannot Be Implemented As a Library"*, PLDI 2005: a
compiler designed independently of threading cannot guarantee the correctness of
what it emits, with speculative stores, bit-field coalescing and register
promotion as the three failure modes. Heroes emits C11 and clang supplies the
memory model — **but the thread-oblivious compiler in that paper is now Heroes'
own emitter**, and an emitter that hoists a refcount load across what is really a
synchronisation point is not saved by C11 in its output. That is the deeper
reason creation belongs to the runtime rather than to a header a program names,
and it does not expire when a platform grows `<threads.h>`.

**R8 — The owner tag is the mechanism to reach for if a second door is found, and
it is written down now so it is not re-derived.** Lean 4 and CPython 3.13t — the
two closest ancestors on the historian's table, both refcounted, both C-emitting,
both admitting threads they did not start — make the uniqueness predicate answer
**false** for any value that has escaped its owning thread. Lean:
`lean_is_exclusive(o)` is `lean_is_st(o) && rc == 1`, with `lean_mark_mt(o)`
deep-marking the reachable graph on escape, justified by measurement — *"the
additional test is much cheaper than memory fences on modern hardware."* It is
not A (no CAS on the hot path), not B (unescaped values still mutate in place),
not C (it marks what IS shared rather than proving nothing is). **Its price here
is a bit that does not exist**: `refcount < 0` already means *static literal*, so
a sign tag is taken, and a new header field moves `HERO_RUNTIME_ABI` and costs a
seed regeneration — which the ffi seat's ABI veto covers. Deferred on that price,
not on its merits.

**What the conservative resolution would have been**, recorded so the author can
take it: repair `cow.c` with option A and leave the rest alone. It changes least.
It would also have shipped a busy flag guarding a window no seat could reach,
while the channel that is actually open — a `[T]` crossing to C inside a callback
signature, at exit 0, with an empty stderr — stayed open, and while the guard
went on being the only thing between a Heroes program and that channel.

## Predictions to score at M-isolated-threads close

1. **compiler-engineer**: closing the door lands in **≤ 20 added lines confined to
   `selfhost/check/ffi.hero`** plus one golden with its `#~`; **zero** lines under
   `runtime/parts/` and **zero** under `selfhost/emit/`; `heroes test
   selfhost/main.hero` reports **583 passing with no existing golden changed**.
   Falsified if any of those four numbers moves.
2. **ffi-pragmatist**: with the door closed, `examples/sqlite/main.hero`,
   `examples/ledger/db/sqlite.hero`, `qsort`, `atexit`, `pthread_create` and
   raylib's `SetTraceLogCallback` all build at exit 0 with **zero characters
   changed**. Falsified if any `.hero` under `examples/` or `tests/golden/` needs
   one character.
3. **ffi-pragmatist**: `runtime/parts/spawn.c` lands with `HERO_RUNTIME_ABI` at
   **21** and no seed regeneration. Falsified the moment a declaration in
   `heroes_runtime.h` changes *shape* rather than being added.
4. **ffi-pragmatist**: the ten thread programs will be `i64`-in, `i64`-out **and
   that will not be enough**, because a data-parallel example wants a slice to
   cross and today's descriptors copy shallowly. Falsified if all ten need only
   scalars.
5. **historian**: Heroes will need an attach API for foreign callbacks, as D, Go,
   CPython and Java each did. Falsified if the milestone closes and the guard's
   refusal is still sufficient for every program in the corpus.

## Predictions, scored at the close — 2026-09-06

**1. compiler-engineer: FALSIFIED, and it was right about the mechanism.** It
predicted *≤ 20 added lines confined to `selfhost/check/ffi.hero`* plus a golden,
*zero under `runtime/parts/`*, *zero under `selfhost/emit/`*, and *583 passing
with no existing golden changed* — *"falsified if any of those four numbers
moves"*. Two hold and two do not. The recursion itself **is eight lines**, so the
shape was priced correctly. What moved is the blast radius, for two reasons the
seat could not have priced: the diagnostic needed a message of its own in
`selfhost/ffi_errors.hero` (+25), because `ffi_type`'s generic text ends by
listing *a function type as a parameter* among what a header may declare and
therefore contradicts itself for this case; and CLAUDE.md §11's line ceiling
forced `selfhost/check/ffi_sweep.hero` out of the file (+121, with `checker.hero`
+4). Measured `git show --stat b0d8d927`. **The two that hold are the two the
sitting cared about**: zero lines under `runtime/parts/` and zero under
`selfhost/emit/`, so the repair really was erased in the frontend, and 583 tests
pass with no existing golden touched.

**2. ffi-pragmatist: HOLDS.** With the door closed, `examples/sqlite/`,
`examples/ledger/`, `qsort`, `atexit` and `pthread_create` build with **zero
characters changed** — measured across the whole corpus: the net's `corpus` suite
is green at 54 programs and no `.hero` under `examples/` or `tests/golden/` was
edited by this repair. Verified twice: at step 5 (7 callback declarations inside
`extern` groups, 0 naming a container) and at the close.

**3. ffi-pragmatist: HOLDS.** `runtime/parts/spawn.c` landed with
`HERO_RUNTIME_ABI` at **21** and no seed regeneration for it — `cmp seed/heroes.c`
was verified identical after the runtime change, because a runtime change does
not move the emitted C.

**4. ffi-pragmatist: HOLDS, and it is the one worth reading.** It predicted *the
ten thread programs will be `i64`-in, `i64`-out and that will not be enough*. The
first half is exactly what shipped. The second half is **the surprise, and the
seat's own reasoning is what makes the scoring interesting**: it is not enough for
the reason the seat gave — a data-parallel example wants a slice, and today's
descriptors copy shallowly — and it turned out **not to matter**, because sending
the slice's INDEX and building the slice inside the thread does the same work with
none of the risk. Ten programs were written that way and not one is worse for it.
So the prediction is right about the mechanism and wrong about the consequence,
which is the more useful half to have in the record.

**5. ffi-pragmatist: NOT SCORED, and it is named rather than lapsed.**
`link "pthread"` failing on Windows with `LNK1104` was never reached, because the
resolution put threads in the runtime and no program names `pthread` at all. It
returns the day one does.

**6. llm-ergonomist: NOT SCORED — its subject did not land.** The prediction was
about generated samples under a body-scoped `$if` form, and panel 114 refused
that form. It is not falsified and not met; it waits on the form, and R7 of that
sitting is where the form's terms live.

## Author's verdict

**RATIFIED 2026-09-06 by the author, in full**, together with panel 114's: *"I
ratify all the decisions."* Given at the close, after the ten example programs had
run on all three platforms.

What was ratified, in the order it matters: that the sitting's question was the
wrong one and the record says so (R1); that defect 014 is the repair and it makes
the compiler agree with `spec:224` (R2); that unconditional copying does not land,
on two vetoes and a measured 24.50 s against 0.36 s (R3); that the
compare-and-exchange is **deferred on soundness rather than on cost**, with its
return condition written (R4); that thread creation is the runtime's and its entry
point is a checked isolation boundary rather than a portable spelling (R5); that
attachment is priced now rather than discovered later (R6); that Boehm's 2005
argument is recorded because it outlives the portability one (R7); and that the
owner tag is the mechanism to reach for if a second door is found (R8).
