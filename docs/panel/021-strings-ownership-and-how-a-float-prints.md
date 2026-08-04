# Panel 021 — strings, the ownership pass, and how an `f64` prints

**Convened** 2026-08-04, before M5b's first line. **Trigger**: CLAUDE.md §4 —
architecture (a new pass class) and observable semantics (`f64` rendering, which
panel 006 deferred to exactly this milestone). **Status**: `provisional — author
ratification pending`.

Two vetoes, and they converge on the same missing paragraph. Two **silent** spec
defects found by a judge that was asked about something else. One point deleted
outright for having no call sites, and one whose premise turned out to be false on
this machine.

---

## The proposal, verbatim

> M5b: `str` reaches C, values acquire lifetimes, and the one number panel 006 left
> undecided gets decided. Seven points.
>
> 1. **The ownership pass is the first IR→IR pass**, and it runs between lowering
>    and the emitter: `ownership::run(&mut program)` inserts `incref`, `decref` and
>    `cow_check` as **real instructions**, so `--dump-ir` shows them and the emitter
>    stays a printer. Monomorphisation (M6) becomes the second such pass. The
>    representation is §4.20's and spike 04 froze it: `ptr`/`len`/`refcount`, always
>    NUL-terminated with `len+1` allocated so `.cstr()` is free.
> 2. **Cleanup is a walk over a fixed table, not a liveness analysis.** Every
>    refcounted slot is **zero-initialised in the prologue** (`HeroStr s = {0}`), and
>    every exit edge decrefs every refcounted slot unconditionally — a decref of a
>    null string is a no-op. This is the one place the M5a rule "nothing is
>    initialised in the prologue" is broken, and it is broken deliberately: the
>    alternative is a liveness pass, and a decref of an uninitialised local is
>    undefined behaviour rather than a wrong answer. Cost:
>    `-Werror=uninitialized` stops covering `str` slots.
> 3. **Panel 019's reason for slots gets cashed here.** "Release live locals and copy
>    out `@` parameters on every exit edge" is a walk over `Function::slots` in order;
>    with phi nodes it would be a dataflow question about who owns a value on which
>    edge. The pass therefore needs no new analysis at all: one pass over the blocks
>    inserting increfs at definitions and decrefs before terminators.
> 4. **`f64` prints round-trip-exact and readable**, and never `%.17g`. The rendering
>    is the shortest of `%.15g`, `%.16g`, `%.17g` that `strtod`s back to the same
>    bits — the classic loop, about ten lines — so `0.1` prints `0.1` rather than
>    `0.10000000000000001`. Two riders: a value with no fractional part still prints a
>    decimal point (`1.0`, not `1`), and the runtime never calls `setlocale`, so the
>    decimal separator cannot move under a golden. `inf`/`-inf`/`nan` are the three
>    special spellings.
> 5. **The str surface M5b reaches**: literals, `+`, the six comparisons, `len(s)`,
>    `s[i]`, `slice(from:, to:)`, `to_str` of `int`/`f64`/`bool`. Everything whose
>    type mentions an array waits for M5c.
> 6. **The verifier becomes phase-indexed** — `verify(program, checked, phase)`,
>    and the failure names the pass.
> 7. **`run/` goldens gain a sanitiser configuration.** Every case also compiles and
>    runs under `-fsanitize=address,undefined`, and a leak is a failure. It is the one
>    that can see a missing `decref`, which no output comparison can.

---

## The verdict table

