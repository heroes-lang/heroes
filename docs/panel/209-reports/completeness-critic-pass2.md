# Panel 209, the completeness critic, pass 2: the reports against each other and the world

Started 2026-10-10 17:18:03 CEST (`date`). Folder:
`.claude/worktrees/scratch-b15/209-critic/`, the pass-1 worktree of the trunk
at `87794631`; no git command run in it; its `./heroes` is the pass-1 build
(seed, 16:33). The engineer's `heroes-r1` and `heroes-r1b` under
`209-compiler-engineer/` are run read-only on programs copied into my folder.
Time box 20 minutes, to about 17:38; what is not reached is written as unrun.
Appended as it goes.

## 1. Run on the engineer's `heroes-r1b`, programs in `critic-probes2/` (17:21:12 by `date`)

Each `../../209-compiler-engineer/heroes-r1b run <file>` from my folder:

| program | result |
|---|---|
| `t3B.hero`, the blind t3-B program byte for byte | **two** messages: `unknown_name` 4:9 fix `certain` *rename to `total`*, and `never_rebound` 8:5 on `limit @= 10` (no fix). **Line 2 is not refused**: the engineer's suppression holds; the four t3-B readers' third refusal (line 2) does not occur. Two messages for two mistakes, as the readers' repair (`total`, `limit = 10`) says |
| `at_arg_only.hero`, `db @= 0` then `open(@db)` | exit 0, prints 7: a write through an `@` argument counts as a re-binding with no new line; the engineer's half of the ffi/engineer disagreement holds, the ffi condition is met |
| `b2_own_example.hero`, `b: u8 @= 255` then `print(b + 1)` | `never_rebound` 2:5: the warden's half holds, B2 as drafted refuses its own § 2 example; W1b-min's `b: u8 = 255` is needed |
| `at_eq_arg.hero`, `bump(@=k)` | one `expected_expression` 6:10, fix `certain` *write `@k`*: holds |
| `op_eq_habit.hero`, `v @= 1` then `v @= v + 1` (the historian's `op=` habit, on a declared cell) | **two messages for one mistake**: `never_rebound` 2:5 and `shadowed_binding` 3:5, neither naming `@` nor offering a fix. A shape beside the repaired `totl` one: the `suggested` table does not cover it |
| `shadow_inner.hero`, `v @= 0` then `v @= 1` inside an `if` | the same pair, 2:5 and 4:9: Go's issue-377 hole is closed (the historian's condition met on the prototype), and the message pair is the shape above |
| `elem_write_only.hero`, `xs @= [1, 2]` then `xs[0] @ 5` | exit 0, prints 5: an element write re-binds |
| `at_param_never_written.hero`, `function f(@n: i64)` whose body never writes `n` | exit 0, prints 2, under r1b and (same program, my `./heroes`) today: **B2's class one level up, an `@` parameter nothing writes, is refused by nobody and asked by no seat** |

Counts in my copy (`find selfhost -name '*.hero' | xargs grep -hcE ...` summed): `@ nullptr` births 0, `@ [` ending its line 20, the four `cannot_infer` classes 1559; 1559 + 20 = **1579**, the engineer's pattern figure reproduces, so 1657 = 1579 + 78 is the measured keep count and 1559 its class-pattern lower bound; the ffi's 22 + 6 are over the 165 cells of 21 FFI example files, another denominator. No contradiction among the three.

Also measured (17:29): `heroes-r1b check` over the 426 `.hero` of `tests/golden/run/` (`xargs -P 2`, 3 s): **58 `never_rebound` cells in 28 files**, the tree the engineer left unrun; so R1b's migration is 151 + 19 + 5 + 58 = 233 cells before `tests/golden/check|emit|ir|fixedbugs` and `examples/` beyond the 21 FFI files are read. `heroes-r1 fmt selfhost/resolve/unused_sweep.hero` exits 2 with the comment guard (*would move the comment on line 37*), reproducing the engineer's § 5; `suite_canonical.hero:121` runs `fmt <file>` against the file, so `canonical` is red for every file holding an old-form cell between the token's commit and the tree's rewrite: the two must be one commit, or `fmt` must print the old form until then. `seed/heroes.c` holds `@` on 132 lines (`grep -c '@'`), the diagnostics' strings (`keywords.hero:288` *declare a mutable with `@`: `v: i64 @ 0`*): the engineer's *the seed carries no `@`* is false as a byte claim and immaterial, the seed being regenerated; what matters is that those strings are messages the migration must move.

## 2. Claims asserted and not measured

