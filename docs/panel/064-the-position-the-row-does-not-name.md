# Panel 064 — the row was falsified at a position its text does not name

**Lane: full** (llm-ergonomist, historian, spec-warden, ffi-pragmatist,
compiler-engineer). Convened 2026-08-15.
**Provisional — author ratification pending.**

Five seats, five `object`-or-stronger verdicts, and **they object to five
different things**. One is a **veto**, and it decides the sitting.

## The proposal, verbatim

> **Lane: full.** design.md Part 6 is the language's list of permanent refusals, and
> CLAUDE.md §12 holds a refusal to a feature's standard: *"a Part 6 row must name the
> program or the compiler fact that would make it wrong."*
>
> **What the row says (design.md:2444-2449):** *"a program the closure list or
> §4.19's ladder needs, binding a C `long`, `size_t` or `unsigned long`
> **parameter**, whose correct spelling is the same on all three CI legs. Produce
> one and this row is wrong."*
>
> **What is produced:** the `size_t` clause — `extern function malloc(size: u64) ->
> ptr` is silent on all three CI targets. The `long` clause survives. And
> `unsigned long` behaves like `long`, not like `size_t`.
>
> **Q1** — what does a half-produced falsifier oblige: amend, stand, or withdraw?
> **Q2** — if `size_t` leaves the row, is it a fact, a name, or nothing?
> **Q3** — the cost of the vocabulary the row defers.
> **Q4** — is a good error message better than a type vocabulary here?
>
> **Conservative default:** Q1 = amend, dropping `size_t`. Q2 = a fact, not a name.
> Q3 = not now. Q4 = the message.

## Verdict table