| judge | verdict | section | cost / measured | prediction | condition to lift |
|---|---|---|---|---|---|
| compiler-engineer | **VETO** on point 2's *sufficiency claim* and point 7's leak gate | design.md Part 5's ownership bullet — "the runtime cannot know where a scope ends; **only lowering can**" — still normative, never amended | pass **150–200** lines; `Op` arms **~23** across 4 exhaustive matches (`ir/values.rs` gains **0**, because it reads through `uses::operands` — the payoff of M5a's split); M5b ≈ **600–700** non-test lines | at M5b close the ASan configuration reports **zero failures** on a program leaking one `str` per iteration and on `print(a + b)`, because `detect_leaks` is unsupported on Darwin arm64 | the convention written down (+0 or +1 arguments, sweep restricted to `Local`/`Synthetic`) · owning temporaries handled with **no fixpoint in the pass** and the invariant *checked* · a leak instrument that works here |
| ffi-pragmatist | **VETO** on point 5 (one missing primitive) | §4.19 read with §1.11 — the ladder's step 3 is "open, query, **read a result**, close" | 21 C files compiled; `sizeof(HeroStr) = 16`, two registers; spike 04 recompiled **unmodified** against both runtimes, ASan-clean, `HeroDesc`/`HeroArrayHeader` byte-identical | add `hero_str_from_bytes` and SQLite's ladder step 3 needs **no shim** — open → prepare → step → `column_text` → `column_double` → finalize → close, all direct `extern`s, `hero_runtime_live() == 0` | `hero_str_from_bytes`/`from_cstr` join the surface · point 3's three ownership rules stated · point 4 adopts `uselocale` · point 7 stops claiming it sees leaks · `cow_check` struck |
| llm-ergonomist | **OBJECT** — accepts column (1), objects to the bundle landing without the two *silent* defects | locality (§1.3), §1.4 | 4/6 exact on the f64 vector from the spec alone; `slice`'s bound a **coin flip** | from the spec alone: **≤1 in 6** exact-file matches today, `1.0`/`1` splitting **~50/50**; with the diff, **≥60%** exact and the `1.0` split collapsing to **≥95%**. `slice`'s `el`/`ell` split is **no better than 70/30** and **0%** produce a diagnostic | the same commit states `slice`'s exclusivity and `len`'s unit, and the vector's `0.0 - 0.0` line becomes `print(-0.0)` |
| spec-warden | **OBJECT** on point 4's content (not provisional — 17 candidates measured) | §1.2, §1.0, §1.4, §12 | baseline **2155**; recommended package **2194** (legacy 2135), headroom 845 → **806**. f64 F2 **+34** · `slice` **+5** · `inf/nan` **+10 → cut** · `len` bytes **+8 → waits** | land F2 + `slice` and the spec measures **exactly 2194 / 2135**; ship without an exponent rule and **≥1** hand-written f64 `.expected` is wrong on first write | the sentence names the **exponent form** and never says "shortest" · the ladder moves to design.md · `inf/nan` cut or f64 overflow reachability decided |
| historian | **OBJECT** on point 4 only (advisory) | the record | 40 verified rows, 6 marked UNVERIFIED | `printf("%.15g", 1e-323)` prints **`9.88131291682493e-324`** — 15 significant digits where Python renders `1e-323` and Java `9.9E-324`, so any subnormal in a golden looks wrong to anyone who checks it | the proposal copies gnulib's subnormal branch, stops saying "shortest", and replaces the `setlocale` promise with a structural guarantee |

---

## Where the judges disagreed, and where they converged

**They converged on the paragraph the proposal does not contain.** The engineer
reached it by reading (`Function::slots` is not where all owning references live)
and the ffi-pragmatist by compiling (`t_param_convention.c`, the *first* program it
wrote for this panel, failed ASan on the first run with a heap-use-after-free). Both
name the same hole: **the calling convention**. Do arguments arrive +1, with the
callee sweeping them, or +0, borrowed? Points 2 and 3 read literally give the first,
which frees the caller's string. The engineer adds the three neighbours — release on
overwrite, incref before return, temporaries — and each has a compiled
counterexample: 999 blocks leaked from a string-building loop under the literal
rule, and a use-after-free from `return prefix + name`.

**The one real disagreement is the shape of the fix for owning temporaries.** The
engineer wants classification-by-defining-op plus a *checked* invariant ("a
refcounted temporary is read only in the block that defines it", ~60 lines, no
fixpoint). The ffi-pragmatist did not test temporaries. Panel 019's own reason for
slots is what settles it: the whole point of choosing slots was that cleanup be a
table walk, and a liveness pass inside the ownership pass would spend that.
**Resolved: the engineer's scheme**, and the invariant is checked rather than
intended — the same move `values.rs` already represents.

**Point 7's premise is false on this machine, measured twice, independently.**
`ASAN_OPTIONS=detect_leaks=1` on Darwin arm64 answers `AddressSanitizer:
detect_leaks is not supported on this platform` and aborts. The ffi-pragmatist then
removed the counter from a program leaking 999 blocks and the ASan build exited 0 in
silence. So the milestone that introduces reference counting would have shipped with
**no leak detector at all**, behind a configuration that looked like one.

---

## Resolution — provisional, author ratification pending

### R1 · Two ops, not three. `cow_check` waits for M5c