- Engineer: `heroes check --apply` migrating 1671 files in one pass, the leaf module bringing `check/walk.hero` under 1870, `cell_stmt`/`bind_stmt` merged, the width-cell note and fix, the `never_rebound` certain fix (needs a `symbol` span, ten lines): all **unbuilt**, said so; each is a line of its own condition, so the landing's lane scores them. `tests/golden/**` under R1b: 58 of `run/` measured here, the other forms unrun.
- Engineer § 9's census prediction (*exactly 55 case births, 8 arrays and 15 width cells where an annotation was dropped*) presumes a migration that strips annotations; the migration the same report lands (`check --apply`, `@` to `@=`, annotations kept) drops none, so the instrument measures a step nobody plans. Register it as the count on the stripped copy, not on the landing.
- Warden: *fires on 1559 of 5926 (26%)* as the `cannot_infer` count: measured 1579 + 63 = 1642 `cannot_infer` plus 15 width `type_mismatch` (28%); the classes `.case` (55) and non-empty arrays of unresolvable elements (8) are missing from its list. *`var` refused at `keywords.hero:288`*: holds (`sed -n 288p`).
- Historian: no shell; its one tree-fact, shadowing refused, the coordinator probed and I re-ran on r1b (`shadow_inner.hero`, `shadowed_binding`).
- Scoring: *all 24 concordant* holds on the t3-B verdicts I read (all four name 4, 8 and 2); the three bodies my grep found saying `arm` mean a `match` arm, not the variant; no report names its folder path or `209-blind` (grep 0 of 24).

## 3. Contradictions, which half is checkable, the command

