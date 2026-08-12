# 003 — comptime and macros, against the generics that already shipped

**Origin.** 2026-08-12 · reasoning session · «ragiona sul fatto se può aver senso
introdurre un sistema di comp time o di macro semplice e che costi può avere e che
benefici può dare e come si coniuga con i generics già introdotti» — asked while
reading `examples/curl/main.hero`, citing Zig and Nim as the languages that ship
this. Read: `design.md` §1.0, §1.2, §1.3, §4.12, §4.19, Part 6, Part 7 ·
`CLAUDE.md` §2, §4, §6, §13 · `spec/heroes-spec.md` · `crates/heroes/src/`
(`ir/mono.rs`, `ir/mod.rs`, `ir/exprs.rs`, `types/decls.rs`, `types/generics.rs`,
`emit/inst.rs`, `own.rs`) · `docs/panel/{029,036,037,038}` ·
`docs/measurements/005-magic-constants.md` · `main` at `2ac0403`. Nine programs
compiled and run in a scratch directory. **No file of code, spec or design
modified.**

## The question

Zig and Nim both ship what the question asks for, and they ship two different
things under it. The session's first job is therefore not to answer yes or no but
to say **what "comptime" names**, because the record has already answered part of
it and cannot answer the rest until the parts are separated.

## What the artifacts say

### Macros are refused three deep; comptime is named once, and not as a proposal

`design.md:2114` opens Part 6 with *"Do not add these. Each violates locality"*,
and `design.md:2124` is a row of that table: `| Macros | the code you read is not
the code that runs |`. `CLAUDE.md` §13 makes Part 6 off-limits, and §6 refuses
Nim's macros and templates **by name**. `design.md:196` gives the reason under all
three — the category to reject is *"constructs whose meaning lives elsewhere"*.

So half the question is closed, and closed against the half of the precedent the
author cited: **Nim's macros are the one implementation `CLAUDE.md` §6 names.**

Compile-time *evaluation* is a different matter. `comptime`, `const-eval`,
`constant folding`, `constexpr`, `CTFE`, `staging` and `partial evaluation` occur
in `design.md`, `CLAUDE.md`, `spec/`, `DESIGN-LOG.md` and `docs/ROADMAP.md`
**exactly once between them**, at `design.md:1821`, and it arrived this morning
with panel 038: naming a C *object* rather than a value is something *"Zig refuses
by accident, as a comptime failure its own tracker calls a bug"*, where Heroes
refuses it by rule. The record's single mention of the mechanism is a case where
the mechanism gets the right answer for the wrong reason.

Everything else about compile-time evaluation is unclassified. It is in neither
Part 6 nor Part 7, so **it is a first visit, not a revision**, and `design.md:118`
puts the whole burden on it: on the closure list, or a measured Part 11 effect, or
a §1-derived argument the panel accepts — *"Neither → it waits, regardless of
elegance"* (`design.md:124`). The closure list (`design.md:134`) has no
metaprogramming row, and `docs/ROADMAP.md` records it as closed.

### The surface already exists and is fully general — measured, not read

A `constant`'s body is parsed by the ordinary block parser (`syntax/decl.rs:89`),
resolved as an ordinary block, and checked with `Want::Value(declared)`
(`types/decls.rs:36-49`). Nothing restricts it. Nine programs, compiled and run at
`2ac0403`:

| body | `check` | `run` |
|---|---|---|
| `10002` | 0 | prints `10002` |
| `10000 + 2` | 0 | prints `10002` |
| `base() + 2`, calling a user function | 0 | prints `10002` |
| `if true` / `1` / `else` / `2` | 0 | prints `1` |
| a binding, then `base + 2` | 0 | prints `10002` |
| `"hel" + "lo"` at `str` | 0 | prints `hello` |
| `[1, 2, 3]` at `[int]` | 0 | prints `3` |
| reads another `constant` | 0 | prints `10002` |
| **two constants reading each other** | **0** | **SIGSEGV at `-O0`, hangs at `-O2`** |

So the language already admits arbitrary computation in a `constant`. What is
missing is not the surface — it is the evaluation. `ir/mod.rs:93` lowers a
`constant` to a zero-argument function; `ir/exprs.rs:174` lowers *reading its
name* to a call; the emitted C for `10000 + 2` is
`if (__builtin_add_overflow(t1, t2, &t3)) hero_panic_overflow(); return t3;`,
executed at every read. `ir/mod.rs:95-97` already names the shortcut and files it
as **the emitter's optimisation, not the IR's concern**.

