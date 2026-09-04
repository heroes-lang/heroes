# 106 — The frame is the sweep's own temporaries, not the slots

**Soundness lane** (compiler-engineer + ffi-pragmatist), 2026-09-03, convened by
M-corpus-depth. No surface, no diagnostic, no spec token — which is what the lane
is for (CLAUDE.md §4; `/panel` § Two lanes). **RATIFIED 2026-09-04** by author
instruction — the verdict section at the foot of this file carries the words and
what the yes settles.

**Both seats vetoed the proposal, and the second one found the answer the
proposal was reaching for.** That is the whole sitting in one line.

---

## The proposal, as put

> Two slots of the same C type whose live ranges do not overlap may share one
> declaration in the emitted function prologue. Nothing else changes: the
> prologue stays (a `goto` may not jump over an initialisation), nothing is
> initialised except a refcounted slot's `= {0}` (panel 021 R3), one `goto`+label
> per basic block stays.

## Why it was asked

Measured by the coordinator before the briefs went out, on this Mac (Apple clang
21.0.0, arm64), over `examples/interpreter/` — the eleven-module tree-walking
interpreter M-corpus-depth step 5 landed:

| what | measured |
|---|---|
| largest emitted frame (`-fstack-usage`, `-O0`) | `h_runexpr_combined` **112,704 bytes** |
| next two | `h_runexpr_joined` 82,000 · `h_synparse_statement` 47,584 |
| one nesting level of the grammar | **36,896 bytes** (compared 9,680 · named 10,288 · primary 7,232 · unary 6,800 · grouped 2,896) |
| all 342 functions | **639,792 bytes** |
| independent cross-check | 8 MiB stack ÷ measured `-O0` ceiling 191 = **43,919 bytes per level** |
| nesting ceilings, binary-searched, all exit 134 with the guard naming the function | `--sanitize` **104** · `-O0` **191** · `-O2` **349** |

M-corpus-depth's plan asked for a 500-deep expression and settled for 60;
`examples/interpreter/run/eval.hero`'s own script-recursion guard sits at 24 for
the same reason. **Author instruction 2026-09-03: the deliberately low limits are
to be raised at their cause and not worked around** — and, given mid-sitting to
both seats, *use always the most robust and safe solution, never the more
economical and compromise one.*

---

## The verdicts

| judge | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **veto** | design.md §1.7 (the ceiling), §1.12 (the alternative), panel 021 R3 as quoted at `selfhost/emit/body.hero:10-13` | proposal ≈200 lines for **0.05%**; the alternative ≈**+25 code lines** over 20 files | at M-corpus-depth close, with `decref_place` landed and sharing not landed, the interpreter's `-O0` ceiling measures **≥ 300** (it measured 311) and `--emit-c` emits **≤ 60,000 lines** (it measured 56,148, from 69,052). Conversely: sharing alone leaves the ceiling **below 210** | withdraws the veto on a corpus program where sharing *plain* slots saves **more than 5%** of that function's `-fstack-usage` (its ceiling over the whole interpreter is 0.05%), or if the sweep is redesigned first |
| ffi-pragmatist | **veto** | design.md §1.11, §4.19, under CLAUDE.md §12's *"reaches furthest at the C boundary"* | after excluding what it proved must be excluded, **476 of 91,460 bytes — 0.52%** | `examples/sqlite/main.hero` extended to ladder rung 4 (`sqlite3_bind_text(..., destructor: nullptr)`) prints **`length: 0`** instead of `length: 53`, **at exit 0**, under the proposal — and prints `length: 53` again under `--sanitize` | lifts it only for a **whole-function gate**: no refcounted slot shared, and sharing only in a function whose emitted body calls nothing outside `hero_*`/`h_*`/`__builtin_*`, no indirect call, no `hero_str_cstr` — plus a run golden per FFI shape, and Principle 0 re-satisfied after the gate |

### What the FFI seat compiled

Three findings, none of them reasoned:

- **A string lent to C is freed while C still points at it.** A `str` bound with
  `SQLITE_STATIC` — a null destructor, so SQLite keeps the caller's bytes — and
  then not read again. Under the proposal with the exit sweep de-duplicated, the
  program answers **`length: 0` at exit 0**: the store expansion decrefs the
  previous occupant, the block is freed, and SQLite reads it. Under `--sanitize`
  the same build answers **`length: 53`**, correctly — **the sanitiser hides the
  defect**, because the read happens inside the uninstrumented system library.
  Verified in plain C with no Heroes at all, to prove the premise rather than the
  emitter: `free(buf)` after `bind_text(..., SQLITE_STATIC)` prints a length,
  exit 0, ASan silent.
