# Panel 187, the completeness critic's first pass, over the briefs

Written as it goes, 2026-10-02 from 20:18 (clock from `date`). No verdict.
The frozen tree is the trunk at `62d65e48` (`git log -1`). My copy is
`<scratchpad>/187-critic/` (`git archive 62d65e48 | tar -x`), its compiler
built inside it from the seed, `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, exit 0, sha256 prefix `b578e9a799ab7469`.
Every command below ran in that copy with that compiler unless it says
otherwise. `<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.

## 0. Which compiler the briefs mean, settled first

- `git rev-parse 62d65e48:seed/heroes.c d4fd12ef:seed/heroes.c` prints
  `914e2585...` twice, `git diff --stat d4fd12ef 62d65e48 -- seed/ selfhost/
  runtime/` is empty, and `git merge-base --is-ancestor d4fd12ef 62d65e48`
  holds. My compiler's sha256 prefix is `b578e9a799ab7469`, the prefix
  `00-facts.md` gives R. **So the frozen trunk's compiler IS what
  `00-facts.md` calls R, the round's**, and what it calls T (`4b44f684`) is
  no longer the trunk's. `00-facts.md`'s Conventions still call T "the
  trunk's compiler" and its § 6 says "T (the trunk, `4b44f684`)": true when
  gathered (18:08 to 18:39, before `b48d02b8` merged the round), stale on the
  tree the seats will copy. A seat that builds its copy at `62d65e48` and
  reads "T" as its own compiler will look for differences that its compiler
  does not have (131-55b's `@6:10`, 130-28's `@2:20`, 130-34d's `@2:14`,
  H-004's `@14:1`, g2/r09's `missing_body@5:1`). **Repair**: one sentence in
  `00-facts.md` or `00-shared.md` saying the seats' compiler at `62d65e48` is
  R, by the sha256 above.

## 1. Check (a): does the coming round move the sitting's facts?

### 1.1 How h158 was built, and a correction to the instruction

`git rev-parse 3a50817f:seed/heroes.c` prints `914e2585...`, the trunk's
seed, and `8cb4ba6c`'s body says *"Seed not regenerated, awaiting the batch
gate (CL-079)"*. A compiler built from `3a50817f`'s seed by clang is the
trunk's: I built one in `<scratchpad>/187-critic-h158/` and its sha256 prefix
is `b578e9a799ab7469`, the same as mine. **No reading was taken with it.**
The coordinator's correction, received during this pass, says the same; I
had already moved to the build below before it arrived.
Lane h158's compiler was built from its sources in that copy,
`./heroes build selfhost/main.hero -o heroes-h158` (exit 0, 20:19:34 to
20:21:02, sha256 prefix `f65f82b51c6c9eaf`), and every reading of (a) is
taken with it (**H**). As a control, the trunk's own `selfhost/` was built
the same way in my copy (`heroes-stage1`, **S**, exit 0, sha256 prefix
`b66ab809ba39bc90`), so a difference between H and K (my seed-built trunk
compiler) comes from the sources and not from the build route.

**H differs, shown before relying on it** (`<scratchpad>/187-critic/proof/`
and `c166/`, the coordinator's probes kept in
`docs/panel/186-briefs/probes/coordinator/adjacent/` and lane h158's
`d166/`):