`Op::Incref` and `Op::Decref` are real instructions in the IR, produced by an IR→IR
pass and printed by `--dump-ir`. This is Swift's design and the record is
unambiguous: SIL has `strong_retain`/`strong_release` as instructions and lists
"retain/release optimization" among the reasons SIL exists; OSSA's `copy_value` /
`destroy_value` can be "validated statically as not containing use after free errors
or leaked memory"; Nim's `injectdestructors` is inspectable through
`--expandArc:<proc>`. And the failure mode of the alternative is recorded: clang
emitted ARC's `retainRV`/`claimRV` as plain calls and LLVM "breaks ARC's autorelease
optimization by separating calls from the marker instructions", fixed by moving the
pairing *into* IR structure (LLVM D92808).

**`cow_check` is struck.** Measured: with `b = a` lowered to incref-only and no
`cow_check` anywhere, every M5b operation on `b` leaves `a` bit-identical — because
`str` is immutable (spec line 47) and there is no mutation primitive to observe
sharing through. It would have zero call sites, which makes it an arm in four
exhaustive matches that nothing emits, and a verifier invariant no test can make
fire (CLAUDE.md §9). It arrives at M5c, where `push` gives it a call site.

One rider from the record: SIL marks its refcount instructions
`MayHaveSideEffects` specifically to stop passes reordering them. Heroes has no
reordering passes, so the note goes in the verifier's doc rather than in code.

### R2 · The convention, written down — both vetoes' condition

Five rules. Each one has a compiled counterexample in the panel's scratch, and each
one gets a `run/` case that fails without it (CLAUDE.md §9).

1. **A plain parameter is borrowed (+0).** It is excluded from the cleanup sweep,
   which covers `SlotKind::Local | Synthetic` only. Heroes parameters are immutable,
   so a borrowed parameter is never overwritten and never needs a decref — and the
   caller increfs nothing per argument. Counterexample: `t_param_convention.c`,
   heap-use-after-free at `runtime.c:76`.
2. **An `@` parameter is moved in and moved out.** The prologue's `h0_s = *ph0_s`
   takes no reference, every reassignment decrefs the slot's previous value, and the
   exit edge writes `*ph0_s = h0_s` **instead of** decrefing that slot. The callee's
   copy-in consumes the caller's reference and the copy-out hands one back; M5a's
   pointer ABI is unchanged.
3. **A store into a refcounted slot increfs the new value *before* decrefing the
   old.** `s @ s` otherwise frees and then increfs a dead buffer. Counterexample:
   `t2_overwrite_loop.c` under the literal rule — **999 blocks leaked**, exit 134
   only because a counter was watching.
4. **A returned value is increfed before the exit sweep.** Counterexample:
   `t2_greet_broken_c.c` — silent at `-O0`, `heap-use-after-free` under ASan.
5. **An owning temporary is decrefed at the end of its defining block.** A value is
   owning iff its defining op allocates (concat, `slice`, `to_str`, a `str`-returning
   call). No fixpoint, no liveness: the scheme is made safe by a *checked* invariant
   at phase `Owned` — a refcounted temporary is read only in the block that defines
   it. `print(a + b)` leaks without this.

Two consequences recorded rather than fixed here. `ir/asserts.rs` computes both
comparison operands in the test block and reads them in the abort block, which
falsifies rule 5's invariant for `assert s == "x"`; `Abort::Assert` is gated to M6,
so the fix (store refcounted assert operands into synthetic slots) lands with it,
and the fact is in the queue so it cannot arrive as a surprise. And **the answer to
"is this type refcounted?" gets exactly one home** (`ir/layout.rs`), because M5c's
descriptor pass asks the same question and two answers is the failure `uses.rs` was
split out to prevent.

### R3 · Zero-init stands, with the rider that makes it honest

Point 2 is adopted, and the record licenses it verbatim: clang's ARC specification
says storage for a `__strong` object "may be properly initialized by filling it with
the representation of a null pointer, e.g. by acquiring the memory with `calloc`",
and that operating on uninitialised such storage is undefined behaviour. rustc does
the alternative and then optimises it — `ElaborateDrops` inserts *drop flags* where
initialisation is not statically known — so zero-init **is** the drop flag, encoded
in the pointer, and it cannot have the bug rustc's own docs warn about (flags cleared
in the wrong order).

But the cost is worse than the proposal says, and it was measured. Without zero-init
clang reports `error: variable 't1' is uninitialized when used here` **for a
by-value struct too** — so `{0}` converts a **compile error** into a silent `""`.
The rider: **`ptr == NULL` is the one non-value**, and every runtime entry point
panics on it (`panic: read of an unassigned str slot — this is a compiler bug`). The
compensating IR check is a phase-`Owned` invariant that every slot is stored before
it is loaded on every path, reusing the dominators `values.rs` already computes —
the fixpoint stays in the verifier, not in the pass.

