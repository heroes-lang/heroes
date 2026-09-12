# 005 — M4: lowering, and where the sugar dies

Milestone M4 · 2026-08-04 · panel 019.

## 1. Goal

design.md Part 10 step 6 calls this "the heart": the checked tree becomes a
three-address IR with explicit basic blocks, and Part 5's sugar table is erased on
the way in. `heroes build [--dump-ir]`.

Module born: `ir/`, sixteen files, none over 300 lines. Two files added to the
frontend's answers (`Checked::written_types`) and one command added to the surface.
**304 tests** (was 259): 277 crate, 3 golden harnesses over 42 cases, 21 surface,
6 harness-level.

Three outcomes:

1. **design.md's 320-line acceptance program lowers and verifies** — 35 functions,
   226 basic blocks, one hole reported — and for the first milestone in five it
   produced *no new defect in the program itself*. That is the datum: the
   frontend's recorded answers were complete enough for a pass that reads them
   instead of re-walking the tree.
2. **The dump is an artifact, not debug output**, at **4.77 IR lines per source
   line** against the panel's ceiling of 12 — with a byte-identical-twice test, the
   cheap analogue of §7's double-emit diff a milestone before there is any C.
3. **Panel 019 changed the design in five places**, every one of them because a
   judge compiled something or read something rather than argued: the linkage on a
   callee, the destination on a void call, the place as a store's target,
   construction as an instruction, and the cast at the FFI boundary.

## 2. What surprised — shapes and rules

- **Two rows of Part 5's table cost nothing, and the reason is a surface rule
  written years earlier.** "Named arguments → positional, after checking labels"
  looks like a pass. It is not: §4.9 checks the label written at a position against
  the parameter *at that position*, so arguments were already in declaration order
  before lowering existed. Erasing the label means not carrying it. A rule that
  refuses to reorder is a rule the middle end never has to implement.

- **The `for` loop needs four blocks, and the fourth one is the whole lesson.**
  `continue` must land on the *step*, not on the test, because the increment owes
  itself to that path as much as to the fall-through. A lowering that jumped to the
  test produces a program that compiles, type-checks, passes every existing test,
  and hangs. Nothing in a type system can see it. That is what an adversarial
  golden is for.

- **`&&` is not an operator.** §4.14 says it short-circuits, and a construct that
  evaluates its right side on one edge only *is* a branch. The IR has no `and`
  instruction, no `or`, and no `match` either — a `switch` on a variant tag, or a
  chain of comparisons where `_` is what closes it. The surface's "only
  destructuring construct" has no counterpart at all one pass down.

- **GHC is the wrong precedent for the right reason.** Everyone cites it for
  building a separate desugared tree; what Core actually bought GHC was **Lint**,
  "an 100% independent check on the type inference engine". So the check shipped
  without the tree, and it earns its keep immediately: `preds` recomputed from the
  terminators, every value produced before it is read, and every returning block
  copying out every `@` parameter — §4.8's "copy-out happens always", which panel
  000 named as the place this project would actually stall, is now an assertion
  rather than an intention.

- **Slots beat SSA, and the decisive evidence came from the institution with the
  most to lose by saying so.** LLVM tells frontend authors not to build SSA ("we
  strongly recommend … alloca + load/store … unless there is an extremely good
  reason not to"), noting clang does exactly that for local mutable variables. QBE:
  "phi instructions are NOT necessary". rustc MIR chose non-SSA in 2015 for
  diagnostics rather than performance and has not migrated in eleven years. Every
  verified migration to SSA bought generated-code quality, which this project has
  renounced. ~550 lines avoided.

- **A dump's notation is a design surface, and two of its decisions were coin
  flips until somebody read the artifact cold.** `store total, $t0` is dest-first;
  LLVM's `store` is value-first, so a reader who has seen LLVM guesses wrong — and
  `t0: int @ 0` is *legal Heroes*, so the namespace luck that made the sample
  readable does not hold in general. Both are fixed by notation rather than by care:
  the destination is always left of `=` or `<-`, and `$` marks everything the
  compiler invented. `$` cannot appear in a Heroes program, which is what makes the
  rule "no diagnostic speaks in the IR's vocabulary" a grep instead of a habit.

- **Three blocks in the panel's own sample had exits nobody saw.** Out-of-bounds
  aborts, overflow aborts, division by zero aborts — so `index` and `int` `add` are
  not ordinary instructions, and a pass written on the assumption of one exit per
  block would be wrong and would compile. Every abort-capable instruction now
  carries a trailing `!`, which also distinguishes `add!` on `int` from `add` on
  `f64`: the mark is a type distinction as well as a warning.

- **An unmangled name is a silent FFI bug, and it is legal Heroes today.** A
  top-level `function open` reaching C unmangled replaces libc's `open()`: clang
  says nothing and the program prints `7` instead of a file descriptor. `open` is
  not among the 23 reserved names. The IR now records linkage on every callee for
  that one reason.

## 3. What broke and why

- **The appendix printed `x: ?` for `sqrt`.** Symptom: an `extern`'s parameter
  types rendered as the error type. Cause: lowering asked the *locals* table for a
  parameter's type, and an `extern` has no locals — there is no body to use them in,
  so the resolver never makes any. Fix: `types/lower.rs` now records every written
  type in `Checked::written_types`, keyed by its arena node, and lowering asks that.
  The defect mattered beyond the dump: the emitter writes a C prototype from exactly
  those types.

- **`.num _` made a slot called `_`.** Symptom: `store _ <- $t5` in the dump, and
  the slot's type rendered as `?`. Cause: the payload was read and bound for a
  pattern that binds *nothing* (§4.7 — `_` never binds), so lowering then asked the
  checker for the type of a binding it had never made. Fix: skip the payload read
  when the binding is `_`. The same bug had a twin in `for _ in xs`, found by looking
  for it once the shape was known.

- **A golden case refused to compile, correctly.** `assert add(2, 3) == 5` — and
  `add(a: int, b: int)` has two parameters of one type, so §4.9 makes the labels
  mandatory. The case was wrong, the compiler was right, and the case now says so in
  its own comment. This is the thesis working on the person writing the tests for it.

- **The panel's record miscounted the appendix.** It says six `test` blocks; there
  are seven. Recorded here rather than silently corrected, because the panel's
  numbers are cited elsewhere.

## 4. Left on the record

- **`???` reaching `build` was decided by default.** §4.16 says a file with holes
  type-checks everything else and produces no binary. It lowers, `--dump-ir` prints
  `= ???`, and the summary line says "no binary while the file has holes". Queued.
- **An out-of-range `int` literal is a new diagnostic class** — the first one this
  pass owns, and CLAUDE.md §4 makes a diagnostic class its own panel trigger.
  Nothing before M4 needed a literal's *value*, so it was invisible until now.
- **A function that runs off its end is not rejected.** For a `()` result that is
  correct; otherwise it is a program error the frontend does not catch, and lowering
  does not invent a value. `-Werror=return-type` is the net at M5a. Queued as a
  diagnostic class.
- **`-Wconditional-uninitialized` and `-Werror=format`** for CLAUDE.md §7 at M5a:
  the ffi-pragmatist measured a `ptr` slot stored only inside a loop, read after it,
  segfaulting with clang silent under the current flag set.
- **Predictions now scorable**: the engineer's 12-lines-per-source-line ceiling
  (4.77, pinned by a test) and the ergonomist's collision probe (falsified by design
  — the sigil shipped, so `t0` the slot and `$t0` the temporary cannot be confused).
  The rest wait for metric 2 and M5b.

