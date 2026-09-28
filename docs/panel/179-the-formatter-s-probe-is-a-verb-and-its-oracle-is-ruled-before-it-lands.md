# Panel 179 — the formatter's probe is a verb, and its oracle is ruled before it lands

2026-09-27, M-agreed-retention, at trunk `fa813325` with lane g's formatter at
`83ac68c1`. **Retro-record** of a tool-surface change the author decided in
conversation on 2026-09-26, in Italian, meant as: *put it in, I am telling you
to; convene the panel meanwhile*. The sitting decides the probe's shape, cost,
judge and place in the harness, not whether it exists. Briefs:
`docs/panel/179-briefs/`. Reports: `docs/panel/179-reports/`; the historian's
and the completeness critic's were written out by the coordinator from their
final messages, the harness having refused their files, with only a header
added.

**Three seats and a critic, not five**, on CLAUDE.md § 4's *choosing only the
seats whose input differs* (CL-023): the llm-ergonomist's input is the spec
alone and the proposal changes no spec sentence (the spec-warden measured the
spec names no tool at all), and the ffi-pragmatist's is the C a binding needs,
which the proposal reaches nowhere. Written here so the author can overrule
both.

**The machine.** The Mac rebooted at 23:04 on 2026-09-26 and wiped the
scratchpad under `/private/tmp`, taking the three skeptic seats' generators
with it; every file they wrote was recovered from their transcripts into
`/Users/joseph/Temp/heroes-recovery-2026-09-26/`, which the seats read. The
load read 7 to 16 on 8 cores through the sitting (another agent's generators),
so **no seat quotes a wall clock**; the critic's costs are instructions retired
and cycles, which do not depend on load.

**Four errors in the shared brief, all found by seats.**
- *Spec § 12 names `fmt`*: false. § 12 is *Tests and holes*, and `grep -c fmt
  spec/heroes-spec.md` reads 0: the spec names no tool (spec-warden).
- *The 38 fixtures of `comments101/`*: that directory holds 30. The four
  fixture directories of the formatter defects hold 38, and with
  `paramcomment095` 41, which is what the 42 `fmt` rows cover (compiler-engineer,
  critic).
- *The generators found every silent move and refusal defects 096 to 101
  record*: false and imprecise. 097 and 098 are not formatter defects; 096, 099,
  100 and 101 were filed on 2026-09-25 from hand reproducers, the day before the
  generators existed; what the generators found are the later shapes (`d02`,
  `k01`, `s8`) recorded in the lane's fixtures and module comments (critic). The
  measured case for the probe is a different one, below.
- *886 files*: the trunk's count; lane g at `83ac68c1` has 933 under
  `selfhost`, `tests` and `examples` (compiler-engineer, confirmed by the critic).

## The proposal

From one `.hero` file the parser accepts, generate the variants the skeptic
seats generated (a comment inserted at every position, trailing and own-line,
at several indents; every bracket broken after every token with a comment; CRLF
and no-final-newline copies), run the formatter on each, and check exit 0, a
fixpoint, the same tree, and every comment keeping its owner block and its
neighbour tokens; report the first variant that fails with its text; exit 0
when every parsing variant holds, 1 when one fails, 2 when the tool cannot run.
Flag on `fmt` or subcommand: the sitting's question.

## The verdict table

| | compiler-engineer | spec-warden | historian (advisory) |
|---|---|---|---|
| **verdict** | object, to two sentences; approve the shape and cost | approve, with conditions | approve |
| **section** | §1.1, §1.7, Part 5, §3.5, Part 11 metric 3, §4.15 | §1.6 (delta 0, measured), §1.2, §3.5, §3.3 | precedent |
| **cost** | about 360 counted lines in three modules, all reuse; 0.5% of the compiler's 72,110 lines | 0 spec tokens (8861 real, 6693 vendored, measured); a flag 24 help tokens, a verb 47 to 49 | — |
| **shape** | a subcommand: its artifact is a report over generated programs, its operand a file or directory; `mutate` is the precedent | a flag on `fmt`: same input, and a verb needs a proven overload | same-input checks are formatter flags, a family-of-inputs run is a verb |
| **prediction** | `suite_layout` counts each module at or under 300 and the three at or under 420; `heroes probe walk.hero` reports 39,731 variants for two families | spec real 8861 and vendored 6693 after the landing; help 72 lines and 925 ± 3 tokens for a flag (74 and 948 to 950 for a verb); the fixtures give 7,982 generated, 6,350 parsing | none of ten formatter CLIs lists a generator of deformed inputs within twelve months; Black's `--safe` stays default-on |
| **condition** | (a) an independent owner model as a second judge, or the module doc saying the probe sees walks and not rules; (b) report a site, never the text; no whole-tree run in the net | in process, never a spawn per variant; a pinned variant-count floor; no verb without a measured overload; `--probe` refuses `--in-place` | — |

## What the sitting measured

**The port is exact.** The compiler-engineer wrote the two main deformations in
Heroes (`zz_probe179.hero`, 241 counted lines) and ran them beside the
recovered Python on `selfhost/check/walk.hero`: 10,364 comment insertions and
29,367 bracket breaks on both sides, zero difference.

**The probe finds what four repair rounds left.** Run against lane g's committed
formatter at `83ac68c1`: 12 refusals at exit 2 among the parsing variants of the
30 `comments101` fixtures (`innermost.hero` T/O/B31 not a fixpoint, T/O/B39
output that does not parse; `parens.hero` T/O/B59 and T/O/B116 a comment the
guard sees moved), and 5 of 225 parsing bracket breaks in a 1-in-100 sample of
`walk.hero`, the first a comment between `token.` and `Span` in a parameter's
type (compiler-engineer). The critic reproduced the six on `parens.hero`. This,
not the history of 096 to 101, is the measured argument for the probe.

**The counts reconcile once the families are named.** The compiler-engineer's
7,154 and the spec-warden's 7,982 over the fixtures differ by exactly the two
families only the second ran: `multi` (9 variants per file, 270) and the
parenthesis wrap (558). Neither seat's number is wrong; they count two products
(critic).

**Three judges disagree on 21 of 456 parsing variants of one 53-line file.** On
`parens.hero` the guard passes 450 variants at exit 0; the independent owner
model (`rd.py`) flags 13 of them and the all-token alignment (`rd2.py`) 18, in
three classes: a comment crossing a comma, a comment trailing a parenthesis the
formatter drops, and a remark placed by the ascending-run rule `owners.hero`
states on purpose (critic). None is an unambiguous defect. **Until those three
are ruled, a second judge is either permanently red or a copy of the first.**

**One in five variants never reaches the formatter**, 120 of 576 on
`parens.hero`, discarded because they do not parse; their diagnostics are
`expected_group_close`, `expected_args_close`, `expected_function_type`,
`expected_parameter_type` and three more. Among them is a finding: spec § 0
says a NEWLINE inside brackets *may fall between any two tokens*, and the
compiler, with design.md §4.15, plants a terminator there after a line-ending
token, so a break before an operator, a `:` or a `,` is refused (critic;
reproduced on the trunk and filed as **defect 104**).

**The cost.** A spawned `heroes fmt` of a fixture retires 38 to 46 million
instructions, 3 to 4 ms of one core by the ratio to `walk.hero`'s carried 0.3 s
(critic, load-independent); the compiler-engineer's 36 ms per judged variant was
user time at load 16 and is nine times that, cause unprofiled. The whole tree
under every family is 1.5 to 2.2 million variants by per-line rates, tens of
CPU hours (compiler-engineer, an inference); under the thin `multi` family alone
it is 9 per file, about 8,400 for 933 files (critic, arithmetic, unrun).

**The rule has two sentences that pull apart.** `.claude/rules/cli-surface.md`
says *a subcommand if it answers a different question, meaning a different
artifact class* and *a new top-level verb needs a proven overload of an existing
one, never a new capability*. For a new capability with its own artifact class
the two disagree; the compiler-engineer read the first and the spec-warden the
second, both correctly (critic). And `fmt`'s command row takes `.file_operand`
(`selfhost/cli/table.hero:130`), so a flag cannot take the directory the
spec-warden's own whole-tree form needs (critic, checked).

**Exit codes.** The proposal's exit 1 for a failing variant would give one
formatter defect two codes: `fmt` reports the same finding as exit 2, *this is a
compiler bug* (critic).

## The resolution — `provisional — author ratification pending`

1. **A subcommand, `heroes probe [path]`**, a file or a directory, defaulting
   to the formatter's fixture directories. Its artifact is a verdict over
   generated programs and its operand a directory, which `fmt`'s row cannot
   carry; `mutate` is the shape already in the table. **The admission is the
   author's decision of 2026-09-26**, stated as that and not stretched from the
   stopping rule, and `.claude/rules/cli-surface.md` gains a sentence naming the
   tension between its two sentences and how this sitting read it.
2. **In process.** The probe calls the lexer, the formatter and the guard as
   functions; it never spawns `heroes` per variant.
3. **Every family the seats ran**, named in the module doc and counted
   separately in the report: comment insertion (single and multi), the bracket
   break, the parenthesis wrap; with CRLF and no-final-newline copies (the
   shapes g3's `drive.py` made and g4 dropped), comments at columns that are not
   a multiple of 4, and long and non-ASCII comments (the gaps the critic found
   in the generators, measured to hold today).
4. **Two judges, and the three open cases ruled here.** The guard (fixpoint,
   tree, owner block, neighbour tokens) and an independent reader's owner model,
   the port of `rd.py`, which must agree; a disagreement is a failure naming the
   judge that failed. The rulings: **(a)** a comment that crosses only a comma
   is kept, since the comma is a separator the canonical form places and the
   comment still trails the same element; **(b)** a comment trailing a
   parenthesis the formatter drops is kept if it trails the same code on the
   same line, since the tree holds no parentheses; **(c)** the ascending-run rule
   `owners.hero` states is the canonical form, and the independent model encodes
   it as its one stated exception, with the fixture `ascending.hero` pinning it.
5. **A site, a reduced reproducer, and the discards.** The report gives counts
   per family (generated, parsing, held, refused), the discards by diagnostic
   class, and for the first failure its file, family, token index and the first
   lines the texts differ on, plus a reproducer reduced by deleting lines while
   the failure persists; never the whole text.
6. **Exit codes as `fmt` gives them**: 0 when every parsing variant holds; **2**
   when one fails, the formatter accusing itself as `fmt` does for the same
   finding; 1 when the seed itself has diagnostics.
7. **In the net: the fixtures under every family, and the whole tree under the
   thin one.** A row over the 41 fixture files of the five formatter-defect
   directories, unstrided if it measures under 60 s of user time on a quiet
   machine, otherwise with a deterministic stride written in the row; a second
   row over the whole tree with `multi` alone. Each row pins its variant-count
   floor per family, so a generator that silently produces fewer goes red. The
   full single-comment and bracket-break run over the tree stays a run by hand,
   **required before a push that touches `selfhost/print/`**, written in
   `.claude/rules/verification.md`.
8. **What lands first.** The 17 refusals found at `83ac68c1`, and anything the
   probe finds on its first run, are repaired in lane g before the probe's rows
   land; the probe lands in its own lane after lane g merges.

**What conservative would have been** (CL-040): `fmt --probe` on one file, the
guard as the only judge, the text of the first failure, the fixtures only.
Refused: a flag cannot take the directory the whole-tree row needs, a guard that
judges itself sees a broken walk and never a wrong rule, and a 108,422-byte
failure text is not a reproducer.

**What the conditions compel.** The compiler-engineer's objection is met by
items 4 and 5; the spec-warden's conditions by items 2, 6 and 7, and its
objection to a verb without an overload is answered by item 1's admission
rather than overruled: the author decided the capability, the sitting its
artifact class.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | the probe's modules each at or under 300 counted lines and together at or under 420 (the resolution's item 4 adds a fourth module; the bound is scored on the three the seat named) | the probe's landing |
| compiler-engineer | `heroes probe selfhost/check/walk.hero` reports 39,731 variants for comment insertion and bracket break | the probe's landing |
| compiler-engineer | the fully judged run over `walk.hero` against `83ac68c1` refuses at least 1% of the parsing bracket breaks (5 of 225 in the sample) | a number to take at the landing, not a prediction |
| spec-warden | the spec reads 8861 real and 6693 vendored after the landing | the probe's landing |
| spec-warden | `heroes --help` grows to 74 lines and 948 to 950 vendored tokens (the verb form) | the probe's landing |
| spec-warden | the fixtures give 7,982 generated and 6,350 parsing (four families, 30 files) | the probe's landing, on the fixture set item 7 names |
| historian | none of the ten formatter CLI references it cites lists a generator of deformed inputs within twelve months; Black's `--safe` stays default-on | 2027-09-27 |
| critic | `parens.hero` gives 576 variants, 456 parsing, 6 refused at `83ac68c1` | scored in the sitting, reproduced against the compiler-engineer's count |

## Author's verdict

*Pending: `docs/work/DECIDE.md` carries this sitting as `panel 179`. Work
proceeds on the provisional resolution: lane g repairs the 17 refusals first,
then the probe lands in its own lane.*

## The landing, 2026-09-28

The probe landed in its own lane (M-agreed-retention step 27, `e8ed8732`,
merged `49e50f34`), on items 1 to 8 above, after lane g's merge as item 8
asked. Its gate, Linux arm64 and the Windows box are in the lane's commit and in
defect 121's record, the one defect its first run found and the lane repaired.

**A second exception in the reader, which is the author's to ratify.** The
reader disagreed with the guard on 640 of the first run's 664 failures, every
one a fixture holding two `extern` groups under one head with a comment directly
above the second head: `fmt` merges the groups (panel 036's canonical form) and
prints the comment above the member it documents, while the reader read the
second head as a logical line. The lane resolved it as the reader's rule (d), *a
head the output does not have is no logical line in the variant either*, read
from the two files as the guard's `merged_heads` does. Item 4 called ruling (c)
the reader's one stated exception; (d) is a second, queued with this sitting's
ratification in `docs/work/DECIDE.md`.

**One deliberate difference from the seats' generators**: `parengen.py` named
the literal kinds by words the JSON dump never prints, so the seats' parenthesis
wrap only ever wrapped names and booleans; the port wraps literals as the script
meant (2139 variants over the fixtures against the seats' 1209).

**Owed before the push that carries this**, since it touches `selfhost/print/`,
and unrun at the landing: the by-hand run `.claude/rules/verification.md` now
states, the single and bracket families over `selfhost`, `tests` and `examples`
and the fixtures unstrided, about 4.4 CPU hours at `-O2` by that rule's own
arithmetic.

### The predictions, scored at the landing (the lane's measurements)

| seat | prediction | score |
|---|---|---|
| compiler-engineer | each module at or under 300 counted lines | **held**: the largest, `probe/reader.hero`, 296 |
| compiler-engineer | the three modules together at or under 420 | **falsified**: deform, judge and command 936; the probe with its reader and reduction 1,324 |
| compiler-engineer | `heroes probe selfhost/check/walk.hero` reports 39,731 variants for comment insertion and bracket break | **held** on the text of `83ac68c1` (10,364 and 29,367); today's `walk.hero` gives 39,740 |
| compiler-engineer | the fully judged run over `walk.hero` refuses at least 1% of the parsing bracket breaks (a number to take at the landing) | taken: **0 of 22,860** on today's `walk.hero`, every variant held by both judges |
| spec-warden | the spec reads 8861 real and 6693 vendored after the landing | **falsified as numbers** (8999 and 6794, moved by panel 180's landing, `74203a64`); the zero delta held, since the probe changes no byte under `spec/` |
| spec-warden | `heroes --help` grows to 74 lines and 948 to 950 vendored tokens | **falsified**: 75 lines, 1001 vendored |
| spec-warden | the fixtures give 7,982 generated and 6,350 parsing | generated **held**; parsing **falsified**, 6,560 under today's parser (probably panel 180's repair; not verified) |
| historian | none of ten formatter CLIs lists a generator of deformed inputs within twelve months | checkable 2027-09-27 |
| critic | `parens.hero` gives 576 variants, 456 parsing, 6 refused at `83ac68c1` | generated **held** (576); none refused today |

**Corrected 2026-09-28, at M-agreed-retention's close**
(`docs/records/done/2026-09-28-2138-the-predictions-of-m-agreed-retention-scored.md`):
the spec-warden's fixture row is **falsified on both counts**. Its *generated
held* rests on the recovered Python generators and not on the probe that
landed, and its 6,560 sums the probe's single, multi and bracket rows with the
seats' paren row, a figure no single run prints. The landed probe over the 30
files as at `83ac68c1`, unstrided, on `e8ed8732`'s compiler and on the trunk's
at `a63cf9ed`, reads 8,405 generated and 6,977 parsing against 7,982 and
6,350; the whole generated difference is the paren family, 981 against 558.
The compiler-engineer's *number to take* is scored void there, since the
configuration it names, the probe judging `83ac68c1`'s formatter, was never
built.