- **An address C retains outlives every use the IR can see.** `setvbuf(3)` holds
  the buffer's address until `fclose`, and the emitter already marks that slot
  `__attribute__((unused))` in the same prologue. Sharing it: **SIGSEGV, exit
  139, no Heroes message** — and the stack guard (panel 104) correctly stays
  silent, because it is not a stack fault. §1.12's promise breaks with nothing
  said.
- **What is *not* at risk, checked rather than assumed**: sizes and alignments
  (sharing demands the same C type, so `sizeof`/`_Alignof` are identical by
  construction — `struct tm` 56/8, `i32[4]` 16/4, a by-value record 24/8); a
  `partial` record (built with a designated compound literal, so C11 6.7.9p21
  zeroes the unnamed members — measured over 0xAB-filled storage); and the
  **ABI**, which does not move: sharing lives inside one generated body.

### What the compiler seat measured

**The proposal is aimed at 9.6% of the frame and delivers at most 0.05%.**
Every declaration in the interpreter's emitted C, weighted by a compiled
`sizeof` probe:

| | declarations | bytes | share |
|---|---|---|---|
| total prologue locals | 13,814 | 491,393 | 100% |
| author/synthetic **slots** | 1,340 | 47,051 | **9.6%** |
| SSA **temporaries** (`tN`) | 12,474 | 444,342 | **90.4%** |
| refcounted (`= {0}`) | 12,078 | 480,800 | **97.8%** |

In `h_runexpr_combined`, slots are **2,849 of 91,460 bytes — 3.1%**. And
**98.2% of slot bytes are refcounted**, which cannot be shared at all:
`selfhost/ir/own.hero:257-262` reads every swept slot at every returning block,
so no two of them are ever disjoint. Merging a pair anyway is a **heap-use-after-free
at exit 134**, run under ASan. Making them mergeable means a liveness-directed
sweep — which is panel 021 R3, reopened without saying so, and the refusal is
quoted in the very module the proposal edits.

**The bottleneck is the sweep's own load temporaries: 58.5% of all declared
frame bytes.** `own.hero:259` mints a fresh SSA value per swept slot per
returning block, `t203 = h6_f0; release(&t203);` — and every one is
**address-taken**, so clang's mem2reg can never promote it and it is pinned to
the frame for the whole function. 6,452 such pairs in the interpreter; 86.7% of
`h_runexpr_combined`'s declared bytes.

Removing them, measured by rewriting the emitted C and rebuilding:

| | -O0 | -O2 |
|---|---|---|
| interpreter frames | 639,792 → **305,008 (−52.3%)** | 349,776 → **130,304 (−62.7%)** |
| `h_runexpr_combined` | 112,704 → **17,744** | 74,800 → **9,472** |
| one grammar nesting level | 36,896 → **21,120** | 20,032 → **10,960** |
| **nesting ceiling** | **191 → 311** | **347 → 581** |
| the compiler's own frames | 6,563,376 → **4,707,024 (−28.3%)** | — |
| the seed | 846,804 → **786,062 lines (−7.2%)** | — |

Correctness, all run: output byte-identical at exit 0; **ASan and UBSan clean**;
the full flag set including `-Werror=uninitialized -Werror=conditional-uninitialized`
with **zero warnings**; and **a compiler linked from the rewritten C compiles
`selfhost/main.hero` and emits byte-identical C** — 30,371 pairs removed across
2,907 functions, the fixpoint preserved, which subsumes the double-emit
determinism check.

**And no liveness pass is needed for it.** The temporary is produced and consumed
by adjacent instructions in one block *by construction*, which is why a purely
syntactic rewrite reached the fixpoint.

**Does clang already do this at `-O2`?** No, and it cannot. Program-wide it
recovers 45.3% by register promotion, not by coalescing; an address-taken
top-scope declaration is never promoted and never merged. The isolated witness,
compiled: four same-typed disjoint address-taken locals at top scope give a
192-byte frame at `-O2`, and the same four in nested blocks give **96 bytes at
both `-O0` and `-O2`**. So the repair is **additive on top of `-O2`**.

