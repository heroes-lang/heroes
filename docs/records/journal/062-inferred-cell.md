# 062 — M-inferred-cell: a cell declared by its own symbol, typed by its value, and the tree rewritten through the compiler's own fix

## Goal

Panel 209's resolution as the author ratified it on 2026-10-10 at 17:57
(`docs/panel/209-a-cell-is-declared-by-its-own-symbol-and-typed-by-its-value.md`):
a mutable cell is declared by its own symbol, `@=`, and re-bound by `@`; its
type is optional and inferred from its value as a binding's is; a cell nothing
re-binds is a compile error with a certain fix writing `=`; the mutation of a
name nobody declared says how a cell is declared; and what a cell's value
cannot say, an empty container or a bare `nullptr` where a handle was meant, is
demanded at the birth. Landed in the order the sitting measured, four commits in
lane `lane-inferred-cell`: the rule first (`484c57ec`), the symbol with the type
still required and the tree rewritten (`65cd1145`), the type optional
(`e2165913`), the documents (`7a0a7b2e`); then this close.

## What surprised

- **A whole-tree rewrite is the compiler's own fix applied, and the census is
  the gate.** `v: T @ e` is told `old_cell_symbol` with a certain fix writing
  `@=`, and `check --apply --in-place` over every root moved 1,181 files in
  step 2; two cases where `--apply` stops at the lexer, a bracket left open,
  were rewritten by hand. The proof that nothing was missed is not the
  rewrite's log but a census afterwards, every tracked golden and example and
  the two module roots checked by the new compiler, one process per file:
  `old_cell_symbol` nowhere.
- **A string is the one place the compiler cannot see.** The pass over the
  string literals of the compiler's and the harness's tests rewrote
  twenty-five mutations `n @ n + 1` as `n @= n + 1`; a mutation written `@=`
  reads as the expression before it and a diagnostic the test did not expect,
  twelve own tests and four of the net's went red at once. A scanner over
  every literal, a `@=` with no `: ` before it on its line, found them all.
- **A restore from HEAD is where a rewrite is lost.** The gallery's programs
  had been restored after the first pass and not rewritten again; the census
  found them, and `check --apply` moved them in seconds.
- **A program the compiler refuses passed a green gate.** The compiler built
  from step 1's own commit refuses `examples/gallery/12-interpolation.hero`
  (`never_rebound`): ten of the gallery's fourteen programs are built, checked
  or run by no suite, the entry-point premise `corpus` and `emission` share.
  Defect 606, `adjacent`; the example repaired by the rule's own fix.
- **A new token joins every predicate that lists the old one, and a golden
  about one message is what finds the one it missed.** Four scans of "a
  binding line", `at_prefix`'s classifier and the call's argument scan under a
  bracket left open each named `.at` and `.eq`; with `@=` a declaration under
  an open `(` was told `expected_end_of_line` a second time.
- **The ceilings held by moving what belongs elsewhere, and the budget moved
  with its reason.** `check/walk.hero` stays at 1870 because what no birth can
  hold, `()` and the bare `nullptr`, lives in `check/empty_binding.hero`;
  `grammar_expr.hero` is at 1084 because the two symbols after an expression
  are read by `cell_errors.hero`, a new leaf at the top; `selfhost/parse/`'s
  budget rose 8699 to 8735 for the two readers of the symbol, written beside
  the row, never moved to fit a count.
- **A write into a poisoned place was a second message.** `mismatch.compare`
  kept quiet on a poisoned expectation and `mismatch.shown`, the literals'
  path, did not: `xs[0] @ 1` under `xs @= []` was told `expected ?`. One
  guard, the same as the other's.
- **Thirteen emission reds found a wrong rule before any golden did.** The
  first build of step 3 told `cannot_infer` on `db: CDb @= nullptr` too, an
  annotated cell, because the guard did not ask whether the type was written;
  the `emission` suite's thirteen refused programs said so in one line each.
- **Two files lost a write once, between parallel commands, and the cause is
  not known.** Four parser edits made by `sed -i` in one command were printed
  back changed and were found unchanged minutes later, their modification
  times earlier than the command; redone one file at a time, every write
  held. Recorded as a question, not a premise: no instrument here can see it.

## What broke and why