### R4 · `HeroStr` is passed **by value**, and the reason is the FFI

`HeroStr { const char *ptr; int64_t len; }` — 16 bytes, two registers on arm64,
refcount and a magic word in a header immediately before the bytes, NUL-terminated
with `len+1` allocated so `.cstr()` is free (§4.20's stated highest-return
decision).

The decisive experiment is not ergonomics. Written by pointer, the wrong
`str`→`cstr` conversion **compiles clean with an explicit cast** and passes a
refcount word to `sqlite3_open`; written by value it is `error: operand of type
'HeroStr' where arithmetic or pointer type is required` — *inexpressible*. §4.19's
guarantee is that a wrong FFI type is a compile error, and the value type is what
keeps it one.

Two corrections to the proposal's own text. **Spike 04 did not freeze the string** —
it froze the array header and the descriptor, and there is no `HeroStr` anywhere in
the tree; the claim is withdrawn. And §4.20's "(`ptr`, `len`, `refcount`)" cannot be
read literally: a by-value copy with an inline refcount diverges, so the refcount
lives in the heap block.

One hazard the value shape carries, found while testing something else:
`HeroStr fake = {(const char *)sqlite3_column_text(st,0), n};` compiles with **zero
warnings under `-Weverything`**, and decrefing it writes into SQLite's own heap
block — silent corruption, ASan-clean, because the write lands inside a valid
allocation. An 8-byte `magic` in the header turns it into `panic: not a Heroes
string block — a str was fabricated from a foreign pointer`. `HERO_RUNTIME_ABI`
bumps to **2**.

### R5 · `hero_str_from_bytes` joins the surface — the ffi veto

Without it **no `extern function` in the language may have return type `str`**, and
§4.19's own acceptance ladder is unwritable: step 3 is "open a database, run a
query, **read a result**, close", and every C library returns strings as borrowed
pointers. `hero_str_from_bytes(const char *, int64_t)` and `hero_str_from_cstr` make
an owning copy; `hero_str_cstr` is the free direction. One primitive, and the
founding constraint keeps its acceptance test.

### R6 · How an `f64` prints

**The rendering.** A round-trip-exact ladder, with the branch gnulib's shipped
version has and the proposal's description omits: for a subnormal the loop starts at
precision **1**, not 15, and it is bounded above rather than fixed at three tries.
The historian's prediction was checked and holds — `printf("%.15g", 1e-323)` prints
`9.88131291682493e-324`, fifteen significant digits for a value Python renders
`1e-323`. The word **"shortest" is struck everywhere**: `5e-324` renders
`4.94065645841247e-324` under the ladder, which round-trips and is not shortest, and
Java shipped `1.9999999999999998E23` for eighteen years to prove the distinction
matters. The algorithm lives in design.md §4.9, where line 1147 already promised
it — not in the spec.

**Locale is answered structurally, not by promise.** The proposal's guarantee is
measurably worthless: under `de_DE.UTF-8` the ladder prints `0,1.0` — malformed,
because the decimal-point rider's `strpbrk(buf, ".eE")` finds no `.` and appends one
— and the round-trip check **cannot see it**, since `strtod` reads the same locale
and is wrong consistently. PEP 331 is the shape of the hazard: CPython never called
`setlocale`, GTK+ did, and CPython broke anyway; llama.cpp printed `0,000000` for
GGUF metadata in 2025 for the same reason; PostgreSQL's `float4out`/`float4in` have
the identical exposure. Rust, Go, Python-since-2.4 and C++17's `<charconv>` all
removed the dependency structurally. **Adopted: a lazily-created `newlocale(…,"C",…)`
and `uselocale()` around the render**, which fixes `snprintf` **and** `strtod` in one
window, is POSIX 2008, present on Darwin and glibc, and thread-local. 11 lines,
measured correct under `de_DE.UTF-8` and `it_IT.UTF-8`.

**`1.0`, not `1`.** The ergonomist's argument is the decisive one and it is about the
golden harness rather than about taste: under `1`, `print(price * to_f64(count))` and
`print(5 * 2)` emit **identical bytes**, so an accidental `int`→`f64` drift stays
green forever — and the drift is exactly what `1 + 2.0` being a compile error exists
to prevent. Lua is the closest precedent, a language that gained a second numeric
type and disambiguated the same way (though it did so locale-dependently, which R6's
first half repairs). Rust prints `1` from `Display` and needed `Debug` to recover the
distinction; Go prints `1` from `%v` and its `%#v` `.0` was filed as a bug.

