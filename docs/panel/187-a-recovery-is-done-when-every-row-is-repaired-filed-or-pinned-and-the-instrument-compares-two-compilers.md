# Panel 187: a recovery is done when every row is repaired, filed or pinned, and the instrument compares two compilers

2026-10-02 to 2026-10-03, a full panel: `compiler-engineer`, `spec-warden`,
`historian`, `llm-ergonomist` (five blind sessions, one per program, run
outside the repository), and the completeness critic over the briefs first
(from 20:18) and over the reports after (to 00:13). The seats sat on the
trunk frozen at `07ccb72a` from 22:13; the briefs were written on `62d65e48`,
repaired after the critic's first pass and re-based on `07ccb72a` before any
seat was launched (each repaired brief keeps the text the critic read as
`<name>-before-the-critic.md`). The briefs are `docs/panel/187-briefs/`, the
reports `docs/panel/187-reports/`. No `ffi-pragmatist`: the question is the
parser's, not the C boundary's.

## The proposal

Defects 130 and 131, the parser's recovery, were `systemic` by the author's
rule of 2026-10-02 (*D1a D2a D3a*): a defect open after three batches goes to a
sitting. 130 took seven batches and 131 four, each first pass adding shapes.
The question the rule hands a sitting: **what is a finished recovery,
measurably**, so that the cluster can end. Five questions
(`187-briefs/00-shared.md`): Q1, the definition of done, routes (1a) to (1h);
Q2, row 130-34a, `x = [1, 2` over `print(x) )`; Q3, defect 166's cause,
`parse/opening.hero`'s `drop_line`; Q4, what becomes of what stays open, and
whether `g2/r09`'s second message is false; Q5, what design.md §4.17 promises
and where a finished recovery is written.

## The verdict table

