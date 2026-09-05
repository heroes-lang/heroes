# 018 — Where the guard goes, and three places it does not

Date: 2026-09-05 · M-c-callbacks, before step 0 ·
**four runtimes raced, one prediction wrong, and the answer was in none of them.**

## What this is for

Panel 111 refused a milestone that would have shipped four measured classes of
memory corruption, and its R8 says the callback permission is *the door*: today
neither FFI door admits a function value, so the refusal is, by accident, the only
thing between a Heroes program and `hero_str_incref`'s race. Something has to
close the room before the door opens.

The author approved the historian's condition 3 — the **registration handshake**
CPython and OCaml have both required of a foreign thread for thirty years, and
which in CPython outlived even the removal of the global lock. This is what it
costs, measured four ways.

## Provenance

| what | value |
|---|---|
| compiler | built from `seed/heroes.c` at `844c3bbd`, `clang -I runtime seed/heroes.c runtime/runtime.c` |
| machine | the author's Mac, arm64, Apple clang 21.0.0; load 2.1–2.5, nothing stray (`uptime`, `ps -r`) |
| the benchmark | `heroes test selfhost/main.hero` — 572 tests, the largest Heroes program there is |
| method | four **separate tree copies**, identical but for `runtime/`, each built from the same seed, each run twice, interleaved control-first |
| the witnesses | panel 111's own five probes, re-run from `docs/panel/111`'s scratch |

Every arm reports `572 tests, 2 failed` — the same two, artifacts of a partial
copy (one measures `spec/heroes-spec.md`, one walks `examples/`, neither copied).
Identical in every arm, so the comparison holds.

## The guard, as prototyped

A `_Thread_local bool` set by `hero_args_set` — *"the one call every generated
`main` makes first"*, in that function's own words, and already where panel 104
put the stack guard for the same reason. Every shared structure asks it before it
is touched; a thread that did not start the program gets a named abort instead of
corruption. It lives in `runtime/parts/alloc.c`, which design.md §4.20 makes the
one place a thread question is answered, and which became true and instrumented
six hours before this measurement (`23de5b5b`).

## The race

| arm | where the guard is | pass 1 | pass 2 | against the control |
|---|---|---|---|---|
| **control** | nowhere | 34.36 s | 35.21 s | — |
| **B** | the allocator alone | 35.67 s | 34.65 s | **inside the noise**, the sign flips |
| **C** | every str/array/map, via `pthread_self()` | 42.04 s | 40.68 s | **+19%** |
| **D** | every str/array/map, via `_Thread_local` | 37.19 s | 37.00 s | **+6.6%** |

**Arm C is the finding, and it is a prediction of the coordinator's that was
wrong.** The brief for it said `pthread_self()` on arm64 is *"a register read"*
while a `_Thread_local` access *"can cost a call"*, so C would be the cheap way to
keep the wide guard. It is the **worst of the four**, nearly three times D's cost
against the same control and for the same coverage. `pthread_self()` plus
`pthread_equal` in a hot path is not free on Darwin, and the thread-local it was
meant to replace is the cheaper mechanism by a wide margin. Reasoning produced the
opposite of the truth; the only reason it is known is that both were built and run.

**Arm D is the honest cost of the obvious placement**: +6.6%, on every rebuild,
for ever. CLAUDE.md §15 is in capitals about this — *"do not make the compiler's
performance worse, that would be a tragedy"* — and it is a **runtime** cost rather
than a compile-time one, because the guard lives in the runtime that is linked
into every Heroes program. The compiler is simply the largest Heroes program there
is, which is why it is the benchmark.

**Arm B is free and buys less.** It catches what allocates — panel 111's class 2
(the process-wide comparison queue, reached from *pure Heroes with no shared
value*) and class 3 (copy-on-write) — and misses class 1, a shared refcount
touched without allocating.

## What the guard catches, where it is placed

Panel 111's five witnesses, rebuilt against arm D's runtime:

