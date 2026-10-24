# 069 — M-thesis-harness: the thesis, measured

## Goal

Row 70 of the chain, opened on 2026-10-20 (`a0a8d1249`): design.md Part 11's
metrics 2 and 4, first-try completion and turns to green under the compiler's
feedback, measured for the first time, by a program of the harness and not a
verb of `heroes` (`tests/harness/thesis.hero`, the reading of panel 212's R1
extended), and proved on real input before a held-out run spends on it. Until
then the thesis, *every plausible LLM mistake is a compile error*, had one half
measured, metric 3's planted mutants (measurement 024), and panel 011's verdict
stood: a measured mechanism, an unproven claim. The milestone's page carries
its reasoning, the run order and what the author owed: the forty held-out
tasks and twenty in reserve, the three keys, the ratifications.

## What surprised

**The control arm could judge and could not run.** `--permissive` was a flag
of `check` alone, so the comparison Part 11 calls *the only one that isolates
design from training-data familiarity* had no binary to run hidden cases on.
Panel 226's compiler-engineer measured that a filter answers 17 of the 27
dropped rules and not the other seven; step 1 gave `build`, `run` and `test`
the flag and each of the seven a runtime meaning the strict build never sets,
26 witnesses in the new `control` form.

**The trial's tasks could not move the primary.** On the assistant's ten
tasks green@3 read 28, 29 and 30 of 30 in every arm for Haiku 5.5, Sonnet 5.5
and Opus 5.5 (measurement 050), so a suite like them shows the strict arm's
cost at turn 1 and never its benefit (decision 2172, whose first numbers were
Haiku 5.5's answers repaired by the others, defect 2176). The rule that followed is the held-out suite's shape: every
task a trap, a mistake the strict arm refuses and the control arm runs to a
wrong output on a hidden case, proven before the freeze by a hand mutant that
`dry` grades.

**A run kept answers from a model it had not asked.** Every turn-1 request
named the plan's first model (defect 2176), the `claude -p` harness named as
its answerer whichever model its usage listed first (2179), and a run kept an
answer under the asked model's name and price (2173). Measurement 049 stands
corrected underneath three times; measurement 050 asked the trial again with
every model named and read every answer's `answered_by`.

**Without the specification no model wrote a program that ran right**, and
under it the strict arm paid at turn 1 and was level by turn 3 (measurement
050). The `none` context's first prompt told the model it was given the
complete specification and then that none was given (defect 2169).

**The most frequent turn-1 refusal of the trial was one built-in's argument
type.** Thirteen of ninety strict samples under `spec` drew `type_mismatch`,
all thirteen `repeat` given an `i64` count, which panel 227's sentence answered
(`repeat`'s count is a `u64`, defect 2174) and which, for the same reason, the
worked diagnostic of the variant context does not show: a gain measured on the
tasks that produced it would read as the document teaching their answer.

**The seal held against the session that built it.** No model read a held-out
task: the author wrote them in a folder `.claude/settings.json` and the hooks
deny, the hook refuses `plan` and `run` over an opened archive from any shell a
model reads, and the held-out run is the author's act in their own terminal.
The instrument's own digest, `instrument` in the pre-registered block, turned
out to bind both ways: it keeps the grader from a change after the data, and it
keeps out a harmless one too, so the runs' transcripts are sealed by `openssl`
by hand rather than by a new word.

## What broke and why

- **The instrument's truth about its own runs** (2173, 2176, 2178, 2179, 2180,
  2181): which model answered, which request a cap kept back, whose provenance
  a resumed folder holds, which effort a request carried. Each was found by
  reading the answers a run kept, never by a suite, and each is a field the
  report now reads back.
- **The sandbox on the CI's legs** (2166, 2167): a test naming `/bin/sh` on
  Windows, and Linux's two limits in one `ulimit`, which dash refuses, so a
  model's program ran with no file or memory bound until the leg went red.
- **The report** (2170, 2171): a count of verdicts ending in 0 read as none,
  and tables naming less than they counted.
- **The compiler, through the trial's answers** (2174, 2175, 2183, 2184, 2185,
  2186): the built-ins' sentence wrote `slice` with nothing to slice and
  `repeat` without its count's type; a `certain` label fix wrote a label the
  call held already; a fallible operand was refused with no `.must()`.