| judge | verdict | section | measured cost / delta | prediction | condition |
|---|---|---|---|---|---|
| **compiler-engineer** | **`veto`** on the vocabulary | design.md §1.7 (`:367-383`), Part 5 item 7 (`:2228-2242`), §1.0 (`:110-140`), §1.1; CLAUDE.md §11 | `clong` as a `Ty` variant: **3 loud sites** (`emit/ctype.rs`, `types/counted.rs`, `types/render.rs`) and **20 silent allow-list gates** across 25 files; honest total **~120-180 lines, 11-13 files**. As an `IntKind` case: cheaper and worse. **The alternative: 9 lines, one file**, `extern_record.rs` 208 → 217, 559/559 tests green | If `clong` lands as a `Ty` variant, `cargo build` reports **exactly 3 errors** in those three files and no others; and by that milestone's close **at least one program passing `heroes check` at exit 0 reaches an `unreachable!` or a `hero_unreachable()`**. The arm exists today: `xs: [i64[4]] @ []` does exactly this, from `Ty::Fixed`, the previous width-carrying variant added under the same rule | (1) `grep -nE '\blong\|size_t\|time_t\|clock_t' runtime/*.h` non-empty, **or** `library/source.hero` gains a second `extern` group naming a system header → §1.0 fires and the veto dies. (2) Produce a pair of equal class, size and signedness with **different ABI** on any CI leg → the relaxation is unsound and `clong` is the answer |
| **ffi-pragmatist** | `object` + **scoped veto** on the `_Generic` probe | design.md §1.11 (`:424`), §4.19 (`:1925`), the row (`:2444-2449`); CLAUDE.md §12, §2, §11 | `clong`/`culong` bind every unbindable field on **4 targets, 0 errors**. `culong` for `size_t` **fails on Windows and i686** → a third name is required. `u64` for `size_t` refused on **4 of 7** targets clang reaches | `record ldiv_t { quot: i64, rem: i64 }` added to the existing `extern "stdlib.h"` group in `tests/golden/fixedbugs/ffi-word-width.hero` produces `ffi_field_type` on **macOS and Windows** and **exit 0 on Linux**, with no `--target` anywhere. Red or green on all three ⇒ the field measurement is wrong | (1) a non-identity field check lands **and an array field still falls to a clean `ffi_field_type` at exit 1**, not clang's raw error at exit 2 (panel 062's veto class); (2) no closure-list or ladder program needs a `long`-identical field. **The `_Generic` veto is unconditional** short of C gaining a way to name a parameter's type |
| **spec-warden** | `object` | design.md **Part 6 preamble** (`:2317-2326`) + CLAUDE.md §12; §1.6 for the count. **And the row is Part 7 item 10, not a Part 6 row** — the proposal and the brief both misfile it | `heroes measure`, binding maximum: before **3374**, after **3374** — the conservative default spends nothing. A `size_t`/`long` sentence **+37**; the `spec:74` aliasing repair **+22** minimal / **+34** naming `free`. Headroom **722** | Amend as proposed and no test tells the two rows apart: `size_t` occurs **5 times** in `crates/heroes/src` and will still be 5 at M-selfhost-probe, while a `run/` golden pairing `take_ul(unsigned long)` with `take_size(size_t)` emits identical `ffi_parameter_type` text | (a) a binding **on the closure list or a ladder rung** takes a `size_t` parameter; (b) the compiler gains a probe distinguishing `size_t` from `unsigned long`; (c) **the falsifier is restated over the canonical type** instead of three C spellings — zero tokens, approved today |
| **historian** | `object` (advisory, narrow) | precedent | Seven FFI-bearing languages surveyed; **all that faced this shipped a name**: Nim `clong {.importc:"long".}`, cgo `C.long`, Dart `Long`/`Size` (2.17, 2022-05), Rust `c_long` (1.1.0, `core::ffi` 1.64.0 2022-09-22), Zig `c_long`, .NET `CLong` (net6.0, 2021). **Swift is the counter-precedent and it did not hold**: `CLong = Int` by pointer width, broken by Windows LLP64, repaired with a per-platform `#if` still in `CTypes.swift` today | A golden `extern function malloc(size: i64) -> ptr` produces **two different `ffi_parameter_type` texts** across the CI matrix — `unsigned long long`, no caveat, on Windows; `unsigned long` **plus** the `word_width` caveat on Linux/macOS — and the Unix text's portability advice is **false** for `size_t`. Scored at **M-ffi-ladder** | (1) clang prints `'size_t' (aka 'unsigned long')` → the message can carry what the name would, and Q4 stands; (2) produce a language that emits C, has fixed-width integers only, and got portable `long`/`size_t` bindings out of a **diagnostic** rather than a name, and held. Seven surveyed, none found |
| **llm-ergonomist** | `object` (spec-only, blind to the repo) | `spec:195` against `spec:194` | 14 typed slots across 5 declarations: **14/14 right on macOS**, its own claim of 1/14 silently wrong on Windows — **refuted below**. Proposes `clong`/`culong`, parameter-and-field only, `to_clong`/`to_i64`, any operator a compile error | Over n=20 fresh models, spec-only: ≥16/20 write `offset: i64` today; ≥16/20 write `offset: clong` under the proposal, residual failures **loud rather than silent** | Run the **documentation-only arm** — current types plus one contrast clause, n=20. Silent-wrong below 25% ⇒ withdraws and takes the docs fix. **Pre-emptive veto** on per-target `extern` groups or target predicates, on locality |

## What the sitting actually found, and it is not what it was convened for

**The row is falsified — at `field` position, and the row's text says `parameter`
in bold.** Two judges reached this independently from opposite directions, and a
third had already produced the first instance while sitting on panel 063.

On Darwin arm64, `int64_t` is `long long` and `time_t` is `long`: **same width,
same sign, distinct types**. Panel 060's field assertion asks for type identity, so:

```
Sized   { size_t n; }    declared u64  → exit 1, error[ffi_field_type]
Timed   { time_t t; }    declared i64  → exit 1, error[ffi_field_type]
Clocked { clock_t c; }   declared u64  → exit 1
Offed   { long long o; } declared i64  → exit 0
Offt    { off_t f; }     declared i64  → exit 0
```

The unbindable set, measured against real SDKs at this project's own twelve flags —
**9 of 24 POSIX fields**: `struct timespec.tv_sec/.tv_nsec`, `struct
timeval.tv_sec`, `ldiv_t.quot/.rem`, `struct
rusage.ru_maxrss/ru_minflt/ru_majflt/ru_nvcsw`. And **`z_stream.total_in`,
`.total_out`, `.adler`** — all eight Heroes widths refused — which is *the struct
you cannot use zlib without*.