- ffi *R1b must count `@` arguments* vs engineer *it already does*: engineer right by construction, `at_arg_only.hero` exit 0; the ffi condition is met and B2's text still owes the words (W1b-merge2's clause).
- Warden *B2 refuses its own § 2 example* vs the draft: warden right, `b2_own_example.hero` `never_rebound`; the landed text writes `b: u8 = 255`.
- Engineer *one message on `totl`* vs four t3-B readers *line 2 too*: engineer right on the prototype, `t3B.hero` gives 4:9 and 8:5, nothing at 2; the readers' line 8 is a real second mistake (`limit`), not a cascade.
- Engineer's 1657 vs brief's 1559 vs ffi's 22+6: three denominators, § 1; carry 1657 for `selfhost/` (lower bound, `check` may stop early).
- Engineer *15 width cells told at a distance with no fix* vs warden *no sentence is needed*: both true, one is a diagnostic and the other a spec sentence; the resolution takes both (no spec sentence, a note and `guess` fix on the mutation's `type_mismatch` naming the declaration).
- Warden *R10 object* vs engineer, ffi, historian *R10 approve*: the warden objects to R10 as the landed language (+12 vendored, two production shapes); the others approve it as the first commit. Not a disagreement once the synthesis says R10 is a step and `W1b-min` is the text.
- Engineer *object R0* vs ffi, warden *approve R0*: a verdict split, not a fact; the measured ground for R0 is R9's message, which every seat approves under any route.

## 4. Predictions with no instrument, or an instrument that does not exist yet

- Historian's prediction 3 and warden's prediction 3 name operators (`@=` where `@` was meant; `x = e` to `x @= e`) that `docs/metrics/operators.md` does not hold: `heroes mutate` exists (`selfhost/cli/table.hero:158`), the rows are owed by the landing before they can score. Rows 13 and 14 cite *mandatory type on declaration (§4.4)* as the catching rule and go stale with R1.
- The llm-ergonomist's row in the draft table carries no prediction: the 24 sessions are the experiment and their one unread shape is below (§ 7).
- Ffi prediction: scored true under `heroes-r1` (engineer's p13: `type_mismatch` at the call, no `cannot_infer`) and discharged under `heroes-r1b` (`cannot_infer` at the declaration); the synthesis should write both halves.

## 5. A framing fact handed and not checked

- Every seat took *the trunk is the tree this sitting prices* (brief, from my pass 1); the round holds 6264 declarations, and the R1b counts (233 so far) are the trunk's. The landing lane counts on the round.
- The warden carried 1559 as a `cannot_infer` count (§ 2); the engineer's 78 moves it.
- The historian was handed *Heroes refuses an inner block declaring an outer name* as a question and the coordinator answered it; checked here on r1b.
- Nobody checked `seed/heroes.c`'s `@` sentence (§ 1): immaterial but false.

## 6. The question the sitting should have asked

- **B2's class one level up**: an `@` parameter the callee never writes compiles today and under r1b (`at_param_never_written.hero`, prints 2). *A cell nothing re-binds is a compile error* and *an `@` parameter nothing writes* are one rule; the spec says only the first. A decision issue, not a condition on this landing.
- **The `op=` habit's diagnostic**: `v @= v + 1` on a declared cell is `never_rebound` plus `shadowed_binding`, no fix, no word of `@` (§ 1). The blind seat shows 0 of 12 writers do it; §4.17 still owes one message with a `certain` fix *re-bind: `v @ v + 1`* when the shadowed name is a cell, and the never_rebound on the outer cell suppressed where a `shadowed_binding` named it (the `suggested` table's shape, three lines by the engineer's price). The same pair for an inner-block `@=`.
- **The landing's commit shape**: token and tree rewrite in one commit or `canonical` is red (§ 1); R1b's rule and its 233+ rewrites to `=` in one commit or `heroes test selfhost/main.hero` is red (151, 120 in test blocks) and the net does not compile (5 in the harness).
- **The `.case` birth** (55 in `selfhost/`): `cannot_infer` under R1, never put to a blind writer (t1/t2 values were a string and `{}`); the reading that would show whether a writer annotates `k @= .lparen` unprompted is unrun.

## 7. The blind reports

- `context`: 24 of 24 answer with what reached them. Waves 1 and 2 (12) saw the project's git status (commit subjects naming panels and defects, the untracked `209-briefs/`, `209-reports/`): none of it names the language's rules or the question; by panel 183 the coordinator re-ran clean. Waves 3 and 4 (12) saw only their own repository's status and *the working directory path*: that path ends in `t3-B`, the arm's letter; no report names it or reasons from it (grep `209-blind|scratch-b15|t[123]-[AB]` over the 24 bodies: 0), so no yes in the voiding sense. Name the path leak in the method for next time: a neutral folder name.
- A/B differences rest on the specification: briefs byte-identical across arms (pass 1, `cmp`); t3's extra refusals (8, and the readers' 2) come from B2's sentence alone; t1/t2's `=` for `t` and `@=` for the cells likewise. The task's wording *say which lines it refuses* is plural in both arms and led neither.

## 8. The resolution the synthesis should adopt (CLAUDE.md § 4), and the landing it must say

**R1b, with every condition the seats built, landed in the engineer's order and the warden's text**: `@=` declares, `@` re-binds, `: Type` optional on both; a cell nothing re-binds (by `@`, by a field or element write, or by an `@` argument) refused with a `certain` fix writing `=` and keeping the annotation; a bare `nullptr` birth `cannot_infer` at its line with the cell poisoned so the call's `type_mismatch` is silent; `(@=x` one message, `certain` fix; R9's repair (`unknown_name` at a mutation carries a `guess` fix *declare a new cell: `v @= …`*); the width class told with a note naming the declaration and a `guess` fix annotating the cell; the `op=` pair above made one message; R1b silent on a name the resolver already refused (the engineer's self-test shape, three lines). Spec text `W1b-min` (real 9850, 3 over the trunk, measured by the coordinator's refresh), the never-re-bound rule merged into § 5's use sentence with the `@` argument clause, § 2 written `b: u8 = 255`. R0, R2 to R8, R10 as a landing, R11 refused for the seats' measured reasons; R8 under the engineer's veto; R10 kept as the first commit's state only.

**What the landing must say so that it does not fail at its gate**: (1) one commit for the token, the printers and the tree's rewrite of 1671 files by `heroes check --apply`, with the 23 `.expected`, 32 `.applied`/`.fixed`, the harness's 5 cells, `canonical` green over it; (2) one commit for R1b's rule and its rewrites to `=` (233 measured so far: 151 `selfhost/` of which 120 in test blocks, 19 `examples/`, 5 harness, 58 `tests/golden/run/`; the other golden forms and the round's 338 extra declarations counted by the lane; every one read as a `=` in disguise, the engineer's condition); (3) ceilings: `check/walk.hero` under 1870 by the leaf module, `grammar_expr.hero` under 1085 by the merged statement, `lent_with_eq` outside `selfhost/parse/` or a DECIDED row for its budget, `layout` 0 failed; (4) the compiler's own tests: three `@=` expectation texts, the refused-`@` shape, the enclosing-fix case read; (5) the `spec` suite's four count rows pasted with the refresh, `grammar` green as measured; (6) `operators.md` rows 13 and 14 rewritten and the two new rows added, the 839 planted declarations re-planted, panel 187's R2 run after the push; (7) the outward-facing surface: 25 lines of `site/src` (a push publishes), 19 of `docs/design.md` §4.4 with its reason rewritten (the symbol, not the type, is the mark), the TextMate grammar and `highlight.ts`, the `var`/`let` messages of `keywords.hero`; (8) the census over every golden tree, `check` `run` `emission` `determinism` `corpus` `fixes`, since the checker refuses what it did not; (9) the two decision issues: the ratification, and the `@` parameter nothing writes.

Finished 2026-10-10 17:33 CEST (`date` read at the last command 17:29:26, the writing after it).

Correction, same minute: the close above estimated 17:33; `date` at the write read 2026-10-10 17:30:34 CEST, inside the 20-minute box.