| probe | K (62d65e48) and S | H (3a50817f's sources) |
|---|---|---|
| 165, `+` alone over `1 => "one"` (`s09_plus`) | `continuation_outside_brackets@4:9[g] expected_pattern@4:9`, `--apply` writes nothing | `continuation_outside_brackets@4:9[C]` alone, *delete the `+`*, applied clean |
| 166, `-` alone over `x => "one"` (`s18_name_below`) | `continuation_outside_brackets@4:9[C] expected_pattern@5:9 expected_pattern@5:9` | `continuation_outside_brackets@4:9[C] expected_pattern@5:9` |
| 166, `-` alone over `1.5 => "one"` (`s19_float_below`) | `continuation_outside_brackets@4:9[g] expected_pattern@5:9 expected_pattern@5:9` | `continuation_outside_brackets@4:9[C] expected_pattern@5:9` |

### 1.2 Every row the briefs state, on K and on H

Script `<scratchpad>/187-critic/rerun-critic.py` (modelled on
`p187/rerun187.py`: `check --json`, `check`, `parse`, `check --apply` and a
check of what it writes; `--permissive` for 131-22), output
`rerun-critic.txt`, run 20:23:48 to 20:23:58: 147 files, the 59 rows of
`lane-recovery-b8/audit/open-ids.txt` with the audit's own follow-ups, the six
shapes of item 130's Class line, the facts session's `followups/`,
`followups-beside/`, `recon/` and `131-41a-siblings/`, 23 probes of 165 and
166, and the blind seat's five programs. Row 131-42's 42 files by
`c4r-critic.py` (from `p187/c4r187.py`) in a `cp -c -R` clone of the audit's
mirror, output `c4r-critic.txt`.

- **K is R.** Every one of the 95 files of the facts session's
  `p187/rerun187.txt` reads on K exactly as its `R` line (script comparison,
  95 common, 0 differ). **S reads as K on all 147** (0 `S!=K`).
- **H moves no audit row, none of the six shapes, no follow-up, no
  reconstruction and no blind program**: of 147 files, the 15 that move are
  all 165/166 probes (below). Row 130-34a reads
  `expected_separator@3:14` on K and H; its four follow-ups the same. Row
  131-42: 42 of 42 files at one `unclosed_bracket` on K and on H, 0 differ.
  **So F2's 16 open audit rows, F3's counts table and its six shapes hold on
  H as they hold on K.**
- **Defect 166's four shapes** (`docs/work/DEFECTS.md` item 166 at
  `62d65e48`, lane h158's `d166/n1`, `n2`, `n3`, `n9`): the same on K and H,
  each `continuation_outside_brackets@8:<col>` then `expected_pattern@9:9`
  twice. **The brief's "four shapes still read two messages at one place"
  holds on both.**
- **What does move, and it bears on Q3 and Q4**: on K, the frozen tree the
  seats copy, **seven** of these probes read `expected_pattern` twice at one
  place, not four: item 166's own line shapes `s18` and `s19`, and `n8` (a
  `-` over a group), besides the four. On H, four. Two more read
  `continuation_outside_brackets` twice at one place on K (`q3`
  `@5:5 @5:5`, `q8` `@5:9 @5:9`) and once on H. A seat at `62d65e48` that
  runs item 166's own title shape finds it still doubled and may read the
  brief's "four" as wrong; the brief should say the four are what stands
  AFTER `8cb4ba6c`, which is not in the frozen tree.