**`struct stat` is not the casualty it was reported as.** `st_size` and `st_blocks`
are `long long` and bind fine; it is affected only through the embedded
`st_mtimespec`, whose `tv_sec` and `tv_nsec` are `long`. Correcting the
coordinator's own note, which listed it whole.

**The corpus numbers bound how much this is worth, and both compiling judges
volunteered them against their own conclusions:**

| library | affected fields |
|---|---|
| raylib | **0 of 167** |
| sqlite3 | **0 of 196** |
| curl (structs) | 0 — but 21 of 108 *parameters* |
| SDL3 | 2 of 854 (`SDL_GPU*CreateInfo.code_size`) |
| zlib | 5 of 30, and they are the three that matter |
| POSIX | 9 of 24 |

The library that proved the field feature at 349/349 gains **nothing**.

**And `partial` does not rescue it.** The record *is* the header's struct, so
omitting a field keeps `sizeof` and every `offsetof` exact: the struct stays
passable and the field becomes **unnameable**. That is bug-proof and **not
complete**, and CLAUDE.md §12 asks for both. A zlib binding that can call `deflate`
and cannot read `total_out` is not a zlib binding.

## The two things everyone was wrong about

**1. The ergonomist's central measurement does not survive contact with the
repository.** Its claim — *"1 of 14 silently wrong on Windows"* — is **0 of 14**.
`commands/flags.rs:33` is `FLAGS: [&str; 12]` and carries `-Werror=shorten-64-to-32`
and `-Werror=sign-conversion`; `extern_probe.rs` emits the probe that trips them.
Modelled on four targets by both compiling judges independently:

```
arm64-apple-darwin        exit 0
x86_64-unknown-linux-gnu  exit 0
x86_64-pc-windows-msvc    error: implicit conversion loses integer precision:
                          'int64_t' (aka 'long long') to 'long'   → error[ffi_parameter_type], exit 1
i686-unknown-linux-gnu    same
```

That is not silence. The real parameter-position problem is **source not
portable** — smaller, different, and it corrupts nothing. The ergonomist was blind
to `extern_probe.rs` and `flags.rs` **by construction**, which is what its seat is
for; the finding is that its *diagnosis* was reasoning correctly from a spec that
does not say what the compiler does. That is a spec defect, and it is queued as one.

`offset: i32` compiles on all four targets. `word_width`'s caveat — *"declare the
width it means there"* — points **away** from the one answer that works everywhere.

**2. The coordinator's own proposal was killed by both compiling seats, for
compatible reasons.** The idea was `_Generic` on the function's **address**, to
verify a parameter's type without new vocabulary. It is dead:

- C cannot name parameter *k*. Three attempts, three compile errors, including
  `__builtin_parameter_type`, which does not exist. So the assertion can only cover
  the **whole signature**.
- The emitter can only write what the **Heroes declaration** says, and `ptr` is
  `void *` — deliberately (`emit/extern_record.rs:118-131`: exact-pointer-identity
  made 28 typed-pointer fields across ten raylib structs unbindable). So a binding
  with **every width correct** is refused: `void *` never matches `FILE *`. Every
  SQLite, curl and stdio entry point.
- Casting `&fseek` to the declared type first makes the assertion **vacuous** —
  exit 0, checks nothing.
- And the engineer's sharper point: at parameter position **there is no hole
  today**, so the probe would turn every working `long` binding on Darwin into an
  unbuildable one. *It does not replace the vocabulary; it manufactures the demand
  for it.*

Recorded at length because a coordinator's proposal is the one a panel is least
likely to refuse, and this one was refused twice.

## The resolution — `ratified 2026-08-24` (author instruction, *"ok ratifica anche quelli"*; § Author's verdict below)

**The veto stands: no `clong`, no `culong`, no `csize`.** The engineer holds veto
power on implementation cost and core-vs-sugar, and its case is measured: a new
scalar is **core** under §1.7 (checker *and* lowering *and* backend, erased
nowhere), it is **Part 5 item 7**, it is **not on the closure list**
(`library/source.hero:102-115` has one `extern` group whose whole surface is `cstr`,
`str`, `i64`; `grep -n "long" runtime/heroes_runtime.h` returns 0), and the
positional restriction the ergonomist described as one rule is **20 silent
allow-list gates rustc does not flag**.

