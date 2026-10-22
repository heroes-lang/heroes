# Panel 227: the built-ins' sentence shows what a call takes, and a wrong count is told one way whatever the callee

**Convened 2026-10-21 in M-thesis-harness, after its step 11 (measurement 050), synthesised the
same day**, on defects 2174 (the sentence, https://github.com/heroes-lang/heroes/issues/2174) and
2175 (the message, https://github.com/heroes-lang/heroes/issues/2175), at the author's request of
that day in the session that ran the trial, meant as *do point 1*, point 1 being these two defects
through a sitting as the coordinator had proposed. **Lane: full panel less the ffi-pragmatist**, the
proposal touching no C, no ABI and no binding; a completeness critic before the seats and after
them. **Resolution: provisional, author ratification pending.** Briefs: `docs/panel/227-briefs/`,
repaired after the critic's first pass (`docs/panel/227-reports/critic-briefs.md`, and the shared
brief's own § Repaired). Reports: `docs/panel/227-reports/compiler-engineer.md`, `spec-warden.md`,
`historian.md`, `llm-ergonomist.md` (the blind experiment's result, written by the coordinator from
the six sessions in `blind/`), `critic-reports.md`. Every number below is in the report that holds
it, with the command that produced it.

## The proposal, verbatim (the shared brief)

`spec/heroes-spec.md` § 11, lines 316 and 317:

```
-Built-ins: `print(...)` · `len` · `push` · `slice(from:, to:)` ·
-`chars` · `keys` · `join(xs, sep)` · `repeat(s, n)` · `sort` (ascending: …) ·
+Built-ins: `print(...)` · `len` · `push` · `xs.slice(from:, to:)` ·
+`chars` · `keys` · `join(xs, sep)` · `repeat(s, n)` (`n` a `u64`) · `sort` (ascending: …) ·
```

and in the checker, a `slice` call whose first argument is missing told as that, one message
naming the value sliced, and no `wrong_label` whose guess writes `from:` a second time. Routes B
(every built-in typed), C (the message alone), D (`repeat` taking an `i64`) and, from the critic's
first pass, E (a built-in's wrong count told as a declared function's is) were put beside it.

## What was measured

- **The price**: route A's sentence is **+13 real tokens**, 10,286 → 10,299 in the ledger's unit
  (`count_tokens` on `claude-opus-5` answered 10,292 and 10,305, the wrapper of 6 probed as `measure
  --refresh` probes it, the requests and answers kept beside the copies), **+10** on each vendored
  table (7708 → 7718, 7846 → 7856). With it the consolidation block has spent **251 of its 1,500**
  (design.md §1.6), which still binds: measurement 050 is the trial, not *the thesis harness has
  run* (the spec-warden, four reasons read).
- **What the trial's answers did** (050's `build/thesis-runs/00fb571c-v3`, re-read by the critic
  twice and by the spec-warden): Haiku 5.5 wrote a `slice` with nothing to slice in **10** of its
  turn-1 answers, **9** refused for it at turn 1 (the tenth refused first for a parse error, met the
  `slice` message at turn 2 and went green at turn 3 in every arm); Sonnet 5.5 and Opus 5.5 none, 0
  of 58 answers calling `slice`. `repeat`'s count was refused in **13 of 13** turn-1 answers that
  failed on it, across three tiers, from 2 of the 10 tasks. Of the 9, **8 went green at turn 2 in
  every arm, control included**, each arm with its own turn 2, so the strict arm's misleading
  `wrong_label` cost nothing measurable; of the 13, **12 wrote `to_u64(...).must()` at turn 2**
  unprompted. **No route can move 050's primary**: green@3 strict against control is concordant on
  all 90 pairs (the critic, `verdicts.jsonl`).
- **The blind experiment** (`llm-ergonomist.md`): six `claude -p` sessions on Haiku 5.5, three per
  sentence, each given the specification, two tasks and a brief, the variant in no folder. Under
  today's sentence **6 of 6 programs were refused for `repeat`'s count**; under route A's, **0 of 6**,
  every one writing `to_u64(...).must()`; by session 3 of 3 against 0 of 3 (Fisher two-sided p =
  0.1), by program p = 0.0022. **None of the 12 programs wrote a `slice` with nothing to slice**,
  under either sentence: the experiment has no reading on that half. Route A's two refusals are a
  binding named `all`; renamed, all six route-A programs check at exit 0. A first run of six was void
  (every `context` answered *yes*: Claude Code's default system prompt carried the repository's git
  status and five commit subjects naming the trial), and the run that counts replaced the system
  prompt with one sentence naming the folder, under `--bare`, `Read` alone (`blind/README.md` in the
  briefs). Four of the six readers named `len`'s result type as unstated (the critic).
- **The message routes, built** (`compiler-engineer.md`): route A's message, a special case keyed to
  `slice`'s label table, tells `slice(from: 0)` as `wrong_arity` and `line.slice(from: 0)` as
  `builtin_shape`, one mistake two codes, leaves `slice()`, `slice(line, from: 0)`, `slice(0, 5)` and
  26 other built-ins as today, +58 lines and `check/walk.hero` 4 over its `DECIDED` row. **Route E**
  tells every short or long count of the 27 fixed-count built-ins as `wrong_arity`, the receiver
  counted, with a note giving the call's shape where the first position is unlabelled, in the rule a
  declared function already follows; built as **E2**, +75 lines, the `@`-marker rule moved out of
  `walk.hero` along the seam it shares so `walk.hero` reads 1862 and no row moves; `check` 704 / 0,
  `full` 41 / 0, `permissive` 22 / 0, `fixes` 1004 / 0, `annotations` 1196 / 0, 1740 tests; four
  existing cases change their code (defects 458's and 439's, whose headers already say *the count is
  the built-in dispatch's alone to tell*). **The width note**, built on E2 as E4, +12 lines: an
  integer of another width told with `` `to_u64(x)` converts it to `u64?` `` and no fix, the
  conversion §4.17 uses as its own example; the seed builds E4 and stage 2 and stage 3 emit the
  same C, 26,759,615 bytes. **The census, run by the critic**: on all 4,098 tracked `.hero` files E4
  gives the exit code today's compiler gives, 1,904 at 0 and 2,194 at 1; only the four moved
  goldens differ, in their code.
- **Which arm sees it**: `wrong_arity` and `builtin_shape` are not thesis rules
  (`selfhost/diag.hero:192-228`), so under either route strict and control print the same one
  message; the routes remove a strict-only `wrong_label` and add no thesis code. The instrument that
  can see a note is **strict minus brief** (`--brief` drops notes, `thesis_grade.hero:114-115`),
  already printed by `report` as exploratory (`thesis_report.hero:375`), not strict minus control.
- **Precedent** (`historian.md`, every line its URL, read 2026-10-21): Python, Kotlin, Lua, Nim and D
  write a method-like built-in with its receiver or first argument; Swift's own test pins *missing
  argument for parameter #1* with no label error on the line, the shape of 2175; Rust tells `core`'s
  `Ok()` with the code and suggestion a user's function gets, through one general argument matcher
  (rust-lang/rust#92364, after #71827 was closed), route E's shape; Go and CPython keep separate,
  thinner messages for their built-ins; Rust tells an `i64` where a `usize` is asked with *you can
  convert*, the width note's shape; Go, Java, Kotlin and Swift chose signed counts, route D's shape,
  and Swift's book gives the reason, to avoid converting.

## Verdicts

| seat | A (sentence) | A (message) | B | C | D | E |
|---|---|---|---|---|---|---|
| compiler-engineer | no objection | **object** (§1.7: two codes for one mistake) | object | object as framed, approve with E's rule | object (§1.12, panel 054) | **approve**, as E2 |
| spec-warden | **approve**, on conditions | — | **veto** (Principle 0, +296 vendored, 20 of 22 entries unmeasured) | no objection | object | no price objection |
| historian (advisory) | approve | carried by E's path | — | — | precedent both ways | precedent for |
| llm-ergonomist (the experiment) | approve the `repeat` half; no reading on `slice` | — | — | — | — | — |

## Disagreements, stated plainly

- **A's message against E's rule.** The compiler-engineer measured A's trigger telling one mistake two
  ways and E's telling it one way for every callee; the historian found precedent for the general
  path (Rust's matcher, Swift's positional message) and the spec-warden has no price objection to
  E. Nobody argues for A's message over E's. Settled for E.
- **The spec-warden's three corrections against the critic's first pass**, re-run by the critic:
  10 and 9 receiverless `slice` answers are both right (written, and refused for it at turn 1); the
  three `to_i64` refusals are an `i64?` used as an operand, not a `to_i64` on a `str` (the critic's
  first pass right, the spec-warden's +7 remedy pricing a fault no answer committed); `all` and `ok`
  are bindings, and `ok` is not in the sentence (the spec-warden right, the shared brief wrong);
  and of the five padding repairs at turn 2, four wrote `to_u64(N - s.len()).must()` and the fifth
  replaced the call with a loop (the critic, correcting the spec-warden's *all five*).
- **The class of the two blocking candidates** (below, R8) turns on readings the reports leave to the
  coordinator, and the coordinator reads both as `blocking`.

## Resolution (provisional: author ratification pending)

**R1. The sentence takes route A's bytes** (closes 2174's sentence): `xs.slice(from:, to:)` and
`repeat(s, n)` (`n` a `u64`), +13 real, +10 vendored, in one commit with ledger row 115, the pins
(`LEDGER_ROWS`, `SPEC_TOKENS`, `SPEC_REAL_TOKENS`, `SPEC_REAL_TAKEN`, `SPEC_DIGEST`) and the real
figure `measure --refresh` reads on the landed bytes. **Paid by a registered prediction**, the
spec-warden's P1 and P2 written into row 115 with the critic's two repairs: scored at the first
held-out run on **the non-trap stratum**, the trap stratum reported beside it, since the author
writes the traps after reading this sitting; and P1 **per count class**, the padding class (`N -
s.len()`) and the parsed-count class each with its own threshold, so the mix the author chooses does
not move the rate. **Why the milestone page's bar** (*repeats across two families or tiers and on
held-out tasks*, which the spec-warden applies to `to_i64` and `len`) **does not stop the `slice`
half**: its shape in the sentence was false, a call the compiler refuses when copied, so it is a
correction of the specification's text and not a rule bought for a reader's slip. The removal branch
is empty (no fact of §§ 8 to 11 is stated twice; the merges cost more).

**R2. The bare names in the sentence are values; a label is written with its colon.** `xs`, `s`,
`n` and `sep` stand for the arguments, and `from:` and `to:` are labels, the notation the
specification already uses (§ 9's `copy(from: a, to: b)`, § 11's `range(from: a, to: b)`). So **a built-in's unlabelled position takes no label**, and the
repair of filing (b) below refuses a label at a position the built-in does not name, as a function
value's label is refused (`label_on_function_value`, panel 128), with a `certain` removal of the
label. Decided here because the sentence and that repair answer the same question (the critic's
question 1): naming those positions in the inventory instead would make `repeat(s: "ab", n: 3)` a
second spelling of the call, and 0 of the 198 programs of this sitting's data (90 turn-1 answers, 96
later turns, 12 blind) wrote one.

**R3. A built-in's count is told before its labels, as a declared function's is: route E, as E2**
(closes 2175). One rule in `selfhost/check/builtin_args.hero` (with the §4.8 `@`-marker rule moved
beside it), the counts in `inventory.arity_of` held by a test to every name of `NAMES`, `wrong_arity`
for every fixed-count built-in given another count, the receiver counted, and the note
`` `slice` is called `xs.slice(from: a, to: b)` or `slice(xs, from: a, to: b)`, the value it works on
first `` where the first position is unlabelled. **The arguments are still walked**, so a mistake
inside one is told beside the count. No `Fix`: the value sliced is not in the call.

**R4. A declared function walks its arguments behind a wrong count too** (filing (c), `adjacent`,
repaired inline in the same files, `.claude/rules/verification.md` § Bounded discovery): without it
route E would make a Tier-1 built-in tell `cut(from: "a" + 1, to: 5)`'s shape with both mistakes and
a declared function with one, the reverse of what §1.7 is cited to remove (the critic's § 4c). The
three lines in `user_call` the compiler-engineer names, and a case.

**R5. An integer of another width is told with its conversion**: E4's note in
`check/mismatch.hero`, worded true of a widening, replacing panel 209's width-class notes rather than
appending to them; no fix, since whether a value that does not fit aborts or defaults is the
author's.

**R6. Route B is vetoed** (the spec-warden: Principle 0's burden unmet for 20 of its 22 entries,
+296 and +298 vendored, six times `DELTA_GATE`, and the critic's G9 measured: a typed fragment turns
`spec/offered` red). **Route D is refused**: panel 054's ruling stands (design.md §1.12; 52 lines
already pass a `to_u64` count, 36 in `selfhost/`; a negative count would become a silent `""` or a
misleading panic inside the primitive). **What conservative would have been**: route C with E's
rule, the specification untouched, 0 tokens; it leaves the sentence's false `slice` shape and the
`repeat` type the blind readers got wrong 6 of 6 times.

**R7. Moving a mistake between two existing codes is a diagnostic-class change**: `check --json`
publishes the code (design.md §4.17's schema 2), every golden's `#~` mark is one, so `builtin_shape`
→ `wrong_arity` on a count is a change to the language's public vocabulary of mistakes, and comes to
a sitting as this one did. The compiler-engineer's reading is adopted beside it: a new code naming a
mistake no existing code names is a new class; a reworded message or a new note under an existing
code is not.

**R8. Filed, each with its reproducer, none widening this sitting's items:**

- (a) **`needs_label`'s `certain` fix writes a label the call already holds**, defect 2183 (`cut(from: 0, to:
  5, line)` takes `fix (certain): write \`to: \``, and `--apply` writes `to:` twice): **`blocking`**,
  by defect 135's rule (*a certain fix with two readings is a guess*): the third argument reads as
  `to` or as the missing first argument written last, and the `certain` tag claims one reading.
- (b) **A label on a built-in's unlabelled position is accepted**, defect 2184, wider than the compiler-engineer
  probed: a label written twice (`slice(to: line, from: 0, to: 5)`) and labels swapped
  (`repeat(n: "ab", s: 3)`) check at exit 0 where a declared function refuses each: **`blocking`**,
  *a wrong one accepted*, a source whose labels state the opposite of what it computes; its repair's
  direction is R2's.
- (c) **A declared function given the wrong count hides a mistake inside an argument**, defect
  2185, `adjacent`, repaired under R4.
- (d) **An `i64?` used as an operand draws `bad_operand … found \`i64?\`` with no note**, defect 2186, where the
  same value in an annotated binding draws `fix (guess): .must()` (the trial's three `to_i64`
  refusals): `adjacent`, a true message less exact than it could be.

2183 and 2184 are repaired before the freeze (R9), in the milestone's next work; 2186 waits by its
class.

**R9. Everything this sitting changes lands before the freeze.** The run's provenance carries the
specification's sha and the compiler's `sources` (`thesis_run.hero:777`), and `report` sets each
against the freeze's preregistered block field by field (`tests/thesis/held-out/README.md`), so the
sentence, route E, the width note and the repairs of R8 change what the freeze pins: before it, or a
row of the run's deviations.

**R10. The case for this resolution is the specification's truth and §4.17 per message, not
Part 11's primary**, which no route moves on 050's data; the effect a note can have is read by strict
minus brief, exploratory. The compiler-engineer's harness prediction (strict and control within one
sample on a receiverless `slice` at turn 2) is kept as stated and recorded as near-vacuous under R1.