| probe | K (62d65e48) | H (3a50817f) |
|---|---|---|
| `s09_plus`, `s12_star`, `q4`, `q5`, `q6` (165) | `cob[g]` (or no fix) `expected_pattern@4:9` | `cob[C]` alone, applied clean |
| `s18_name_below` (166's line) | `cob[C] ep@5:9 ep@5:9` | `cob[C] ep@5:9` |
| `s19_float_below` (166's line) | `cob[g] ep@5:9 ep@5:9` | `cob[C] ep@5:9` |
| `n6` `+` over a name, `n7` `*` over a float | `cob[g] ep@8:9 ep@9:9` | `cob[C] ep@9:9` |
| `n8` `-` over a group | `cob[C] ep@9:9 ep@9:9` | `cob[C] ep@9:9` |
| `q1`, `q2` deeper | `cob[g] unexpected_block@5:1` | `cob[C] unexpected_block@5:1`, applied clean |
| `q3` shallower, `q8` `::` | `cob[g]` twice at one place | `cob[C]` once, applied clean |
| `q7` | `cob[g] ep@4:9 expected_end_of_line@5:20` | `cob[C] expected_end_of_line@5:20` |
| `n1`, `n2`, `n3`, `n9` (166's four) | `cob ep@9:9 ep@9:9` | the same |

(`cob` is `continuation_outside_brackets`, `ep` is `expected_pattern`.)

### 1.3 Lanes land186 and bounded

- **land186.** The task names `9fb57124`; the lane's head is now `7887c09c`
  (10-02 20:17, two commits past it, `bac43e50` and `7887c09c`, which add 69
  lines to `selfhost/cli/layout.hero` and nothing else under `selfhost/`).
  `git diff --stat 62d65e48 9fb57124 -- selfhost/`: 23 files, 1229
  insertions and 161 deletions, in `check/group_fields.hero` (new, 161),
  `check/walk.hero` (32), `label_errors.hero` (78), thirteen files of
  `emit/`, four of `ir/` and `measure/pinned.hero`; **no file of
  `selfhost/parse/`, `parse.hero`, `lexer.hero`, `open_line.hero`,
  `grammar_expr.hero`, `line_above.hero`, `bracket_reach.hero`,
  `line_joins.hero` or `join_fix.hero`** (`git diff --name-only` over those
  paths, empty). The checker change is panel 186 R7's construction of a group
  record over a C union (`construct_record`, guarded by
  `!rec.header.is_err()`), not recovery. Because a reading of file names is
  an inference, I built `7887c09c`'s compiler from its sources the same way
  (`<scratchpad>/187-critic-land186/heroes-land186`, exit 0, sha256 prefix
  `f15b7bd1c8b435be`; its seed is again the trunk's, `914e2585...`) and ran
  the same 147 files (`rerun-land186.txt`) and 131-42's 42
  (`c4r-land186.txt`): **0 of 147 move, 42 of 42 at one `unclosed_bracket`.**
- **land186 moves two documents the seats read.** `git diff 62d65e48
  7887c09c` changes `spec/heroes-spec.md` (one hunk, `@@ -360,5 +360,5 @@`,
  § 13, panel 186 R7) and `docs/design/design.md` (22 lines added at 2393,
  after §4.17, so §4.17's lines 2079 to 2145, 2112, 1948 to 1957, 1967, 1261
  hold, and Part 11's control-arm line 3933 becomes 3955). So **the spec a
  resolution of this sitting would amend is not the spec the spec-warden
  prices at `62d65e48`**: its § 13 changes in the merge. The warden's
  before/after should be priced on both, or the brief should say which.
- **bounded `51330ea8`**: its base is `9faf7462` (on the trunk); `git diff
  --stat 9faf7462 51330ea8` is `.claude/rules/records.md` (7) and
  `tests/harness/suite_records.hero` (852 insertions), nothing under
  `selfhost/`, `seed/` or `runtime/`. It moves no reading. It changes
  `records/lists` and `records/tagged`, the executors of the classes Q4
  files items into; a seat reasoning about what "closes" or blocks a tag
  reads the rule in `.claude/rules/verification.md`, which this commit does
  not change, but the executor that will judge the sitting's filings is the
  merged one. Not run: the three lanes merged together (each was built and
  read alone).
- **Exits**: on K, H and land186 every `check` of the 147 files exits 1,
  every `parse` and applied check 0 or 1, no timeout at 30 s. F3's closing
  paragraph holds on all three.

**(a)'s answer**: no row or count of F2 or F3, none of the six shapes, no
row 130-34a reading and none of 166's four shapes reads differently under
`3a50817f` (built from its sources) or `7887c09c`. What moves under
`3a50817f` is outside the briefs' tables: item 166's own title shapes (`-`
over a name or a float) go from two `expected_pattern` at one place to one,
and 165's `+`/`*` shapes from two messages to one with a certain deletion.
On the frozen tree those title shapes still read twice, which the briefs
do not say.

## 2. False or stale on the frozen tree: the record the briefs quote

### 2.1 Item 130 in `docs/work/DEFECTS.md` is damaged at `62d65e48`

`00-facts.md` § 1.1 quotes item 130's line at `docs/work/DEFECTS.md:23` as
*"130 -- after a `match` whose arm fails to parse, the next statement is
skipped whole, and every mistake in it goes unreported ... `selfhost/grammar_expr.hero`
(`match_expr`) · the enclosing statement's recovery · class: systemic"*. On
the frozen tree that line reads (its em dash written ` -- `, as `00-facts.md`
does):

```
- [ ] **130 -- after a unction main()`, three bound matches ... `y = 3 )` overecovery · **class: systemic**
```

`git diff 4b44f684 8349d264 -- docs/work/DEFECTS.md` shows where: commit
`8349d264` (10-02 18:39, *"Defects 165 and 166 filed, adjacent ..."*) rewrote
line 23 (660 bytes in the round's copy, 355 at `62d65e48`; a character diff
shows two deletions, the item's title from *`match` whose arm* to *in `f*,
and its tail from *`z = 4 )`* to *the enclosing statement's r*) and collapsed
the item's lines 81 to 87 into one line, *"in lane 130, `47849cc9`); and a
misspelled keyword at a by a"*, losing the `recrod Point` sentence and the
head of the paragraph *"**Widened 2026-09-29 by lane 133's agent**: ... the
parser HUNG, forever ..."*. The round's copy at `d4fd12ef` is whole; the merge
`b48d02b8` kept the trunk's side. **A seat that opens item 130 in its copy
finds a line with no title and no "where to look" field.** This is a
records defect for the coordinator (an append-only list losing text, which
`records/appended` may or may not see); I did not edit it. **Repair for the
briefs**: say that item 130's text is to be read at `d4fd12ef` (or
`4b44f684` for the line) until the trunk is repaired.

### 2.2 Positions and lengths that were true on `4b44f684` and are not on `62d65e48`

`00-facts.md` § 1 read `DEFECTS.md` on `4b44f684` and on the round's
uncommitted copy. On the frozen tree (`awk` over item lines,
`grep -n '^    \*\*Class: systemic\*\*'`):

| fact | `00-facts.md` | at `62d65e48` |
|---|---|---|
| item 130's span | 23 to 299, 277 lines (trunk); 23 to 333, 311 (round) | 23 to 332, 310 lines |
| item 130's Class line | lines 287 to 299 | from line 320 |
| item 131's line | `docs/work/DEFECTS.md:301` | line 334 |
| item 131's span | 301 to 495, 195 lines | 334 to 528, 195 lines |
| item 131's Class line | lines 492 to 495 | 525 to 528 |
| the spec's length (§ 5.4) | 427 lines | 432 lines (`wc -l`; panel 186 R6's `b7c510c5` came in with the round's merge; lines 11 to 15, 210 and 214 and every `grep -c -i` count of § 5.4 are unchanged) |
| T, "the trunk's compiler" | `4b44f684`'s | `62d65e48`'s compiler is R (section 0) |

None changes a reading; each is a number a seat will check against its copy
and find wrong.

### 2.3 Other statements true when written and stale now

- `00-facts.md` § 1.1: *"`git log --grep='^Defect 130' main` lists 21
  commits ... the last `cf9e2792`"* and *"b8's five are on the lane's branch
  and the round's tree only"*. At `8349d264` (main at 18:39) the command
  lists 21 (131: 21, 20 subjects), as written. **At `62d65e48` it lists 29
  for 130, the last `a388a056`**; 131 is unchanged. All 67 commit hashes the
  briefs cite exist, are commits, carry the dates and times the § 1 tables
  give (`git log -1` each), and are now all on the trunk.
- `00-facts.md` § 5.1 quotes ruling 6, *"they went to panel 184, which sat on
  2026-09-30 and waits on the author"*: true at the log entry's 00:26 on
  10-01. Panel 184 is **RATIFIED 2026-10-01** (its file, line 248), its R4 (a
  statement after a jump is refused) not yet in the spec; the forgotten `f`
  went to panel 185, **RATIFIED 2026-10-02**, whose R7 refuses it only once
  an author's condition is met; `docs/work/DECIDE.md` reads `**OPEN: 0**`. I
  read one `over-indent` SILENT finding
  (`lane-recovery-b8/inst/base/findings/SILENT/002-over-indent`): it moves a
  statement into an inner loop, not after a jump, so 184's R4 does not reach
  it.

## 3. Check (b): the blind seat's inputs

All run in `<scratchpad>/187-critic/blindcheck/`.

- **The messages are byte for byte the compiler's.** `heroes check
  p<n>.hero` on each `p<n>.hero.txt` copied as `p<n>.hero`: exit 1, stdout
  empty, stderr `cmp`-identical to `p<n>.messages.txt`, all five. They also
  read the same under H and land186 (section 1).
- **The brief's quoted blocks are the files.** The ten fenced blocks of
  `blind/brief.md`, compared by script to the five programs and five message
  files: all ten equal. `<scratchpad>/187-blind/brief.md`, `p<n>.hero` and
  `p<n>.messages.txt` are `cmp`-identical to the trunk's
  `docs/panel/187-briefs/blind/` copies.
- **Each intent has a repair that checks clean and prints what it says**:
  `c1` `for i in range(from: 0, to: 3)`, `c2` `function seven() -> i64`,
  `c3` `record Point` with no `)`, `c4` `x = [1, 2]` and `print(x.len())`,
  `c5` `print(total)`. `check`: exit 0 and no output, all five; `run` (with
  `HEROES_RUNTIME` set to the copy's `runtime/`): `0 1 2`, `7`, `3`, `2`,
  `3`, each exit 0.
- **`<scratchpad>/187-blind/spec.md` is `cmp`-identical** to `git show
  62d65e48:spec/heroes-spec.md`.
- **No `CLAUDE.md` in or above the folder**: a walk from
  `<scratchpad>/187-blind` to `/` testing `CLAUDE.md`, `CLAUDE.local.md`,
  `.claude`, `AGENTS.md` and `.git` found none; `git rev-parse` there says
  *not a git repository*; `/Users/joseph/.claude/CLAUDE.md` does not exist.
- **The files carry no framing**: `grep -i` over `brief.md`, the programs and
  the messages for `panel|defect|lane|batch|audit|recover|systemic|adjacent|blocking|cascad|hidden|selfhost|heroes-lang|/Users|/private|scratchpad|repositor|design.md|ruling|instrument|sitting|coordinator|seat|blind`
  and any three-digit number from 100 to 199: two hits, *120 words* and
  *100 models*.
- **But the folder's own path carries both.** The session runs in
  `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-.../scratchpad/187-blind`,
  and a Claude Code session is told its working directory: the panel number
  (`187`), the word `blind`, and the repository's path in the scratchpad's
  encoded name. Panels 183, 185 and 186 used the same `<NNN>-blind*` shape.
  Whether it matters is the seat's `context` answer; a neutral folder name
  outside the session's scratchpad would remove the question, at no cost.

**Premises in `llm-ergonomist.md` and `blind/brief.md`** (not errors in the
files, framing the experiment's reading rests on):

- *"five short programs of the open rows' shapes"*: **`p3` is not one.** Row
  131-16b is `record Point )` with NO field (`empty_record@1:1`, the `)`
  untold); with fields, `p3`'s program, the `)` is told,
  `expected_end_of_line@1:14`: one mistake, one message (F2's own follow-up
  `a131_16b_then_field`). So `p3` is a second control, and the set holds one
  class (a) program (`p1`), one class (b) (`p2`), the ambiguous `p4` and two
  controls.
- **`p2`'s hidden `)` stands on the line its one message points at**, so a
  model that reads that line sees it. No program tests a hidden mistake on
  another line, which is where (b) rows put theirs: 131-32's `} while (1 ==
  1)` on line 4, `g2/r12`'s `print(1 +)` inside the function, `g4/ti`'s `)`.
  With one (b) program, the sitting's question *"whether a hidden mistake
  costs a model a second turn"* rests on the easiest (b) there is.
- *"a second message (class (a)) costs it nothing but noise"* is the
  hypothesis, written as what the routes disagree about. `p1`'s messages
  include `fix (guess): use \`while\``, which, followed, writes `while (i =
  0; i < 3; i++)`. Whether (a) costs a turn is measured by `p1` only.
