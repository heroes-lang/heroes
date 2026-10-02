# Panel 187, the measured facts: what a finished recovery is, for defects 130 and 131

Gathered 2026-10-02 between 18:08 and 18:39 for the sitting. No brief, no
verdict: every number and every output below comes from a command named beside
it, run in this session. Nothing in the repository was edited, built, committed
or pushed; the round's worktree was only read and its binary copied. The trunk
was clean when this began and at 18:39 shows staged changes of another session
(`docs/measurements/010-spec-budget-ledger.md`, `docs/work/DEFECTS.md`,
`seed/heroes.c`, `selfhost/cli/assemble.hero`, a new
`selfhost/cli/layout.hero`), none of them this session's; main's head is then
`8349d264` (defects 165 and 166 filed, one commit above `4b44f684`), and `git
diff --stat 4b44f684 8349d264 -- seed/ selfhost/ runtime/` is empty, so T is
main's committed compiler at 18:39 too.

**Conventions.**
- `scratchpad/` stands for
  `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/`,
  and `p187/` for `scratchpad/p187/`, this file's folder.
- **T** is the trunk's compiler: `p187/heroes-trunk`, built here by
  `clang -I runtime seed/heroes.c runtime/runtime.c -o p187/heroes-trunk` run in
  the repository at `4b44f684` (exit 0, `p187/build-trunk.txt`), sha256
  `794e11a5eb83b3e8...`; `cmp` byte-identical to
  `scratchpad/lane-round1002b/heroes-trunk`. `git diff --stat 69e65357 4b44f684 -- seed/ selfhost/ runtime/`
  and `git diff --stat 2bb45a96 4b44f684 -- seed/ selfhost/ runtime/` are both
  empty, so T is also lane recovery-b8's base compiler (`2bb45a96`).
- **R** is the round's compiler: `cp` of
  `.claude/worktrees/lane-round1002b/heroes` (built 17:19 from the seed
  regenerated 17:18 after `d105a1e5`) to `p187/heroes-round`, `cmp` identical,
  sha256 `b578e9a799ab7469...`. `HEROES_RUNTIME` was set to that worktree's
  `runtime/` for every run. The worktree's head moved twice while this was
  gathered: to `a388a056` at 18:28 (`git show a388a056 -- selfhost/` changes
  one comment line in `selfhost/parse/headless.hero`, the rest is
  `site/src/lib/claims.ts`), then to the round's closing commit `d4fd12ef` at
  18:36, not on main at 18:39 (`git merge-base --is-ancestor`). `d4fd12ef`'s
  `seed/heroes.c` is the 17:18 seed (`git rev-parse d4fd12ef:seed/heroes.c`
  and `git hash-object seed/heroes.c` both `914e2585...`), `git diff --stat
  a388a056 d4fd12ef -- selfhost/ runtime/` is empty, and the worktree's
  `./heroes` stayed `cmp` identical to `p187/heroes-round`: **R is the
  compiler of the round's closing commit.**
- A diagnostic is written `code@line:col`, with `[C]` when it carries a
  `certain` fix and `[g]` when its fixes are `guess` only, read from
  `check --json`. In quoted compiler text, the em dash the compiler prints is
  written ` -- ` (this file carries none); the raw text is in the
  `*.plain.txt` file beside each case.
- The re-run of every audit row: `p187/rerun187.py`, output
  `p187/rerun187.txt` (65 row headers: the 59 audit rows of lane
  recovery-b8's `audit/open-ids.txt` and the 6 shapes of section 2.4; 95
  files), every file copied first into `p187/cases/<id>/`. A first run
  (`p187/rerun187-first.txt`) read an empty `--json` output at exit 0 as a
  parse failure; the reader was corrected (an empty output at exit 0 is a clean
  program, measured on `cases/131-56a/nested_one_line_suites.hero.R.applied.hero`:
  `check`, `check --json` and `check --brief` all print nothing at exit 0) and
  the second run differs from the first only in that word.
- Comparison with the lane's own runs (a script in this session, sorted
  diagnostics per file): R equals lane recovery-b8's final `heroes-r5`
  (`lane-recovery-b8/audit/rerun-r5.txt`) on all 89 audit files, and T equals
  its `heroes-base` (`audit/rerun-base.txt`) on all 89. The round's redo
  `d105a1e5` moved no audit row against `ce5caf89`.

## 1. The two items as they stand

Read from `docs/work/DEFECTS.md` on the trunk (`4b44f684`), from the round
tree's copy (`.claude/worktrees/lane-round1002b/docs/work/DEFECTS.md`,
modified and uncommitted there), and from `git log` (hash, date and subject
of every cited commit checked with `git log -1` and
`git merge-base --is-ancestor <hash> main`).

### 1.1 Item 130

- **Its line** (`docs/work/DEFECTS.md:23`): *130 -- after a `match` whose arm
  fails to parse, the next statement is skipped whole, and every mistake in it
  goes unreported* ... `selfhost/grammar_expr.hero` (`match_expr`) · the
  enclosing statement's recovery · **class: systemic**.
- **Its length** (awk over the item's span): lines 23 to 299 on the trunk,
  277 lines; lines 23 to 333 in the round tree, 311 lines (lane recovery-b8's
  five dated repair lines and the round's redo line, 34 lines by `diff`, are
  not on the trunk).
- **Its Class line**, trunk lines 287 to 299, quoted: *"**Class: systemic**,
  2026-10-02 (the author's *D1a*, `.claude/rules/verification.md` § Bounded
  discovery): six batches on the trunk's record (recovery-b1 to b6) and a
  seventh in lane recovery-b8, which leaves one row, `130-34a`, that needs a
  ruling panel 183's reach rule does not reach (inside an open `[` a name-led
  line is an element, so the `)` pairs with the `[`); to a sitting with 131 on
  what a finished recovery is. Seen beside it by lane recovery-b8, for that
  sitting and not repaired: a function among a variant's cases dropped silently
  (`g2/r12`), a bodiless method in a record costing two messages (`g2/r09`),
  `return match f(n` over its arms swallowing them (`hc/h09`), `if f(1 +) )`
  hiding its stray `)` (`g4/ti`), an extern member whose `(` is left open over
  a body (`g1/b07`), a function nested in an orphan block costing two messages
  (`g1/a11`), under `scratchpad/lane-recovery-b8/p1/`."*
- **Before the recovery batches** (git log, all on main): filed `0338b598`
  2026-09-28 18:58; lane 130's rounds `0a9435eb` 2026-09-29 00:31, merged
  `0ba7b084` 01:43 (*defects 130 and 131, first round*), `47849cc9` 02:21,
  `60dfc867` 03:49; the item widened through 2026-09-29 by lanes 130, 132, 133,
  X1, X2 and Y; the item's line *"Batch gate 2026-09-30 00:07 (`53a5e0a9`,
  lanes X3 and X4 beside the trunk's 129 to 133, X1, X2 and Y): the full net,
  every suite at 0 failed"* (`53a5e0a9` is dated 00:08 by git).