- **The records' instruments** (2189, 2190): a listing GitHub gives in part
  read as whole, and a full hash written bare in a sentence never asked of git
  (2190 was this session's own wrong hash on issue 2186, corrected beneath).

## What landed, and what carried forward

**The parts**, in the milestone's run order, each its own commit: the row
opened and its page (`a0a8d1249`); panel 226, the control arm that builds and
runs, `build`, `run` and `test` taking `--permissive` and every dropped rule a
runtime meaning (`9ca6f1dfe`); the instrument, `tests/harness/thesis.hero` and
its modules, built and tested at no cost (`c1b4c8ed3`), and the records that
say what it does with the CI's attestation of a sealed suite (`661b3049b`); the
assistant's ten tasks for the trial (`3add0597a`); the trial, measurement 049
corrected three times underneath and measurement 050 asking it again with
every model named (`743ae6888`, `63c78c54b`); the kit for writing the held-out
tasks (`7d92b2619`); panel 227's sentence on the built-ins (`f34f944c3`) and
its compiler half (`872dbcaf8`, `be9009cac`); the freeze, the suite and its
reserve sealed with their digests, their labels and the pre-registered
prediction, pushed before the run (`a4e09e061`); the variant contexts
(`b3defeb49`, `f305307aa`); measurement 051, the variants' pilot and five
predictions scored without a held-out task (`d1274e73d`); defect 2186's repair
(`1cfe72228`, merged at `0021ab0ec` with its round's gate green); the site's
`Measured` page (`9d319e861`, merged at `c711ae0b2`); the ledger's five rows and
design.md §1.6's sentence (`b0b70b5de`); measurement 052, the held-out run.

**The number the milestone exists for**: metric 2 ran for the first time, on
forty held-out tasks written by the author, every one a trap, three samples,
four models, three arms, under a protocol frozen and pushed before the run, the
canary clean (164 probes, 0 completed) and no deviation from the
pre-registration (all 21 protocol fields). **The primary is inconclusive**:
green@3 strict minus control on Sonnet 5.5, 116 and 116, +0.0 points, the
interval [+0.0, +0.0], no discordant pair on any model; published as
inconclusive, by the author's choice 1a. The strict arm paid at turn 1 (Sonnet
5.5 −5.0 points, Opus 5.5 −9.2) and the messages repaid it by turn 3; the silent
programs were the same programs in every arm, and no answer of the 480 declared
a `variant`, so the traps built on one had nothing to catch. The rewrite rate,
§1.2's second factor, read 0.198 strict and 0.147 control on Sonnet 5.5, and
design.md §1.6 says so from today.

**Carried forward**, each an issue with a home: the three-condition experiment
on the messages (issue 2080) and the `codex exec` rows (issue 2182), moved to
M-compared-languages on the author's decision, which also owns the first
factor, program tokens by `count_tokens`; whether the specification should show
a worked diagnostic, now with its number (no measurable effect), for the sitting
that would buy or refuse it; and defect 2190, a bare hash a comment's prose
names never asked of git.

## Tagged over a clean list, 2026-10-24

**`m-thesis-harness` is placed on the commit this section is written in**, over
a list asked of GitHub before it was placed: no `blocking` or `systemic` defect
open (2139, 2143, 2158 and 2190, `adjacent`), and no open decision. Panels 226
and 227 were read by the author before the freeze (issues 2159 and 2187), and
this close's three decisions are filed closed: the freeze's five choices
(2191), the worked diagnostic (2192) and § 13's three sentences, kept (2193).

**Corrected 2026-10-24, the same evening.** The list above was clean by
`records/tagged` when it was asked and not once the tag stood: defects 2139
and 2143, filed 2026-10-18 and open at `m-witnessed-seed`, count as `blocking`
from the second tag placed since they were filed, and that tag is this one.
The check judges the newest tag, so before this one was placed it judged
`m-witnessed-seed`, and it was not asked again between the placing and the
push (defect 2198); the session heroes-lang-51 read it red at 21:40, after the
push. By the author's *B* (decision 2200) the two are carried past this tag,
`records_github.CARRIED` excusing it over exactly them, and they are the next
batch's first items. The same evening the attestation the author re-ran with
the keys stopped on an AppleDouble member macOS's `tar` had packed into the
suite's archive (defect 2197), repaired in the same round without sealing the
archives again, since their digests are the pre-registration's.
