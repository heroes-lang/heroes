# Panel 111 — the emitter could already carry it, the runtime could not

Date: 2026-09-05 · convened at the opening of **M-isolated-threads** · full five
seats, differentiated inputs · **two vetoes, both on design.md §1.12**

Convened because CLAUDE.md §4 makes a sitting mandatory before the language
changes, and because `docs/ROADMAP.md` § M-isolated-threads carries panel 030
R6's standing rider: *"before M8c the record states whether the C11 backend can
express the intended concurrency model at all … if the answer is 'not yet
known', the deferral is a bet and is logged as one."*

The rider is answered — **yes**, `docs/measurements/017`, and the sitting
confirmed it from two more directions. What the sitting then found is that the
question was the wrong one to have been worrying about.

## The proposal, verbatim as it went to the seats

1. **WIDTH.** M-isolated-threads delivers **data parallelism only**: a Heroes
   function runs on an OS thread and is joined, through the FFI. No mailbox, no
   message queue, no scheduler. design.md Part 7.13's mailbox is deferred with a
   return condition.
2. **THE PERMISSION.** Lift the FFI refusal for a function value in `extern`
   **parameter** position only. `start: (function(ptr) -> ptr)` becomes a legal
   extern parameter; **result position and record-field position stay refused**.
   Measured spec cost: **+26 tokens** (3799 → 3825).
3. **THE RUNTIME.** `hero_live_blocks` and `hero_live_scratch` become
   `_Thread_local` **and** `hero_runtime_check_leaks()` gains a per-thread half,
   so a worker-thread leak is not silenced.
4. **NOT in this milestone**: isolated per-thread heaps, copying at boundaries,
   and any `spawn` form in the language.

The sharp question put to three seats: point 2 without point 4's isolation lets
an author run Heroes code on a foreign thread against a **shared** heap. Is that
a §1.12 hazard the panel must refuse, or what §1.11 already implies?

**It is a §1.12 hazard. Two seats built the program and ran it.**

## The verdicts

| seat | verdict | section | measured cost / delta | condition |
|---|---|---|---|---|
| **compiler-engineer** | **veto** (narrow, lifts on 3 conditions) | §1.12, §4.19 | permission **+32/−1, one file**, `check/ffi.hero` 240 → 266 code lines; sound landing predicted **≥150 lines across ≥4 files** | race2 green under ASan+TSan · `qsort` binding exits 1 not 2 · a written answer on `const void *` |
| **ffi-pragmatist** | **veto** on the package; **adopt** for point 2 alone | §1.12, §4.19 | permission binds **3 of 8** real callbacks; `HERO_RUNTIME_ABI` unmoved at 20 | point 2 ships alone with a new `ffi_callback_type` class · 1/3/4 wait for atomics + a COW protocol + 18 more thread-locals + a per-thread stack guard |
| **spec-warden** | adopt-with-conditions | §1.6, §1.2, §1.4 | **3799 → 3825 (+26)** verified; recommended wording **+16**, net **+8** with a measured **−8** removal | land the shorter wording · the `ffi_type` message gains "a function type" in the same commit · **split the ballot** · name the no-shared-value invariant |
| **llm-ergonomist** | adopt (no veto) | the thesis | forced guesses **13 with / 14 without**; guesses whose wrong answer **compiles**: **1 with / 3 without** | flips to object if callbacks are written at Heroes-natural widths and clang accepts them silently |
| **historian** | object → adopt-with-conditions (advisory, no veto) | precedent | no language restricts a C callback **by position**; every refcounted runtime locked, isolated, or went atomic | split by **lifetime**, not position · or make refcounts atomic · or require a registration handshake · point 3 never lands alone |

## What was measured, and it is the whole sitting

### The backend was never the problem

`function worker(arg: ptr) -> ptr` emits `void * h_main_worker(void * h0_arg)`,
which **is** `void *(*)(void *)`. Handed straight to `pthread_create` with no
cast under `-std=gnu11 -Wall -Werror`, it runs and prints from the other thread,
exit 0 — measured by the coordinator (`docs/measurements/017` § 3) and again
end-to-end by the ffi seat: `create rc=0`, `499500`, `joined, returned 0x2a`,
`RUN exit=0`, *"zero marshalling, zero boxing, zero thunk"*.

