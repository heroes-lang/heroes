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

## What landed, and what carried forward

Moved verbatim from `docs/ROADMAP.md` on 2026-08-12, when the ROADMAP became a
file about what is next (CLAUDE.md §14). The identifiers are the ones this
milestone was built under.

**M3 closed 2026-08-04, tag `m3` — the frontend is complete. M3a the
resolver, M3b the bidirectional checker, M3c the data rules, M3d errors as a
product. Panels 015 (the resolver's rejection set), 016 (the command surface) and
017 (the type system's four edges) all decided and ratified; nine judges ran, and
three of them changed the design on evidence rather than argument.

Three measurable outcomes. **design.md's 317-line acceptance program type-checks
with zero diagnostics** — getting there found a twelfth `T`-where-`T?` site that
panels 002/006 had missed, the fourth defect that program has surfaced in four
milestones without ever being run. **Metric 3 ran for the first time**: 379
mutants, 94% caught against 76% in the control arm, recorded with provenance in
`docs/measurements/001-metric-3.md` — the project's first number about its own
thesis, with the columns that show *no* effect named as plainly as the ones that
do. **spec v1 landed and the freeze is over**: seven panels' amendments, 160
measured tokens of removals spent, 2139 binding max (from 2048 — the number read
2136 until panel 018's sweep landed, which cost +3 measured against a table that
had sold its shape as −4 on v0: deltas measured on v0 did not transfer), and the spec is
now *tested* against the compiler — because it had been briefing models into a
reserved-word error for four milestones.

259 tests (was 110 at M2): 240 crate, 3 golden harnesses over 28 cases (5
adversarial from M3a, 5 from M3b–d, 4 bulk, 12 inherited, 4 with `.fixed`
expectations), 16 CLI surface tests where there were none. `heroes` itself was
rebuilt on panel 016's verdict: one strict table-driven argv parser, exit codes
0/1/2 printed in `--help`, `--dump-<stage>` per design.md §3.5, `--in-place`
where `--write` used to be, and a stopping rule in CLAUDE.md §10 so the surface
does not sprawl.
Next: M4, desugar and lowering — Part 5's sugar table erased on the way into the IR, a
three-address IR with explicit basic blocks, `heroes build --dump-ir`. Nothing
blocks it. Carried in: `()` inside a container is still accepted, a `match` over
`bool` cannot be written exhaustively, and the same-typed-argument rule's cost on
the FFI boundary wants a panel.**


### M3 — split in four (each with its own runnable artifact)

- **M3b — Checker core ✅** (2026-08-04)**:** bidirectional ⇐/⇒, one shared join
  rule for every branching construct, and a jump with no type (panel 017 A).
  **Runnable:** `heroes check <file>` — the appendix type-checks clean.
- **M3c — Data ✅** (2026-08-04)**:** records, variants with payload,
  exhaustiveness, the `_` ban, the same-typed-argument rule, `ok`/`fail` in ⇐
  mode, and a declaration rejected as an inline arm body (panel 017 D).
- **M3d — Diagnostics as a product ✅** (2026-08-04)**:** §4.17's rich form (the
  line, the caret, the note carrying the other end, the tagged fixes), `.fixed`
  goldens that assert an applied `certain` fix checks clean, §4.16's `???` output
  capped at 5 and deterministic, `heroes check --json` (schema 1), `--brief`,
  `--apply`, and `--permissive` — the control arm metric 3 needs.
  **Metrics 1 and 3 have run**; metric 2 still needs a model or paced sampling
  with author-written tasks (`harness/README.md`), so the thesis keeps its
  measured mechanism and not yet its measured claim.


