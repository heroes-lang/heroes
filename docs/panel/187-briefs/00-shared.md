# Panel 187, the shared brief: what a finished recovery is

Written 2026-10-02 by the coordinator on the trunk at `62d65e48`, then
repaired after the completeness critic's first pass
(`docs/panel/187-reports/completeness-critic-briefs.md`; the text it read is
`00-shared-before-the-critic.md`) and re-based on the trunk at `07ccb72a`, where
the round of 2026-10-02's third gate is merged. Every number below names the
command or the file that produced it. `00-facts.md` in this directory holds
the facts an agent of the coordinator's measured between 18:08 and 18:39 on
`4b44f684` and on the round's compiler, cited as F1 to F6; **where the
sitting's head reads otherwise, § Measured on the sitting's head says so, and
it wins.** What was not run says so.

## Why this sitting

The author ruled on 2026-10-02 (`.claude/rules/verification.md` § Bounded
discovery, the author's *D1a D2a D3a*) that a defect is `blocking`,
`adjacent`, `systemic` or `improvement`, that a defect still open after three
batches is `systemic` and goes to a sitting, and that a round ends when its
items are repaired, not when nothing more can be found. Defects 130 and 131
are `systemic` by that rule: 130 took seven batches (recovery-b1 to b6 and
b8) and 131 four (F1), and each batch's first pass found new shapes of the
same items. The question is the one the rule hands a sitting: **what is a
finished recovery, measurably**, so that the cluster can end.

## What stands open, measured (F2, F3)

On the round's compiler, which `00-facts.md` calls R (sha256 prefix
`b578e9a799ab7469`) and which **is the trunk's compiler at `62d65e48`** (the
critic, § 0: the same seed, no compiler file changed since `d4fd12ef`;
`00-facts.md`'s "T, the trunk's compiler" is `4b44f684`'s, no longer the
trunk's):

| | rows open | (a) a second message for one mistake | (b) a mistake told only once another is fixed | (c) a false message | (d) a `certain` fix that means something else or is refused anew | (e) exit 2 or a crash | (f) none of these |
|---|---|---|---|---|---|---|---|
| 130, audit rows | 1 | 0 | 1 (`130-34a`, by the audit's reading) | 0 | 0 | 0 | 0 (or 1, by the lane's reading) |
| 131, audit rows | 15 | 8 | 5 | 0 | 0 | 0 | 2 |
| beside 130, six shapes | 6 | 1 | 2 | 0 (one admits a (c) reading, below) | 0 | 0 | 3 |

Every run ended at exit 0 or 1 (F3, the closing paragraph). By the author's
classes of 2026-10-02, (a) and (b) are `adjacent` content: no wrong value,
no false message, no wrong certain fix, no crash. They are `systemic` by
their history, not by their kind.

**One shape admits a (c) reading** (the critic, § 4): `g2/r09`, a
`function` without a body inside a `record`, gets `expected_field` at the
`function` and then `missing_body`, *"a `function` needs an indented body
... (found the end of the block)"*, its excerpt and caret under the next
line, `function main()`, which has a body; it names no function. Read where
it points, it says `main` lacks a body. A false message is `blocking`, never
deferred. Q4 asks which it is. The blind seat's `p3` is this shape.

**Defect 166** (`adjacent`): its cause is `parse/opening.hero`'s
`drop_line`, which stops a failed arm's recovery at the first word of a line
of the text inside a line the lexer joined (the stop defect 131 added at
`9811d4cd`). On `62d65e48` **seven** probes read `expected_pattern` twice at
one place (item 166's own title shapes, a `-` over a name or a float, and a
`-` over a group, besides four more); on lane h158's compiler (`8cb4ba6c`,
merged with the round) **four** do, the four item 166 names (the critic,
§ 1.2; `docs/work/DEFECTS.md`, item 166).

**Item 130's text**: commit `8349d264` (18:39) cut its title line and seven
lines of its body, and every tree since carried the damage; the round
restores it byte for byte from `d4fd12ef` (the critic, § 2.1; the round's
commit `50644159`). On `07ccb72a` the item is whole.

## The recovery instrument (F4)