**The spec sentence** is the warden's F2 form, **+34 measured**, naming the exponent
form. `inf`/`-inf`/`nan` are **cut from the spec** (−10) and implemented in the
runtime: reaching one requires f64 overflow or `inf - inf`, and §4.14's unqualified
"division by zero aborts" makes `0.0/0.0` unreachable — naming a spelling the reader
cannot reach is redundancy spent where no error occurs (§1.4). The reachability
question is queued.

**And the panel's own test vector was wrong.** `0.0 - 0.0` is `+0.0` under IEEE-754
§6.3 in every rounding mode except roundTowardNegative; all four candidate columns
printed a negative zero for it. The vector becomes `print(-0.0)`. Caught by the judge
that was only supposed to be reading the columns.

### R7 · `slice(from:, to:)` — `to` is **excluded**, and the spec says so

**+5 measured, the best value on the table.** The words "inclusive", "exclusive" and
"half-open" appear nowhere in the spec or design.md; the only evidence is one line of
the design brief's appendix, which is not the prompt. The ergonomist called it a coin
flip and named the consequence: `slice` is how a self-hosting lexer extracts every
token, `"el"` and `"ell"` both compile, and the golden that would catch the error is
written by the same guess that made it.

`len`'s unit **waits**: "indexed in bytes" plus "`len` bounds the index" is a
one-step derivation (§1.0), the minimal fix measures +8, and it is pre-registered for
M5c if the corpus proves otherwise. The discriminating probe is queued —
`print(len("è"))` is 2 under bytes and 1 under characters, and the panel's own
all-ASCII vector could not have told the difference.

### R8 · The phase lives on the `Program`

