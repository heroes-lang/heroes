# Panel 234, the ffi-pragmatist's first brief: SQLite, thin and thick

Read `00-shared.md` first, then `conventions-draft.md`, `probe/README.md`
and `blind/README.md`. This brief is item D alone, and it is the material of
the sitting's reader test, so it is due before your seat's other brief: the
coordinator builds the test from what you write here, and the spec-warden
registers its predictions on it before any session runs. **You write both
shapes, not the coordinator**, because the coordinator's own recommendation
was *thick*, and the instrument that judges a recommendation is not built by
the one who made it. Write both as well as you can; a thin shape written
badly would decide nothing.

**Your worktree** is `.claude/worktrees/234-ffi-sql/`, detached at the commit
that adds these briefs; build your compiler there from the seed and check its
key (`00-shared.md`, *The tree*). Your scratch is
`.claude/worktrees/234-ffi-sql-scratch/`. Your report is
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/234-reports/ffi-pragmatist-sql.md`,
written as you go, and what you deliver goes under
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/234-reports/ffi-pragmatist-sql/`.

## The two shapes, defined so that only the thickness differs

Both are reached by the same line, `use core/database/sql`, which binds `sql`,
and both stand on SQLite through `extern "sqlite3.h" link "sqlite3"` (spec
§ 13's own example and `examples/sqlite/main.hero` are where to start).

- **Thin**: `core/database/sql.hero` is the C module itself: the `extern`
  group and, beside each member, the one Heroes function a program calls,
  named as C names it without the `sqlite3_` prefix (`sqlite3_open` is
  `sql.open`, `sqlite3_column_int64` is `sql.column_int64`), its parameters in
  C's order, every status SQLite answers handed back as the integer C answers
  and compared by the program with the group's constants (`sql.SQLITE_ROW`,
  `sql.SQLITE_DONE`, `sql.SQLITE_OK`), the handles the group's records with
  their marks (`acquires`, `consumes`). Only what a Heroes call needs is added:
  a `str` lent as `cstr`, a text column validated into a `str`.
- **Thick**: `core/database/sql.hero` is an API over a C module beside it,
  `core/database/sql/sqlite3.hero` (the thin layer, which no program `use`s):
  every operation that can fail answers a `T?` with a code (`conventions-draft.md`
  §3), a statement's life is not the program's to manage unless it asks, and
  the shape is the one you judge a Heroes programmer would want, with no
  integer status reaching the program. Name its functions by the author's rule
  (a Go name only where the contract is Go's; Go's own package here is
  `database/sql`), **with the boundary `conventions-draft.md` §2.2 proposes**
  (F23): Go's `(T, error)` and a Heroes `T?` are one contract, so `Open`,
  `Exec`, `Query` and `Close` keep their names where they keep that contract; a
  sentinel or a `found`/`ok` flag is another. If you judge that boundary wrong,
  say so in your report and name the shape by the boundary you would adopt,
  stating it.

Each shape covers SQLite as far as the three tasks of `blind/README.md` need
(open an in-memory database, execute a statement, insert with a bound
parameter, query rows and read an integer and a text column, tell a `UNIQUE`
refusal from success, close), and no further: the test compares two faces of
one capability, not two capabilities. **The tasks take their data from
`args()` and read no file** (F1: `to_i64` refuses a `str` and the language has
no `split`, so a file would have measured a text parser as much as the
package); their statements and cases are the coordinator's, in
`docs/panel/234-briefs/blind/Q/tasks/Q1` to `Q3` (`task.md`, and `caseN.args`,
one argument per line, with `caseN.want`). **Q1's second case has no
argument**, so SQL's `sum` is `NULL` (F21): how each shape hands a `NULL` to
the program is part of its thickness, and your report says what each of yours
does. **Q2's and Q3's hidden cases hold an apostrophe in a name** (`o'neil`,
`d'arc`), so a program that writes a value into its SQL text breaks and one
that binds it does not.

## What you deliver, under `docs/panel/234-reports/ffi-pragmatist-sql/`

1. `thin/core/database/sql.hero` and `thick/core/database/sql.hero` with
   `thick/core/database/sql/sqlite3.hero`, canonical (`heroes fmt --in-place`),
   each module with its `test` blocks.
2. **A reference program for each task in each shape**,
   `thin/q1.hero` to `thin/q3.hero` and `thick/q1.hero` to `thick/q3.hero`,
   each the program a careful Heroes programmer would write from the card
   alone, taking its data from `args()` as the task says and printing exactly
   what the task asks; run each on its task's three cases (the lines of
   `caseN.args` as its arguments) and compare with `caseN.want`, writing the
   outputs beside it, `q1.case1.out` and so on. They are the tasks' proof, as a
   held-out task's `reference.hero` is; a case your reference and the
   coordinator's truth disagree on is a finding for your report, not a truth
   to change.
3. **The card for each shape**, `thin/card.md` and `thick/card.md`, in the
   format `blind/README.md` gives: the `use` line, every type, every
   function's signature and one line, every constant a program reads, every
   failure code. A card is all a reader of the test will have besides the
   specification, so it must be true of the module and sufficient for the
   tasks, and no more.
4. **The runs that show both shapes work**: `heroes test` on a root driver for
   each, and each reference on its inputs, on this Mac and in the Linux arm64
   container (`docker run --rm -v <your worktree>:/w -w /w
   heroes-linux-arm64:latest ...`; build the compiler inside it from the seed;
   whether `sqlite3.h` and `libsqlite3` are in that image is a question you
   answer by running it). Write what ran where in your report.

## Your report

When the four deliverables stand, write at the top of your report the line
`DELIVERED: <the time>` and the commands that re-run them; then say, as a
judge, which shape you would adopt and why, what each shape gets wrong for a
programmer (an integer status compared with the wrong constant, a statement
never finalised, a handle used after close: say which of these the compiler
refuses, which the runtime aborts on, and which run silently, measured), and
whether a third shape you did not write would beat both. Same report shape as
every seat: verdict, section, cost, prediction, condition.