One plausible mistake at a time planted in 641 programs (63,631 lines), 96
operators, 13,594 mutants, each run under `check --brief` and
`--permissive`, then `--apply`. On lane recovery-b6's gate run and lane
recovery-b8's two runs alike: **ONE 12,838** (one message, on a site line),
**EXTRA 439** (150 of them `brace-else-chain`, which ruling 4 says is not a
defect; 52 `c-for`, the shape of 131's rows 33a, 54a, 54b), **SILENT 23**
(`forget-f` 17, which panel 185's R7 decided on 2026-10-02; `over-indent`
6), **ELSEWHERE 14**, **HIDDEN parse-stage 4 of 14,684** pairs (each with
`bracket-open` first). **Not alike: its class (d) flags**, APPLY-OTHER (*a
certain fix that compiles and means something else*) 0 on recovery-b6's gate
and 20 on both of recovery-b8's runs, APPLY-NEW 0 and 19, every one operator
`int` (§ Measured on the sitting's head).

Four facts about the instrument a route that leans on it must answer:
- **It is not in the repository.** It is a Python tool in the scratchpad
  (`<scratchpad>/instrument/tool/recovery.py` and `judge.py`), which a reboot
  empties; CLAUDE.md § 10 says *never a script*. The tree's own mutation
  tool is `heroes mutate` (`selfhost/cli/mutate.hero` and six modules under
  `selfhost/mutate/`, design.md Part 11's metric 3), which counts killed and
  survived, not ONE, EXTRA, ELSEWHERE or HIDDEN, and which no suite runs.
- **Its corpus is a snapshot**: the trunk's tree at `c85bccb8` (2026-09-30),
  planted from a fixed lexer so successive runs plant the same mutants. A
  suite over the live tree plants different mutants as the tree changes, so
  a ratchet on raw totals would compare different corpora.
- **Its cost**: 13.8, 14.5 and 15.8 minutes of wall at `--jobs 3` for the
  three runs (`run.json`), beside a full net of 13 to 20.
- **Its run on the sitting's compiler** is § Measured on the sitting's head;
  none existed when the briefs were first written.

## The rulings in force (F5)

The seven rulings of 2026-10-01
(`docs/records/log/2026-10-01-0026-seven-rulings-the-recovery-batches-applied-written-where-they-can-be-read.md`),
which that entry records as the coordinator's, open to the author's
reversal, with no panel for 1 to 5 and 7 (its line 19); ruling 6 went to
panel 184, which that entry says *"waits on the author"*: true when written,
and panel 184 was ratified on 2026-10-01 at 22:24 and panel 185 on
2026-10-02 at 03:10 (`docs/records/done/`). Panel 183's resolution and
verdict. And design.md §4.17 (lines 2079 to 2145), which says three things
this sitting turns on, quoted in Q5. `grep -n -i` over design.md for `one
message`, `second message` and `cascad` finds nothing (F5.3); the spec says
nothing of recovery (F5.4). The instrument's docstring calls its rule
§4.17's (F4).

## The questions

**Q1. The definition of done for the parser's recovery.** Routes the
coordinator and the critic can name, none built:
- **(1a)** every audit row closed, today's reading, which seven batches have
  not reached;
- **(1b)** the instrument's totals held by a suite in the net, ratcheted as
  they improve (ONE, EXTRA, SILENT, HIDDEN parse-stage, and the APPLY
  flags), with no row of class (c), (d) or (e) ever standing, and every
  other open row filed `adjacent` apart, one item per cause. It must answer
  the four facts of § The recovery instrument: where the instrument would
  live in the tree (`heroes mutate`'s recovery arm is one home), which
  corpus it plants into (a frozen one, or rates), its cost in the net, and
  its baseline;
- **(1c)** the classes alone: (a) and (b) are `adjacent` by kind and the
  cluster's items close as such. An `adjacent` item *"becomes `blocking`
  when two milestone tags have been placed since it was filed"*
  (`.claude/rules/verification.md` § Bounded discovery), so (1c) refiles the
  rows with a clock;
- **(1d)** a structural change to the recovery that closes classes (a) and
  (b) wholesale (for instance one resynchronisation rule per statement, or
  the parser told where the lexer joined lines, which is 166's cause), its
  cost the compiler-engineer's to measure;
- **(1f)** what stays is pinned as a known cost: every audit row either
  closed or pinned in a golden with its reason, so a change in either
  direction is seen; the tree holds the shape already,
  `tests/golden/check/panel-183-a-statement-inside-a-bracket-closed-below-is-the-rules-known-cost.hero`
  (panel 183's R3);
- **(1g)** §4.17's own measure: done as a count of exchanges, a mechanical
  loop that applies what the first message says and re-checks, over the
  audit rows or the instrument's pairs, or as the blind seat's one-turn rate;
- **(1h)** the seven rulings adopted or reopened by this sitting: a
  definition that rests on ruling 4 (150 of EXTRA's 439 are not a defect)
  inherits a ruling nobody ratified;
- **(1e)** a route nobody listed.

**Q2. Row 130-34a** (F6): `x = [1, 2` over `print(x) )` inside a function.
The audit's reading: two mistakes, the missing `]` told only once the `)` is
deleted. Lane recovery-b8's: one mistake, a closer of another kind, told once
in words that do not name the `[`. Which is the program's author's, and what
is owed. The row's program is wrong under both readings (F6, re-run by the
critic: (A)'s two edits leave `bad_operand@3:5`, `print` of an array; (B)'s
one leaves `unknown_name@3:11`), so the case alone cannot settle whose
reading is the author's; the blind seat's `p4` gives an intent
(`x.len()`) that can. A route not listed before: one message reworded, as
panel 183's route (b) did for the opener, naming the `[` and both edits so
the reader chooses.

**Q3. Defect 166's cause**: should the parser know where the lexer joined
lines, so that a failed line's recovery drops through a joined line; is that
one change of the recovery's architecture or a local repair. **A third
route, measured by the critic** in its own copy of `62d65e48` (§ 4, Q3): one
condition of `drop_line` made never true (`parse/opening.hero` line 310,
`if c.pos != from && ...`) and the compiler built from those sources: all
seven doubled probes read one `expected_pattern`; of 147 files nothing else
moves; the `check` golden form reads 406 passed, 0 failed; the compiler's own
tests 1036 passed; `fixedbugs-131-a-body-in-a-tab-margin.hero`, the golden
the stop came with, prints the same 41 errors with and without it. Unrun: the
full net, the census, the instrument, the formatter's probe, the platforms.
A question, not a premise: the compiler-engineer's to confirm or refute.

**Q4. What becomes of what stays open**: under the route Q1 adopts, which of
131's fifteen rows and the six shapes are filed `adjacent` (and grouped how),
which are repaired first, whether items 130 and 131 close, whether 166 joins
the cluster's filing, and **whether `g2/r09`'s second message is false**
(`blocking`, repaired at once) or true and less exact than it could be
(`adjacent`).

**Q5. What the contract says, and where it is written.** design.md §4.17
says, of one error: it *"carries all the context needed to fix it"* (line
2090); *"The model fixes it in one turn, without opening anything"* (lines
2111 and 2112); and its measure is to *"count the number of exchanges needed
to make a broken program compile, before and after"* (lines 2126 and 2127).
The reading *"a model fixes the program in one turn"* is lane recovery-b2's
paraphrase (`lane-recovery-b2-items.md` line 5), which the instrument's
docstring and the seven rulings repeat as §4.17's. Read per error, a hidden
mistake (class (b)) breaks nothing at line 2111 and adds an exchange to the
measure at 2126; read per program, it breaks the promise. **Which reading
does the contract hold**, and is a second message (class (a)) a broken
promise only when its context or its fix misleads (line 2090)? **And where
is a finished recovery written**: design.md §4.17 is a diagnostic class, a
panel's to change (CLAUDE.md § 4); `.claude/rules/verification.md` §
Bounded discovery or `.claude/rules/diagnostics-and-goldens.md` are
process, which the author amends with no panel. Where it is written decides
who may change it; which sentence of the spec, if any, changes.

## Measured on the sitting's head

On `07ccb72a`: the round of 2026-10-02's third gate, `6bec7c8c`, and three
records commits after it that move no compiler, spec, design or test file
(`git diff --stat 6bec7c8c 07ccb72a -- selfhost/ seed/ runtime/ spec/
docs/design/ tests/` is empty). Measured by the coordinator on 2026-10-02:

- **The compiler** built from the seed: sha256 prefix `60cc49e98b56ddc0`; the
  seed's own is `568bce290b6ed3bb` (38,648,442 bytes), its fixpoint by `cmp`
  at the gate. Build yours and compare.
- **The rows**: the critic's re-run script with its compilers set to
  `62d65e48`'s and this head's (`<scratchpad>/187-head/rerun-head.py`), over
  its 147 files in 71 groups, one process at a time, 21:37:24 to 21:37:35 by
  `date` (`<scratchpad>/187-head/rerun-head.txt`): **132 read the same on
  both; the 15 that move are all probes of 165 and 166**, as the critic read
  them on lane h158's compiler (its report, § 1.2). So F2's sixteen open audit
  rows, F3's table, the six shapes (`g2/r09` among them), row 130-34a and the
  five blind programs read on this head as on `62d65e48`.
- **Defect 166 on this head**: its four shapes (`n1`, `n2`, `n3`, `n9`) still
  read `expected_pattern` twice at one place; its title shapes (a `-` over a
  name, over a float) and `n8` read it once.
- **The instrument** on this head's compiler (`<scratchpad>/inst-187/round3/`,
  21:55:28 to 22:08:58 by `date`, lane recovery-b6's gate plan, 13,594
  mutants, 0 errors): ONE 12,838, EXTRA 439, ELSEWHERE 14, SILENT 23, LEGAL
  280, identical to lane recovery-b8's runs; **HIDDEN parse-stage 0 of 14,684
  pairs**, where lane recovery-b8's r2 read 4, all with `bracket-open` first
  (that run predates b8's last three repairs, which the trunk now holds);
  later-stage hidden 1,038, by design; APPLY-OTHER 20 and APPLY-NEW 19, all
  operator `int`, filed as defect 172 (below).
- **Closed**: defects 159 and 165 (`2caca562`). **Restored**: item 130
  (`50644159`; what is known of the cut,
  `docs/records/log/2026-10-02-2141-item-130-cut-in-8349d264-and-restored-what-cut-it-is-unknown-the-commit-did-not-read-its-diff.md`).
- **Filed beside the sitting** (`07ccb72a`): defects 167 to 190, from the
  coordinator's queue and the round's open questions, ten `blocking`. **Six
  are of the cluster's kind**, each `adjacent`, and Q4 covers them: 177 (a
  `match` whose arms fall inside a bracket left open, one extra message per
  arm), 178 (a separator habit told once per line), 179 (`@return 1`, two
  messages), 180 (`1..2`, told twice), 181 (`1.5.2`, told as a field access),
  182 (an operator alone on a line deeper than the arms, two messages). One
  `blocking` touches the instrument's class (d): 172, the `certain` fix `i32`
  for `int` in an `extern` group, which `check` cannot know and `build`
  refuses where the header's type is 64 bits; a route that counts class (d)
  counts it.
- **The spec**: 432 lines; § 13 changed by panel 186's R7 (`git diff --stat
  62d65e48 6bec7c8c -- spec/`, 5 lines in and 5 out). design.md §4.17 is at
  lines 2079 to 2145 as before; the round's 22 lines of design.md are in
  §4.19.
- **F1's commit counts**: `git log --oneline --grep='^Defect 130' 07ccb72a`
  lists 30 commits, where F1 read 21 on the trunk before the evening's merges
  (the merges of lane recovery-b8 and the restore `50644159` add nine); the
  same for `^Defect 131` lists 21.
- **Positions** in `docs/work/DEFECTS.md` moved with the filings: read the
  items by number.

## The frozen tree and your copy

The trunk is frozen at `07ccb72a` from the seats' launch to the synthesis.
`<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
Your copy is `<scratchpad>/187-<seat>/`, made with `git archive 07ccb72a | tar
-x`, your compiler built inside it from the seed (at `07ccb72a` the seed is the
fixpoint of `selfhost/`; its sha256 prefix is in § Measured on the
sitting's head; check yours). The audit and its cases are
`<scratchpad>/audit-130-133/` (read only; copy what you need), lane
recovery-b8's re-run tooling `<scratchpad>/lane-recovery-b8/audit/`, the
critic's re-run script `<scratchpad>/187-critic/rerun-critic.py` (copy it),
the instrument `<scratchpad>/instrument/` (do not run it yourself: ask the
coordinator, with its size). **A copy made by `git archive` has no `.git`**:
to check F1's commit facts you may read the trunk's history,
`git -C /Users/joseph/Temp/heroes/heroes-lang log` and `show` of commits at or
before `07ccb72a`, and nothing else inside the trunk except writing your own
report. Never build, run or read inside another seat's copy or a lane's
worktree. **No paid run** beyond the five sessions the llm-ergonomist's
brief names; no seat runs `heroes measure --refresh` (an API call): the
spec-warden prices on the vendored tables and the coordinator runs one
refresh on a drafted spec if the resolution adds spec text. At most three
processes at once, no timing. Your report is
`docs/panel/187-reports/<seat>.md` in the TRUNK, written as you go: a verdict
per route, what you built and ran, your cost, a falsifiable prediction, the
condition that would change your verdict. English, no em dashes.