## What landed, and what carried forward

Moved verbatim from `docs/ROADMAP.md` on 2026-08-12, when the ROADMAP became a
file about what is next (CLAUDE.md §14). The identifiers are the ones this
milestone was built under.

**M4 closed 2026-08-04, tag `m4` — the middle end exists. Part 5's sugar
table is erased on the way into a three-address IR with explicit basic blocks,
**slots and no phi nodes** (panel 019, unanimous, on LLVM's own advice to frontend
authors), and `heroes build [--dump-ir]` is the fifth verb.

Three measurable outcomes. **design.md's 320-line acceptance program lowers and
verifies** — 35 functions, 226 basic blocks, its one hole reported — and for the
first milestone in five it produced *no new defect in the program*: the frontend's
recorded answers were complete enough for a pass that reads them instead of
re-walking the tree. **The dump is an artifact, not debug output**: 4.77 IR lines
per source line against the panel's ceiling of 12, deterministic and byte-identical
twice (the cheap analogue of §7's double-emit diff, a milestone before there is any
C), explicitly **not** version-stable — LLVM's own stance on `.ll`. **Panel 019
changed the design in five places**, each because a judge compiled or read something
rather than argued: linkage on every callee (an unmangled Heroes `function open`
silently replaces libc's — compiled, prints 7, no diagnostic), at most one
destination on a call (`dst = call print(x)` is a hard clang error), a place as a
store's target, construction as an instruction, and an explicit cast at the FFI
boundary. Two vetoes were lifted by amending design.md §4.12 and Part 5 rather than
by argument (CLAUDE.md §12: spec beats compiler).

304 tests (was 259): 277 crate, 3 golden harnesses over 42 cases (`tests/golden/ir/`
is born with 14 — one per live sugar row plus 5 adversarial, marked UNVERIFIED —
and inherits `check/`'s `UPDATE_GOLDEN` ban), 21 CLI surface tests, 6 harness-level.
`ir/` is sixteen files, none over 300 lines, and `verify.rs` is GHC's Core Lint
without GHC's tree: it asserts §4.8's "copy-out happens always" on every exit edge,
including the error side of `?`.
Next: M5a, scalars run — `int`/`bool`/`if`/`while`/functions/`print` → C → binary,
the mangler, `#line` on change, the `-Werror` set, and the double-emit determinism
test that stays green forever. Nothing blocks it. Carried in: `???` reaching `build`
and the out-of-range `int` literal are both decided-by-default and queued as their
own diagnostic classes; a function that runs off its end waits for
`-Werror=return-type`; `-Wconditional-uninitialized` and `-Werror=format` want
adding to CLAUDE.md §7.**


### M4 — Desugar + lowering ✅ (2026-08-04, tag `m4`)
Part 5's sugar table erased **on the way into the IR** — no desugared tree, and
`--dump-ir` is the evidence (one golden per row); three-address IR with explicit
basic blocks, **slots and no phi nodes** (panel 019). Named-arg check ordered
before monomorphisation, which is itself an IR→IR pass at M6. `heroes build
[--dump-ir]`, and an IR verifier that runs in tests — GHC's Core Lint without
GHC's tree.
**Runnable:** `heroes build examples/gallery/00-first.hero --dump-ir` · `heroes build
examples/gallery/05-mutation.hero --dump-ir` (the `@` copy-out chain on every exit
edge) · `heroes build <file>` alone, which says what it lowered and what does not
exist yet. The witness is the appendix, read out of design.md by a test: 320 lines,
35 functions, 226 basic blocks, lowered and verified. (Queued for the author:
hand-desugar three constructs, against the IR text.)