---

## Where the seats disagreed

**They did not, on the verdict** — and that is worth stating rather than
smoothing, because it is the lane's whole risk: two judges who agree may be two
readings of one thought. Here they are not. They vetoed **different halves**:

- the FFI seat vetoed on **soundness** — sharing breaks values that C holds, and
  it proved it with two compiled programs, one of which the sanitiser hides;
- the compiler seat vetoed on **vacuity** — after the exclusions the FFI seat
  proved necessary, the proposal is worth 0.05%, and 200 lines including the
  compiler's first CFG fixpoint.

Either veto alone refuses the proposal. Together they also agree on the number:
the FFI seat's 0.52% and the compiler seat's 0.05% are the same conclusion from
two different exclusion sets, and both are far under any threshold worth 200
lines. **The one thing the FFI seat's condition asks that the compiler seat's
answer does not need**: a whole-function gate over calls into C. `decref_place`
needs no gate, because it removes a temporary rather than sharing storage — no
address is reused, no lifetime changes, and the FFI seat's two failing programs
are untouched by it.

---

## The resolution — provisional, and the robust one rather than the conservative one

**CLAUDE.md §4 says a synthesis adopts the most conservative resolution. The
author amended that on 2026-09-03, mid-sitting**: *use always the most robust and
safe solution, never the more economical and compromise one.* The conservative
resolution here is *do nothing* — both seats vetoed, so the proposal dies and the
ceilings stay where they are. That is refused, and this is what is adopted
instead:

1. **Slot sharing does not land, in any form.** Not gated, not narrowed, not
   for plain slots only. It is unsound where C holds a value, and where it is
   sound it is worth 0.05%. The FFI seat's gate is not adopted either: a
   whole-function condition that must be re-checked on every emitted body is a
   second invariant to keep true for a half-percent.
2. **The sweep's load temporary goes, and that is M-corpus-depth's next step.**
   A new IR op — `decref_place`, minted by `selfhost/ir/own.hero`, consumed by
   `selfhost/emit/inst.hero`, emitting `release(&slot)` directly — so the frame
   loses 58.5% of its declared bytes and the recursion ceiling roughly doubles.
   It is not a language change: no token, no grammar rule, no spec sentence, no
   §1.7 core construct, no type-checker arm.
3. **The store's `old` temporary (`own.hero:150`) is measured and filed, not
   done in the same step.** It is 9.1% more, and it genuinely needs a temporary —
   the store sits between the load and the decref — so removing it needs a
   generated per-type `assign(&slot, &value)`. Refcount plumbing is 67.6% of
   declared bytes in total; this step takes the 58.5% that needs no analysis, and
   the rest is a separate question with its own price.
4. **The interpreter's two low limits are raised to what the repair measures**,
   in the same milestone and never ahead of it: the nesting test at 60 and
   `run/eval.hero`'s `DEPTH` at 24 are re-derived from the ceilings measured
   after the repair, in all three configurations, with the numbers written into
   the program as they are today.
5. **Panel 021 R3 stands.** Nothing here makes the exit sweep liveness-directed;
   it makes the sweep cheaper to emit. The zeroed refcounted slot and the
   unconditional sweep are exactly as ratified.

**What a veto of this resolution would compel**: if the author refuses
`decref_place`, the ceilings stay at 104/191/349 and M-corpus-depth's 500-deep
requirement is closed as *refused with a measured reason* rather than met — the
programs keep their limits and the comment explaining them becomes permanent
rather than provisional. That is a legitimate outcome and it is the one this
sitting exists to make visible.

---

## Predictions to score

| origin | prediction | checkable at |
|---|---|---|
| compiler-engineer | with `decref_place` landed and sharing not landed, the interpreter's `-O0` nesting ceiling is **≥ 300** and `--emit-c` emits **≤ 60,000 lines** (it measured 311 and 56,148 in its copy) | M-corpus-depth, the step after this sitting |
| compiler-engineer | if slot sharing landed **alone**, the `-O0` ceiling would stay **below 210** | never, unless the author overturns resolution 1 — recorded as the counterfactual it is |
| ffi-pragmatist | `examples/sqlite/main.hero` at ladder rung 4 under the proposal prints `length: 0` at exit 0, and `length: 53` under `--sanitize` | never, unless resolution 1 is overturned; the reproducer is kept |
| ffi-pragmatist | no implementation of the proposal keeps that program correct while sharing more than **532 of `h_runexpr_combined`'s 91,460 declared bytes** | as above |
| coordinator | after `decref_place`, `examples/interpreter/`'s own nesting test moves from 60 to a number **at least three times** it, measured in all three configurations rather than chosen | M-corpus-depth close |