- **The report's `reading` heading asks for *"every mistake you found that the
  messages did not name"***, which tells the seat, before it writes `c1` to
  `c5`, that some mistakes are unnamed. That is the variable class (b) is
  about; a model repairing a program in practice is not told. A second
  session, or a heading that does not announce it, would not prime it.
- **All five programs in one session see each other** (`.claude/skills/panel/SKILL.md`
  § 2: *one session per task when the tasks must not see each other*). With
  `p5` a certain-fix control beside four that are not, the set shows what is
  being measured. Five sessions at the readings' observed 0.25 to 0.48 USD
  each would be about 1.25 to 2.40 USD; not run.
- *"the sitting's two earlier readings cost 0.25 to 0.48 USD"*: panel 187 has
  none. Panel 186 had three, 0.48 (its synthesis, line 43), 0.38
  (`186-blind-2/run.json`) and 0.25 (`186-blind-3/run.json`). `claude
  --version` reads `2.1.285`, the version panels 184 to 186 ran
  `claude-opus-5-5` on, so the named run can use the sitting's model.

## 4. Check (c): the routes of Q1 to Q5

### Q1 (stated as a question; its routes carry premises)

- **(1b) "at today's values" names a baseline nobody has measured.** No
  instrument run exists on the frozen tree's compiler: its `compiler_sha` is
  the binary's sha1 (`tool/recovery.py:62-67`), R's is `d24885f477fa0866`,
  and none of the 62 `run.json` files under `<scratchpad>` (recursive, my
  folders excluded) carries it; the newest is b8's r2 at 15:45:46. Not
  searched: `.claude/worktrees/`. The values the briefs give are b8 r2's
  (before b8's R3 to R5) and rGate's. No seat can check "today's values"
  from its copy.
- **(1b) "held by a suite in the net": the instrument is not in the
  repository.** `git ls-tree -r 62d65e48` has no `recovery.py` or `judge.py`;
  its six tracked `.py` files are three hooks, two panel probes and
  `site/serve.py`. The instrument lives in `<scratchpad>/instrument/tool/`,
  which a reboot empties (SKILL.md § 3c). A suite needs it inside the tree,
  and CLAUDE.md § 10 says *never a script*. **The in-tree home no brief
  names**: `heroes mutate` exists (`./heroes` lists `lex parse check build run
  test fmt mutate probe measure`), metric 3 of design.md Part 11, in
  `selfhost/cli/mutate.hero` and six modules under `selfhost/mutate/` (1,622
  lines by `grep -c ''`), a port of the archived Rust `mutate`; it counts
  killed and survived, not ONE, EXTRA, ELSEWHERE or HIDDEN, and no harness
  suite runs it (`grep -l '"mutate"'` finds only `selfhost/cli/table.hero`).
  **Route nobody listed**: (1b) built as `heroes mutate`'s recovery arm.
- **(1b)'s corpus is a snapshot.** The instrument plants into
  `instrument/tree`, the trunk at `c85bccb8` (09-30 01:57), with a fixed
  lexer, *"so successive runs plant the same mutants"* (F4). A suite over the
  live tree plants different mutants as the tree changes, so a ratchet on raw
  totals compares different corpora. The route needs a frozen corpus, or
  rates; the briefs say neither.
- **(1b)'s cost is unpriced**: 13.8, 14.5 and 15.8 minutes of wall at
  `--jobs 3` for the three runs (`run.json` `wall_minutes`), beside a whole
  net that `.claude/rules/verification.md` gives as 13 to 20 minutes.
- **(1b) lists ONE, EXTRA, SILENT and HIDDEN, and says no class (d) row may
  stand; the instrument's own (d) counter stands at 20.** `APPLY-OTHER`, *"a
  certain fix that compiles and means something else"*, is 0 on rGate and 20
  on b8 base and r2 (`APPLY-NEW` 0 and 19), every one operator `int`
  (`insttotals-critic.txt`, byte-identical to the facts session's). The
  lane's *"by design of the mutant"* is a reading, not a measurement.
  `00-shared.md`'s F4 paragraph says the three runs read *"alike"*, which is
  true only of the classes it lists; it omits the APPLY flags.
- **(1b)'s SILENT moves with another sitting**: its `forget-f` 17 are panel
  185's R7, not the recovery's (section 2.3).
- **(1c) "close as such"**: an `adjacent` item *"becomes `blocking` when two
  milestone tags have been placed since it was filed"*
  (`.claude/rules/verification.md` § Bounded discovery). So (1c) refiles the
  rows with a clock, which Q1 does not say.
- **The premise that every open row is (a), (b) or (f)**: `g2/r09`'s second
  message is `missing_body@5:1`, *"a `function` needs an indented body ...
  (found the end of the block)"*, its excerpt and caret under `function
  main()`, which has a body, and it names no function (plain output in
  `cases/b8-g2-r09/r09.hero.K.plain.txt`). Read where it points, it says
  `main` lacks a body. F3 considers (f) and (a) for this shape, not (c). By
  the author's classes a false message is `blocking`, never deferred.
  **A question for the sitting, not a finding of mine**: is that message
  false?
- **Routes nobody listed**:
  - **(1f) pin what stays as a known cost.** This tree already holds the
    shape: `tests/golden/check/panel-183-a-statement-inside-a-bracket-closed-below-is-the-rules-known-cost.hero`
    (panel 183's R3). Done would be every audit row either closed or pinned
    in a golden with its reason, so a change in either direction is seen.
  - **(1g) §4.17's own measure.** design.md lines 2126 and 2127: *"count the
    number of exchanges needed to make a broken program compile, before and
    after"*. Done as a count of exchanges (a mechanical loop that applies
    what the first message says and re-checks, over the audit rows or the
    instrument's pairs), or as the blind seat's one-turn rate. The briefs use
    the blind reading as evidence for Q1 and Q5, not as a candidate
    definition.
  - **(1h) the seven rulings, adopted or reopened.** The log entry records
    them as *"the coordinator's, not the author's ... each open to the
    author's reversal"*, panel *"none for 1 to 5 and 7"* (lines 19 and 65 to
    67). `00-shared.md` calls them *"the rulings in force"*. A definition of
    done that rests on ruling 4 (150 of EXTRA's 439 are not a defect)
    inherits an unratified ruling; whether this sitting ratifies them is a
    route the question admits.

### Q2 (a question; one route unlisted, and one fact a seat needs)

- **Route nobody listed**: one message reworded, as panel 183's route (b)
  did for the opener, naming the `[` at 2:9 and both edits, so the reader
  chooses between (A) and (B) instead of the compiler.
- The row's program is wrong under both readings: (A)'s two edits leave
  `bad_operand@3:5` (`print` of an array) and (B)'s one leaves
  `unknown_name@3:11` (F6, re-run on K: the same). With no stated intent, the
  case cannot settle *"which is the program's author's"*; the blind `p4`
  gives an intent (`x.len()`) that does.

### Q3 (stated as a choice of two; measured here, a third)

The stated cause **holds by experiment**. In `<scratchpad>/187-critic-p166/`
(`git archive 62d65e48`), one condition of `parse/opening.hero`'s
`drop_line` was made never true (line 310, `if c.pos != from && ...` to `if
c.pos < 0 && ...`, keeping `text` used) and the compiler was built from
those sources (exit 0, 20:37:10 to 20:38:37):

- all seven doubled probes (166's four, its two title shapes `s18` and `s19`,
  and `n8`) read **one** `expected_pattern`, not two;
- of the 147 files, those seven move and **nothing else**
  (`rerun-p166.txt`);
- the `check` golden form, `heroes run tests/harness/main.hero --
  ./heroes-p166 check`: **406 passed, 0 failed**;
- the compiler's own tests on those sources: **1036 tests, all passed**;
- `fixedbugs-131-a-body-in-a-tab-margin.hero`, the golden `9811d4cd` added
  with the stop (`drop_line`'s own comment gives the reason, *"after an `=>`
  or an `else` over a line whose margin holds a tab the lexer puts no line
  end"*), prints the same 41 errors with and without it.

So **a route nobody listed: the stop narrowed or removed, a local repair of
one condition**, smaller than *"the parser told where the lexer joined
lines"*. Unrun: the full net, the census, the instrument, the formatter's
probe, the platforms. A question, not a premise: the compiler-engineer's to
confirm or refute.

### Q4 (a question)

Open items it does not name: 166's four shapes (does 166 join the cluster's
filing?), the `g2/r09` (c) reading above (which would make one shape
`blocking`), and the instrument's 20 APPLY-OTHER flags if (1b) is adopted.

### Q5 (its first clause is a premise)

*"design.md §4.17 promises one turn"*: the sentence, lines 2111 and 2112, is
about one error: *"The model fixes **it** in one turn, without opening
anything"*, *it* being an error that *"carries all the context needed to fix
it"* (line 2090). Read literally, a hidden mistake breaks nothing there; it
adds an exchange to the measure at 2126 and 2127. A second message breaks
line 2090 only when its context or fix misleads. The per-program reading is
lane recovery-b2's paraphrase, *"(design.md §4.17: a model fixes the
**program** in one turn)"* (`<scratchpad>/lane-recovery-b2-items.md` line
5), which the instrument's docstring and the seven rulings repeat as
*"design.md §4.17's"*. **Repair**: quote §4.17's sentence in Q5 and ask which
reading the contract holds, rather than presupposing one. The spec-warden's
window, *"around its line 2090 to 2115"*, stops before 2126 and 2127; §4.17
runs from 2079 to 2145.

**A home Q5 does not list**: the definition could live in
`.claude/rules/verification.md` § Bounded discovery or
`.claude/rules/diagnostics-and-goldens.md` (process, amended by author
instruction with no panel, CLAUDE.md § 4) rather than design.md (a
diagnostic class, which needs a panel). Where it is written decides who may
change it.

### Numbers a seat is handed and cannot check from its own copy

- **F1's batch counts, dates and commit facts**: a seat's copy is `git
  archive`, with no `.git`, and `00-shared.md` says *"Never ... read inside
  ... the trunk"*. "Seven batches", "four", "21 commits" (the basis of
  `systemic`) are unchecked by any seat as briefed. I checked them against
  the trunk's git (section 2.3): they hold as of 18:39.
- **"today's values"** of the instrument (above).
- **The spec's binding token count for a new sentence.** `heroes measure
  spec/heroes-spec.md` in my copy, no `--refresh`: maximum 6928 (vendored),
  `real` 9169 against the 10240 ceiling (pinned, *claude-opus-5,
  2026-10-02*); in land186's copy maximum 6923, `real` 9164. The warden may
  not run `--refresh` (an API call), so its price for an amended spec is
  the vendored figure, and the binding `real` delta is unmeasured unless the
  coordinator runs one refresh.

## 5. Check (d): the historian's brief

Handed as fact rather than as something to verify, every one without a
version or a date:

1. **Go**: *"the compiler's 10-error limit and `-e`"*, a count and a flag
   stated as fact; and `go vet`, an analyser of code that compiles, is put
   beside them as if it bounded error recovery.
2. **GCC and Clang**: `-fmax-errors` and `-ferror-limit` as fact; *"the 'in
   file included from' chains"* are include context, presupposed relevant to
   recovery.
3. **rustc**: *"the 'cascading error' suppression"* as a named mechanism, and
   `-Z treat-err-as-bug` as part of how rustc *"bounded"* its recovery. Its
   documented purpose is the thing to verify (my unverified understanding:
   a debugging flag).
4. **Elm**: *"one error at a time, by design"*, a characterisation handed as
   the precedent. It is the claim to verify, and for which phase (parse,
   types) and which version.
5. **Swift and TypeScript**: *"best-effort recovery, the suppression of
   follow-on errors"*, a characterisation, not a question.
6. **Its only input is `00-shared.md`**, so 641 programs, 13,594 mutants and
   "seven batches" reach a seat with no shell. Fine as context, not citable as
   measured.

**Precedents the brief does not ask about** (names for the historian to
verify; from my memory, unverified, not facts): corpus measurements of
recovery quality (Ripley and Druseikis 1978 on Pascal syntax errors;
Diekmann and Tratt 2020, *Don't Panic!*, counting cascading errors on a Java
corpus), and golden pins of every message a broken program gets (rustc's UI
tests and their `.stderr`; clang's `-verify` with `expected-error`
comments), the precedent for route (1f).

## 6. What I re-ran and what holds; what I did not re-run

**Re-run on the frozen tree, holding**: F2, all 95 files of
`p187/rerun187.txt`, equal on K to its R line, and 131-42's 42 files; F3's
closing paragraph on exits; F4's totals (the reader's output byte-identical),
corpus rows, 96 operators, `per_op` 150, `ternary` 12 to `paren-condition`
280, 79 of 96 between 150 and 170, the three runs' start times, wall minutes
and `exit 0` log endings, the docstring's sentence, EXTRA by operator (sum
439), the 4 parse-stage HIDDEN all with `bracket-open` first, and the grep
of 15 operator names (10 in neither file; `arm-thin-arrow`, `range-dots`,
`brace-else-chain` in the audit only; `bracket-open` in both); F5, every
design.md and spec `grep` count and line number (but the spec's length,
section 2.2), the seven rulings' words, panel 183's line numbers, the
known-cost golden's path, `lane-recovery-b2-items.md` line 5 and
`diagnostics-and-goldens.md` lines 22 and 23; F6, its four follow-ups;
`00-shared.md`'s *"the stop defect 131 added at `9811d4cd`"* (`git log -L`
over `drop_line`'s head: that commit alone); `progress.md` line 44; every
path in `compiler-engineer.md` (each exists; `open_line.hero`,
`line_above.hero` and `bracket_reach.hero` are under `selfhost/`, not
`selfhost/parse/`).

**Not re-run**: the instrument (the briefs forbid it; about 15 minutes of wall
at `--jobs 3`, 13,594 mutants, one run on the frozen tree's compiler would
give (1b) its baseline); the past gates' counts F1's tables quote (the
record's words of gates that cannot be re-run); the round tree's uncommitted
`DEFECTS.md` (I read `d4fd12ef`'s committed copy instead); the three lanes
merged together (each was built and read alone); lane notes other than
`progress.md` line 44.

## 7. Repairs, in one list

1. Say the seats' compiler at `62d65e48` is R (section 0), and correct T's
   label.
2. Say item 130's text in `DEFECTS.md` is damaged at `62d65e48` (section 2.1),
   and where to read it whole; the damage itself is for the coordinator.
3. Update the positions, the spec's length and the `git log` count (sections
   2.2, 2.3), and ruling 6's panel-184 sentence.
4. Say on the frozen tree 166's title shapes still read twice, and the four
   are what stands after `8cb4ba6c` (section 1.2).
5. Q1: give (1b) its measured basis or call it unmeasured: no run on R, the
   instrument outside the tree, `heroes mutate` as its in-tree home, the
   snapshot corpus, the cost, the APPLY flags; add (1f), (1g), (1h).
6. Q2: add the reworded-message route. Q3: add the measured local route.
   Q5: quote §4.17 and ask which reading.
7. Put the `g2/r09` (c) reading to the sitting as a question.
8. Blind: `p3` is a control; `p2` is the easiest (b); the `reading` heading
   primes; one session or five; the cost sentence is panel 186's.
9. Historian: write items 1 to 5 of section 5 as claims to verify.
10. Say whether seats may run `git -C <trunk> log`, or that F1 is unchecked
    by them.