- **Symptom**: twelve own tests red after step 2's string pass, `discarded_value`
  where `marker_mismatch` was expected, counts of zero. **Cause**: a mutation
  inside a test string rewritten `@=`. **Fix**: the scanner's `--fix`, 25 lines
  in 17 files (`65cd1145`).
- **Symptom**: `surface` 4 red, three `fmt` rows and one JSON byte offset.
  **Cause**: step 1 had rewritten the fixtures' decorative cells `=` and the
  rows pinned `fmt`'s old output, red at HEAD unrun; the offset moved by one
  `=`. **Fix**: the rows say what the fixtures hold (`65cd1145`).
- **Symptom**: `fixedbugs-131-a-bracket-left-open-at-its-line-end-is-one-message`
  two messages. **Cause**: `grammar_expr.call_args` read `.at` and `.eq` as a
  binding line below a `(` never closed and not `.at_eq`. **Fix**: the four
  scans and the classifier take `.at_eq` (`65cd1145`).
- **Symptom**: `layout/budget`, `selfhost/parse/` 8735 past 8699. **Cause**:
  `annotation.cell_symbol` and `at_prefix.lent_with_eq`. **Fix**: the row
  raised with the reason beside it (`65cd1145`).
- **Symptom**: `expected ?, found i64` at a write into a cell born `[]`.
  **Cause**: `mismatch.shown` had no poison guard. **Fix**: the guard
  (`e2165913`).
- **Symptom**: `emission` 13 red, `cannot_infer` at `db: CDb @= nullptr`.
  **Cause**: `empty_binding.poisoned` asked `cell` and not whether the type
  was written. **Fix**: `bare_cell: cell && ty.is_err()` (`e2165913`).
- **Symptom**: `canonical` red on two new modules. **Cause**: written by hand,
  not formatted. **Fix**: `fmt --in-place` (`e2165913`).
- **Symptom**: `not-a-place.expected` moved. **Cause**: a clause about `@=`
  appended to `@`'s message too. **Fix**: the clause only for `@=`
  (`e2165913`).

## What landed, and what carried forward

**The milestone's four items, closed**: the never-re-bound rule with R9 (step
1), the `@=` token and the tree's rewrite (step 2, issue `…-1920-…`), the type
optional and inferred (step 3, `…-1921-…`), the documents and the instruments
(step 4, `…-1922-…`): the spec at **10,024 real tokens** (+3 on the trunk the
sitting measured, the warden's own price for W1b-min scored exact; 7636
vendored, ledger rows 107 and 108), design.md §4.4 and §4.5, two rows of
`docs/metrics/operators.md` with their operators, the site's two editions, and
the compiler's messages writing `v @= 0`.

**Carried**: defect 606 (`adjacent`, the gallery no suite compiles); the
critic's decision on an `@` parameter the callee never writes
(`…-1737-…`, open with its default); the task `…-1923-…` closed, the migration it was filed for
done by hand, and the verb's walk over a tree re-filed as defect 607
(`improvement`). After the push, as the chain
says: panel 187's R2 over the two new operators' sites, the census, and the
formatter's probe over `selfhost/print/`'s change.

**The gate, 2026-10-10 23:29 to 23:52, on the compiler built from the
regenerated seed, the fixpoint verified byte for byte** (`seed/heroes.c`
1,307,983 lines): the compiler's own tests 1596 passed, 0 failed; the net's
own 332 passed, 0 failed; the whole net, six suites at a time and `cache`
alone: emission 1229, annotations 1022, fixes 965, check 668, wholes 595, descriptors 595, warnings 565, determinism 539, lines 501, run 500, surface 407, unsupported 242, corpus 55, full 38, ir 29, records 28, probe 27, spec 23, permissive 16, emit 12, special 10, grammar 9, runtime 8, cache 7, layout 6, unseen 3, units 3, order 3, canonical 2. The floor `full` told, 37 against 29, raised in the
closing commit.

**Status at the close**, for `docs/ROADMAP.md` § Where we are: the compiler
607 modules and 158,586 lines of Heroes, the seed 1,307,983 lines of C; the
spec 7636 vendored and 10,024 real against 10240; the contract 8167 real;
sittings 207, journals 63 numbered files, milestone pages 54, measurements 39,
issues 1,976 of which 74 open, questions 466 of which 441 open.

**The chain entry**: row 84, `m-inferred-cell`, closed 2026-10-10, this
journal.