The cfront fear panel 030 R6 registered — a C emitter frozen around forms that
cannot carry non-local control flow — **does not reach this model**. Stack
switching and CPS are what green threads and coroutines need, and Part 7.13
refuses both by name (`design.md:2565-2573`). An OS thread is a C function call.

### The permission is cheap, and it is not enough

The compiler seat prototyped it: **+32/−1 in one file**. The position
distinction the proposal assumed had to be built **already exists** —
`ffi_signature` calls `crosses_the_boundary` at two separate sites and
`ffi_field` is a different function. 572 tests before and after, unchanged.

But the ffi seat compiled the callbacks real libraries take, and the permission
covers **3 of 8**:

| binding | header wants | clang |
|---|---|---|
| `pthread_create` start | `void *(*)(void *)` | **clean** |
| `atexit` | `void (*)(void)` | **clean** |
| raylib `SetSaveFileTextCallback` | `bool (*)(const char*, const char*)` | **clean** |
| `qsort` compar | `int (*)(const void*, const void*)` | `error: incompatible function pointer types` |
| `signal` handler | `void (*)(int)` | `error` |
| `sqlite3_exec` | `int (*)(void*,int,char**,char**)` | `error` |
| `sqlite3_busy_handler` | `int (*)(void*,int)` | `error` |
| raylib `SetTraceLogCallback` | `void (*)(int,const char*,char*)` | `error` |

`-Wincompatible-function-pointer-types` is a **default error** in Apple clang
21, with no `-Werror` at all. The permission covers exactly the callbacks whose
every scalar is pointer-width or absent, and §4.19 already said why: *"What the
boundary lacks is `c_int` and `const`, not a marker."*

**`qsort` does not bind**, confirmed independently by both compiling seats:
`ptr` emits `void *`, `cstr` emits `const char *`, the header says `const void *`,
and the spec has no const-pointer spelling. Meanwhile
`selfhost/emit/ctype.hero:376` promises in its own comment *"which is what makes
qsort and every raylib callback expressible."* Measured: it does not.

**And this is where three seats collided without talking to each other.** The
historian's recommended narrow landing was *"land the synchronous class now
(`qsort`, `bsearch`)"* — the callbacks whose lifetime ends with the call, which
is the only axis any surveyed language actually restricts on. Two other seats
measured that class **empty**: `qsort` is the synchronous class's headline and
it does not compile. The escape route one seat recommended was closed by two
others' measurements. That is what differentiated inputs are for.

### The permission silently switches off a check we already have

The compiler seat's witness — two programs identical but for one parameter's
declared type:

```
probe/A.hero  compar: ptr                       -> exit 1, error[ffi_parameter_type]
probe/B.hero  compar: (function(ptr,ptr)->i32)  -> exit 2, "internal error"
              grep hero_ffi_probe B.c  ->  malloc only. No qsort probe at all.
```

`emit/extern_probe.hero`'s `c_type_of` returns `fail("no C spelling")` on
`.function_ty`, so `parameter_list` fails and the whole extern's probe is
skipped. **`qsort`'s other three parameters stop being checked against
`stdlib.h`.** That file's own comment priced this exact defect once before, for
group records. Same shape, one milestone later.

Three of four adjacent shapes are then **exit 2 blaming the compiler** — which
is precisely what `selfhost/check/ffi.hero`'s preamble says the vocabulary
refusal exists to prevent.

### The runtime corrupts memory, and it takes one line of ordinary Heroes

Both compiling seats built it. Four classes, every one reachable from safe code.

**A shared `str`, 32 threads, ASan, 10 runs: 9 reported `heap-use-after-free` or
`attempting double-free`.**

```
READ of size 8 ... thread T1
    #0 hero_str_hdr_checked str.c:59
    #1 hero_str_incref      str.c:68
    #2 h_race_churn         race.c:146     <- emitted Heroes
```

Without a sanitizer the refcount drifted to **6290–9785 instead of 1**, and two
runs in twelve died with `panic: not a Heroes string block — a str was fabricated
from a foreign pointer`. **That message is a lie**: nothing was fabricated; the
block was freed under a live reference.