Not in `verify`'s signature. `Program { phase: Phase }` with `advance_to()`
asserting monotonicity: ~15 lines, **zero call-site churn** against 22 for the
signature, and — the reason that decides it — `verify(p, c, Lowered)` on an owned
program would silently skip the new checks, which is a verifier that quietly stops
checking. rustc puts the phase on the body and the pass name in the message
(`validate_body(tcx, body, format!("after pass {pass_name}"))`); LLVM's
`--verify-each` exists "for cases where it is suspected that a pass is creating an
invalid module but it is not clear which pass is doing it"; Go runs `checkFunc`
"between each phase" under `-d=ssa/check/on`, which caught a real ARM bug in
`runtime/malloc.go` (golang/go#22499). All three quotations were audited verbatim and
all three survive — including the issue number.

`--dump-ir` means **the IR the emitter sees**, post-ownership, which is what the
ROADMAP already promises. `--dump-ir=lowered` waits under §10's third clause. One
unpriced cost, named now rather than at the gate: `tests/golden/ir/` is
`UPDATE_GOLDEN`-forbidden, so every IR golden gaining a refcount op is hand-edited
labour.

### R9 · The leak instrument, because ASan is not one here

`ASAN_OPTIONS=detect_leaks=1` on Darwin arm64: `AddressSanitizer: detect_leaks is
not supported on this platform`, exit 134. Measured by two judges independently, and
then demonstrated: 999 leaked blocks, counter removed, ASan build **exit 0 in
silence**.

- **The sanitiser configuration stays**, for use-after-free and double-free, which
  it does catch — it caught two real bugs in the panel's own C on their first run,
  which no output comparison would have. It stops being described as a leak
  detector.
- **`hero_runtime_live()` / `hero_runtime_check_leaks()`**: a live-block counter in
  the runtime, asserted at exit. 12 lines, portable, deterministic, and it names a
  count instead of a stack. It caught both injected defects
  (`panic: 999 heap blocks still live at exit (a missing decref) — this is a
  compiler bug`). A `fixedbugs`-named case is named after the leak it first caught.
- **Shape** (CLAUDE.md §10): a **flag**, `--sanitize`, on `build` and `run` — same
  question, same input, different codegen. Not a subcommand. The `runtime.o` cache is
  already keyed on flags and design.md already records that one `.o` is correct across
  `-O0`, `-O2`, `-flto` and the sanitisers. The latent inconsistency that optimisation
  level is chosen by *verb* while sanitising is chosen by *flag* is queued, not fixed:
  unifying it is a larger surface nothing must type.

### R10 · The subset note is derived, not maintained

`gate.rs`'s `SUBSET` string is hand-written and its only test asserts the message
*contains* "the backend emits" — so the enumeration is untested and grows at M5b,
M5c, M6 and M7 while the machine-readable truth sits ten lines below it. It becomes a
function over the same match, so the note and the table cannot disagree by
construction — CLAUDE.md §10's own pattern ("one argv table parses and prints the
help, so they cannot disagree"). The ergonomist also asked for two clauses and both
are adopted: **"no other built-in is emitted yet"**, without which its own suggested
rewrite (which calls `len`) is a coin flip, and a wording change from "no change to
this file will fix this" to something that reads as *not yet* rather than
*impossible* — because `str` graduating from the refused list to the supported one is
what tells a reader to rewrite the program rather than escalate a language question.

### What a veto would have compelled

The engineer's, unlifted: an ownership pass whose cleanup walk is documented as
sufficient and is not — leaking one string per iteration in every string-building
loop the self-hosted compiler contains, with CI green, because the configuration
that was supposed to catch it cannot. The ffi-pragmatist's, unlifted: no `extern`
function in the language may return `str`, and §4.19's ladder — the test design.md
calls "the architecture holds" — is unwritable at step 3.

---

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| compiler-engineer | the ASan configuration reports zero failures on a per-iteration `str` leak and on `print(a + b)` — `detect_leaks` unsupported on Darwin arm64 | M5b close (already confirmed by two judges; the counter is the answer) |
| ffi-pragmatist | with `hero_str_from_bytes`, SQLite's ladder step 3 needs **no shim** and reports `hero_runtime_live() == 0`; without it, no `extern` may return `str` | M7 |
| llm-ergonomist | from the spec alone: ≤1 in 6 exact f64 expected-files today → ≥60% with the diff; the `1.0`/`1` split 50/50 → ≥95% `1.0`; `slice`'s `el`/`ell` split no better than 70/30 with **0%** producing a diagnostic | metric 2's first paced run |
| spec-warden | F2 + `slice` measures **exactly 2194 binding / 2135 legacy**, headroom 806; and if `inf`/`nan` were kept, **zero** of M5b's `run/` goldens would exercise them | M5b step where the spec lands |
| spec-warden | ship without an exponent rule and ≥1 hand-written f64 `.expected` is wrong on first write (any \|v\| < 1e-4 or ≥ 1e16) | M5b close |
| historian | `printf("%.15g", 1e-323)` prints `9.88131291682493e-324` — 15 digits where Python prints `1e-323` | now (confirmed) |

## Watch list

- **`while true` in a value-returning function** still needs an unreachable
  `return` (M5a's over-rejection, Rust's too).
- **f64 overflow and `0.0/0.0` reachability** is unanswered: §4.14 says division by
  zero aborts without qualifying the type, so `inf` and `nan` may be unreachable
  states with three spellings. Decide before any golden prints one.
- **`len`'s unit** is derivable but unstated; the discriminating probe is
  `print(len("è"))`, and the panel's own vector was all-ASCII.
- **`range(a, b)`'s upper bound** has the same textual gap `slice` had. +13 measured
  for both; `slice` alone bought at +5 because `range`'s idiom appears in design.md.
- **`assert` on a refcounted operand** is a use-after-free under R2's rule 5; gated
  to M6 with `Abort::Assert`, queued so it cannot surprise.
- **Cycles.** Reference counting leaks them, and Nim needed ORC for exactly this. Safe
  for `str`; becomes false the first time a refcounted type can cycle — M5c's
  `variant` with a recursive payload is that moment.
- **`printf` tie-breaking is unstandardised** (0.25 → `0.3` on MSVC, `0.2` on
  glibc). Whether it can differ at 15–17 *significant* digits is UNVERIFIED; if it
  can, a golden's bytes are a golden about libc, and the ladder must be replaced by
  Ryū.
- **Optimisation level by verb, sanitiser by flag.** Unifying them is a larger
  surface nothing must type (§10).

---

## DESIGN-LOG

Appended with this session. The amendments it compels — spec F2 + `slice`,
design.md §4.9 (the ladder), §4.20 (`HeroStr` by value, the ABI stamp, the leak
counter), Part 5's ownership bullet (the pass is IR→IR, not "during lowering"),
CLAUDE.md §7 (`--sanitize`, the convention) — land in their own commit citing this
file.