## Predictions to score

- **P1** (`repeat`, row 115's payment): at the first held-out run, under the `spec` context with R1's
  sentence, on the non-trap stratum, of the turn-1 answers calling `repeat`, at most half refused at
  its count pooled (13 of 13 in 050), Sonnet 5.5 and Opus 5.5 together at most a quarter; each count
  class scored on its own where it holds at least 10 answers, else `lapsed`. If the padding class is
  refused at more than twice the parsed-count class's rate, `len` (an `i64`) goes to a sitting with
  that number (four of six blind readers already named it unstated). Scored at M-thesis-harness's
  first held-out run, `lapsed` at its close if that run has not happened.
- **P2** (`slice`, row 115's payment): at the same run, non-trap stratum, of Haiku 5.5's turn-1
  answers calling `slice`, at most 10% with nothing to slice (10 of 26 in 050), Sonnet 5.5 and Opus
  5.5 at most 2%; scored if at least 20 Haiku answers call it, else `lapsed`.
- **P3** (price, unpaid): the landing's `measure spec/heroes-spec.md --refresh` reads `real 10299`.
- **P4** (lines, the compiler-engineer's): R3 lands at +75 ± 10 `code_lines` with `check/walk.hero`
  at or under its row and no `DECIDED` row raised. Scored at the landing commit by `layout`.
- **P5** (exploratory): strict minus brief on green@2 for samples whose turn 1 holds a built-in's
  wrong count or an integer of another width is reported at the first held-out run, with no
  threshold.

## At the landing, 2026-10-21

The compiler half (R3, R4, R5) landed in the lane `thesis` over the trunk at `7d92b2619`, the
compiler seat's E4 tree applied whole and defect 2185's repair beside it, **gated as one tree**,
which answers the critic's gap 5 (the combination never run together): the two-stage build from
the seed and its fixpoint, stage 2's and stage 3's emissions `cmp`-identical at 26,763,178 bytes;
the compiler's own tests 1740 passed; the net's own tests 416 passed; the full net, 32 suites,
8929 passed and 1 failed, `records/verdicts`, this sitting's decision not yet filed; the census of
`check --brief` over the 4,099 tracked `.hero` files, the trunk's compiler against the lane's,
0 exit codes differing. `check/walk.hero` reads 1870 against its row of 1870 with R4's three
lines, its predicate `typed_alone` in `check/waiting.hero`, every case of `ExprKind` named. R1, the
sentence, is its own commit, asked of the author before it is made, since its push publishes the
site.

**Added 2026-10-22**: R1 landed at `f34f944c3` on the author's yes to publish it, `measure --refresh`
reading real 10,299 (P3 a hit); R8's two `blocking` filings were repaired at `be9009cac`, defect
2183 (a label the call holds already is a guess, with a note, at all five kinds of call) and
defect 2184 (a label at a built-in's unnamed position is `wrong_label`, dropped `certain` where it
names nothing and a guess where it names another position), the compiler's own test
`"buffer-crossing ".repeat(count: 512)` in `selfhost/cli/io.hero` the one such label in the tree,
found by the gate's stage 3; the full net 8937 passed and 0 failed, and the CI green on its four
platforms. So everything this sitting changes stands before the freeze, as R9 asks.

## Author's verdict

Pending: the decision issue `Panel 227:` holds the ratification.

## Process notes

- **The critic read the briefs first, and the seats were spared their errors**: a line number, a
  count of 9 that was 7 alone, two samples said green that never were, `count_tokens` figures with
  no stored answer and in another unit than the ledger's (re-measured and kept, 10,286 and 10,299),
  and route E, the route this sitting adopted. The briefs carry § Repaired.
- **The blind seat moved from Sonnet to Haiku** on the critic's G1 (Sonnet never made the `slice`
  mistake), three sessions per sentence inside the author's 2 × 3 USD; spent 0.1649 USD in all.
- **A hard stop outranked the panel skill's letter**: the blind folders are inside the root
  (`build/w/N`), where the skill puts them in the scratchpad; the default system prompt still
  carried git, and `--system-prompt` with `--bare` was what made the sessions blind. The skill's
  blind-seat paragraph is stale beside the hard stop and is the author's to amend (CLAUDE.md § 4: the
  skills are amended by author instruction).
- **Writes outside the root, each found and removed**: the spec-warden wrote three helper scripts in
  the session's scratchpad and removed them a minute later, said first in its report; the fetch tool
  saved two PDFs and seven page images of the historian's reads under the session's `tool-results/`,
  removed by the coordinator with two earlier PDFs of panel 226's; the critic's reading of a report
  was persisted there by the harness, removed. What stays there is the harness's own cache of long
  tool outputs, as the transcript is.
- **The llm-ergonomist's verdict was written by the coordinator, who proposed route A**; its numbers
  are mechanical and the critic re-ran each, and its one wording from the brief's framing (*a
  function named `all`*, a binding) was corrected before this sitting was committed.
- **Route E came from the critic's first pass**, so the critic's second reading of it is not
  independent, as it says; the compiler-engineer built and gated it.