| batch | lane | dated repair lines (commit, git time, what the line says it closed) | gate (item's words, commit) |
|---|---|---|---|
| b1 | recovery-b1 | `f17bbe90` 09-30 00:37 a member's line ends after its member (X4's n1); `5cdc7288` 00:45 a case's fields in braces are read as its fields (X4's n2); `f4e991da` 01:07 a refused literal is read as the literal it stands for at any depth (X4's n4); `2fd79070` 01:16 an orphan block that goes on with the line above is named once and read | 2026-09-30 01:48, `1fc77e31`: full net 3852 passed 0 failed over 26 suites, own 903, net's own 184; Linux x86-64, arm64, Windows 903 and 19 suites |
| b2 | recovery-b2 | `41807577` 02:46 a declaration opening its line at column 0 names the brackets a mistake left open (b1's U2); `04b51f08` 02:49 an orphan under a whole arm read closed, its case added | 04:45, `e5cc73eb`: full net 3873 and 0, own 911; census 1358 files moved the six new cases |
| b3 | recovery-b3 | `52b2d378` 06:58 past a closer of another kind the parser resumes where the lexer closed the bracket, and an extern group whose header failed has its signatures read | 09:38, `77b8ca98` (batch closed at `46846f97` 09:10): full net 3907 and 0; instrument ONE 11,933, EXTRA 1,326, APPLY-NEW 0, APPLY-OTHER 0, 17 hidden (baseline 153) |
| b4 | recovery-b4 | `e5076b57` 16:27 panel 183 R1; `ab36aa61` 16:40 panel 183 R2 | 20:12, `84430015`: own 934, full net 3,970 and 0; instrument ONE 11,955 to 12,448, EXTRA 1,336 to 861, ELSEWHERE 32 to 14; integrated `3cc3b553` 22:44, trunk fast-forwarded 00:03 on 10-01 |
| b5 | recovery-b5 | `d422815d` 10-01 19:09 a body's braces laid out by the lexer as their fix leaves them (ruling 1, C2); `d6af2934` 21:45 a head with no name before its braces told once (ruling 2, C5) | 10-01 23:00, `a8b04ea8` (*C1, C2, C3 and C5 with `538d70ac`*): own 993, full net 4,240 and 0; instrument EXTRA 850 to 532, ONE 12,427 to 12,745, HIDDEN pairs 16 to 12 |
| b6 | recovery-b6 | `8c907fa5` 10-02 01:09 an opener the rule ended is named still open at the line and word that ended its reach (panel 183 R3, route (b)); `d99b12f8` 01:48 a comment line hides no kept line end; `e85385a9` 02:16 an orphan block below a failed line that can open none is named once (C4); `dc83367b` 02:30 a block under a head nothing reads is read as the kind the head's words name (group (c)); `6d49b827` 03:32 another language's comment is one message and a comment (group (e)) | 10:25 by the item (closing commit `ae08ed93` 10:26): own 1,006, full net 4,318 and 0; census 1,642 files, 70 outputs in 35 files; instrument ONE 12,745 to 12,838, EXTRA 532 to 439, HIDDEN pairs 12 to 4, APPLY-NEW and APPLY-OTHER 0; Linux x86-64 1,006 tests, 4,171 and 0 |
| b8 | recovery-b8 (branch `lane-recovery-b8`, **not on main**) | `525fe2dd` 15:24 a line that failed past what it declares drops only its line (b6's d01, d04); `26358f9c` 15:46 a function among a record's fields told once (b6's e14, f06); `4ca2c20a` 16:04 a block comment over several lines is one comment (b6's lc12); `276908b3` 16:29 the body a head's open bracket took in is read as its body (rows 130-H-004, 130-34d); `ce5caf89` 16:34 a closer one too many past a failed statement is told (row 130-28); records `4e500c91` 16:35 | merged into the round's tree at `2dc1a9d7` 16:52; the round's gate found the compiler's own tests 1,036 and 3 failed, culprit `ce5caf89` (0 failed at `276908b3`), redone at `d105a1e5` 17:17; after it, own 1,036 passed, net's own 187 passed; the full net's parallel pass 17:21 to 17:37, 22 of 25 suites at exit 0, `probe` and `lines` red on floors and `emission` on its 36 blessings, the floors raised and the blessings read file by file after (`lane-round1002b/progress.md`); the round's closing commit `d4fd12ef` 18:36, *"The round of 2026-10-02's second gate closes: lanes recovery-b8, land186 and h158 under one gate on this Mac, two of recovery-b8's commits redone, the seed regenerated once"*, not on main at 18:39 |

The round tree's item carries the b8 lines and the redo line, read with `diff`
of the two files' item spans; the redo line, quoted: *"**2026-10-02, the round's
gate of lanes recovery-b8, land186 and h158, `ce5caf89` redone**: past a
statement that ended with its own block it told the next statement's closer a
second time at its column (the compiler's own tests, 3 failed at the gate and
at the lane's merge `2dc1a9d7`, 0 at its `276908b3`); redone at `d105a1e5`, the
search bounded by what the drop of the failed line takes, gated by its own
cases; the rest is owed at the round's gate."*

**Batches**: seven by its Class line (recovery-b1 to b6, and b8): four on
2026-09-30 (b1 to b4), one on 2026-10-01 (b5), two on 2026-10-02 (b6, b8);
before them, lane 130's two rounds on 2026-09-29 and the X3/X4 gate at
2026-09-30 00:07. `git log --grep='^Defect 130' main` lists 21 commits, the
first `0338b598` (filing), the last `cf9e2792` (b6's records line); b8's five
are on the lane's branch and the round's tree only.

### 1.2 Item 131

- **Its line** (`docs/work/DEFECTS.md:301`): *131 -- a block head whose line
  failed reports its missing body as a second mistake* ...
  `selfhost/grammar_expr.hero` (the body checks after a block head,
  `match_expr`'s arms check) · `cursor.at_reported_error` · **class:
  systemic**.
- **Its length**: lines 301 to 495, 195 lines, the same in both trees.
- **Its Class line**, lines 492 to 495, quoted: *"**Class: systemic**,
  2026-10-02 (the author's *D1a*, `.claude/rules/verification.md` § Bounded
  discovery): four batches (recovery-b1, b2, b4, b5) and 15 rows open on
  `2bb45a96` by lane recovery-b8's count; to the same sitting as 130."*
- **Before the recovery batches** (git log, on main): filed `a6eab736`
  2026-09-28 23:03; then the commits whose subject starts `Defect 131`,
  `ae67fb5d` 09-29 01:08, `9811d4cd` 03:07, `9b1f1701` 03:40, `104dcb00`
  05:02, `dff40ca8` 05:41, `d7a58da1` 09:09, `9ca7dc7b` 11:10, `b6fddb3c`
  17:55, `7dc10692` 23:42; the item widened through 2026-09-29 by lanes 130,
  132, 133, X1, X2 and Y and by the `fixes` suite lane 129 landed (its own
  lines); and the same X3/X4 gate line as 130's.

| batch | lane | dated repair lines | gate |
|---|---|---|---|
| b1 | recovery-b1 | `cda5ee4c` 09-30 01:29 a result type written after `:` or `=>` is told as that habit, with the certain fix that writes `->` (X3's report) | 01:48, `1fc77e31` (as 130's) |
| b2 | recovery-b2 | `de991c25` 03:10 a group, call or index whose opener the lexer named never closed ends at its line (U4); `48a433f4` 03:27 braces the lexer named never closed end where it named them (U1); `6795bedd` 03:48 a literal that never closed takes its line's closers (U3); `ac62cde9` 04:06 a head that left a bracket open is told once (U5) | 04:45, `e5cc73eb` (as 130's) |
| b4 | recovery-b4 | `4441148b` 11:42 a sigil before a name is one message at its `@`; `3a93d155` 15:24 a head inside a bracket left open names no missing body (panel 183's critic, the regression of `52b2d378`) | 20:12, `84430015` (as 130's) |
| b5 | recovery-b5 | `4f0db097` 10-01 17:41 an indentation habit is one message (ruling 5, C1); `df2e13ca` 21:22 one closer or one quote left out costs one message, the opener named (C3); `538d70ac` 22:01, a Defect 131 commit (*a line going on inside a bracket the lexer named stops at the closer it gave an outer opener*) with no dated line in item 131, named in both items' b5 gate line | 10-01 23:00, `a8b04ea8` (as 130's) |

**Batches**: four by its Class line (b1, b2, b4, b5): three on 2026-09-30, one
on 2026-10-01. `git log --grep='^Defect 131' main` lists 21 commits, 20 whose
subject starts `Defect 131` (the 21st, `ebae3241`, is defect 132's round, a
line of its body starting `Defect 131`), the first `a6eab736`, the last
`538d70ac`. For 130 the same command lists 21, all 21 subjects starting
`Defect 130`. The item was also widened 2026-09-30 by panel
183's completeness critic (the `52b2d378` regression, 42 of 3,000 single
missing closers; its row is 131-42 below).

## 2. Every audit row still open, on the round's compiler

**Which rows are open**: each of the 59 rows of lane recovery-b8's
`audit/open-ids.txt` (32 of 130, 27 of 131) was re-run on T and R
(`p187/rerun187.txt`), plus the five rows whose audit source is `?`, run from
lane recovery-b6's reconstructions (`lane-recovery-b6/audit/recon/b1a.hero`,
`b1b`, `b2a`, `b2b`, `b4`, copied to `p187/cases/recon/`, run by hand, both
compilers identical), plus row 131-42's 42 files (`p187/c4r187.py`, section
2.3). Each output was read against the last column of the row in
`scratchpad/audit-130-133/open-rows.md.txt` (what the audit said should be
told) and, for the rows the lanes closed, against their notes
(`lane-recovery-b6/notes.md`, `lane-recovery-b8/progress.md`).

**Result**: 130 has **1** audit row open on R (130-34a), 131 has **15** (16a,
16b, 22, 32, 33a, 41a, 52a, 53a, 53b, 54a, 54b, 55a, 55b, 56a, 56b), the same
rows lane recovery-b8's progress.md lists open after its batch. T and R print
the same on all of them except 131-55b (R tells one more, the `)`). The audit's
case files are under `scratchpad/audit-130-133/cases/<id>/` (and its own/
follow-ups under `audit-130-133/own/`); copies with every output are under
`p187/cases/<id>/`. The follow-up programs written in this session (what an
author gets after doing what a message says) are `p187/cases/followups/` and
`p187/cases/followups-beside/`, outputs in `followups.txt` and
`followups-beside.txt` there.

### 2.1 Defect 130: one row open

**130-34a** (b3's items B2: `u2_p`), case
`audit-130-133/cases/130-34a/u2_p_stray_inside_body.hero`:
```
function f()
    x = [1, 2
    print(x) )
```
- T and R: `expected_separator@3:14`, no fix: *expected `,` or a new line
  between one element and the next, found `)`*. `--apply` leaves it as it is.
- The audit row says: *"OPEN: two mistakes, one message at the stray `)`; the
  missing `]` untold; panel 183 R1/R2 reach it by no clause (a `[`, a
  name-led line)"*. Section 6 has its two readings and their measurements.

### 2.2 Defect 131: fifteen rows open

**131-16a**, case `audit-130-133/cases/131-16a/vjunk_none.hero`: `variant T )`
with no case below.
- T and R: `empty_variant@1:1`, *a `variant` needs at least one case, indented
  one level below it* (caret on `variant T`). The `)` at 1:11 is not told.
- Follow-up `own/a131_16a_then_case.hero` (a case `a` below): T and R
  `expected_end_of_line@1:11`, *... after `variant T`, found `)` -- its cases go
  on the lines below it, one level deeper*.
- Audit: *"OPEN: caret now on its head, but the `)` untold: with a case below
  (own/a131_16a_then_case) EEOL@1:11 is new on both"*.

**131-16b**, case `cases/131-16b/rjunk_none.hero`: `record Point )` with no
field.
- T and R: `empty_record@1:1`, *a `record` needs at least one field, indented
  one level below it*. The `)` at 1:14 is not told.
- Follow-up `own/a131_16b_then_field.hero`: `expected_end_of_line@1:14`
  (found `)`).
- Audit: *"OPEN: same: with a field (own/a131_16b_then_field) EEOL@1:14 new on
  both"*.

**131-22**, case `cases/131-22/record_head_quoted_across_a_join.hero`:
```
record
    x: i64
    y i64
```
- T and R, normal arm: `continuation_outside_brackets@1:1[g]`, *a line outside
  brackets ends its statement, and this one cannot end with `record`: the next
  line goes on with it one level deeper, where a block would begin; write the
  statement on one line, or break it inside parentheses* (fix, guess: *write
  the statement on one line*); `expected_field_type@3:7`, *expected `:` and the
  field's type, found a name (`i64`)*.
- T and R, `check --permissive` (design.md Part 11's control arm, *the same
  compiler with the thesis-bearing checks disabled*, line 3933):
  `expected_end_of_line@2:6`, *expected the end of the line after `record x`,
  found `:` -- its fields go on the lines below it, one level deeper*;
  `expected_field_type@3:7`.
- Audit: *"OPEN (control arm): normal arm two mistakes two messages;
  `--permissive` still says `after record x` on both"*.

**131-32**, case `cases/131-32/f5_do_while.hero`:
```
function main() {
    do {
        y = 3 4
    } while (1 == 1)
}
```
- T and R: `missing_body@1:17` (*a `function` needs an indented body -- one
  level deeper, exactly 4 spaces (found `{`)*); `expected_end_of_line@2:8`
  (*expected the end of the line, found `{` -- one statement per line, no
  semicolons*); `expected_end_of_line@3:15` (found `4`). Line 4, `} while
  (1 == 1)`, is not told.
- Follow-up `own/a131_32_debraced.hero` (braces gone): `unexpected_block@3:1`
  (*this block is indented deeper than anything that opens one*) and
  `missing_body@5:1` (*a `while` needs an indented body ... (found the end of
  the block)*).
- Audit: *"OPEN: the habit is told as a brace after a name, the `} while` line
  untold; debraced (own/a131_32_debraced) costs UB@3:1 MB@5:1, new on both"*.

**131-33a**, case `cases/131-33a/f6_c_style_for.hero`: `    for (i = 0; i < 3;
i++) {` over `        y = 3 4` and `    }`.
- T and R: `unexpected_character@2:15` and `@2:22` (*`;` is not part of the
  language's syntax*); `for_missing_in@2:9[g]` (*`for` iterates -- `for x in
  xs`; found `(` -- a loop over a condition is `while`*, guess: *use `while`*);
  `missing_body@2:29` (*a `for` needs an indented body ... (found `{`)*);
  `expected_end_of_line@3:15` (found `4`). `--apply` leaves it as it is.
- Audit: *"OPEN: one habit, four messages (b2 item M3)"*.

**131-41a**, case `cases/131-41a/unclosed_function_type_params.hero` (the same
text as `lane-recovery-b1/p4/l10_b_fn_type_params.hero`):
`function f(g: (function(i64 -> i64)` over `    print(1)`.
- T and R: `unclosed_bracket@1:11` and `unclosed_bracket@1:15` (*`(` opened here
  is never closed*); `expected_function_type_params_close@1:29` (*expected `)`,
  or `,` and another parameter type, found `->`*);
  `expected_function_type_arrow@1:36` (*expected `->` and the result type -- a
  function type always states what it returns, `()` when nothing, found end of
  line*).
- Its siblings, run on R with `--brief`: `l10_c_fn_type_group.hero`,
  `function f(g: (function(i64) -> i64)`, the function type's `)` written:
  `unclosed_bracket@1:11` alone; `l10_a_params.hero`, `function f(a: i64`:
  `unclosed_bracket@1:11` alone. Follow-up with a `)` added at the line's end
  instead (`followups/f131_41a_closer_added_at_end.hero`):
  `unclosed_bracket@1:11`, `expected_function_type_params_close@1:29`,
  `expected_function_type_arrow@1:36` (found `)`).
- Audit: *"OPEN: four messages (U5 named two unclosed_bracket and three parser
  messages; now two and two)"*.

**131-52a**, case `cases/131-52a/b3_a.hero`:
```
variant Shape
    {
        r: f64
    }
    square
```
- T and R: `indentation_jump@3:1` (*indentation jumps from level 0 to level 2;
  a block opens one level at a time*); `empty_variant@2:5` (*the cases of
  `variant Shape` go on the lines below it, one level deeper, and never in
  braces (found `{`)*); `expected_end_of_line@3:10` (*... after the case `r`,
  found `:` -- one case per line, its fields on the lines below it, one level
  deeper*).
- Follow-ups: the two brace lines deleted, margins kept
  (`followups/f131_52a_braces_deleted.hero`): `indentation_jump@2:1` and
  `expected_end_of_line@2:10`, the braced run's other two messages; deleted and
  re-indented (`f131_52a_braces_deleted_reindented.hero`):
  `expected_end_of_line@2:6` alone.
- Audit (b4's output then: `expected_case@2:5 expected_case@4:5`): *"OPEN: two
  for one"*. Lane recovery-b6's notes: *"52a (changed: indentation_jump@3:1
  empty_variant@2:5 EEOL@3:10)"*, listed still open; lane recovery-b8 lists it
  open. Neither records a reason past the audit's (grep of `52a` over both
  lanes' notes, b5's, b7's and `scratchpad/close/`).

**131-53a**, case `cases/131-53a/m2_a_paren.hero`: `function f(): i64 )` over
`    return 1`.
- T and R: `expected_end_of_line@1:13` (*expected the end of the line, found
  `:` -- a `function`'s body goes on the lines below its head, one level
  deeper*). The `)` at 1:19 is not told.
- Follow-up `own/a131_53a_arrow.hero` (`->` written):
  `expected_end_of_line@1:21` (found `)`).
- Audit: *"OPEN: the old message, no result-type fix; with `->` written
  (own/a131_53a_arrow) EEOL@1:21 new on both"*.

**131-53b**, case `cases/131-53b/m2_e_no_body.hero`: `function f(): i64 )`
and no body.
- T and R: `missing_body@1:13` (*a `function` needs an indented body -- one
  level deeper, exactly 4 spaces (found `:`)*).
- With a body written it is 131-53a's program (`expected_end_of_line@1:13`,
  found `:`), and with `->` written too, `expected_end_of_line@1:21` (found
  `)`): three runs to see three mistakes.
- Audit: *"OPEN: missing_body at the `:`"*.

**131-54a**, case `cases/131-54a/l12_a_for_ever.hero`: `    for (;;)` over
`        print(1)`.
- T and R: `unexpected_character@2:10` and `@2:11` (`;`),
  `for_missing_in@2:9[g]` (guess: *use `while`*). The audit recorded this fix
  `[C]`; it is `[g]` on both now (lane b6's notes: *"54a ([C] to [g])"*), so
  `--apply` leaves the program as it is.
- Follow-up `own/a131_54a_semis_gone.hero` (`while ()`):
  `expected_expression@2:12` (found `)`).
- Audit: *"OPEN: three for one; the certain use-while writes `while (;;)`,
  whose `;` gone (own/a131_54a_semis_gone) costs EE@2:12, new on both"*.

**131-54b**, case `cases/131-54b/l12_b_c_for.hero`: `    for (i = 0; i < 3;
i++)` over `        print(i)`.
- T and R: `unexpected_character@2:15` and `@2:22`, `for_missing_in@2:9[g]`.
- Audit: *"OPEN: three for one"*.

**131-55a**, case `cases/131-55a/arrow_past_two_dedents.hero`:
```
    if n > 0
        k = match n
            0
    => 5
    print(k)
```
- T and R: `expected_arm_arrow@5:14` (*expected `=>` and the arm's value, found
  end of line*); `expected_expression@6:5` (*expected an expression, found `=>`
  -- a value, a name, a call, ...*).
- Follow-ups: `=> 5` moved onto line 5 and line 6 deleted
  (`followups/f131_55a_arrow_moved_up.hero`): `heroes parse` exits 0 and the
  next stage speaks, `unused_binding@4:9` and `unknown_name@6:11` (`k` is
  bound inside the `if`); `=> 5` written on line 5 with line 6 kept
  (`f131_55a_arrow_added_line6_kept.hero`): `expected_expression@6:5` alone.
- Audit: *"OPEN: two for one"*.

**131-55b**, case `cases/131-55b/m4_d_mistake_after.hero`: 131-55a with line 6
`    => 5 )`.
- R: `expected_arm_arrow@5:14`, `expected_expression@6:5`,
  `expected_end_of_line@6:10` (found `)`). T: the first two only.
- Audit: *"OPEN: two for one, and the `)` at 6:10 untold"*.

**131-56a**, case `cases/131-56a/nested_one_line_suites.hero`:
`    if n > 0: if n > 1: print(1)`.
- T and R: `trailing_colon@3:13[C]` (*a block is opened by the indentation
  alone -- drop the `:` and write what follows it on the line below, one level
  deeper (this is not Python)*, certain: *write it on the line below, one level
  deeper*); `missing_body@3:23` (*an `if` needs an indented body -- one level
  deeper, exactly 4 spaces (found `:`)*).
- `--apply` writes the two `if`s and the `print` on three lines, each one level
  deeper; that program checks clean on T and R.
- Audit (then): *"OPEN: applied costs TC@4:17[C], a new code for the inner
  colon"*.

**131-56b**, case `cases/131-56b/m5_a_three.hero`:
`    if n > 0: if n > 1: if n > 2: print(1)`.
- T and R: `trailing_colon@3:13[C]`, `missing_body@3:23`; `--apply` writes four
  lines and the result checks clean on both.
- Audit (then): *"OPEN: applied costs TC@4:17[C] MB@4:27, new"*.

### 2.3 Rows closed on the round's compiler (for the record)

R's output on the row's own case, and why it meets the audit's last column.
Every one prints the same on T.

| row | R | the audit's ask, met by |
|---|---|---|
| 130-17a, 21c | `expected_end_of_line@1:15 expected_end_of_line@2:8[C]` | the comma inside the braces told (ruling 1) |
| 130-17b | `...@1:10 ...@2:11[C] missing_body@6:17 expected_expression@7:14` | the comma at 2:11 told |
| 130-21a | `...@1:14 ...@2:11[C] expected_field_type@3:7 ...@6:15 ...@7:10[C] ...@8:10` | both commas told |
| 130-22c | `...@2:11 unexpected_block@3:1 ...@3:15` | the orphan named in the first run |
| 130-23 | `reserved_word@2:5 expected_end_of_line@4:19`, the audit's output unchanged | the message's words now say *a `record` is declared at the top level ... each function in this block is declared at the top level*; following them (`followups/f130_23_record_and_function_at_top.hero`) costs `empty_record@1:1` (the class had no field) and the `)`; the function alone at the top, the `)` alone |
| 130-27 | `...@1:15 ...@2:12 ...@3:15[C]` | the comma told |
| 130-28 | `base_prefix_case@2:11[C] base_prefix_case@3:10[C] expected_expression@2:18 expected_end_of_line@2:20 expected_expression@3:16` (T lacks `@2:20`) | the stray `)` told (b8's R5, redone `d105a1e5`) |
| 130-30b | `...@1:10 ...@2:11[C] ...@5:14` | the comma told |
| 130-H-001 to H-014 | each pair now tells its second mistake with the code its second alone gets (`second_only.hero`, `debraced_*.hero`, all 14 read in `rerun187.txt`), e.g. H-004 `unclosed_bracket@12:23 unexpected_block@14:1` (T lacks `@14:1`), H-009 `unexpected_character@7:1[C] expected_array_length@9:20` | the second mistake told, and the first once (H-002's braced `else` chain twice, `missing_body@38:71 @40:24`, ruling 4) |
| 130-34b | `unclosed_bracket@2:9 expected_end_of_line@5:14` | the stray `)` told |
| 130-34c | `unclosed_bracket@2:10 expected_end_of_line@3:11` | the stray `)` told |
| 130-34d | `expected_params_close@2:5 expected_end_of_line@2:14` (T lacks `@2:14`) | the stray `)` told (b8's R4) |
| 130-B1a | recon `b1a.hero`: `indentation_not_multiple_of_4@2:1[g] missing_body@1:17` | margins told in the braced run, once per run (ruling 5) |
| 130-B1b | recon `b1b.hero`: `indentation_not_multiple_of_4@2:1[C] missing_body@1:17 expected_end_of_line@3:12` | margins and the `)` told |
| 130-B2a, B2b | recon: `expected_name@1:8` (*... `record Point`, its fields on the lines below it, one level deeper, and never in braces*), `@1:9` for `variant` | ruling 2's words |
| 130-B4 | recon `b4.hero`: `...@1:10 ...@2:11` | the two fields on a line told |
| 131-39a, 39b | `...@1:10 ...@2:11[C]`; `...@1:11 ...@2:8[C]` | the comma inside an unclosed `{` told |
| 131-42 | 42 of 42 files at exactly one message, `unclosed_bracket` (`p187/c4r187.txt`, run inside `p187/c4r-mirror/`, a `cp -c -R` clone of `audit-130-133/c4r/mirror`); T the same; `zzc4r_063dd43b38850e72` names the `[` at 583:16, and line 583 is 74 characters, its removed closer at column 75 | one message per missing closer; at the audit B read 91 messages, 35 files at two and 7 at three |
| 131-51 | `unclosed_bracket@2:12 expected_type@2:9` | the `[` named; the `2` where a key type goes, a second mistake (lane b8's reading) |
| 131-52b | `empty_variant@2:5` alone | one message |
| 131-C2a, C2b | `expected_expression@8:9[C]`, `@13:9[C]`, applied clean | one message, its fix clean |
| 131-BH-008, 009, 019, 020, 022 | `unclosed_bracket@3:13`, `@3:31`, `unterminated_string@3:61`, `@6:51`, `@6:84` | one message each |

### 2.4 Not audit rows: the six shapes item 130's Class line names for this sitting

From `scratchpad/lane-recovery-b8/p1/`, run in the same script
(`rerun187.txt`, ids `b8-*`), follow-ups in `p187/cases/followups-beside/`.

- **g2/r12** (`g2/r12.hero`): `variant V` over `    red` and `    function f()`
  over `        print(1 +)`. T and R: `expected_case@3:5`, *expected a case
  name, found `function` -- a variant case is a name on its own line, with its
  fields indented below it*. The `print(1 +)` is not told; with the function at
  the top level (`r12_function_moved_to_top.hero`), `expected_expression@5:14`.
- **g2/r09** (`g2/r09.hero`): `record Q` over `    x: i64` and
  `    function f() -> i64` with no body. R: `expected_field@3:5` (*... a
  `record` holds only its fields ..., and a `function` in it is declared at the
  top level of the file, taking the record as a parameter*) and
  `missing_body@5:1` (*a `function` needs an indented body ... (found the end
  of the block)*, the caret on line 5, `function main()`). T:
  `expected_field@3:5` alone, with the shorter words *one field per line,
  `name: type`*. Moved to the top level (`r09_function_moved_to_top.hero`), T
  and R: `missing_body@6:1`.
- **hc/h09** (`hc/h09_match_open.hero`): `    return match f(n` over its arms
  `0 => 1`, `_ => 2`. T and R: `unclosed_bracket@2:19`, *`(` opened here is
  still open at line 6, where `function` begins a line no bracket can hold*.
  With the `)` written (`h09_paren_closed.hero`) `heroes parse` exits 0 and
  the resolver says `unknown_name@2:18` (the case's own `f`).
- **g4/ti** (`g4/ti.hero`): `    if f(1 +) )` over `        print(1)`. T and R:
  `expected_expression@5:13` (found `)`). The stray `)` at 5:15 is not told;
  with the operand written (`ti_operand_written.hero`),
  `expected_end_of_line@5:17`.
- **g1/b07** (`g1/b07_extern_member_open.hero`): in `extern "m.h"`,
  `    function f(x: i64 -> i64` over a deeper `        y = 1`. T and R:
  `unclosed_bracket@2:15` (*still open at line 5*) and
  `expected_extern_signature@3:9`, *expected a `function`, a `constant` or a
  `record`, found a name (`y`) -- an `extern` group holds what the header
  declares, one per line*. With the `(` closed (`b07_paren_closed.hero`),
  `extern_has_body@3:1`, *an `extern` declaration has no body*.
- **g1/a11** (`g1/a11_use_clean_block_decl.hero`): `use geom` over an indented
  `    function g()` and `        y = 4 )`. T and R:
  `expected_declaration@2:1` (*found an indented block*),
  `expected_expression@2:5` (found `function`), `expected_end_of_line@3:15`
  (found `)`). Dedented (`a11_function_dedented.hero`): only
  `expected_end_of_line@3:11`.

## 3. Each open row by what it costs a program's author

(a) a second message for one mistake; (b) a mistake told only once another is
fixed; (c) a message that is false; (d) a `certain` fix that writes a program
meaning something else, or one refused anew; (e) exit 2 or a crash; (f) none of
these. Description, from section 2's runs; where a second reading changes the
class, the reason says so.

### Defect 130 (audit rows)

| row | class | reason |
|---|---|---|
| 130-34a | **(b)** by the audit's reading; (f) by the lane's | two mistakes by the audit's reading: the missing `]` is not told until the `)` goes (deleted: `unclosed_bracket@2:9`, *still open at line 5*); by the lane's reading one mistake, a closer of another kind, told once in words that do not name the `[` (section 6) |

### Defect 131 (audit rows)

| row | class | reason |
|---|---|---|
| 131-16a | **(b)** | the `)` after `variant T` is told only once a case is written (`@1:11`) |
| 131-16b | **(b)** | the `)` after `record Point` is told only once a field is written (`@1:14`) |
| 131-22 | **(f)** | normal arm: two mistakes, two messages; only the control arm (`--permissive`) is open, its true message quoting `record x`, a head joined across a line break the author wrote as `record` alone over a field |
| 131-32 | **(b)** | the do-while line `} while (1 == 1)` is untold until the braces go, and then costs two new messages (`unexpected_block@3:1`, `missing_body@5:1`) |
| 131-33a | **(a)** | C's three-clause loop header costs three messages (two `;`, `for_missing_in`) beside the brace's own `missing_body` (ruling 4) and the `4`'s own message |
| 131-41a | **(a)** | the function type's one missing `)` costs three messages (`@1:15`, `@1:29`, `@1:36`; with it written, `l10_c`, only `@1:11` stays); the `@1:15` names the group's `(`, which the line's last `)` closes once the function type's `)` is written (`l10_c`), and adding a closer at the line's end, as `@1:15` reads, leaves the other three standing |
| 131-52a | **(f)** by ruling 1; (a) if the braces' layout and its margin are one habit | three messages, each naming one edit (delete the braces, dedent, write the field under a case); the braceless run gets exactly the other two, as ruling 1 asks; the jump message counts levels with the `{` line gone (line 2 stands at level 1 as written) |
| 131-53a | **(b)** | the `)` is told only once `->` replaces the `:` (`@1:21`); the `:` is told by the old message, without the habit's `->` fix |
| 131-53b | **(b)** | two hidden in a row: the body's absence told, then the `:` (`@1:13`), then the `)` (`@1:21`), three runs |
| 131-54a | **(a)** | `for (;;)` costs three messages; its fix is `[g]` now, so no certain fix writes `while (;;)` (the audit's (d) half is gone) |
| 131-54b | **(a)** | C's three-clause header costs three messages |
| 131-55a | **(a)** | the arm's `=> 5` written two dedents out costs `expected_arm_arrow@5:14` and `expected_expression@6:5`; moved up, both go |
| 131-55b | **(a)** | the same two for the misplaced `=> 5`; its `)` is told now on R |
| 131-56a | **(a)** | two colons of one habit cost `trailing_colon[C]` and `missing_body@3:23` (which names as missing a body written on the line); the one certain fix writes a program that checks clean |
| 131-56b | **(a)** | three colons, two messages, the third colon untold but repaired by the same certain fix, whose program checks clean |

### The six shapes of section 2.4 (not audit rows)

| shape | class | reason |
|---|---|---|
| g2/r12 | **(b)** | the mistake inside the function among the cases (`print(1 +)`) is told only once the function leaves the variant |
| g2/r09 | **(f)** on R by the measure; (a) by the lane's reading | two edits owed (move the function out, write its body), two messages, the second's caret on `function main()`'s line; moved out, it is refused for its body; T told only the first (b) |
| hc/h09 | **(f)** | one mistake, one message; the arms are read inside the open bracket and hold no mistake here |
| g4/ti | **(b)** | the stray `)` is told only once the operand is written |
| g1/b07 | **(f)** | two mistakes, two messages; the second names the body line as a group member, and with the `(` closed it is `extern_has_body` |
| g1/a11 | **(a)** | one function indented under `use geom` costs `expected_declaration@2:1` and `expected_expression@2:5`; dedented, both go |

### Counts

| | (a) | (b) | (c) | (d) | (e) | (f) | total |
|---|---|---|---|---|---|---|---|
| 130, audit rows | 0 | 1 (34a; (f) by the lane's reading) | 0 | 0 | 0 | 0 | 1 |
| 131, audit rows | 8 | 5 | 0 | 0 | 0 | 2 (22; 52a, (a) by the other reading) | 15 |
| 130, the six shapes | 1 | 2 | 0 | 0 | 0 | 3 (r09, (a) by the lane's reading) | 6 |

No open row prints exit 2, crashes or hangs: every run in `rerun187.txt` (242
exits read, 224 at 1 and 18 applied programs at 0, no timeout at 30 s),
`c4r187.txt` (84 at 1, 60 s timeout), the 192 `*.plain.txt` and
`*.permissive.txt` files (all `exit 1`) and the follow-ups (32 `check` at 1;
`heroes parse` 8 at 0 and 4 at 1, all in `followups/followups.txt`) ended at 0
or 1.

## 4. The recovery instrument

**What it is** (`scratchpad/instrument/tool/recovery.py`, its docstring): one
plausible mistake at a time planted in every program `heroes check` accepts, in
a snapshot tree (`scratchpad/instrument/tree`, the trunk's tree at `c85bccb8`),
from the tokens of a fixed lexer (`instrument/tree/heroes`), so successive runs
plant the same mutants; *"their rule is design.md §4.17's: one mistake costs
one message, none is hidden, and a `certain` fix repairs the mistake it
names"*. Corpus (`base/report.md` § Corpus): 1350 tracked `.hero` files, 650
accepted alone, 9 left out as dear, **641 programs, 63,631 lines mutated**;
96 operators (`counts.json`), `per_op` 150 (`run.json`), from 12 mutants
(`ternary`) to 280 (`paren-condition`), 79 of the 96 between 150 and 170.

**The command** (each run's `run.json`, by `lane-recovery-b8/inst/run_after.sh`
and its b6 twin): `python3.14 tool/recovery.py --compiler <heroes> --out <dir>
--jobs 3 --compare <previous dir>`, run in `scratchpad/instrument`, each over
a plan copied from the run before it (census, baseline, `plan-singles.jsonl`,
`plan-pairs.jsonl`): b8's from `lane-recovery-b6/inst/rGate`, b6's from
`lane-recovery-b5/inst/rGate` (each lane's `inst/run_after.sh`). rGate: compiler
`lane-recovery-b6/heroes-gate`, started 09:03:09, 13.8 minutes. b8 base:
`lane-recovery-b8/heroes-base` (the trunk's compiler, `2bb45a96`), 14:50:38,
14.5 minutes. b8 r2: `heroes-r2` (b8's R1 and R2), 15:45:46, 15.8 minutes. All
three logs end `exit 0`.

**What each total means**, in the instrument's words (`tool/judge.py`
`classify` and `flags_of`; the report's legend): per mutant, the normal arm
(`check --brief`) and the control arm (`check --brief --permissive`); then
`check --apply` and `check --brief` on what it writes.
- **SILENT**: exit 0, a wrong program accepted.
- **ONE**: one diagnostic, on a site line (the mutated lines).
- **EXTRA**: more than one diagnostic.
- **ELSEWHERE**: one diagnostic, off the site.
- **LEGAL**: the spec accepts the mutant with the same meaning, so exit 0 is
  right.
- **APPLY-NEW**: what `--apply` writes carries a code the first run did not
  report. **APPLY-SAME**: `--apply` changed the text and a diagnostic of the
  first run stands. **APPLY-OTHER**: `--apply` wrote a program that checks
  clean and whose `heroes fmt` differs from the original's, *a certain fix that
  compiles and means something else*.
- **HIDDEN** (pairs, `report.md` § Pairs): two mutants in one program; a pair
  counts where its second mistake alone had a diagnostic on its own lines;
  HIDDEN is a pair whose second site then gets none. Split by the second's
  stage with `heroes parse`: a parse-stage second hidden is *the recovery's
  failure (defect 130's class)*; a later-stage second is hidden by design,
  since `check` runs a stage only if the one before said nothing.

**The totals**, summed from each run's `counts.json` by `p187/insttotals187.py`
(output `p187/insttotals187.txt`):

| | rGate (b6's gate) | b8 base | b8 r2 |
|---|---|---|---|
| mutants (errors) | 13,594 (0) | 13,594 (0) | 13,594 (0) |
| ONE | 12,838 | 12,838 | 12,838 |
| EXTRA | 439 | 439 | 439 |
| ELSEWHERE | 14 | 14 | 14 |
| SILENT | 23 | 23 | 23 |
| LEGAL | 280 | 280 | 280 |
| APPLY-NEW / APPLY-OTHER / APPLY-SAME | 0 / 0 / 31 | 19 / 20 / 31 | 19 / 20 / 31 |
| `--apply` changed / clean / restores | 3,598 / 3,567 / 3,545 | 3,598 / 3,548 / 3,506 | 3,598 / 3,548 / 3,506 |
| HIDDEN, parse-stage seconds, normal arm | 4 of 14,684 | 4 of 14,684 | 4 of 14,684 |
| HIDDEN, later-stage seconds (by design) | 1,038 of 1,116 | 1,038 of 1,116 | 1,038 of 1,116 |
| HIDDEN, control arm | 897 of 15,434 | 897 of 15,434 | 897 of 15,434 |
| control arm ONE / EXTRA / SILENT / ELSEWHERE | 12,560 / 424 / 297 / 33 | the same | the same |

- **The APPLY flags of b8 base and r2** are all operator `int` (19 and 20): in
  an `extern` group the certain fix writes `i32` for `int` (*C's `int` is
  `i32`*), where the mutant had replaced an `i64`; read in
  `lane-recovery-b8/inst/base/findings/APPLY-OTHER/001-int.txt` and
  `APPLY-NEW/001-int.txt` (the second one: the applied `i32` then meets an
  `i64`, `mixed_arithmetic`). Lane recovery-b8's progress.md attributes it to
  *"the `int` swap to `i32`, recovery-b7's L6, by design of the mutant"*.
- **EXTRA 439 by operator** (b8 base and r2 alike): `brace-else-chain` 150
  (ruling 4 says not a defect), `c-for` 52 (the shape of rows 131-33a, 54a,
  54b), `option-type` 48, `slice-type-go` 47, `interp-js` 24, `arm-thin-arrow`
  18, `nullish` 16, `range-dots` 12, `arm-colon` 10, `interp-swift` 9,
  `map-type-generic` 8, `bracket-open` 7, `case` 6, `force-unwrap` 6,
  `payload-parens` 6, `forget-f` 4, `missing-operand` 3, `pattern-comma` 3,
  `over-indent` 2, `single-quotes` 2, `string-open` 2, `arm-default` 1,
  `for-of` 1, `plus-plus` 1, `switch` 1 (sum 439). A grep of 15 of these
  names in `open-rows.md.txt` and in items 130 and 131: `option-type`,
  `slice-type-go`, `interp-js`, `nullish`, `arm-colon`, `interp-swift`,
  `map-type-generic`, `force-unwrap`, `payload-parens` and `c-for` appear in
  neither; `arm-thin-arrow`, `range-dots`, `bracket-open` and
  `brace-else-chain` appear in the audit's pair names (e.g. H-005's
  *bracket-open then range-dots*), `bracket-open` also in both items' b5 gate
  line, and `case` only as the common word. The other ten were not searched.
- **SILENT 23**: `forget-f` 17 and `over-indent` 6 (ruling 6: the language's
  reading, sent to panel 184). **ELSEWHERE 14**: `over-indent` 8, `forget-f`
  3, `missing-operand` 3.
- **The 4 parse-stage HIDDEN pairs** all have `bracket-open` as the first
  mistake (4 of 164); their seconds are `assign-equals`, `brace-else-chain`,
  `colon-one-line`, `over-indent`, one each.
- **No run on the round's compiler, nor on b8's R3 to R5**: `lane-recovery-b8/inst/`
  holds `base` and `r2` only, and `find scratchpad/lane-round1002b -maxdepth 2
  -name counts.json` finds nothing. The audit rows that moved after r2 (130-28,
  130-H-004, 130-34d, 131-55b) moved on R3 to R5, which no instrument run has
  read.

## 5. The rulings in force

### 5.1 The seven rulings

`docs/records/log/2026-10-01-0026-seven-rulings-the-recovery-batches-applied-written-where-they-can-be-read.md`,
its table: *"seven readings of design.md §4.17 (one mistake costs one message,
at the mistake, none hidden) for the recovery cluster, defects 130 to 133,
taken by the coordinator as a lane's item list's default (CLAUDE.md § 3)"*;
*"panel: none for 1 to 5 and 7; 6 went to panel 184"*; recorded *"As the
coordinator's, not the author's: rulings taken as a lane's default, each open
to the author's reversal"*. The seven, quoted:

1. *"**A braced block's lines are judged as they will be once the braces are
   gone** (batch 2's B1): one level, exactly 4 spaces, deeper than the head's
   line, so a margin, a `,` or a `)` the unbraced program would refuse is told
   in the same run, once per block, with the code the unbraced program gets;
   the braces' own message is unchanged."* (Landed by b5's `d422815d`.)
2. *"**`record {` with no name** (batch 2's B2): the one token is told once,
   and its words say both what is missing and where the fields go (`record
   Point`, its fields on the lines below it, one level deeper); the same for
   `variant {` and every head that takes a name before a block."* (Landed by
   `d6af2934`.)
3. *"**The Allman `empty_record` and `empty_variant` are not a defect**: the
   code names what the parser sees, a head whose indented block is empty
   because the `{` stands at the head's column ... No new code."*
4. *"**A braced `if`/`else` chain costs one `missing_body` per braced block**,
   each block's braces being that block's mistake, as two braced functions are
   two; the recovery instrument's `brace-else-chain` EXTRA is not a defect."*
5. *"**One indentation habit is one mistake** (batch 4's C4, moved to batch
   5): told once per run of consecutive lines that carry it, at the run's first
   line, with the code each line gets today and words that name the run's
   extent; the fix re-indents the whole run and is `certain` only where the
   run's levels map one to one onto multiples of four ..., a `guess`
   otherwise; a line in the run that also holds another mistake still has that
   mistake told."*
6. *"**`forget-f` and `over-indent` SILENT are the language's reading today**
   ..., so not a recovery defect: they went to panel 184, which sat on
   2026-09-30 and waits on the author."*
7. *"**A certain fix is judged by what `check --apply` writes**, and a later
   stage's first message after it (`unknown_name`, `type_mismatch`, an unused
   binding) is read as the next stage speaking, not as the fix's lie, since
   `check` runs a stage only when the one before said nothing ... the recovery
   instrument itself counts every new code as `APPLY-NEW` ..., so a row it
   flags is read by hand against this ruling, never waved through."*

### 5.2 Panel 183

`docs/panel/183-the-reach-of-an-unclosed-opener-ends-at-a-line-no-bracket-can-hold.md`.
The resolution (lines 98 to 114), quoted in part:
- *"**R1. The reach of the brackets still open ends at a line whose first word
  no bracket holds in a program that compiles** ... at a margin no deeper than
  the margin of the statement the outermost bracket opened in, and for `else` a
  margin strictly shallower; the lexer names every opener still open there,
  `unclosed_bracket` at the opener's own position as today, and lays the line
  out as what it begins."*
- *"**R2. And, inside a `(`, a line after a kept NEWLINE that opens with a
  name** (the critic's `PELNi`): the spec's own sentence refuses it, the census
  moved nothing, and it halves (b)'s hidden class."*
- *"**R3. Where the bracket the rule finds open is closed later in the file**,
  the message the rule writes is not *never closed* ...; if it cannot, the five
  are recorded as the rule's known cost and the author chooses."*
- R4, the design.md §4.15 sentence (5.3 below); *"**R5. No spec sentence**"*;
  R6, the landing's measurements.
- Line 114: *"**Found beside the sitting and carried to the list, not to this
  resolution**: batch 3's `52b2d378` ... raises the diagnostics on 42 of 3,000
  single missing closers from 101 to 253 and `missing_body` from 14 to 142, on
  bodies that exist ...: defect 131's class, the recovery cluster's next item.
  The head class, a head's own `(` left open with the stray closer in its body
  (165 of the critic's 591 pairs), no route here reaches: the cluster's item
  too."*

The author's verdict (lines 125 to 171): *"**RATIFIED 2026-09-30** ...
Recorded as a reading"*; *"**What it does not settle**: the landing itself ...
and the head class, defect 131's and the cluster's"* (line 138); the landing
recorded 2026-10-01: *"**R3 did not land, on its own measurement**"*, its
shapes pinned in
`tests/golden/check/panel-183-a-statement-inside-a-bracket-closed-below-is-the-rules-known-cost.hero`;
*"**The author's choice, 2026-10-01** (... *3b*, recorded as a reading): route
(b), the opener's message reworded to say what the rule measured, the line and
the word where the bracket's reach ended, ... the count of messages unchanged;
and route (c), a narrower R3, prototyped in the recovery lane and measured ...
before any sitting sees it."* Route (b) landed as b6's `8c907fa5`; route (c)
was prototyped in b6's scratch and not landed (`lane-recovery-b6/notes.md`,
*"Route (c) prototype (scratch only ...)"*).

### 5.3 design.md

`grep -n -i` over `docs/design/design.md`, counts as printed:
- `recover`: **4** hits, none about the parser's recovery after a mistake
  (1425 a `cap` that *no iteration strategy can recover*; 1543, Rust's `Debug`
  needed *to recover the distinction* of a number's type; 1926 and 3448,
  §4.15's *unrecoverable* indentation case, *the one accepted silent-error
  case in the language*).
- `cascad`: **0**. `second message`: **0**. `one message`: **0**.
- `one mistake`: **1**, line 1967, the continuation rule (panel 181): *"... are
  one mistake, refused by one diagnostic at the line break,
  `continuation_outside_brackets`. The lexer emits it ... and `check
  --permissive`, where the code is a thesis rule, reads the join."*
- `one diagnostic`: **2**, line 1261 (*"that rejection lives in the parser, so
  it is one diagnostic instead of the unused-binding message plus a second
  one"*) and 1968 (above). `one turn`: **1**, line 2112 (§4.17).
- `4\.17`: **4** hits (667, 1806, 1902, and the heading at 2079).
- **§4.17, "Errors written to be read by a model"** (lines 2079 to 2144), its
  rule sentences: *"**Every error carries all the context needed to fix
  it**"*; *"The model fixes it **in one turn**, without opening anything."*;
  *"**Fixes are tagged `certain | guess`, and only `certain` fixes are
  machine-applicable.** ... a model will apply whatever the compiler
  blesses"*; *"This is also the most measurable thing in the project: count the
  number of exchanges needed to make a broken program compile, before and
  after."* The words *one mistake, one message* and *none hidden*, which the
  seven rulings, the instrument and item 131 (*"design.md §4.17: one mistake,
  one message"*) attribute to §4.17, are not in it.
- **§4.15, the unclosed opener** (lines 1948 to 1957, panel 183's R4): *"An
  unclosed opener is a compile error, reported at the opener. Its reach ends at
  the end of the file, or earlier at the first line inside the brackets whose
  first word no bracket holds in a program that compiles (...) at a margin no
  deeper than the statement the brackets opened in, strictly shallower for
  `else`, or, inside a `(`, at a line that opens with a name after a line that
  kept its NEWLINE: there the lexer names every opener still open and lays the
  line out as what it begins. Without that, one missing `)` would silently
  swallow the rest of the file's layout, or pair with a stray closer below and
  never be named."*
- **Part 11, the control arm** (line 3933): *"Every trial runs against `heroes
  check` *and* `heroes check --permissive` -- the same compiler with the
  thesis-bearing checks disabled ..."*

### 5.4 The spec

`grep -c -i` over `spec/heroes-spec.md` (427 lines): `recover` 0, `cascad` 0,
`one mistake` 0, `one message` 0, `second` 0, `diagnostic` 0, `reported at` 0,
`never closed` 0, `unclosed` 0, `message` 0, `certain` 0. `error`: 18 hits,
each saying what is a compile error (line 34 *a tab ... compile error*, 45,
48, 69, 132 ...), none how a mistake is reported or how many messages it
costs. The sentence row 130-34a turns on, lines 11 to 15: *"Inside `(` `[` `{`
a NEWLINE never ends a statement. A line there keeps its NEWLINE when it ends
with a literal, `?`, `???`, a closing bracket, or a name or keyword other than
`function` and `fail`; that NEWLINE may stand only before a closing bracket or
a `,`, or where a production writes it"*; line 210, `"[" [ Expression { Sep
Expression } ] "]"`, and line 214, `Sep = "," | NEWLINE .`

### 5.5 The contract's words beside them

- `.claude/rules/verification.md` § Bounded discovery (lines 396 to 443), the
  classes the sitting's outcome is read into: **`blocking`** includes *"a
  false message, a `certain` fix that writes a program meaning something else,
  a correct program refused, a wrong one accepted"* and *"an exit 2 where the
  author can be told"*, *"never deferred"*; **`adjacent`**: *"real, found
  beside the work, none of the above (a second message for one mistake, a
  mistake told only after the first is fixed, a true message less exact than
  it could be) ... It becomes `blocking` when two milestone tags have been
  placed since it was filed"*; **`systemic`**: *"a defect that has taken
  **three batches** and is still open, or one whose remaining rows need a
  ruling no rule reaches; it stops the lanes on it and goes to a sitting"*;
  *"**A milestone is tagged** over zero `blocking` and zero `systemic`
  items"*. Section 3's classes (a) and (b) are the words of `adjacent`, (c),
  (d) and (e) of `blocking`.
- `.claude/rules/diagnostics-and-goldens.md` lines 22 and 23: *"**A `certain`
  fix repairs the defect the diagnostic names. A fix that leaves the defect
  standing is a `guess`, however well it compiles**"*. The file has no hit on
  `recover`, `cascad`, `one mistake`, `one message` or `hidden` (`grep -c -i`).
- Lane recovery-b2's item rule (`scratchpad/lane-recovery-b2-items.md` line
  5), the criterion the audit rows were written against: *"one mistake, one
  message, and none hidden (design.md §4.17: a model fixes the program in one
  turn). A diagnostic that is only debris of another is held or not made; a
  real second mistake is said in the same run; a `certain` fix repairs the
  mistake it names and writes a program whose parse-stage diagnostics are a
  subset of the first run's."*

## 6. Row 130-34a

**Source** (`audit-130-133/cases/130-34a/u2_p_stray_inside_body.hero`, origin
`lane-recovery-b2/shapes/u2_p_stray_inside_body.hero`):
```
function f()
    x = [1, 2
    print(x) )

function main()
    print(1)
```

**What the compilers print**: T (the trunk, `4b44f684`) and R (the round) the
same, `expected_separator@3:14`, no fix: *"expected `,` or a new line between
one element and the next, found `)`"*, caret on the `)`. `heroes parse` exits 1
on both. `--apply` leaves the text as it is. The same on lane recovery-b8's
base and final compilers (`rerun-base.txt`, `rerun-r5.txt`).

**The two readings, in the lane's words.** Lane recovery-b8's progress.md line
44: *"G3: 34a is ambiguous (inside a `[` a name-led line is an element; the
`)` closes the `[` by C3's rule)"*; item 130's Class line: *"one row,
`130-34a`, that needs a ruling panel 183's reach rule does not reach (inside an
open `[` a name-led line is an element, so the `)` pairs with the `[`)"*. Set
against the audit's own row:
- **(A) two mistakes**, the audit's: a `]` left out at the end of line 2 and a
  stray `)` at the end of line 3; one message, at the `)`, the missing `]`
  untold. The audit row: *"panel 183 R1/R2 reach it by no clause (a `[`, a
  name-led line)"*; R2's clause, in R4's words, is written for a `(` (*inside
  a `(`, at a line that opens with a name after a line that kept its
  NEWLINE*).
- **(B) one mistake**, the lane's: line 2 ends with a literal, so inside the
  `[` it keeps its NEWLINE, which the array production writes as a separator
  (spec lines 11 to 15, 210, 214); `print(x)` is then a third element, and the
  `)` stands where the `]` goes, a closer of another kind, which C3's rule
  (`df2e13ca`: *one closer ... left out costs one message, and the opener a
  missing closer was for is the one named*) pairs with the `[`.

**Measured beside them** (the audit's own follow-up in `rerun187.txt`, the
others in `p187/cases/followups/`, T and R the same):
- the `]` written, the `)` kept (`audit-130-133/own/a130_34a_fixed.hero`):
  `expected_end_of_line@3:14`, *found `)` -- one statement per line, no
  semicolons*;
- the `)` deleted, no `]` (`f130_34a_closer_deleted.hero`):
  `unclosed_bracket@2:9`, *`[` opened here is still open at line 5, where
  `function` begins a line no bracket can hold*;
- both done, reading (A)'s program (`f130_34a_both_fixed.hero`): `heroes
  parse` exits 0, and the checker says `bad_operand@3:5` (*`print` takes any
  integer, a float, `bool` or `str`, found `[i64]`*), the case's own
  `print(x)`;
- the `)` replaced by `]`, reading (B)'s one edit
  (`f130_34a_reading_b_closer_swapped.hero`): `heroes parse` exits 0, and the
  resolver says `unused_binding@2:5` and `unknown_name@3:11` (*nothing named
  `x` is in scope*): `x` read inside its own initializer.

So by (A), an author who deletes the `)` the message points at is told the
missing `]` on a second run; by (B), one edit makes the parse clean and leaves
a program whose `x` is read inside its own initializer. The message's words
(*between one element and the next*) read line 3 as an element, as (B) does,
and name neither the `[` nor a missing `]`. The original's `heroes parse`
exits 1 on T and R, and (B)'s one-edit program's exits 0 on both
(`followups/followups.txt`).