**`a == b` on nested arrays, 8 threads: exit 139 (SIGSEGV), stderr empty.**

```
heap-buffer-overflow WRITE ... thread T7
    #0 hero_eq_push  array.c:216
    #1 hero_array_eq array.c:231
    #2 h_geq_deep    g_eq.hero:17          <- the author's own `if a == b`
```

The compiler seat reached the same buffer from a different program and got
`heap-use-after-free` inside **`hero_grow_kept` at `alloc.c:150`** — the
function this milestone's step 0 landed six hours earlier. `hero_eq_queue`
(`runtime/parts/array.c:194-197`) is measurement 017's own item 3, it is absent
from point 3, and **its own comment predicted this**: *"the day it arrives, this
queue is shared mutable state across threads and must become `_Thread_local`."*

**Copy-on-write: `ys: [i64] @ xs` then `ys[0] @ mark`, two threads, exit 139 all
four runs**, with `attempting double-free` in `hero_array_unshare`. `cow.c`
decides whether to copy with `if (a->refcount == 1)` — a test-and-mutate that
**atomics do not fix**.

**And §1.12's own instrument dies on the worker.** The same recursive Heroes
function: `MAIN exit=134 panic: stack exhausted in hstack.down` ·
`WORKER exit=132, stderr empty`. `hero_stack_lo/hi` are the main thread's, and
`sigaltstack` is per-thread. `_Thread_local` cannot repair it: the bounds must
be measured *on* each thread, and the proposal's own premise is that no Heroes
code runs first on a foreign thread. (That half is panel 107's and
M-thread-stacks', already scheduled.)

### Point 3 is broken twice over

**Its mechanism does not work on this platform.** A `_Thread_local` counter read
from inside a `pthread_key_t` destructor reports **0** while the thread leaked:

```
worker:     self=0x16d87f000  &live=0x102d15dd0  live=3
destructor: self=0x16d87f000  &live=0x102d15dd0  live=0
```

Thread-local storage is already zeroed when the destructor runs on Darwin arm64.
A per-thread leak check *written the way point 3 describes* **can never fire**.
Both seats found this independently; both found the same working shape (the
balance in the key's own value, ~26 lines of C), and both named its two
remaining holes: the main thread's destructor never runs, and a thread still
alive at exit is never counted.

**And it names 2 of 18.** Measured across `runtime/parts/*.c`: **22 mutable
file-scope objects, 4 already `_Thread_local`, 18 not.** Point 3 names two. The
sixteen it misses include `hero_eq_queue` and its three companions,
`hero_dir_names/_count/_room` (bound at `selfhost/cli/process.hero:50-52`) and
`hero_run_words/_count` — the argv buffer for process spawn, fixed at 257 slots,
**which is on §1.0's closure list**. A raced count walks off it and `execvp`s a
half-built argv. This is CLAUDE.md §1's *repair shipped without its adjacent
shapes*, and the adjacent shape was written down in the file.

**The historian's sharpest point, which no measurement can soften**: the three
wrong leak counts are today the **only instrument reporting the underlying
race**, and point 3 removes it. *Race present, instrument silent.*

### The counter question has a cheaper, safer answer nobody had priced

The compiler seat measured **one `_Atomic` counter, gate unchanged**: 20M
malloc/free + counter pairs, both orders, `-O2` — 19.8/19.0, 18.9/17.5,
20.9/18.6, 17.3/18.8 ns. **The sign flips**; the effect is inside the noise on
the uncontended path, which is every non-threaded program's path. The ffi seat
measured the refcount side of the same question at **+1.7%** (0.88 → 0.89 s over
20M pairs).

One gate, no key, no destructor, no Windows `FlsAlloc` arm, no second allocation
site — and it **cannot make a worker-thread leak invisible**. It fixes the
counter class and **does not** fix the COW test-and-mutate, `hero_eq_queue`, or
the stack guard.

### What the document already says, and one sentence in it is false

design.md:2536 says *"no aliasing means no data race is expressible, since there
is no shared state to protect."* The historian: **true of Heroes values, false
of the refcount.** Two threads that alias no value still race on
`hero_str_incref`. The document knows this at `:2578-2586`, where it names the
two v1 invariants — a single-point allocator and a narrow never-inlined refcount
boundary — *"so no inlining can smear refcount arithmetic across code that a
thread-local counter would later have to change."* M-isolated-threads step 0
discharged the allocator half yesterday. **Nobody has discharged the refcount
half, and point 2 is what makes it load-bearing.**

### The precedent, and it is one-directional

Every refcounted runtime the historian examined did one of three things: took a
global lock (CPython 1992–2023, Ruby pre-3.0), isolated the heaps (Perl, PHP,
Erlang, Ractors), or made the counters atomic (OCaml 5, Swift, Nim's opt-in
`--mm:atomicArc`). **No case was found of shared non-atomic refcounts plus real
OS threads that ended well** — reported as a failed search, not an impossibility.

CPython's price for removing the lock is on the record: PEP 703, created
2023-01-09, Final for 3.13, requiring biased reference counting and immortal
objects at **5–6% single-threaded**; PEP 779 set phase II's bar at ≤15%. And
CPython's answer to a foreign thread calling in was never *allow it*: the
registration handshake **outlives the lock** and is still required on a
free-threaded build.

**The closest precedent is Nim, ingredient for ingredient**: C callbacks by
calling convention, reference counting, a shared heap, non-atomic counters whose
official documentation says they *"do not use atomic instructions and do not have
to"* — because entire subgraphs are moved between threads, which is the
isolation this proposal defers. What happened when threads arrived anyway:
`nim-lang/Nim#18326`, 2021-06-22, four threads, **SIGSEGV and double free**, with
`--threadanalysis:off` on its own command line. CLAUDE.md §6 says to copy Nim's
surface and never its implementation; this is the one place where copying its
**sequencing** would be copying the implementation.

Exactly one surveyed language added nothing to its type system and still got
threads: **Erlang**, and it paid with per-process heaps and copying at the
boundary — which is point 4.

## Resolution — provisional, author ratification pending

CLAUDE.md §4: the synthesis takes **the most robust and complete resolution,
never the cheapest and never a compromise**, and records what the conservative
one would have been.

**R1 — The proposal as briefed is refused.** Two vetoes on §1.12, each with a
running program. A veto is a refusal, not a price.

**R2 — M-isolated-threads delivers Part 7.13's isolation, or it delivers
nothing.** Point 4 stops being the deferred half and becomes the deliverable.
The ffi seat says it in one line: *"If the panel takes the isolation route
instead — per-thread heaps plus copy at boundaries — I withdraw my condition
entirely; that is the robust resolution CLAUDE.md §4 asks for, and it makes the
corruption unreachable rather than merely fixed."* The historian's ledger says
the same from the other side: Erlang is the one language that needed no type
system change, and per-process heaps are what it paid. The milestone's **name
stops being false** by the scope moving to meet it, rather than the name moving
(CLAUDE.md §14: an id makes no claim a later milestone can falsify — here the
same milestone was falsifying it).

**R3 — Point 1's width survives.** Data parallelism only; no mailbox, no
scheduler. design.md:2563 already says *"the first and probably only rung Heroes
needs is plain data parallelism"*, and no seat argued otherwise.

**R4 — The permission leaves this milestone and becomes its own, `M-c-callbacks`.**
It is bought entirely by §1.11 and CLAUDE.md §12's FFI-completeness instruction,
which needs no thread at all; Principle 0 binds per form, and bundled it let the
weaker ride the stronger's ticket. Three seats asked for this split
independently. What that milestone owes, all measured here:
- the **`c_int`/`const` vocabulary**, or a design.md Part 8 wart naming
  const-qualified pointee parameters as the known hole with its return
  condition — because without it 5 of 8 real callbacks do not bind and
  `ctype.hero:376`'s promise about `qsort` is false;
- **`c_type_of` gains its `.function_ty` arm**, so the extern's probe is not
  silently switched off for every other parameter;
- a **new diagnostic class** — `ffi_callback_type`, exit 1 on the `.hero` line —
  so a wrong callback signature is the compiler's message and not clang's
  internal error at exit 2;
- the **`ffi_type` message gains "a function type"** in the same commit, or the
  spec and the compiler contradict each other on day one;
- the spec sentence at the **spec-warden's shorter wording**, not the
  proposal's: it states the parameter-only restriction that the proposal's
  wording never mentions, at **+16** against +26, and the measured **−8**
  removal of `Neither may be an absolute path.` brings it to **+8 net**. The
  proposal's colon was orphaned — it introduced a spelling and pointed at an
  example containing no function pointer.
- the ergonomist's second sentence, on the callback's own parameter widths and
  `(function() -> ())`, is **owed or the restriction is under-specified**; it is
  the only thing that makes `atexit` writable from the document alone.

**R5 — Point 3 does not land in any form.** Its mechanism is measured broken on
this platform, it names 2 of 18, and it would delete the only instrument
currently reporting the race. Where a counter answer is needed, the measured
candidate is **one `_Atomic` counter with the gate unchanged**, inside the noise
on the uncontended path.

**R6 — The refcount half of design.md's own v1 invariant is now open work.**
`:2578-2586` names it; step 0 discharged the allocator half; nothing has
discharged this one. It is scheduled at M-isolated-threads because that is where
it becomes load-bearing.

**R7 — design.md:2536 is corrected.** *"No aliasing means no data race is
expressible"* is true of Heroes values and false of the refcount, and the
sentence is exactly the one a reader would use to conclude a worker thread is
safe. `spec:47`'s *"There are no mutable globals"* is the spec-side twin. Both
are language documents, so both ride a sitting; this is it.

**R8 — the permission is the door, so it does not land before the refcount is
atomic.** This is the coordinator's, found reading the sitting back rather than
raised by a seat, and it is written here because leaving it out would leave the
next reader to fall into it. Today **neither door admits a function value**, so
no author can start a thread at all: the FFI refusal is, by accident, the only
thing standing between a Heroes program and `hero_str_incref`'s race. R4 removes
it. After that, `extern "pthread.h" link "pthread"` plus one deliberate line
binds `pthread_create` — and what corrupts next is not the author's C, it is
**Heroes' own runtime**, which is exactly the distinction §1.12 draws when it
calls not corrupting memory *"a goal of the language, not a quality of its
implementation."*

The ffi seat attached the atomic refcount and the copy-on-write protocol to
points 1/3/4. They attach to the permission too, for the same measured reason,
and the price is already known: **+1.7%** on the refcount (0.88 → 0.89 s over
20M pairs) and a sign that flips inside the noise on the allocator's counter.
`cow.c`'s `if (a->refcount == 1)` is a test-and-mutate and needs a protocol
rather than an atomic, which is the harder half and the one that must not be
forgotten because the cheap half is easy.

So `M-c-callbacks` carries both as landing conditions, or it lands behind
whatever does. What it must not be is a milestone that ships the door and leaves
the room unsafe — which is the shape this whole sitting refused one level up.

**What the conservative resolution would have been**, recorded so the author can
take it: adopt nothing, leave M-isolated-threads open and unscoped, and let the
next sitting re-derive all of this. It changes least. It also leaves four
measured classes of memory corruption undocumented in the record and a milestone
whose name is falsified by its own plan, which is why §4's *robust over
conservative* rule exists.

## Appended 2026-09-05, after ratification — R9, and it is not covered by the yes above

The author ratified R1 through R8. **R9 was proposed and approved separately, the
same day**, and is written here rather than folded into the resolution because a
record that lets a later approval look like an earlier one is a record that cannot
be used to check anything.

**R9 — a foreign thread is refused by name, and the refusal is emitted into the
callback rather than built into the runtime.** This is the historian's condition
3, the registration handshake CPython and OCaml have both required for thirty
years and which in CPython outlived the removal of the global lock. The author's
approval, in English: *"yes"*, to the proposal that the runtime notice a thread it
does not know and stop cleanly instead of corrupting.

**It replaces R8's mechanism and keeps R8's rule.** R8 said the permission must
not land before the refcount is atomic. R9 is stronger and cheaper: atomics repair
class 1 and leave classes 2, 3 and 4 open, while the handshake makes all four
**unreachable** — a program that would corrupt stops and says what it touched.
§1.12 asks that a Heroes program not segfault and not corrupt memory; a named
abort is that promise kept, not an exception to it.

**The placement is the author's, out of a question rather than an instruction.**
Shown arm D's +6.6%, the author asked *"if the cost is only at compile time it is
not a big problem; if it is at runtime, then yes, let us look for the
compromise"* — and answering that meant saying where the cost is paid, which
meant asking where a foreign thread *enters* rather than where the corruption
shows. The coordinator had been pricing three wrong placements; the question is
what moved the guard to the right one, and it is recorded here because the first
draft did not.

**And `docs/measurements/018` is why it is emitted rather than installed.** Four
runtimes were built and raced on `heroes test selfhost/main.hero`: the guard at
every str/array/map costs **+6.6%** through a `_Thread_local` and **+19%** through
`pthread_self()` — the coordinator predicted the second would be the cheap one and
was wrong by a factor of three, which only building both revealed. At the
allocator alone it is free and buys less. **In the emitted callback's entry it is
free where it does not apply**, because it is not emitted into a binary that
passes no callback — which is the compiler and all 45 corpus programs — and it
sits exactly where the danger enters, since C can reach Heroes code only through
an address Heroes handed it.

Measured on this sitting's own five witnesses: four of five stop with a message
naming what was touched, against `heap-use-after-free` in ten ASan runs of ten
before. **The fifth is not caught and stays open by name**: a recursive Heroes
function that allocates nothing never reaches a guarded point and dies of stack
exhaustion at exit 132 with an empty stderr. That is panel 107's half and
`M-thread-stacks`', which exists for it.

**What this makes one step rather than two**: the guard cannot land before the
permission, because the functions it guards are the ones the permission admits,
and the permission must not land before the guard, because it is the door. R8's
shape survives; only its mechanism changed.

## Author's verdict

**Ratified 2026-09-05 by the author**, in full: *"and I also ratify R1 to R8"* —
R1 through R8 as they stand, including the refusal of the proposal this sitting
was convened to adopt.

**And R4's one undelegatable half was answered in the same breath.** The sitting
proposed `M-c-callbacks` and left its placing to the author, because
`docs/work/SCHEDULED.md` puts a new id's position there. The author placed it
**at row 34, ahead of M-isolated-threads**: *"can we put M-c-callbacks in right
away? I would not wait too long to do it."* The chain renumbered 34..55 to
35..56, and the coordinator's recommendation — callbacks first, isolation second,
because the permission is unblocked and the isolation is architectural — was the
one taken.

What the yes settles: the proposal is refused and does not return in this shape;
M-isolated-threads' deliverable is the isolation itself; the callback permission
is a milestone of its own and carries R8's atomic refcount and copy-on-write
protocol as landing conditions; and the thread-local counter plan is dead in
every form, not deferred.

What it does not settle: the five predictions below are unscored, and each names
the milestone that scores it. R7's correction to `design.md:2536` and the spec
sentence R4 owes are language changes that ride this sitting's authority but have
not been written yet.

## Predictions to score

| seat | prediction | scored at |
|---|---|---|
| compiler-engineer | if the permission lands as +32/−1 with no new reader under `selfhost/emit/`, then `grep -c hero_ffi_probe` over any extern with a function-typed parameter returns **0 for that extern**, and a `qsort` golden exits **2** not 1. The sound landing is **≥150 code lines across ≥4 files**; falsified if the reader lands under 95 code lines and the `qsort` probe still exits 1 | M-c-callbacks close — **SCORED 2026-09-05 at M-c-callbacks close — RIGHT about the size, antecedent never held.** The landing is **171 code lines across 12 files** under `selfhost/`, **121 across 8 files** under `selfhost/emit/`, and it DID bring a new reader (`emit/callback_guard.hero`, 73 code lines) — so ≥150/≥4 holds and the falsifier's *under 95* conjunct is false. The consequent is false anyway: `qsort` is **exit 1**, `error[ffi_callback_type]`, on the author's line. `docs/journal/033-c-callbacks.md` § Predictions, scored |
| ffi-pragmatist | with the permission and no shim, **all five** of `sqlite3_exec`, `sqlite3_busy_handler`, `signal`, `qsort`, raylib `SetTraceLogCallback` are `error: incompatible function pointer types`, and exactly three compile. Second: a `qsort` binding exits **2**, not 1, because `ffi_declared.hero` classifies neither an argument-position function-pointer mismatch | M-c-callbacks close — **SCORED 2026-09-05: RIGHT on the count, wrong on the membership, wrong on the mechanism.** Measured over **all eight**, one probe each. *Exactly three compile*: RIGHT — `atexit`, `pthread_create`, `sqlite3_busy_handler`. But `sqlite3_busy_handler` is on this seat's own list of failures and **binds at exit 0**, and the fifth failure is `nftw`, unnamed here: one swap. *All five are `error: incompatible function pointer types`*: FALSE — every refusal is this compiler's message at **exit 1** on the author's line (`ffi_callback_type` for `qsort`, `sqlite3_exec`, `nftw` and `SetTraceLogCallback`; `ffi_type` for `signal`'s result position), so no probe reaches clang's text at exit 2 and the second half falls with it. raylib's row was reached through `extern "raylib.h" package "raylib"`, its cause being that `va_list` is `char *` on Darwin arm64 and a non-const `char *` has no spelling. `docs/journal/033-c-callbacks.md` § Predictions, scored |
| historian | with points 1/2/3 and not 4, a four-thread program whose start routine only **reads** a pre-existing `str` reports a TSan data race in `hero_str_incref`/`decref` **while `hero_runtime_check_leaks()` reports zero leaks on the same run**. Falsified if TSan is clean over 1000 runs, or the leak checker still fires | M-isolated-threads close |
| llm-ergonomist | under the document without the sentence, ≥60% of first-try attempts declare the callback `ptr` and ≥30% pass `nullptr`, giving a program that compiles at exit 0 and dies on a null function pointer; first-try working rate **0%**. Under the document with it, **0%** declare it `ptr` | M-thesis-harness (metric 2) |
| spec-warden | at M-isolated-threads close, `heroes measure spec/heroes-spec.md` reads **≤ 3830** — i.e. the callback permission needs no further spec text. It exceeds that if the callback's own **result** width needs stating, since *"a result may be wider than C's"* **reverses** for a function C calls back | M-isolated-threads close |

## What the brief got wrong, collected

Five, and every one was caught by a seat rather than by the convener. This is the
fourth sitting in a row where that has happened, and the count is kept because
CLAUDE.md §1 exists for exactly it.

1. **"Does the emitter need anything at all? No."** False. `c_type_of` refuses
   `.function_ty` and drops the probe for the whole extern.
2. **"Result and field position stay refused"** is the wrong axis. No surveyed
   language restricts a C callback by position; they restrict by type or by
   lifetime. Field position is *already* refused in writing at zero tokens — the
   spec's field list is closed — so the brief's "the spec is silent" was
   half false.
3. **Point 3's mechanism** does not work on Darwin arm64, and it names 2 of 18.
4. **The orphaned colon**: the proposed sentence ends `...top-level function:`
   and the example under it contains no function pointer.
5. **CLAUDE.md §7's "Four classes now"** for exit-1 clang failures is stale —
   measured today there are **five**; `extern function puts(s: i64)` gives
   `error[ffi_parameter_type]` at exit 1. Unrelated to this sitting, filed here
   because a seat measured it.

One more the seats put as a **question rather than a premise**, correctly: on
glibc `pthread_t` is an `unsigned long`, not a pointer, so `@t: ptr` against
`pthread_t *` may not compile at all on the Linux leg. No seat had a Linux box.
CLAUDE.md's three-platform rule makes it owed before any commit.

## Process notes

- The frozen snapshot shipped **without `vendor/tokenizers/`**, so `heroes
  measure` exited 2 until the spec-warden copied it in. The next brief that
  hands out a snapshot includes it.
- The working tree was frozen from the briefs going out to this synthesis, and
  all four seats that could write confirmed the repository was untouched.
- Two seats reached the same `hero_eq_queue` corruption from different programs,
  and two reached the same broken `pthread_key_t` mechanism independently. That
  is what differentiated inputs buy, and it is why the soundness lane would have
  been the wrong call here: the ergonomist's blind A/B and the historian's Nim
  ledger are each load-bearing in the resolution above.