**What lands instead — 9 lines, one file.** `emit/extern_record.rs`, one branch
before the existing one, for **integer fields only**:

```rust
if let Ty::Int(kind) = checked.types.get(ty) {
    let spelling = kind.c_type();
    return Some(format!(
        "_Static_assert(__builtin_classify_type({place}) == 1 \
         && sizeof({place}) == sizeof({spelling}) \
         && (((__typeof__({place}))-1 < 0) == (({spelling})-1 < 0)), \
         \"{FIELD_ASSERTION} {c_type} {member}\");"
    ));
}
```

208 → 217 lines. **559/559 tests green.** The relaxation is **strictly monotone** —
an exact-identity match necessarily has the same class, size and sign — so it
cannot regress an accepted program, and that is provable rather than tested.

Why it is right and not merely cheap: **type identity is the wrong proxy for an
integer field, because C's integer ABI is width and sign alone.** For floats,
pointers and records identity **is** the ABI, and the branch keeps them there —
`classify_type` returns 8 for float, 5 for pointer, 4 for bool, 3 for enum — so
panel 060's `f64`-over-`float` counterexample is still refused and enums stay
refused exactly as today.

It also satisfies the pragmatist's **stated condition 1**: the integer branch sits
**before** the array branch, so a fixed-array field never reaches it and still falls
to a clean `ffi_field_type` at exit 1, which was panel 062's veto class. By its own
words that seat stands down.

And it is built on **`__builtin_classify_type` + `sizeof` + a sign comparison, not
on `__builtin_types_compatible_p`** — against the coordinator's instruction, and the
engineer gave the reason: `types_compatible_p` would need an *enumeration* of which
C types are 64-bit-signed here, which is CLAUDE.md §11's premise about the world.
Class, size and sign ask the value in hand. It removes a `_Generic` from the integer
path, shrinking panel 063's own hazard surface, and compiles clean under
`-pedantic-errors`.

**The four questions, answered.**

- **Q1 = amend, and the row is Part 7 item 10, not Part 6.** Both the proposal and
  the convening brief misfiled it; the spec-warden caught it. Part 7 **defers**, so
  falsifying a reason repairs the reason and never admits the feature. The
  amendment: the row narrows to **parameter position**, where its falsifier is
  **still not produced**, and records that **field position was falsified and
  closed by a mechanism rather than a vocabulary**. The disjunctive text is
  restated over the **canonical** C type rather than three C spellings — the
  spec-warden's condition (c), zero spec tokens, which it approved in advance.
- **Q2 = a fact, and the fact is narrower than the proposal wrote it.** There is no
  platform in reach where `size_t` ≠ pointer width — the ergonomist guessed and was
  right. But `u64` for `size_t` is refused on **4 of 7 targets clang reaches**
  (i686, armv7, wasm32, x32). `size_t = u64` is a premise about **64-bit targets**,
  not about `size_t`, and §11 names that failure mode. The fact is recorded with
  its scope attached.
- **Q3 = not now**, by the veto, and with the honest note that a vocabulary is the
  only thing that buys **one spelling for three legs**. Option 2 buys *bindable on
  the machine in front of you*. Which of those the row's falsifier demands is the
  question the amendment settles, and it settles it the narrow way.
- **Q4 = neither.** The message was refuted on its own terms — its advice is
  **false for a `size_t` author** — and the vocabulary was vetoed. The answer is a
  **mechanism**, which is the option the proposal did not list.

