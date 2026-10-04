- [x] **005 — The parser accepted a continuation at bracket depth zero** | Date: 2026-08-28, `/decide` answer `5a`. **Found by `heroes fmt` refusing to format a file, and the refusal was right.** | **Status: fixed 2026-08-28** — `selfhost/grammar_expr.hero`, `ends_the_expression`. | moved here 2026-09-03 from `docs/defects/005-a-continuation-the-language-does-not-have.md` by author instruction — the directory is gone, its text is below, unedited except that its `## ` headings became bold leads | Severity: **★★** — a program that two ratified rulings forbid compiled, ran, and produced a meaningful answer. Nothing was silently wrong; a form that should not exist simply existed.


  Date: 2026-08-28, `/decide` answer `5a`. **Found by `heroes fmt` refusing to
  format a file, and the refusal was right.**

  **Status: fixed 2026-08-28** — `selfhost/grammar_expr.hero`,
  `ends_the_expression`.

  Severity: **★★** — a program that two ratified rulings forbid compiled, ran, and
  produced a meaningful answer. Nothing was silently wrong; a form that should not
  exist simply existed.

  **How it was found, which is the part worth keeping.**

  `docs/defects/003` gave `heroes fmt` a guard that re-reads its own output. One
  shape then refused for a week:

  ```
  function f(c: str) -> str
      return match c
          "a" => "b"
          _ => c
      + "x"
  ```

  ```
  error: `fmt` produced source that does not parse
  error: this is a compiler bug — `…` was NOT changed
  ```

  The obvious reading was that the printer had a hole, and that is how it was
  written down: *"one shape `heroes fmt` still cannot spell"*. The item put the
  question the other way round as well — **which of the two artifacts is wrong?**
  — and that is the question that turned out to matter.

  **What the program meant.**

  ```
  $ heroes parse v7.hero --dump-ast
    function f(c: str) -> str
      return (match … + "x")

  $ heroes run v7run.hero
  bx
  ```

  So the `+ "x"` was really being applied to the `match`'s result, across a dedent,
  at bracket depth zero. `heroes check` exited 0.

  **Why that is not allowed.**

  Two ratified places say so in almost the same words.

  **Panel 007, Amendment B**, the bracket clause: continuation is *"inside `(` `[`
  `{`"* and nowhere else. The sitting's own § Disagreements records the
  ergonomist arguing for a depth-0 continuation and the engineer showing the clause
  unsound; the resolution took *"continuation inside brackets only"*.

  **design.md:1794**, in the section that rule lives in:

  > At bracket depth zero every line's indentation is structural: a long expression
  > is broken inside parentheses or not at all.

  So `fmt` was correct to refuse: **there is no canonical spelling of a form the
  language does not have.** CLAUDE.md §12 settles the rest — spec beats compiler,
  and the compiler had the bug.

  **The cause.**

  `grammar_expr.hero`'s `binary()` parses an operand and then climbs while the next
  token is a binary operator. An `if` or a `match` in expression position carries a
  **block**, and parsing that block consumes the dedent that closes it — so by the
  time `binary()` looks again, the next token *looks* adjacent to the operand when
  it is on a later line at a shallower indent, belonging to the next statement.

  The existing golden `tests/golden/check/depth-zero-continuation.hero` guards the
  other half of the same rule and passes:

  ```
      total = 1 +
          2  #~ expected_expression
  ```

  There the continuation line is **indented**, so the lexer hands the parser an
  indent and the parser says `expected_expression`. The dedented case had no
  guard, because nothing in the token stream distinguishes it — the block ate the
  dedent.

  **The repair.**

  One predicate and one early return. `ends_the_expression(a, id)` answers whether
  an operand carries a block; `binary()` returns immediately when it does, because
  the expression ended with the block.

  The enumeration is exhaustive with no catch-all, which is panel 060's lesson in
  the formatter applied here: a new expression kind that carries a block must be
  made to answer, and a `_ => false` would answer *"no"* silently.

  **`out + match c` is untouched**, and the reason is structural rather than lucky:
  there the control form is the RIGHT operand, which is the only side its block can
  be on. `print_fmt.hero`'s `binary_control_tail` says the same thing from the
  printer's side, and the two now agree by construction instead of by accident.

  The diagnostic is the one that already existed — `expected_expression`, on the
  author's own line, pointing at the `+`. **No new diagnostic class, so no panel**:
  this is a compiler made to obey a ruling it already had.

  **What it changed about `fmt`.**

  That file is now **exit 1** — the input has diagnostics — instead of **exit 2**,
  the compiler accusing itself. The attribution is the honest one, and it is what
  `/decide` answer `5a` bought.

  **The lesson, and it is about the instrument rather than the bug.**

  **A guard that reports a compiler bug can be reporting the truth about a
  different compiler part than the one it points at.** The output guard said *"the
  printer produced something that does not parse"*, which was accurate and
  misleading: the printer had produced the only spelling available for a tree the
  parser should never have built. Two of this file's three sentences of diagnosis
  came from asking `--dump-ast` and `run` what the program MEANT, rather than from
  reading the printer.

  Both tests that used this shape to make a guard FIRE were rewritten as predicate
  tests in the same commit, and the reason generalises: **a firing test that
  depends on a live defect stops firing the day the defect is repaired, which is
  the day it stops being a test.** §9 asks for a check that fires, and it has to
  keep firing.
