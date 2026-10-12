---
kind: defect
area: spec
milestone: none
filed: 2026-10-12
commit: self
github: none
---

- [x] **676 — The reserved-word registry says texts the compiler no longer prints, and has no row for `int`** | `spec/reserved-words.md` gave `fn`'s example as `function f(x: int) -> int`, `var`'s as `v: int @ 0`, `const`'s as `constant MAX: int` and `null`'s answer as `int?` with no word of `nullptr`, where `declared_word` prints `i64`, `v @= 0` and the `nullptr` clause; it had no row for `int`, which the language has refused since 2026-08-12; its `return;` row said a text the compiler does not print; its `@name` paragraph gave `v: int @ 0` as the declaration; its keywords' paragraph left out `nullptr`; and no check compared a row to the compiler | `spec/reserved-words.md` · `selfhost/keywords.hero` (`declared_word`) · `tests/harness/suite_spec.hero` (`registered`) · **class: blocking**

    **Origin:** found 2026-10-12 by the coordinator's audit of the editor grammar, on the author's request to bring the plugin up to date; the lane ran each row the same day, one scratch program per foreign word under `./heroes check`, and `return;`, a bare `return` and the `@` before a name each in a program of its own.

    **Class: blocking**, 2026-10-12: a false page in public. The registry is a file of `spec/`, and six of its statements were false: four rows, the `return;` row and the `@name` paragraph's declaration; the keywords' paragraph left out `nullptr`, and the table had no row for `int`.

    What was measured, 2026-10-12, with the lane's compiler built from the seed at `b74acf87`. Each foreign word in the place its own repair fits (`struct Point` at the top level, `let x = 5` in a body, `x: int` as a type) printed `declared_word`'s message byte for byte, and in a body the declaring words print another, naming where a declaration goes (`selfhost/foreign_at.hero`). `return;` in a function `-> i64` prints `` `;` is not part of the language's syntax ``, with a certain fix deleting it, and a bare `return` there prints ``this function returns `i64`, so `return` needs a value``; neither is the row's *`return` with no value only in functions returning `()`*. `@v = 1`, `@v: i64 @= 0` and `@v` alone each print `expected_expression` at the `@`, naming `v @ e`, `v: <type> @= <value>` and `f(@v)`. `int` was deleted from the language and refused at `3280ffeaa` (M-sized-integers step 3, 2026-08-12), and `selfhost/` refused it from its first commit, `2212ba5b3`; the table's four rows have said `int` since the registry was written at `d10fbec74`, 2026-08-03.

## The repair

Each row now says what the compiler prints, the `int` row added, and the
`return;` row takes the compiler's text: it is refused with a prescription, the
`;` deleted, so the row was corrected rather than left. A second row holds the
bare `return` the old row was about, ``this function returns `i64`, so `return`
needs a value``. The `@name` paragraph names the three legal forms the message
names, and the keywords' paragraph `nullptr` and `@=`. The opening paragraph
says that a word standing where its own repair does not fit is told otherwise.

The check that was missing is `registered` in `tests/harness/suite_spec.hero`,
labelled `spec/registry`: a row naming foreign words is held to the arm of
`declared_word` that refuses them, read from the source, the same words and its
text the arm's message after `error: `, whitespace aside; a row naming no
foreign word is run, each by a program of `registry_probes()`, and a row no
probe runs is red; the keywords' paragraph names `keyword()`'s words. Red
before the registry moved, in the run that also held the grammar's checks:
`spec: 26 passed, 5 failed`, its `spec/registry` naming the four rows, the
missing `int`, `nullptr`, and the two `return` rows; green after,
`spec: 31 passed, 0 failed`. `REGISTRY_TOKENS` is 1139, from 916, measured by
`heroes measure`'s vendored maximum as the check measures it: +223, the `int`
row, the second `return` row and the sentences that say where the text
differs, a document outside design.md §1.6's budget.

**No sitting was convened**, and the author may overturn that: no behaviour of
the lexer, the grammar or the checker moved, and the registry was brought to
rulings already ratified, panel 209 for `@=` (ratified 2026-10-10) and `int`
refused since `3280ffeaa`.

**Closed 2026-10-12** at the commit that writes this section, judged by `spec`,
`canonical`, `records`, `unseen` and the net's own tests in the lane.