Two facts follow that no proposal may use as evidence, and they are handed off
below rather than argued here: the ninth row is a silent-error class the compiler
does not diagnose, and it fails two different ways depending on a flag.

### Seven capabilities under one word

| | capability | what Heroes has | what actually refuses it |
|---|---|---|---|
| C1 | fold a constant expression | the surface, unrestricted; no evaluator | `CLAUDE.md` §13 + panel 037 — an optimisation with no measured need. **Part 6 does not touch it** |
| C2 | run a function at compile time | nothing | Principle 0. **Part 6 does not touch it** |
| C3 | types as compile-time values | §4.12 generics, spent the other way | §4.12 + panel 029. **Part 6 does not touch it** |
| C4 | reflection over types | field-walking `eq`/`hash`, generated, at zero surface cost | Part 6 — but the **Ruby row** (`design.md:2132`), not the Macros row |
| C5 | conditional compilation | `runtime/hero_os.h` absorbs target differences | Part 6's Macros row, literally |
| C6 | macros proper | nothing, refused three deep | Part 6's Macros row. **Zig has no macros either** |
| C7 | read a header at compile time | delegation to clang: panel 036 for signatures, panel 038 for values | already answered, at +28 spec tokens |

The column that matters is the last: **for four of seven, the thing that refuses
it is not Part 6.** Answering the author's question with "Part 6 says no" would be
right about C5 and C6, wrong about C4's reason, and simply unresponsive about C1,
C2, C3 and C7.

### The generics interaction, in both directions

Zig has no generics. `fn ArrayList(comptime T: type) type` is a function returning
a type, and monomorphisation falls out; the type argument is **written at the call
site**. §4.12 spends the budget the other way and forbids exactly that:
*"Functions only, no constraints, always inferred"* (`design.md:1423`). So C3 does
not *add* to §4.12 — it **replaces** it.

Whether that is a saving is arithmetic, and §4.12's own rationale
(`design.md:1431-1440`) is the template: generics were adopted because they make
the compiler **smaller**, turning seven container functions from compiler magic
into 149 lines of `library/source.hero`. Measured against that today:
`ir/mono.rs` 393 + `types/generics.rs` 140 = **533 lines**, and they are only a
saving if the thing they replace is deleted.

Keeping both costs three things:

- **The pass has nowhere good to sit.** A polymorphic body is checked once, with
  `Ty::Generic` live, so evaluation cannot precede monomorphisation — and
  `mono` must precede `own`, because `is_refcounted` answers `false` for
  `Ty::Generic`, *"which is right for `T = int` and a leak for `T = str`"*
  (`ir/mono.rs:10-16`, panel 029 R1). A comptime pass is pinned into the one slot
  between them, and it is a slot that *creates values* the ownership pass was
  designed never to see created.
- **Aggregates have no static form to be created into.** A compile-time `str` has
  one (`HERO_STR_LIT`), but `[1, 2, 3]` emits `hero_array_new` followed by three
  `hero_array_push` calls each releasing the array it grew from — verified in the
  emitted C. A comptime array would have to be **serialised back into IR**, a
  lowering direction that does not exist.
- **Termination stops being provable.** `ir/mono.rs:24-33` refuses a depth limit
  on the record — MLton total for twenty-five years *because* SML bans polymorphic
  recursion, undecidable inference (Henglein 1993; Kfoury–Tiuryn–Urzyczyn 1993),
  and *"Rust is the warning: it accepts at type-check and blows up at codegen…
  late and unattributable"* — and refuses it **structurally** instead. A
  Turing-complete evaluator cannot have a structural argument. Zig ships
  `@setEvalBranchQuota`, whose default budget is 1000 backward branches; Nim ships
  a VM iteration limit. **That is not a cost, it is a contradiction** with a
  decision this compiler already took, wrote down, and enforces with a diagnostic.

The conservative reading of the counter-case is a *restricted* evaluator, Rust
`const fn`-shaped: no unbounded loops, no recursion. That does have a structural
termination argument. It is answered below, on evidence rather than on principle.

### The cheap answer landed this morning, and it closes the measured pain entirely

`docs/measurements/005-magic-constants.md` is the only measurement in this
repository that touches the question: `heroes mutate`'s twelfth operator,
`typo-digit`, found **5 mutants, 0 killed**, and all five were C header values
hand-copied into `.hero` files, with no sixth site in the corpus. Panel 038's
answer was a body-less `constant` inside an `extern` group — **clang evaluates it,
Heroes does not** — at a measured **+28** spec tokens.

