# Panel 187, the shared brief: what a finished recovery is

Written 2026-10-02 by the coordinator, on the trunk at `62d65e48`. Every
number below names the command or the file that produced it; the measured
facts, each with its command, are `00-facts.md` in this directory (gathered
by an agent of the coordinator's on the round gate's compiler, the seed
regenerated at 17:18, its section numbers cited below as F1 to F6). What was
not run says so.

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

On the round gate's compiler (it reads as lane recovery-b8's final
compiler on all 89 audit files, `00-facts.md` § Conventions):

| | rows open | (a) a second message for one mistake | (b) a mistake told only once another is fixed | (c) a false message | (d) a `certain` fix that means something else or is refused anew | (e) exit 2 or a crash | (f) none of these |
|---|---|---|---|---|---|---|---|
| 130, audit rows | 1 | 0 | 1 (`130-34a`, by the audit's reading) | 0 | 0 | 0 | 0 (or 1, by the lane's reading) |
| 131, audit rows | 15 | 8 | 5 | 0 | 0 | 0 | 2 |
| beside 130, six shapes | 6 | 1 | 2 | 0 | 0 | 0 | 3 |

Every run ended at exit 0 or 1 (F3, the closing paragraph). By the author's
classes of 2026-10-02, (a) and (b) are `adjacent` content: no wrong value,
no false message, no wrong certain fix, no crash. They are `systemic` by
their history, not by their kind.

Beside them, defect 166 (`adjacent`): its cause is `parse/opening.hero`'s
`drop_line`, which stops a failed arm's recovery at the first word of a line
of the text inside a line the lexer joined (the stop defect 131 added at
`9811d4cd`); four shapes still read two messages at one place
(`docs/work/DEFECTS.md`, item 166).

## The recovery instrument (F4)

One plausible mistake at a time planted in 641 programs (63,631 lines), 96
operators, 13,594 mutants, each run under `check --brief` and
`--permissive`, then `--apply`. On lane recovery-b6's gate run and lane
recovery-b8's two runs alike: **ONE 12,838** (one message, on a site line),
**EXTRA 439** (150 of them `brace-else-chain`, which ruling 4 says is not a
defect; 52 `c-for`, the shape of 131's rows 33a, 54a, 54b), **SILENT 23**
(`forget-f` 17, `over-indent` 6: ruling 6 sent them to panel 184),
**ELSEWHERE 14**, **HIDDEN parse-stage 4 of 14,684** pairs (each with
`bracket-open` first). **No instrument run exists on the round's compiler,
nor on lane recovery-b8's last three repairs** (F4, its last bullet): a
sitting that wants the instrument's numbers on today's compiler asks for a
run (about 15 minutes at `--jobs 3`, F4's command), and the coordinator runs
it.

## The rulings in force (F5)

The seven rulings of 2026-10-01
(`docs/records/log/2026-10-01-0026-seven-rulings-the-recovery-batches-applied-written-where-they-can-be-read.md`),
panel 183's resolution and verdict, and design.md §4.17, which promises an
error the model fixes *in one turn* (`docs/design/design.md` around line
2112) and **never says "one mistake, one message"** (`grep -n -i` design.md
for `one message`, `second message`, `cascad`: 0 hits, F5.3). The
instrument's own docstring calls its rule §4.17's (F4); the spec says nothing
of recovery (F5.4).

## The questions

**Q1. The definition of done for the parser's recovery.** Routes the
coordinator can name, none built: **(1a)** every audit row closed, today's
reading, which seven batches have not reached; **(1b)** the instrument's
totals held by a suite in the net at today's values or better, ratcheted as
they improve (ONE, EXTRA, SILENT, HIDDEN parse-stage), with no row of class
(c), (d) or (e) ever standing, and every other open row filed `adjacent`
apart, one item per cause; **(1c)** the classes alone: (a) and (b) are
`adjacent` by kind and the cluster's items close as such; **(1d)** a
structural change to the recovery that closes classes (a) and (b) wholesale
(for instance one resynchronisation rule per statement, or the parser told
where the lexer joined lines, which is 166's cause), its cost the
compiler-engineer's to measure; **(1e)** a route nobody listed.

**Q2. Row 130-34a** (F6): `x = [1, 2` over `print(x) )` inside a function.
The audit's reading: two mistakes, the missing `]` told only once the `)` is
deleted. Lane recovery-b8's: one mistake, a closer of another kind, told once
in words that do not name the `[`. Which is the program's author's, and what
is owed.

**Q3. Defect 166's cause**: should the parser know where the lexer joined
lines, so that a failed line's recovery drops through a joined line; is that
one change of the recovery's architecture or a local repair.

**Q4. What becomes of what stays open**: under the route Q1 adopts, which of
131's fifteen rows and the six shapes are filed `adjacent` (and grouped how),
which are repaired first, and whether items 130 and 131 close.

**Q5. Where the contract is written**: design.md §4.17 promises one turn; is
a hidden mistake (class (b)) a broken promise and a second message (class
(a)) not; which sentence of design.md, and of the spec if any, says what a
finished recovery is.

## The frozen tree and your copy

The trunk is frozen at its head from the seats' launch to the synthesis (`git
log -1` when you start; the coordinator names it in your launch message).
`<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
Your copy is `<scratchpad>/187-<seat>/`, made with `git archive <head> | tar
-x`, your compiler built inside it from the seed. The audit and its cases are
`<scratchpad>/audit-130-133/` (read only; copy what you need), lane
recovery-b8's re-run tooling `<scratchpad>/lane-recovery-b8/audit/`, the
instrument `<scratchpad>/instrument/` (do not run it yourself: ask the
coordinator, with its size). Never build, run or read inside another seat's
copy, the trunk or a lane's worktree. **No paid run** beyond the one the
llm-ergonomist's brief names. At most three processes at once, no timing.
Your report is `docs/panel/187-reports/<seat>.md` in the TRUNK, written as
you go: a verdict per route, what you built and ran, your cost, a
falsifiable prediction, the condition that would change your verdict.
English, no em dashes.