---

## Author's verdict

**Ratified 2026-09-04** (author instruction, *"ratify and push"*, given after
the repair was built, measured and shipped).

**What the yes settles**: slot sharing is refused permanently and in every form
— not gated by the ffi-pragmatist's whole-function condition, not narrowed to
plain slots — and `decref_slot` is the shape this IR has: an op that names a
slot rather than a value. It was already built when the yes came, so what the
ratification settles is not whether to do it but whether it was right to: the
frames −54.0% on `examples/interpreter/` and −35.6% on the compiler itself, the
recursion ceiling 191 → 314 at `-O0`, the seed 846,804 → 721,238 lines, the
fixpoint holding byte for byte, 560 own tests and 1,474 net checks green.

**What it does not settle**, unchanged from what this section said while it was
pending: the store's own `old` temporary (`own.hero:150`, 9.1% more of declared
bytes), which needs a generated per-type `assign(&slot, &value)`; and whether
the exit sweep should ever become liveness-directed, which is panel 021 R3 and
stays ratified as it stands.

**And one thing the yes exposed rather than settled.** The sitting's whole
subject was the recursion ceiling, and the repair roughly doubled it — but the
milestone's 500-deep requirement is still unmet at 83, because the corpus runs
`--sanitize` and that configuration tops out at 166. Measured the same day the
ratification came, on this Mac, and it changes what that failure means: the
same interpreter linked with **64 MB of stack** — the number panel 058 ratified
for Windows in August, which this compiler already passes on that platform and
on no other — reaches **2,537 levels at `-O0` and 1,347 under `--sanitize`**,
against 314 and 166 with the 8 MB the operating system hands a program here. So
the ceiling is not the frames and it is not the language: it is a link-line
decision this compiler makes on one platform out of three. That is a question
for a sitting of its own, filed in `docs/work/DECIDE.md`, and it is **not**
this one's to answer.

**What a yes settles**: that slot sharing is refused permanently and in every
form — not gated by the ffi-pragmatist's whole-function condition, not narrowed
to plain slots — and that `decref_place` lands as M-corpus-depth's next step, in
its complete form: the IR op, the emitter arm, the `uses.slots_of` line that
keeps every swept slot counted as read, the DECIDED-table raises the added lines
need, the 86 re-blessed emission traces with the diff read (§9), the seed
re-baselined, and the fixpoint checked byte for byte. It also settles that the
interpreter's nesting test and its `run/eval.hero` `DEPTH` are re-derived from
the ceilings measured **after** the repair, in all three configurations — the
milestone's 500-deep requirement is met if the measurement allows it and closed
as refused-with-a-reason if it does not.

**What it does not settle**: the store's `old` temporary (`own.hero:150`, 9.1%
more of declared bytes), which needs a generated per-type `assign(&slot, &value)`
and is a separate question with its own price; and whether the exit sweep should
ever become liveness-directed, which is panel 021 R3 and stays ratified as it
stands. A **no** keeps the ceilings at 104 / 191 / 349, keeps the two low limits
where they are with their measured comments, and closes M-corpus-depth's
500-deep requirement as refused with a measured reason rather than met.

## What the lane gave up

The full panel's other three seats. The spec-warden had nothing to weigh: zero
tokens move. The historian would have had something — C compilers have coalesced
stack slots since the 1980s, and *why this one does not* is a precedent question
— but the compiler seat answered the operative half by compiling the witness:
clang merges nothing at top scope and everything at block scope. The
llm-ergonomist had nothing: no program a person writes changes shape.

**What the lane nearly missed, and did not**: the FFI seat's finding that
`--sanitize` *hides* the SQLite failure. A soundness lane that had run only the
compiler seat would have refused the proposal on the numbers and never learned
that the corpus's own strongest instrument is blind to that shape. It is written
here so that the next sitting about lifetimes starts from it.
