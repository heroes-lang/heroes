# 052: the first held-out run: the primary inconclusive at +0.0 points with no discordant pair, the strict arm paying at turn one and level by turn three, and no model writing a variant

Measured 2026-10-24 by the session holding M-thesis-harness, its step 13,
over the two runs the author made in their own terminal, a shell no model
reads, from the main checkout at `b3defeb49`, the compiler of sources
`616f959062622f6a` the pre-registration names (`tests/thesis/held-out/prediction.md`,
its block): the held-out suite opened by `unseal` in one shell and graded in
another where no key was set. The session read the runs' counts, `report.md`
and `verdicts.jsonl` (outcomes and diagnostic codes), and counted forms in the
programs with commands printing numbers alone (decision 4a); no task's prose
and no program's text entered it. **This is the held-out run the
pre-registration was written for**, and its primary is read against that block.

## 1. The canary, the gate before the run

`canary-probe build/opened` on the four pinned models: **164 probes, 0
completed, 0 lost** (`build/thesis-runs/c24a5b81/canary`), 41 prefixes, the
suite's and the forty tasks', to each model. No task burned; the reserve stays
sealed.

## 2. What ran

| run | folder | contexts | models | requests | generations | verdicts | lost | spent |
|---|---|---|---|---|---|---|---|---|
| the primary's, the pre-registered protocol | `c24a5b81` | `spec` | the four pinned | 681, 682 attempts | 480 | 1,440 | 0 | $32.55 |

| run | folder | contexts | models | requests | generations | verdicts | lost | spent |
|---|---|---|---|---|---|---|---|---|
| the variants, panel 226's R6 | `c1e3d004` | `spec`, without productions, with one worked diagnostic | Haiku 5.5, Sonnet 5.5 | 262 | 180 | 540 | 0 | $3.44 |

The primary's run asked 681 requests in 682 attempts, one retried and none lost, 8 turns shared between arms whose histories were the same bytes, by its tally; $32.55 of the $100 cap, where `plan` bounded turn 1 alone at $378.58 with every answer at `max_tokens`. With the canary's 164 probes (their cost the instrument does not record) and the variants' $3.44, the held-out runs spent $35.99 and the probes. The variants ran over the held-out suite's tasks 01 to 10, copied whole into
`build/opened/ten/` (the folder the hook holds as it holds `build/opened/`), a
choice made before any of them was graded: the first ten by number.

## 3. The primary: inconclusive, +0.0 points, no discordant pair

*`green@3`(strict, full message) − `green@3`(control) on `claude-sonnet-5-5` in
the `spec` context, over the 40 tasks at three samples each, the cluster
bootstrap over tasks, McNemar's mid-p* (`prediction.md`, its primary). From
`c24a5b81`'s `report.md`, `report --labels tests/thesis/held-out`:

| model | pairs | strict green@3 | control green@3 | difference | bootstrap 95% | discordant |
|---|---|---|---|---|---|---|
| **Sonnet 5.5, the primary** | 120 | 116 | 116 | **+0.0pp** | **+0.0pp to +0.0pp** | **0 / 0** |
| Opus 5.5 | 120 | 115 | 115 | +0.0pp | +0.0pp to +0.0pp | 0 / 0 |
| Fable 5.1 | 120 | 114 | 114 | +0.0pp | +0.0pp to +0.0pp | 0 / 0 |
| Haiku 5.5 | 120 | 110 | 110 | +0.0pp | +0.0pp to +0.0pp | 0 / 0 |

**The outcome is inconclusive**, by the rule written before the run: the
interval's lower bound is not above 0 and its upper bound is not below it. By
the author's choice 1a it is published as inconclusive, with its interval as
measured, and n is not extended after reading it. No pair of any model
disagreed: every sample green in one arm at turn 3 was green in the other, and
McNemar has nothing to weigh. The first secondary reads the same, the silent
programs at turn 3 4 and 4 for Sonnet 5.5, 5 and 5 for Opus 5.5, 6 and 6 for
Fable 5.1, 10 and 10 for Haiku 5.5, control and strict, the same samples in
both, 0 discordant; and so does every stratum and construct class the labels
name, the trap stratum being the whole suite (every task a trap, decision 2172)
and every class at +0.0.

**The numbers expected**, set against what ran:

| expected (author's choice 2a) | measured | |
|---|---|---|
| `green@1` strict below control by 5 to 15 points | Sonnet 5.5 94 against 100, **−5.0**; Opus 5.5 91 against 102, −9.2; Haiku 5.5 86 against 91, −4.2; Fable 5.1 114 and 114, 0 | met on the primary's model, at its lower edge |
| `green@3` strict above control by at least 10 points | +0.0 on every model | **not met** |
| silent at turn 3, strict at most half the control's | equal on every model | **not met** |

The strict arm paid at turn 1, the refusal before any feedback, and the
messages repaid it by turn 3 on every model; what the thesis predicted beyond
that, a silent program the control arm runs and the strict arm refuses, did not
occur once. **What the suite tempted was not taken**: no answer of the 480 at
turn 1 declared a `variant` (21 declared a `record`, 311 used `match`, on
fallible values, numbers and text), though ten tasks are labelled with a variant
(`constructs.txt`), so the traps built on a case forgotten or a `_` over cases
had nothing to catch; the silent programs are mistakes no rule of the language
names, which the strict arm compiles as the control does. Counted over the answers with numbers alone printed, by the census whose rules section 7 writes.

**What the thesis can say from this run, in the pre-registration's words**: an
inconclusive result, the interval [+0.0, +0.0] on the primary; the strict arm's
price at turn 1 measured; its benefit, on forty tasks written to tempt it, not
seen.

## 4. The variant contexts on ten held-out tasks (issues 1139 and 1005)

The strict arm, 30 samples a cell, from `c1e3d004`'s `report.md`:

| model | context | green@1 | green@3 | silent@3 |
|---|---|---|---|---|
| Haiku 5.5 | `spec` | 22 | 27 | 3 |
| Haiku 5.5 | without productions | 22 | 27 | 3 |
| Haiku 5.5 | with one worked diagnostic | 24 | 27 | 3 |
| Sonnet 5.5 | `spec` | 22 | 24 | 6 |
| Sonnet 5.5 | without productions | 22 | 26 | 4 |
| Sonnet 5.5 | with one worked diagnostic | 20 | 25 | 5 |

**Panel 133's third prediction** (issue 1139), *spec plus grammar beats the
spec alone by at most +1 task in 10, and the silent-divergence delta is exactly
0*, read as first-try completion, the metric it was registered against, pooled
over the two models, 60 samples a context: **its first half held**, 44 of 60
with the productions and 44 without, +0 against at most +10 points; **its
second half is falsified as worded**, 9 silent programs with the productions
and 7 without, a delta of 2 samples of 60 where it said exactly 0, a count well
inside what three samples a task can move by chance and still not the 0 it
named. At turn 3 the specification without its productions read 53 of 60
against 51. Its fourth prediction, *at least 1 in 10 FFI-and-pattern attempts
writes a form the productions admit and the checker refuses*, is **lapsed**: no
task of the suite binds C, so the attempts it counts were not asked for.

**Issue 1005's number**, one worked diagnostic: 44 of 60 at turn 1 against the
specification's 44, 52 at turn 3 against 51, 8 silent against 9. **No
measurable effect** on these ten tasks and two models. Whether the
specification should show one is the sitting's to decide on that number
(issue 1005's own terms), and the number is +0 points at turn 1.

**And what the ten tasks showed of the primary, exploratory**: in every
context and for both models, the strict and the control arm agreed on every
pair, at green@3 and on the silent programs, 0 discordant of 180 each; the
silent programs, 3 of Haiku 5.5's and 4 to 6 of Sonnet 5.5's a context, were
the same programs in every arm, mistakes no rule of the language names.

## 5. Panel 007's spec-warden, on every answer of the day

*Zero baseline completions contain a depth-0 illegal break*: 0 of 180 in the
ten-task run, strict arm, turn 1, 0 of 60 in each context. 0 of 480 in the primary run, strict arm, turn 1. With
measurement 051's, 0 of 360 under the specification on the assistant's tasks
and 1 of 90 without it.

## 6. Panel 227's P1 and P2, ledger row 7856's payment

Scored on the whole suite with the declaration the pre-registration writes
(the author's choice 3a): the suite's author had read panel 227 before writing.
Counted by a counting program kept outside the tree, `count227.py`, sha256 `7db331f2419bd0c6f2f871b933ed5a66c109c34cf8b44e2075188359f75b12ad` (its rules: a call's last top-level argument is its
count, a count holding `len(` the padding class, a literal the literal class,
any other the parsed class; a refusal *at the count* a diagnostic whose column
falls inside that argument, from `check --json` with the run's compiler; a
`slice` *with nothing to slice* one called as a function whose first argument
is labelled `from:`), calibrated against measurement 050's own figures, which
it reproduced exactly: 13 `repeat` callers, all 13 refused at the count, 8
parsed and 5 padding, and Haiku 5.5's `slice` 10 of 26.

- **P1, `repeat`**: 2 turn-1 answers call `repeat`, both Opus 5.5's, both of the
  padding class, **0 refused at the count**: at most half pooled and at most a
  quarter for Sonnet 5.5 and Opus 5.5 together, **held, on two answers**; each
  class holds under 10 answers (padding 2, parsed 0), so **each class is
  `lapsed`** by the prediction's own rule, and the padding-against-parsed
  comparison that would send `len` to a sitting cannot be made.
- **P2, `slice`**: Haiku 5.5 calls `slice` in 69 turn-1 answers and **0** with
  nothing to slice, at most 10% asked; Sonnet 5.5 and Opus 5.5 in 165 and 0,
  at most 2% asked; Fable 5.1 in 89 and 0. **Held**, with at least 20 Haiku
  answers as the prediction requires.

Row 7856 is paid. (In measurement 050, before the sentence, the figures were
13 of 13 and 10 of 26; a sign, the tasks and the day being others.)

## 7. The older sittings' predictions, counted

Counted by a second program kept outside the tree, `census-old.py`, sha256 `ef3d98ad752fb5811dbd2d8fd70f4e426eb9175ad3b17e455805c5b55d9484ce`, over the 480 turn-1 answers under `spec`, numbers alone, each rule checked first on the trial's and the pilot's answers and on a
program written to hold each shape once:

| prediction | the rule counted | measured | verdict |
|---|---|---|---|
| panel 042, spec-warden 4: integer-width diagnostics at most 5% of first-try failures, above 15% a net loss | refused programs with a message naming two integer types | 1 of 69 refused, 1.4% | **held** |
| panel 018, the ratified tiebreaker: F stands if colon insertion is under 8% of files | a block opener ending in `:` | 0 of 480 | **held: F stands** |
| panel 144, spec-warden 4: zero programs accumulate through `x.f @ x.f.push(v)` | that form | 0 of 480 | **held** |
| panel 142: at most 5% of first-try programs get a case constructor wrong | a diagnostic on a case construction | 0 constructions in 480 | **lapsed**: no answer constructed a case |
| panel 142: at least 20% write `.rect w h` in a pattern | a case pattern binding two bare names | 0 case patterns in 480 | **lapsed**: no answer matched on a case |
| panel 036, spec-warden: at least 30% of programs that read and write a file fail on `-> ()?` alone | programs calling both | 0 of 480 | **lapsed**: no task writes a file |

Every other prediction a sitting registered against a metric-2 run or this
milestone that these runs could not ask is **lapsed**, by panel 046's R2, with
its reason: those needing a task of their own sitting's shape, an FFI task, an
older specification, a variant arm or a run of readers (the pre-046
observations of panels 002 to 129 among them, which paid nothing), listed in the appendix below with where each stands. The two that paid tokens and could not be asked, panel 177's halves
of rows 6344, 6643 and 6693, are lapsed in the ledger and their clauses
re-argued in decision 2193 (the author's 6a: kept).

## 8. Deviations from the pre-registration

**None**: `report` sets the run's `provenance.json` against the
pre-registration's block field by field and reads every one of the 21 protocol
fields as the block wrote it, the compiler's sources `616f959062622f6a`, the
instrument's digest, the template's and the layout's, the suite's, the
specification's, the models and their efforts, the arms, samples and turns and
the bounds per case. The ten-task variant run is another protocol by
construction (its contexts and models), and is read only as section 4 reads it.

## 9. The formula's second factor, measured for the first time

Design.md §1.2's *real cost = program tokens × (1 + rewrite rate)*, its rewrite
rate as panel 226's R12 defines it, mean(turns to green − 1) over the samples
that converged, per arm, those that did not apart; Sonnet 5.5, `spec`, from
`verdicts.jsonl`'s `green_turn`, `converged` and each turn's `usage`:

| arm | converged | rewrites | rewrite rate | not converged | correction turns | their median, uncached input and output |
|---|---|---|---|---|---|---|
| strict | 116 of 120 | 23 | **0.198** | 4, silent | 23 | **3,619** tokens |
| control | 116 of 120 | 17 | **0.147** | 4, silent | 17 | 3,709 |
| brief | 116 of 120 | 24 | 0.207 | 4, silent | 24 | 3,515 |

A correction turn's tokens are its uncached input and its output by the run's
own `usage`, the thinking billed as output included, the specification read
from the cache in front of every turn left out as the price of every request
and not of a correction. Design.md §1.6's panel-011 sentence is amended with
these numbers, in the shape R12 pre-authorised. **The first factor is not
measured here**: R12 counts *program tokens* by `count_tokens` on the extracted
program, and the instrument has no such word, so the column is owed to the
milestone that gives it one (M-compared-languages measures tokens to green).

## Appendix: every prediction this close owed, and where it stands

Enumerated on 2026-10-24 from the files, by `grep -l "M-thesis-harness"
docs/panel/*.md` (33 sittings), by `grep -l -i "metric-2 run\|metric 2 run\|Part
11 experiment\|metric 2 runs\|until metric 2" docs/panel/*.md` (12), by a wider
search for *metric 2*, *Part 11*, *held-out run* and *harness run* (70 more), by
the ledger's rows naming a scoring at this milestone, by the milestone's own
page and the pre-registration, and by issues 4, 1139 and 1005; `NNN:line` is
`docs/panel/NNN-*.md`.

**Scored in measurements 051 and 052**: the pre-registration's primary and its
three expected numbers (this measurement, section 3); panel 227's P1 and P2
(section 6) and P4 (051 § 5); panel 226's llm-ergonomist on the trial and its
ffi-pragmatist (051 §§ 4 and 6); panel 007's spec-warden (051 § 7, this § 5);
panel 133's third, its fourth lapsed, and issue 1005 (section 4); the formula's rewrite rate (section 9); ledger row 7846's ten
readers (051 § 8); panels 042, 018, 144, 142 and 036 as section 7's table.

**Lapsed, the run or the task they need not asked by this milestone** (panel
046's R2; the clause re-argued where a row paid for it):

- **ledger rows 6344, 6643 and 6693**: panel 177's llm-ergonomist's P1, P3 and
  P4 and its spec-warden's P2 at a second scoring (177:214, 177:216), handle
  tasks for twenty readers; their clauses decided in decision 2193;
- **a count the instrument does not take**: 226:662-664, the spec-warden's
  round-trip median and diagnostic means by `count_tokens`; its cache-share
  half (226:664-665) likewise, the run's `usage` recorded and no price table set
  against its fraction;
- **FFI tasks the suite does not hold**: 175:213, 176:207, 177:219, 111:443,
  112:317, 122:302, 124:445, 139:250, 146:375, 147:259, 148:237, 153:214,
  156:212, 161:181, 163:187, 214:181, and the pre-046 FFI rows 036:207 to 210,
  041:303, 051:138, 052:134, 053:162, 055:201, 056:248, 058:281, 059:257,
  060:393 and 394, 061:353, 062:179, 064:253, 103:215, 108:295, 109:314;
- **a specification other than the one sent, or a variant arm not built**:
  110:260 (its old-spec half) and 110:261, 115:349, 117:253, 119:319 and 321,
  121:365 (its ungated half), 126:213 and 214, 135:255 and 256, 136:209,
  137:223 and 224, 138:183, 144:272 to 277, 155:273, 158:188, 159:227, 214:74,
  and the pre-046 variant rows of panels 002, 003, 006, 008, 009, 013, 014, 015,
  017, 018 (its E arm), 022 to 029, 035, 037, 042:314, 043, 069, 081 to 084,
  088, 127 and 129;
- **a task of their own sitting's shape, which these forty need not hold**:
  105:188, 110:260 (its amended half), 115:349 (its H half), 121:365 (its landed
  half), 159:228, 160:206, 226:666-668's two repairs, and the pre-046
  observations 002:51, 008:117, 010:117, 017:147, 027:217, 028:277, 029:346 to
  349, 035:201, 037:177, 043:254, 054:144, 069:173, 129:196, 034:298 and 299,
  039:240 and 241, 057:270, 087:251 and 252, and the multi-file module rows of
  panels 030 to 033 and 099 to 102;
- **readings or events, not runs**: 226:678 (a provider page, a corpus find);
  226:677 and 679 as events, no task's canary completed and the productions
  variant measured in section 4.

**Already scored or lapsed before this close**: 007:82-83 (panel 226's R13),
037:176, 038:475 to 477, 041:305, 042:316, 092:279, 180:229-236, 181:321,
177:214's landing half, 227:219 (P3, a hit), 226:674-676, 120:307.

**Not this milestone's**: the rows naming M-guide-book, M-program-corpus,
M-ffi-ladder, M-selfhost-fixpoint, a route or resolution not taken, metric 3, a
later sitting, or no milestone at all (panels 089, 090, 119, 120, 110:270, 073
to 079, 048, 059:260, 095, 161:180, 102:231, 039:242, 114, 043:255, 044, 011,
023:290, 118:306, 131, 132, 133:219, 194, 196, 187, 204, 175:214, 176:208, 091,
164, 170, 184, 185, 186, 201, 203, 213), as the enumeration found them.