**What a veto would compel.** If the author overturns the engineer's veto, `clong`
+ `culong` + `csize` land as a `Ty` variant (never an `IntKind` case — `INT_KINDS`
is the surface name table, so that spelling makes them writable everywhere at once
and makes a literal's legality target-dependent), the **20 silent gates** are
enumerated and closed one by one, and the engineer's prediction becomes the
milestone's acceptance test: a program that passes `heroes check` at exit 0 and
reaches an `unreachable!`.

## Process notes — the freeze, third instance

The engineer's first five `heroes build` invocations ran with the default cwd and
wrote five entries into the repository's gitignored `build/` cache. It attributed
them by the source path inside the generated C, removed exactly those five, and
**reported it rather than hiding it**; `git status --porcelain` was empty at start
and end and the tracked tree was never touched.

This is the third sitting in which the copy rule has been paid for (054: prototyped
in the repository; 056: a copied `target/` pointed `CARGO_MANIFEST_DIR` at the real
tree; 064: the default cwd). All three are the same shape — **the copy is made and
then something outside the copy is still reachable** — and the skill's wording
addresses the first two. A queue item carries the third.

The coordinator also ran read-only measurements against the frozen tree during the
sitting (`heroes build` on scratch headers under the scratchpad) and published two
of them to the judges mid-flight. One was **wrong in a way that mattered** — the
`_Generic` probe — and was refuted by both compiling seats. Recorded because the
freeze rule exists to keep the seat that convened a panel from moving the ground
under it, and a coordinator's *measurement* carries the same privilege as its
edits.

## Predictions to score

| judge | prediction | scored at |
|---|---|---|
| compiler-engineer | option 2 ships in **under 30 lines in one file**, `emit/extern_record.rs` stays **under 250** | this sitting's implementing commit |
| compiler-engineer | if `clong` lands as a `Ty` variant: **exactly 3** build errors, in `emit/ctype.rs`, `types/counted.rs`, `types/render.rs`, and no others | only if the veto is overturned |
| compiler-engineer | by the close of the milestone that lands `clong`, **at least one program passing `heroes check` at exit 0 reaches an `unreachable!` or `hero_unreachable()`** — the arm exists today via `Ty::Fixed` | only if the veto is overturned |
| ffi-pragmatist | `record ldiv_t` in `tests/golden/fixedbugs/ffi-word-width.hero`: `ffi_field_type` on **macOS and Windows**, **exit 0 on Linux**, no `--target` | **M-selfhost-probe** close — and it becomes the acceptance test for option 2, which should turn macOS green |
| spec-warden | `size_t` occurrences in `crates/heroes/src` stay at **5**; a `run/` golden pairing `take_ul(unsigned long)` with `take_size(size_t)` emits identical `ffi_parameter_type` text | **M-selfhost-probe** close |
| historian | a golden `malloc(size: i64)` produces **two different `ffi_parameter_type` texts** across the CI matrix, and the Unix one's advice is false for `size_t` | **M-ffi-ladder** |
| llm-ergonomist | n=20 spec-only, `fseek`: ≥16/20 write `offset: i64`; documentation-only arm is the decisive experiment | **M-guide-book**, or whenever a Part 11 arm exists |

## What the sitting produced that was not its question

Recorded here because each was found by a judge measuring something else, and each
is queued rather than fixed:

1. **`xs: [i64[4]] @ []` builds at exit 0 and ships a binary that aborts.** Not a
   compiler panic — the panic is the *program's*, from `hero_unreachable()` emitted
   by `emit/construct.rs:94` under a `#line` pointing at the author's own source.
   CLAUDE.md §7 puts `hero_unreachable()` at *type-system-proven-unreachable*
   points; here the type system proved nothing. The row belongs in
   `emit/gate_types.rs::check_element` (`:127-142`), beside the existing `Ty::Unit`
   row — **one table, not two**: `check_type`'s `_ => {}` at `:79` swallows
   `Ty::Fixed`, and the checker's `fixed_outside_a_group` has exactly one call site.
   ~8 lines, one diagnostic code, one golden pair. **This is the engineer's
   load-bearing fact**: `Ty::Fixed` *is* the positional restriction the brief
   claimed this checker has none of, it is **one milestone old**, and it is broken
   in the tree today.
2. **`gate_types.rs`'s module doc says *"The table is empty of refusals today"***.
   That premise died when panel 062 landed `Ty::Fixed`, and the sentence still reads
   as correct — CLAUDE.md §11's named failure mode, verbatim, in the file whose job
   is to hold the refusals.
3. **`spec:73-74` — *"No aliasing exists anywhere"* — is false.** Producible at
   exit 0 with no diagnostic through `ptr`; a heap-use-after-free is reachable in
   the same way, reported by `--sanitize` in `hero_str_from_cstr`. design.md names
   the exception once (`:1415`) and only under gift 2; gifts 1 and 3 carry none, and
   the spec carries none at all. **This also falsifies the stated reason of Part 6's
   `Borrow checker` row** — the verdict may well survive on cost and scope, but not
   on the sentence it is written over. Priced: **+22** minimal, **+34** naming
   `free`.
4. **`spec:195` is half-enforced.** Wider than the header is refused; **narrower is
   accepted silently** (`putchar(c: i8)` prints `A` at exit 0; `malloc(size: u32)`
   runs). The class matches clang's *"loses integer precision"*, and a narrower
   parameter loses none at the call. The spec forbids what the compiler permits.
5. **`ffi_narrowed.rs:215-217`'s `size_t` comment is a premise about the world and
   is false on Windows.** Same source line: `unsigned long` + a caveat on the Unix
   legs, `unsigned long long` + **no caveat** on Windows — and the caveat's advice
   is wrong for a `size_t` author. The historian's proposed 3-line repair (prefer
   clang's sugared name) **does not close the class**: `memcpy` keeps
   `'size_t' (aka 'unsigned long')` but Darwin's `malloc` prints bare `'unsigned
   long'`, because clang has a **builtin prototype** for it carrying no typedef —
   8 of 10 libc entry points bare by default, **8/8 recovered under
   `-fno-builtin`**. A caveat right per-function is worse than one uniformly wrong.
   The comment should be deleted regardless.
6. **Part 6's own burden is majority-unmet.** The table has **23 rows, not
   fourteen**, and **12 name no falsifier at all**, three days after the decision
   requiring one. And **`Private record fields`** — *"a hidden field makes the type
   unconstructible from outside"* — was refuted by `partial`, which landed
   yesterday: `record Box partial` naming one of two fields constructs, crosses by
   value and prints at exit 0.
7. **All five of panel 062's predictions name milestones that had already closed
   when they were registered.** §1.6 admits a prediction as payment only if it names
   the milestone **at which it is scored**; a past milestone is not a scoring point.
   The **+14** spec tokens they bought must be scored now or the rows marked
   `lapsed` and their clauses re-argued under the removal branch (panel 046 R2 —
   never renewed with a new milestone name). `measure/gate.rs`'s rows `2588` and
   `2959` have the same defect: they say *"M-ffi-ladder rung 5"*, and rung 5 was
   climbed at M-struct-passing close.

## Author's verdict

**2026-08-24: ratified** (author instruction, *"ok ratifica anche quelli"*).

**What the yes is NOT.** It is not a permanent refusal, and the coordinator put it
to the author as one before checking — the correction is on the record here. The
row is **Part 7 item 10, not a Part 6 row** (the spec-warden's own finding in the
verdict table), so `clong`/`culong`/`csize` are **deferred until the closure list
compiles itself**, which is what Part 7's preamble means, rather than rejected
with a falsifier owed under §12. Ratifying it costs the language nothing it could
not take back.

**Verified with the yes**: `clong`, `culong` and `csize` occur **0 times** in
`spec/heroes-spec.md` and **0 times** across `selfhost/` — the veto is still in
force and nothing drifted in. And what landed instead is still there:
`selfhost/emit_extern_field.hero` asserts an integer field by
`__builtin_classify_type` plus `sizeof` plus sign rather than by type identity,
which is the sitting's real content — **C's integer ABI is width and sign alone**,
while for floats, pointers and records identity *is* the ABI and the branch keeps
them there, so panel 060's `f64`-over-`float` counterexample stays refused.

**What the yes settles**: the engineer's veto on a new scalar (core under §1.7 —
checker *and* lowering *and* backend, erased nowhere — and not on the closure
list), and the 9-line relaxation as the answer to the falsifier the row's own text
did not name. The relaxation is **strictly monotone**: an exact-identity match
necessarily has the same class, size and sign, so it cannot regress an accepted
program — provable rather than tested, and that is why it needed no migration.

## Scored at M-selfhost-probe close (2026-08-15)

- **spec-warden, first half: falsified in letter, by one comment.** `size_t`
  occurs **6** times in `crates/heroes/src` today, not 5: panel 063's
  follow-up work added a doc comment naming it (`emit/extern_record.rs:190`).
  All six are comments or message text — the substance (no probe, no
  vocabulary, nothing that tells `size_t` from `unsigned long`) holds
  unchanged. The second half (the paired `run/` golden emitting identical
  `ffi_parameter_type` text) was conditional on adopting the amendment as
  proposed, which the sitting did not; it is not scoreable as written.
