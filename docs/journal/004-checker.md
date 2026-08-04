# 004 — M3b·c·d: the checker, the errors, and the first measurement

Milestone M3b–M3d · 2026-08-04 · panels 016 and 017.

## 1. Goal

Types, then errors, then the number. M3b's bidirectional checker (§4.5), M3c's
data rules (records, variants, exhaustiveness, `_`'s ban, the same-typed-argument
rule, `ok`/`fail` in ⇐ mode), and M3d's "errors are a deliverable, not plumbing"
— §4.17's rich form, §4.16's hole output, `--json`, `--permissive`, `--apply`.

Modules born: `types/` (nineteen single-concern files) and `mutate/` (metric 3).
Rebuilt: the whole CLI, on panel 016's verdict. **259 tests** (was 169): 240
crate, 3 golden harnesses over 28 cases, 16 surface.

Three measurable outcomes, and they are the milestone:

1. **design.md's 317-line acceptance program type-checks with zero
   diagnostics**, and every expression in it that produces a value has a type.
2. **Metric 3 ran for the first time**: 379 mutants, **94% caught against 76%**
   in the control arm — the first number this project has about its own thesis.
3. **spec v1 landed and the freeze is over**, with the spec now *tested* against
   the compiler rather than proof-read against it.

## 2. What surprised — shapes and rules

- **The acceptance program found a defect in a panel's own exhaustive sweep.**
  Panels 002/006 rewrote eleven `T`-where-`T?` sites to `ok(...)` "when applying
  the rule exhaustively". There were twelve: `tokenize`'s own `return out`. Four
  milestones, four defects in that program — a variant case the registry
  reserved, an `assert` where only an expression was allowed, two library names
  the language reserves, and now a missing `ok(...)`. It has never been run.

- **A judge that builds the option beats a judge that reasons about it, and a
  judge that reasons can still fix the builder's design.** Panel 017 asked how a
  jump should be typed. The compiler-engineer built all three answers and
  measured that typing a jump *at all* rejects every function whose body ends in
  `return` — RFC 1216's recorded wrong turn, reproduced in a scratch clone. Then
  the historian, holding only precedent, objected that the winning option's cost
  is *one rule per branch-joining construct* and named its own withdrawal
  condition: one shared join routine with divergence as data. Building that
  satisfied both, and the file is 84 lines.

- **Two judges vetoed the same clause from opposite directions without seeing
  each other's evidence.** Panel 015's two-tier built-in namespace died because
  the ergonomist found it non-local (`xs.map(f)`'s meaning would depend on a
  distant line) and the warden found it an *exception* to a rule the spec already
  states. Differentiated inputs converging is the only agreement that carries
  information.

- **A rule can be loud and still need stating.** The warden's argument that four
  of five rules cost zero spec tokens — because a good diagnostic *is* the
  specification — is correct and was adopted for three of them. It fails for the
  fourth, and the ergonomist's belief probe is why: asked whether a write-only
  cell is an error, a model answered "no" from memory at 60% confidence, about a
  proposition the document did not contain. Confidently answering an unanswerable
  question is the failure the spec exists to prevent, so thirty tokens were spent
  against the warden's explicit condition, and its prediction is falsified on the
  record.

- **The document had been briefing models into a compile error for four
  milestones.** Panel 013 ratified `(function(A) -> B)`; spec v0 still said
  `(fn(A) -> B)`, which the foreign-word registry rejects *with a prescribed
  fix*. Nobody noticed because the spec was frozen and the freeze was never
  re-examined. It is now four tests, and the interesting one is mechanical: no
  word the registry rejects may appear in the spec's code spans.

- **Only running a fix proves it is certain.** The `.fixed` goldens — apply every
  `certain` fix, then check the result — caught `wrong_label` producing `y:: 2`
  on their first run, because the label's span does not include the colon.
  CLAUDE.md §8 says only `certain` is machine-applicable; that is a claim about
  every fix the compiler tags, and a fix that leaves the file broken falsifies
  it.

- **The measurement's honest columns are the ones that show no effect.**
  `typo-ident` scores 100% in *both* arms: typos are caught by names and types,
  which any compiler would have. Naming that in the record matters more than the
  headline, because it is the part a partisan reading would quietly keep.

- **Two mutants survived correctly, and they are panel 011's counter-arm
  appearing by itself.** Turning `v: int @ 0` into `v = 0` is only a mistake if
  something later writes `v`. Where nothing does, the mutation preserves meaning
  — so a survivor is the right answer, and an operator table that expects 100%
  is the thing that is wrong.

- **The gallery is where a rule's price becomes visible.** §4.9's
  same-typed-argument rule is one of the thesis's headline mechanisms and it
  lands hardest on FFI: C's numeric APIs are where same-typed parameters cluster,
  so `hypotenuse(a:, b:)` and `pow(base:, exponent:)` now carry labels at every
  call site. That cost was theoretical until nine readable programs had to pay
  it.

## 3. What broke and why

- **`(fn(A) -> B)` survived the first pass of the v1 amendment**, in the generics
  line rather than the function-type line. Found by the test written to make the
  class impossible, twenty seconds after writing it.

- **A value block ending on a *statement* was treated as if it diverged.** So
  `x = if c` with a loop in the branch bound `x` to nothing, silently. Diverging
  and "ended on a statement" are now two messages, because they are two facts.

- **Binding a `()` reported twice** — once for the binding, once for the read of
  the name it created. The local is poisoned after the first message now, which
  is the same rule the resolver already held.

- **`fmt` deleted a doc comment**, twice in one afternoon. The blank-line rule
  needs to know where a statement *ended*, and the statement's span reaches past
  a `Dedent` onto the next line, so the trailing-comment scan stole the following
  declaration's documentation. Panel 014 had fixed this exact bug once and the
  comment above the line warned about it; the fix is now a narrow helper that
  answers only for list literals, with the warning kept.

- **The formatter invented a blank line after every multi-line list**, for the
  same reason and found by the same new test.

- **A judge's scratch git worktree was committed** as an embedded repository. Now
  ignored, and the commit was amended.

- **Four files crossed the 300-line rule mid-milestone** and were split where
  their concerns split: `exprs`/`expect` are ⇒ and ⇐, `join` is the one branch
  rule, `construct`/`generics`/`access`/`arms` each own one idea, and `errors/`
  is a directory of three. The compiler-engineer had predicted exactly this in
  panel 017 and the prediction is scored correct.

## 4. Left on the record

- **Metric 2 has still never run.** No model has written Heroes from the spec in
  this project's history. The thesis keeps its *measured mechanism* and does not
  yet have its measured *claim* — panel 011's own words, and the reason
  `docs/measurements/001` carries three caveats.
- **The corpus is assistant-written.** `harness/tasks/README.md` requires
  author-written tasks for metric 2 precisely so the measurement is not of the
  assistant's priors; metric 3's corpus inherits that weakness.
- **`print`'s reservation** has PEP 3105 against it: Python made `print` a
  function *so that* a module could replace it, which is how output is captured
  in tests. Heroes has `test` blocks and no capture mechanism.
- **`--no-line` will not be built at M5a**, though CLAUDE.md §7 mandates it:
  panel 016's stopping rule refuses it, and §7 now says so.
- **A `match` over `bool`** is treated as countable, so `_` is banned — but its
  two cases are literals, not `.cases`, so an exhaustive `bool` match is
  currently unwritable. Found while writing `patterns.rs`; queued.
- **`()` inside a container** (`[()]`, `{str: ()}`) still resolves clean. Ten
  lines to reject, and the engineer's condition 4 says C1 is not credited with
  closing it.