| seat | verdict | cost or delta | prediction | condition |
|---|---|---|---|---|
| compiler-engineer | Q3's local route **V1, `drop_line` deleted: approve, built and gated**; (1f) **approve, built** (ten pins); (1e) the parser's line budget **approve, built**; (1c) approve with (1f); Q2's reworded message approve, unbuilt; V5 (the missing body names its function) approve, built; (1a), (1b), (1d), (1g) object; V2 object; `g2/r09` read as false object; no veto | V1: `parse/opening.hero` 300 to 288; the pins 233 lines of cases; the budget +69 lines of harness; V5 12 lines, 24 goldens in words only | `selfhost/parse/` at most 8,696 lines at the next `m-*` tag with the budget, above 9,000 without; V1's instrument totals within 3 mutants (failed, in V1's favour: EXTRA moved by 4) | Q3 changes if V1 hides a parse-stage pair or raises EXTRA (it did neither); (1e) withdrawn if read as a cap a `blocking` repair cannot pass |
| spec-warden | **no spec text, delta 0, measured**; §4.17 per message for the promise, per program for the measure; the promise in design.md §4.17 (D1), the closing in `.claude/rules/verification.md` (D2); (1b) approve on four conditions; (1f) approve; (1h) adopt rulings 1, 3, 4, 5, 7; Q3 V1 approve; Q2 the reword approve; **veto** S1a, S1b (spec sentences on how a mistake is told) and S3 (Q2's spec change); `g2/r09` `adjacent` | 6923 vendored, 9164 real, 432 lines, unchanged | the spec stays at 6923 and 9164 through the landing; a blind reading of rows 131-32 and `g2/r12` repairs the hidden mistake in at least 8 of 10 sessions (paid, unrun) | Q5 changes if Part 11's metric 4 runs and measures a hidden parse-stage mistake costing a turn |
| historian | approve (advisory) a composite: (1f) as the definition, (1d) as the repair, (1b) **in a differential form** as the instrument, (1c) as the filing, (1g) as the measure; object to (1a) and to a ratchet on raw totals; (1h) approve | | P1: a Go-shaped per-line rule moves EXTRA below 439 and HIDDEN parse-stage above 0 (cannot fail on this instrument, the critic); P2: under (1c) without (1d) an `adjacent` item of the cluster turns `blocking` at the second tag | (1b): a production compiler whose CI gates on a recovery total for years (searched, none found) |
| llm-ergonomist, five sessions (22:13 to 22:15, 0.851 USD) | **five one-turn repairs of five**, each correction built by the coordinator on the sitting's compiler and printing what it should | | p1 75 to 90, p2 85, p3 90, p4 85, p5 97 of 100 | p4: *a note like "list opened at 2:9 is not closed" would have made the repair near certain* |

## What the sitting measured

- **The rows on the sitting's head** (`00-shared.md` § Measured on the
  sitting's head): of the critic's 147 files, 132 read the same on `62d65e48`
  and `07ccb72a`; the 15 that move are 165's and 166's probes. F2's sixteen
  open audit rows stand: 131's fifteen (8 of class (a), 5 of (b), 2 of (f))
  and 130-34a, beside the six shapes.
- **The instrument on the head** (13,594 mutants, 0 errors): ONE 12,838,
  EXTRA 439, ELSEWHERE 14, SILENT 23, LEGAL 280; **HIDDEN parse-stage 0 of
  14,684**, where lane recovery-b8's r2 read 4; APPLY-OTHER 20 and APPLY-NEW
  19, every one operator `int`, filed as defect 172 (`blocking`).
- **V1 on the instrument** (16.7 minutes, 0 errors): normal arm EXTRA 439 to
  435 and ONE 12,838 to 12,842; control arm EXTRA 424 to 414; HIDDEN
  parse-stage 0 and 0; APPLY flags unchanged. Mutant by mutant (the critic):
  28 singles and 72 pairs get fewer messages, none more, and no pair's
  second changes its told status.
- **V1 through the gate** (the compiler-engineer, in its own copy): the full
  net 4,752 passed, 0 failed over 26 suites (the nine suites after `records`
  run by name, `records` in a copy with a bare `git init`, since a `git
  archive` copy has no `.git` and the harness stops at the first suite that
  cannot run); the compiler's own tests 1,056; the net's own 197; the seed's
  fixpoint by `cmp`; the census 0 of 1,834 files moved in either arm,
  reproduced by the critic.
- **The front end's growth** (reproduced by the critic): 8,407 lines in 40
  modules at `0338b598` to 19,201 in 109 at `07ccb72a`, 68 new files; the
  defect 130 to 133 commits +11,749 lines.
- **The blind readings**: five programs of 3 to 8 lines, five one-turn
  repairs; the two class (b) programs are 3 and 5 lines and `p2`'s hidden
  `)` sits on the message's line.
- **`g2/r09`'s placement** (both seats, the critic reproducing): of 109
  `missing_body` lines in the goldens, 47 begin *"a `function` needs"* and 15
  of those are told at the next declaration's first word, in 8 cases.

## Disagreements, unsmoothed

- **(1b), the instrument as a gate.** The compiler-engineer objects (3,618
  lines of Python outside the tree; a ratchet rewards adding recovery code);
  the spec-warden approves on four conditions (in the tree, one corpus,
  counters split between the promise and the counted costs, ruling 4 settled);
  the historian approves a differential and objects to a ratchet on totals
  (*counts on a corpus only allow relative comparisons*, Diekmann and Tratt
  2020; TypeScript compares a PR against main on one corpus). **What a
  command settled** (the critic): the *rewards adding code* claim is refuted
  by V1, a deletion every ratchet accepts; the record shows totals moving
  between batches with unrelated work (EXTRA 1,326 to 1,336, ONE 12,448 to
  12,427), so *may not rise* would have gone red twice; and no pair of the
  15,800 puts both mistakes on one line, so a per-line suppression would
  score as a 41 per cent cut of EXTRA (439 to 257) while, on the audit rows,
  it keeps the wrong message on four, hides 55b's stray `)` and locks five of
  the eight class (b) rows. **What no command settles**: whether a rise blocks
  or is read.
- **(1d).** The compiler-engineer judged *the parser told where the lexer
  joined lines* and found it built since panel 181 (`parse/apart.hero`); the
  historian judged a Go-shaped per-line rule. Nobody built the historian's.
- **(1g).** Measured: the certified loop moves 2 of the 16 open rows; the
  blind loop 5 of 5 small programs. Whether a measure with no baseline
  (Part 11's metric 4 has never run) can define done is policy.
- **The spec-warden's drafted texts hold the ratchet the other two seats
  refuse** (the critic, § 4.5): D1's *held no higher than its last count* and
  D2's *may not rise*.
- **The per-program reading of §4.17**: the briefs and the critic's first
  pass called it lane recovery-b2's paraphrase; it is CLAUDE.md § 8's own
  wording since `113b1019` (2026-08-03), which the lanes made a criterion
  (the spec-warden, reproduced by the critic with `git log -S`).

## The resolution: `provisional, author ratification pending`

**R1. Done, measurably (Q1)**: a recovery item closes when **each of its rows
is repaired, filed `adjacent` apart one item per cause with its reproducer, or
pinned as a known cost** in a `tests/golden/check/` case whose header gives
its reason and the ruling it rests on (routes (1f) and (1c)); never a row of
class (c), (d) or (e), which is `blocking` by the author's classes. The
compiler-engineer's ten pins land with it (`panel-187-*`, built and green in
its tree: `check` 426, `annotations` 585, `fixes` 681, each 0 failed), each
cause of its § 1 filed as one `adjacent` item carrying its pin (A1 to A4 of
class (a), B1 to B4 of class (b), C1 repaired by R5's V5), and **items 130
and 131 close into them**. A pinned row is not an open item; its golden
moving, either way, is read at the gate. Approved by all three seats that
judged it; the historian's precedent is rustc's `known-bug`, *"a sentinel
that will fail if the bug is incidentally fixed"*.

**R2. The instrument reads two compilers, never a total (Q1's (1b))**: at
every round whose lanes touch the recovery's files, the coordinator runs the
instrument on the round's compiler and on the trunk's over one frozen plan
(today lane recovery-b6's gate plan over the snapshot at `c85bccb8`), and the
round's closing commit names every moved mutant. **A mutant with more
messages, a parse-stage second newly hidden, or any APPLY flag is a finding,
filed by its class** (an APPLY flag on a `certain` fix is `blocking`, defect
172's class; ruling 7 reads a later stage's first message after a correct fix
as that stage speaking, never a fix that wrote the wrong program, which is
172); fewer messages and none worse is the round's evidence, as V1's run
was. No ratchet on totals: the record and the critic's measurements say what
one would have done. **Its home is the tree** (CLAUDE.md § 10, *never a
script*): the port to `heroes mutate`'s recovery arm, with its plan pinned, is
filed as an item (the spec-warden's first condition, which it calls the
route's repair and not an option); until it lands the scratchpad tool is the
gate's, and a reboot that empties it is recorded as the gate's loss.
*Conservative*: the instrument as an occasional observation, no gate.

**R3. What the measure cannot see is written beside it** (the critic's
question, *what can the measure we make the definition of done not see, and
what would a gate on it reward*): the instrument plants no pair whose two
mistakes share a line and no mistake inside a failed arm's body; no golden
form runs the control arm (`--permissive`), so a control-arm row cannot be
pinned; class (b) has no measurement at the project's sizes (the blind set's
are 3 and 5 lines; panel 183's 20 of 20 were 31 to 105). A rule that
suppresses messages per line or per statement is judged on the audit rows
and the pins, never on the instrument's totals alone.

**R4. Defect 166's cause (Q3): V1 lands, `drop_line` deleted**, the
compiler-engineer's build, gated in its copy as a round's gate would be (the
full net, the census, the instrument above): the parser has been told where
the lexer joined lines since panel 181, and the stop was the one place that
split a joined line, for a tab-margin premise ruling 5 removed. 166 closes
with V1 and its golden (`fixedbugs-166-a-line-joined-under-a-failed-arm-is-dropped-with-it`).
The five true control-arm messages V1 stops telling on the
compiler-engineer's probes (`h01`, `h05`, `h14`, `h16`, `h17`: a second
mistake in the body of a joined arm whose pattern failed, hidden in its
one-line form by every compiler) are filed apart, `adjacent`. V2, the stop
narrowed to a tab margin, is refused: it keeps one debris message on two
probes.

**R5. `g2/r09` is true and `adjacent` (Q4)**: the place is the convention of
every missing body, blessed in 8 goldens, and the critic withdraws its first
pass's (c) reading; read as false, 15 golden lines would be false too. **V5
lands**: the message names the function whose body is missing (*the
`function` `f` needs an indented body ...*), 12 lines in `parse/heads.hero`
and `parse/tails.hero`, 24 goldens moving in words only, codes and places
standing. Defects 177 to 182, filed beside the sitting as the cluster's kind
and run by no seat, are measured by the landing and each repaired, pinned or
left filed under R1.

**R6. Row 130-34a (Q2)**: the grammar's reading is (B) (spec lines 11 to 15,
210 and 214: line 2 keeps its NEWLINE, `print(x)` is a third element, the `)`
a closer of another kind), and S3, the spec change that would make (A) the
compiler's reading, is vetoed (the spec-warden: it refuses a program the
spec accepts). **The message is reworded to name the `[` left open, at its
line and column, and both edits**, so the reader chooses: one message true
under both readings (`parse/list_line.hero:223-228`, whose `separator`
already takes the opener's position). Pinned meanwhile by
`panel-187-a-closer-of-another-kind-inside-a-list-is-one-message`, the two
readings in its header.

**R7. What §4.17 promises, and where it is written (Q5)**: the promise is
**per message**, *"The model fixes it in one turn"* (2111), *it* an error
that *"carries all the context needed to fix it"* (2090); the measure is per
program and comparative, *"count the number of exchanges"* (2126), Part 11's
metric 4, never run. So a hidden mistake (class (b)) and a second message
(class (a)) are costs to count, and a false message or a `certain` fix that
writes another meaning breaks the promise. **design.md §4.17 gains the
spec-warden's D1 with its ratchet clause replaced by R2's differential
reading** (the counted costs *"read at each recovery round's gate against the
trunk's compiler over one frozen corpus, a moved one being the finding"*);
`.claude/rules/verification.md` § Bounded discovery gains D2 with the same
replacement, R1's closing rule, which is the author's to adopt (process,
CLAUDE.md § 4); and CLAUDE.md § 8 and `.claude/rules/diagnostics-and-goldens.md`
lines 19 to 20 read *"to fix the mistake it names"* where they read *"to fix
the program"*, +2 vendored, the author's. **No spec sentence**: the spec
says nothing of how a mistake is told, and Principle 0 asks for none (the
spec-warden's veto on S1a and S1b stands). *Robust beyond it, recorded and not
taken*: D1 with *no mistake of the parse stage is told only once another is
fixed* moved into the promise, blocking; it would make the hand-found (b)
rows `blocking` at once on a premise no instrument has scored (the
spec-warden), and enters through D1's last clause the day metric 4 measures a
hidden mistake costing a turn.

**R8. The seven rulings of 2026-10-01 (1h)**: rulings **1, 3, 4, 5 and 7 are
adopted** by this sitting (the spec-warden; the historian approves; each of
the compiler-engineer's pins names the ruling it rests on); **ruling 2 is
adopted as landed** (`record {` with no name told once; the audit's rows B2a
and B2b are closed), which no seat named; **ruling 6 is superseded** by
panels 184 and 185, ratified (a statement after a jump refused, 184's R4;
the forgotten `f`, 185's R7).

**R9. The parser's line budget (the compiler-engineer's (1e))**:
`tests/harness/suite_layout.hero` holds `selfhost/parse/` to a budget, the
head's own total, 8,696 lines in the layout unit (`layout/budget`, built,
red one line under), so the front end that grew from 8,407 to 19,201 lines
in four days grows by decision: an `adjacent` repair pays for its lines by
deleting or moving others, and **a `blocking` repair that cannot raises the
row with its reason**, as a `DECIDED` row is raised, never refused for it
(robustness outranks compiler size, CLAUDE.md § Precedence). Judged by one
seat; a process rule, the author's to adopt with the rest of R7's process
half. Its premise, written in the constant's comment: a module moved out of
`parse/` escapes it.

**R10. Landing and filings**: one recovery lane after this sitting, in this
order: V1 (R4), the ten pins and the causes filed (R1), V5 (R5), 130-34a's
message (R6), 177 to 182 measured (R5), the budget if adopted (R9); items 130,
131 and 166 close at its gate; the instrument run at that gate as R2 says.
Filed beside the sitting: the instrument's port to `heroes mutate` (R2); the
five control-arm messages (R4); a golden form for the control arm (R3); the
class (b) reading at the project's sizes (the spec-warden's named run, ten
sessions, about 4 to 5 USD, and the 300-line form panel 183's critic left
owed), as an item the author may fund.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | `selfhost/parse/` at most 8,696 lines (its unit) at the next `m-*` tag if R9 is adopted, above 9,000 if not | the tag's commit, `code_lines.py selfhost/parse/*.hero` |
| compiler-engineer | V1's instrument totals within 3 mutants of the head's | **scored, failed in V1's favour**: EXTRA moved by 4, every move fewer messages |
| spec-warden | the spec stays at 6923 vendored and 9164 real through the landing | the landing's gate, `heroes measure` |
| spec-warden | a blind reading of rows 131-32 and `g2/r12` repairs the hidden mistake in at least 8 of 10 sessions | R10's named run, if funded |
| historian | P2: under (1c) without (1d), at least one of the cluster's `adjacent` items is open at the second tag after its filing and counts `blocking` | `records/tagged` at that tag |
| llm-ergonomist | one-turn rates p1 75 to 90, p2 85, p3 90, p4 85, p5 97 of 100 | a run of 100 per program (unfunded) |

## The critic's two passes

**First, over the briefs** (`187-reports/completeness-critic-briefs.md`):
the trunk's compiler was the round's (R), not `4b44f684`'s; item 130's text
had been cut in `8349d264` (restored at `50644159`, its cause recorded in
`docs/records/log/2026-10-02-2141-...`); positions, the spec's length and
F1's commit counts had moved; three routes nobody listed ((1f), (1g), (1h)),
Q2's reworded message and Q3's local route measured on a scratch build;
Q5's premise; `g2/r09`'s (c) reading put as a question; the blind set's `p3`
a second control, its `reading` heading priming, its five programs in one
session; the historian's precedents handed as facts. Every item was
repaired before the seats launched; one was not (no class (b) program whose
hidden mistake sits off the message's line), and one stale file remained,
`blind/p3.messages.txt`, now the new `p3`'s.

**Second, over the reports** (`187-reports/completeness-critic.md`): it
re-ran the central claims of every seat and they held; it settled what a
command could of the (1b), (1d), (1g) and `g2/r09` disagreements; it named
the contradiction no seat named (the drafted ratchet), the measure's blind
spots (R3), and the question the sitting should have asked; and it
corrected its own first pass twice (the per-program paraphrase's origin;
the `g2/r09` reading).

## Author's verdict

Pending: `docs/work/DECIDE.md`, `panel 187`.