Re-measured here at `2ac0403`, after the merge: `typo-digit` **0 sites**, corpus
1252 mutants, 93% / 78%. Sites 5 → 0, which is the number 005 said to report and
not the rate.

Against that same target, the alternatives are:

| answer | sites | spec Δ | new pass | new evaluator |
|---|---|---|---|---|
| write `10000 + 2` in today's body | 5 → 5 | 0 | no | no |
| panel 038's `extern constant` | **5 → 0** | +28 | no | no |
| a restricted (`const fn`-shaped) evaluator | 5 → 5 | > 0 | yes | yes |
| do nothing | 5 → 5 | 0 | no | no |

The third row is the finding. A restricted evaluator closes **none** of the five
sites, because the number is still hand-copied — the authority a header holds is
not something an evaluator can consult. C1 and C2 and the measured pain are
**disjoint**.

### Zig and Nim, sourced

- Zig has no preprocessor and no macros, deliberately: comptime is offered as the
  replacement for both, and the compile-time language runs in a tree-walking
  interpreter inside the compiler.
- `@setEvalBranchQuota` raises a default budget of 1000 backward branches; it can
  only increase it, and its placement is a recurring usability complaint on Zig's
  own tracker (issues #1767, #7407, #11996, #19525).
- Zig's generics *are* comptime: `std.ArrayList(u8)` is a call to a function
  returning a type, and there is no `<T>` syntax at all.
- Nim's VM **disallows FFI at compile time**: any module relying on `importc`
  cannot run in it. This is the same wall `Callee::Extern` presents here, found by
  the language whose FFI design §4.19 copies — so a Heroes evaluator would need a
  refusal set, which is a diagnostic class, which is a `/panel` trigger.
- The honest answer to *"macro semplice"* is C++ `constexpr`: C++11 single-return,
  C++14 loops and locals, C++17 `if constexpr`, C++20 `consteval` + `constinit` +
  constexpr allocation, C++23 `if consteval`. One feature that grew across five
  standards into a family of keywords. **Nothing in the record suggests Heroes
  would be the language where it stayed small.**

## What was settled

1. **Macros are not an open question** — Part 6, `CLAUDE.md` §13 and §6, three
   refusals, and Zig itself does not have them.
2. **Compile-time evaluation is unclassified**, and Principle 0 disposes of it as
   things stand: not on the closure list, no measured Part 11 effect, and the only
   measurement that touches it is closed by an answer that adds no evaluator.
3. **The word bundles seven capabilities**, of which four are refused by something
   other than Part 6 — which is why "Part 6 says no" is not the answer to the
   question that was asked.
4. **Comptime and the generics are not competitors but replacements**, and the
   asymmetry decides the order: comptime is additive and generics are
   load-bearing, so keeping generics and declining comptime forfeits nothing that
   a later version could not add. Part 7 item 5 makes exactly this argument for
   `alias` (`design.md`, Part 7), and it is the ground on which the answer here can
   be *no* without being *never*.
5. **A restricted evaluator would close none of the measured sites.** This is the
   cleanest available demonstration that the feature and the pain are unrelated.

Nothing above is a decision. This session amends no design file and pre-empts no
verdict.

## What stayed open → where it was handed off

- **Does Part 6's Macros row cover compile-time evaluation?** The row's reason
  reads as covering it, but a permanent-rejection list that names one spelling of a
  mechanism and is silent on the other is how a refusal gets rediscovered as an
  idea. Part 6 is design.md Parts 1–11, so it is a panel path — **and the number
  is 039**. → `docs/debrief/DECIDE.md`
- **Whether `docs/reasoning/` is adopted, and in what shape.** This file is the
  directory's only inhabitant here; its README and notes `000`–`002` live on an
  unmerged branch, and the numbering above assumes them. → `docs/debrief/DECIDE.md`
- **Two constants reading each other are not diagnosed** — `check` exits 0, the
  emitted C is two mutually recursive accessors, and the program dies by SIGSEGV
  at `-O0` or hangs at `-O2`. A defect, not evidence for anything.
  → `docs/debrief/DECIDE.md`
- **The spec never says what a `constant`'s body may contain.**
  `spec/heroes-spec.md` shows a single literal; the compiler accepts any block; no
  golden pins the difference. Panel 035's category exactly. → watch list
- **The §4.12 arithmetic** — that generics made the compiler *smaller* — is the
  only admissible argument for or against comptime, and it is a subtraction.
  → `docs/debrief/LEARN.md`
