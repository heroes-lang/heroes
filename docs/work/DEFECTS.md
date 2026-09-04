# DEFECTS — the compiler defects that are still open

Read by whoever attacks a defect. Every item here is a **measured** failure of
the compiler on a program — a crash, a wrong answer at exit 0, a silence where
a message is owed — with its reproducer, its cause where known, and what is
owed. It exists because the author said so on 2026-09-03: *"I do not like the
defect directory … if anything is still open in defect at the end, make one
single file called DEFECTS.md inside work, so that everything is tidy"*.
Seven files under `docs/defects/` became this list and six entries in
`docs/work/DONE.md` that evening; the directory is gone.

**Only open defects live here.** The moment one is repaired its entry is ticked,
gains a *The repair* section with the measurements that prove it, and moves to
`docs/work/DONE.md` — the record — as every other list in this directory does
(CLAUDE.md §3). A defect that stays here after its fix is the shape §3 was
amended to prevent.

**One notation: `- [ ]`.** A finding written as a bare bullet is invisible to
every count in this project. The body of an entry is indented under its line;
it may be long, because a defect's reproducer, cause and measurements are the
entry, not decoration.

Format: `- [ ] **NNN — <title>** | <date, found by> | <status> | <where it came from> | <severity>`, then the body.

- [ ] **011 — two `@` arguments sharing one root compile and run, and design.md has promised a compile error since 2026-08-04** | 2026-09-04, found while retiring `docs/panel/OPEN-QUESTIONS.md`, whose entry claimed panel 010 had closed it | **open**, measured and not repaired: the repair is the checker's and this session's scope was the record | panel 010, ratified 2026-08-04 (`DESIGN-LOG.md:43`, `docs/work/DONE.md:91`), written into `design.md:1287-1289` | **the class this language exists to delete**: exit 0, and the answer depends on which reading of §4.8 the reader assumes

  **The reproducer**, and it is design.md's own first example in today's spelling:

  ```
  function shift(@a: i64, @b: i64)
      a @ a + 1
      b @ b + 10

  function main()
      n: i64 @ 0
      shift(a: @n, b: @n)
      print(n)
  ```

  `heroes check` **0**, `heroes run` **0**, and it prints **10**.

  **Measured 2026-09-04 on a seed build of `7ba6b73e`, all four shapes, and every
  one of them passes.** The first three are the three `design.md:1287-1288` names
  by name as compile errors:

  | the call | `check` | `run` | prints |
  |---|---|---|---|
  | `shift(a: @n, b: @n)` | 0 | 0 | `10` |
  | `shift(a: @p.x, b: @p.x)` | 0 | 0 | `10` |
  | `shift(a: @xs[0], b: @xs[0])` | 0 | 0 | `10` |
  | `bump(a: @n, b: @n, c: @n)` | 0 | 0 | `100` |

  **`10` is the divergence panel 010 sat over, arriving silently.** §4.8's
  copy-in/copy-out gives last-write-wins, so `b`'s `+ 10` lands on `n` after
  `a`'s `+ 1` is overwritten; reference semantics — the model every mainstream
  language installs, which is the sitting's whole argument — would give **11**.
  The panel's words, quoted in the watch-list entry that found this: the two
  readings *"yield different answers"* and *"a model will guess wrong silently"*.

  **What the rule is FOR is stronger than an ergonomic preference**, and it is
  `design.md:1289-1291`: the refusal *"is the precondition that makes §4.10's 'no
  aliasing exists anywhere' true, because two copy-outs landing on one place is
  aliasing of the destination."* So the spec's line 80 — *"No aliasing exists
  anywhere"* — is false of this program, and §12 says the spec beats the compiler.

  **Why nobody noticed, and it is two failures rather than one.** First,
  `design.md`'s three examples are written in a spelling the language no longer
  has: `shift(a @ n, b @ n)` is `error[misplaced_mutable_marker]` today, *"a
  named mutable argument is written `name: @value`"*, with a `certain` fix.
  Anybody who pasted the document's own example got a diagnostic and could read
  it as the rule firing. Second, the same-typed-argument rule fires first on the
  unlabelled form — `shift(@n, @n)` is two `needs_label` errors — so the shape
  only reaches the alias question once both labels are written, which is exactly
  the form nothing tests.

  **The searched-for absence, reported as a search and not as a fact** (CLAUDE.md
  §1: a negative claim rests on the searcher's vocabulary): no diagnostic code for
  this class in `selfhost/` (every `code: "…"` under `selfhost/resolve/` and
  `selfhost/check/` enumerated, ~115 codes, none of them this), no golden under
  `tests/golden/check/` matching `*repeat*`, `*alias*` or `*twice*`, and no
  cross-argument comparison at the call site: `selfhost/resolve/walk.hero:194`
  calls `writes.inout_root` once **per argument**, and `inout_root`
  (`selfhost/resolve/writes.hero`) holds each argument alone to the rule of the
  left of `@`, while `selfhost/check/walk.hero:1459` compares argument *labels*
  and not roots. Panel 010 R6 assigned the work to *"the routine that already
  builds the label→argument map for the same-typed-argument rule"*, at M3c — an
  id that is now `M-data-declarations`, closed. **`grep -n "M3c" docs/work/DONE.md`
  returns nothing**: the implementation was never recorded as done or as owed.

  **A live golden already asserts the rule as if it existed**:
  `tests/golden/run/inout-through-paths-of-a-cell.hero:10` says two `@` arguments
  through one cell are *"panel 010's alias refusal, whatever the indices"*. That
  case pins the legal two-cell side, so the sentence is a claim nothing runs —
  §11's premise that expires in silence, in a test file.

  **What is owed.** The refusal at the call site, keyed on the root a place
  resolves to (`selfhost/grammar_expr.hero:873-875` says why that is a comparison
  of roots and not a dataflow analysis: *"since Heroes has no references, every
  place has exactly one root"*); the `certain` fix `design.md:1288` already
  specifies, passing a copy for the second argument; a `fixedbugs` golden under
  `tests/golden/check/` with its `#~` annotation (§9), named for this defect; and
  the deliberate over-rejection of distinct-index pairs that `docs/work/DONE.md:91`
  records as part of the ratified rule. **And design.md's three examples are
  re-spelled in the same commit** — a document whose reproducer no longer parses
  cannot be used to check anything.