```
d_uaf32       exit 134   panic: … and touched a str.
d_race_tsan   exit 134   panic: … and touched a str.
g_end         exit 134   panic: … and touched the heap.
j_end         exit 134   panic: … and touched an array.
h_thr         exit 132   <stderr empty>
```

Four of five stop cleanly and name what was touched. Against the same programs on
the unguarded runtime, measured the same session: `g_end` under ASan reports a
finding in **10 runs of 10**, and `d_uaf32`/`d_race_tsan` under TSan in **5 of 5**.

**The fifth is not caught and the gap is real.** `h_thr` runs a recursive Heroes
function that allocates nothing, so it never reaches a guarded entry point and
dies of stack exhaustion at exit 132 with an empty stderr. That half is panel
107's and `M-thread-stacks`': the guard must read the calling thread's own bounds
and install an alternate stack **on that thread**, and on glibc no route to that
has been found. It is not a hole in this design; it is the piece that already has
a milestone.

## One number the record should keep, because it makes a test fragile

Panel 111's ffi seat reported `g_end` as **exit 139 (SIGSEGV)** on its runs. Here,
same program, same runtime, ten runs of ten: **exit 133**, every time. No cause is
asserted because none was measured. What it means is that this defect's *symptom*
depends on timing, so a `fixedbugs` case pinning 139 would be flaky and the case
must assert the ASan finding rather than the exit code.

## The answer, and it is in none of the four arms

**It came from the author's question, and the attribution is written down because
the first draft of this file left it out.** Faced with arm D's +6.6%, the
coordinator was treating the number as a single cost to be haggled over. The
author asked a different question — *"if the cost is only at compile time it is
not a big problem; if it is at runtime, then yes, let us look for the
compromise"* — and answering it required saying **where** the cost is paid.
Which required asking where the danger **enters** rather than where it shows.
The separation is the whole finding; the four arms above only priced the wrong
placements.

The guard was being put where the hazard **shows** — every string, every array,
every map, millions of times — instead of where the hazard **enters**.

A foreign thread can arrive in exactly one way: through a callback the author
declared and handed to C. There is no other door, because C can only call Heroes
code through an address Heroes gave it. And the compiler knows precisely which
functions those are: the ones whose value reaches an `extern` parameter.

So the guard belongs at the **entry of the emitted callback**, and nowhere else:

```c
void * h_main_worker(void * h0_arg) {
    hero_thread_require("a callback");   /* only in functions handed to C */
    …
}
```

- A program that passes no callback pays **the control's number**, not because the
  check is fast but because it is **not emitted into that binary at all**. The
  compiler and all 45 corpus programs are in that case.
- A program that passes one pays **one branch per callback call**, at the point
  where the danger enters.
- Coverage is arm D's — the full guarantee — at arm B's price, and better: B still
  paid a branch inside every allocation.

It is not a trade between coverage and speed. The trade only appeared because the
guard was in the wrong place, and three arms had to be built and run before that
was visible.

## What it owes when it lands

The guard cannot land before the permission, because the functions it guards are
the ones the permission admits, and the permission must not land before the guard,
because it is the door. **They are one step, and that is R8's shape rather than an
inconvenience.** With them: `c_type_of`'s `.function_ty` arm, without which the
extern's whole parameter probe is silently switched off; the `ffi_callback_type`
diagnostic class; the `ffi_type` message updated in the same commit; and the spec
sentence at the warden's shorter wording.

## Reproducing it

Each arm is `cp -R seed selfhost <tree>` plus one `runtime/`, built with the one
clang line and run with `/usr/bin/time -p ./heroes test selfhost/main.hero`. The
guard is five edits: the flag and its two entry points in `parts/alloc.c`, the
mark in `hero_args_set` (`parts/os.c`), and the call in whichever of
`hero_malloc_raw`, `hero_array_require`, `hero_str_hdr_checked` and
`hero_map_require` the arm covers.
