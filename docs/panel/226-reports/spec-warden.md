# Panel 226, the spec-warden's report

Written as the sitting ran, 2026-10-20, in the copy `<scratchpad>/226-spec-warden/` (a `git archive`
of `a0a8d1249`, 12,801 files by `find . -type f | wc -l`). Every count below names its command and the
model it was taken on; nothing is recalled from a document. The key was read with `set -a; .
<trunk>/.env; set +a` and sent with `curl --variable %ANTHROPIC_API_KEY --expand-header "x-api-key:
{{ANTHROPIC_API_KEY}}"`; it was never printed. The one endpoint called was
`POST https://api.anthropic.com/v1/messages/count_tokens`, 110 times, free.

- `verdict`: **approve**, conditions below (no spec token is spent; the ceiling is not approached;
  Principle 0's burden is carried by the record itself). Counts are **measured**, not estimated.
- `section`: design.md §1.6 (the budget, its two rules of 2026-10-14, and the panel-011 sentence at
  lines 385 to 387), §1.2 (the formula), §1.1 (what the instrument measures), CLAUDE.md § 2.
- `spec_token_delta`: **0**. `spec/heroes-spec.md` before and after the proposal is the same file:
  10,286 real tokens on `claude-opus-5` (10,292 answered, wrapper 6), which is `REAL_TOKENS`
  exactly; 10,286 + the FFI floor 60 = 10,346 against 16384; headroom 6,098 (`heroes-next measure`).
- `removal`: nothing from the spec, and that is **not** a problem here, since the proposal spends no
  spec token. What must come out is in design.md: Part 11's *single-turn* sentence of item 2
  (superseded, kept as history under it, R12); §1.6's *stops being an audited one (panel 011)* at
  lines 385 to 387 (amended by the landing of the first run's measurement, § 3 below); and §1.6's
  phrase *an ablation by section* (amended to what the run ablates, § 5 below).
- `needed_for_self_hosting`: **no**. Nothing in `selfhost/` needs it; it enters by §1.1.
- `argument`: the spec is the prompt and the prompt was never priced as what the model receives; it
  is now: 10,690 content tokens rendered, of which 10,286 the spec, 321 a 209-word task, 83 the
  template's words and tags; the `system` layout costs 0 more and caches the 10,360-token head, so
  the spec is under a tenth of a Sonnet run's cost and output is the driver (§ 4). The instrument is
  the only thing that can ever collect a registered prediction, six ledger rows having named metric
  2 and none collected (§1.6); it carries Principle 0's burden by that fact. Its text must say what
  §1.2's two factors are read from (§ 3), and §1.6 must stop promising an ablation by section the
  run does not make (§ 5).
- `prediction`: **the strict arm's measured correction round-trip, one full diagnostic fed back plus
  one whole rewritten program, counted on `claude-opus-5` through `count_tokens`, has a median
  between 700 and 1,300 tokens over the held-out tasks on Sonnet 5.5**: inside §1.2's 500 to 2,000,
  and the first time that constant is read rather than asserted. Under 500 and §1.2's second factor
  is cheaper than the document says; over 2,000 and a rewrite is not one program. Beside it: the
  full diagnostic fed back reads a mean of 150 to 300 tokens and `--brief` under 100 (ten shortest
  today: mean 188, 70 brief); the spec's cache-read share of the run's USD on Sonnet 5.5 is under
  10% at 1,500 output tokens and under 4% at 6,000.
- `condition`: **object** if the landing pools the user layout and the `system` layout under one
  prompt sha (§ 2), or if Part 11 keeps *ablation by section* as the next raise's instrument while
  the run ablates the productions alone (§ 5), or if §1.2's *program tokens* are read from
  `usage.output_tokens` with thinking inside them (§ 3). **Veto** if the milestone spends a spec
  token without a ledger row; it spends none by design.

## The instrument, built here

- `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`: `real 8.60`, exit 0, `heroes 0.4.0`,
  `sources 5f45d6514b22d650`.
- `./heroes build selfhost/main.hero -o heroes-next`: `real 83.35`, exit 0, `heroes 0.4.0`, `sources
  7792ecfe50a531e8`. Stage 2 is the judge, as `00-shared.md` asks; both keys match the brief's.
- **Today's ceiling, by grep**: `grep -n 'must fit in' docs/design.md` → line 255, *must fit in 16384
  tokens, measured by `claude-opus-5` through `POST /v1/messages/count_tokens`*. `CEILING` in
  `tests/harness/suite_spec.hero:156-157` is 16384; `FFI_FLOOR` (286-287) 60; `REAL_TOKENS` (170)
  10286 on `REAL_MODEL` `claude-opus-5`, `REAL_TAKEN` 2026-10-18; `SPEC_DIGEST` `31ee4e9b4a253a37`.
- The spec today: 470 lines, 26,601 bytes, sha256
  `212264059c985c2e0cff8deb927894186fbcdaff60028897729611e3b3e8b45d` (`wc`, `shasum -a 256`); 46
  production lines (`grep -cE '^    [A-Za-z]+ += '`). The frozen template `docs/metrics/first-try.md`:
  573 bytes, sha256 `b7f06aab65cd9790483d5a4da1790b7eb211e3fcb0c2a5462424fde479da706b`. Nothing in
  the tree renders its placeholders today (`grep -rn '{{SPEC}}' selfhost tests docs` finds only the
  template and its README).
- How `real` is derived (`selfhost/cli/refresh.hero:144-181`): the request is one user message
  (`count_tokens.request`, `selfhost/cli/count_tokens.hero:45-46`); the wrapper's cost is probed in the
  same invocation with `x` and `x x x x x`, the second exactly four above the first or nothing is
  subtracted; `real` = the document's answer minus (the `x` answer minus 1). Reproduced per id:

  | id | `x` | `x x x x x` | wrapper |
  |---|---|---|---|
  | `claude-opus-5` | 7 | 11 | **6** |
  | `claude-haiku-5-5` | 9 | 13 | **8** |
  | `claude-sonnet-5-5` | 9 | 13 | **8** |
  | `claude-opus-5-5` | 9 | 13 | **8** |
  | `claude-fable-5-1` | 9 | 13 | **8** |

  The spread is 4 on every id, so `x` is one token everywhere and the subtraction is licit.

### The STALE the brief saw is the seed's, not the spec's

`00-shared.md` says *`heroes measure` prints `STALE`, the ledger's last real count being 10,286 on
`claude-opus-5` (2026-10-18) for an earlier digest, so today's 10,292 is +6 on a spec that moved since
and the ledger owes a refresh.* Run here, both compilers on the same file:

- seed-built `./heroes measure spec/heroes-spec.md`: `STALE: the recorded count is for
  589b7195235b8e5d and this file is 31ee4e9b4a253a37.`, exit 1;
- stage-2 `./heroes-next measure spec/heroes-spec.md`: `real 10286 claude-opus-5, 2026-10-18 — the
  binding number`, `Headroom: 6098 against the 16384 ceiling`, exit 0, no STALE.

The seed's `selfhost/measure/pinned.hero` predates the tree (the seed is refreshed only when the
two-stage build goes red, M-bounded-seed's rhythm), so its pin is an older digest's; the tree's pin
IS today's digest. And 10,292 is the wrapper-inclusive answer, 10,286 the wrapper-exclusive pin:
10,292 − 6 = 10,286. **The spec has not moved a real token since 2026-10-18, and the ledger owes
nothing.** What is owed is the rule `00-shared.md` itself states, judge with stage 2, and a correction
under that sentence of the brief. (`.claude/rules/verification.md` § The compiler that judges is a
build artifact is this exact shape, a stale binary's true sentence pointing away from its cause.)

## 1. What the protocol sends, counted

`work/count.sh <model> <body.json>`, one `POST /v1/messages/count_tokens` each, 2026-10-20 between
02:40 and 02:55 CEST; bodies built by `work/bodies.py` from the copy's files and `work/task.md`
(209 words, 1,134 bytes, a word-count program: arguments in, `print` out, exit 2 on no arguments).
The table gives the endpoint's `input_tokens`; **content** is that less the id's wrapper, and it is
**the same number on all five ids** in every row: the four 5.5/5.1 ids and `claude-opus-5` tokenise
this content identically, the only difference being the wrapper (8 against 6).

| body | how it was built | `claude-opus-5` | the four 5.5/5.1 ids | content |
|---|---|---|---|---|
| `spec-alone` | the spec as one user message | 10292 | 10294 | **10286** |
| `task-alone` | the task as one user message | 327 | 329 | **321** |
| `rendered-user` | the template's body (everything below its `---`), `{{SPEC}}` and `{{TASK}}` substituted, one user message | 10696 | 10698 | **10690** |
| `system-head-as-user` | the head alone (instructions + `<specification>…</specification>`), as a user message, to decompose | 10366 | 10368 | **10360** |
| `system-cached` | **the cache layout**: `system: [{type: text, text: <head>, cache_control: {type: ephemeral}}]`, user message `<task>…</task>` | 10696 | 10698 | **10690** |
| `spec-noprod-a` | the spec less its 46 production openers and the 21 continuation lines under them (every line indented five or more spaces following an opener; `work/removed-productions.txt`, all 67 grammar, none prose), doubled blank lines collapsed | 8575 | 8577 | **8569** |
| `spec-noprod-b` | `a` less the two sentences introducing Wirth's notation (*A production is written…* to *…come from the indentation.*); the continuation rule stays | 8423 | 8425 | **8417** |
| `spec-plus-diag` | the spec, a blank line, and `never-rebound-near-miss-says-how-a-cell-is-declared.expected` inside a fence, no other word (issue 1005) | 10470 | 10472 | **10464** |
| `program` | `tests/golden/run/a-field-name-comes-from-the-field.hero`, 55 lines, 1,925 bytes | 738 | | **732** |

What the decomposition says: the template's own words and tags cost **83** tokens (10,690 − 10,286 −
321); the head's instruction and `<specification>` tags **74**; the `<task>` wrapper **9**. **Moving
the spec into a `system` block with `cache_control` costs 0 tokens on every id**: `input_tokens` is
identical to the single-user-message rendering. **The productions cost 1,717 tokens, 16.7% of the
spec** (1,869, 18.2%, with their notation sentences); panel 133's prediction 3 now has the price of
the thing it predicts about. **One worked diagnostic costs 178** inside its fence (173 as bare text,
below). The task is 3% of what the model reads.

The ten shortest `.expected` of `tests/golden/full/`, each as one user turn (a diagnostic fed back):

| turn | file | bytes | `opus-5` | content |
|---|---|---|---|---|
| 01 | `fixedbugs-289-a-column-counts-characters` | 242 | 113 | 107 |
| 02 | `fixedbugs-289-a-control-character-is-written-by-its-code` | 358 | 175 | 169 |
| 03 | `never-rebound-near-miss-says-how-a-cell-is-declared` | 419 | 179 | 173 |
| 04 | `fixedbugs-289-each-excerpt-its-own-gutter` | 435 | 201 | 195 |
| 05 | `fixedbugs-289-a-note-and-a-certain-fix` | 443 | 190 | 184 |
| 06 | `fixedbugs-414-an-open-backtick-is-told-with-the-rest-of-its-line` | 469 | 198 | 192 |
| 07 | `inferred-cell-width-class-names-its-birth` | 475 | 205 | 199 |
| 08 | `fixedbugs-289-a-note-and-two-guesses` | 482 | 204 | 198 |
| 09 | `fixedbugs-532-a-note-names-a-type-with-no-article-chosen-by-hand` | 499 | 210 | 204 |
| 10 | `fixedbugs-271-a-fix-away-from-its-message-s-line-says-where` | 578 | 264 | 258 |

Mean **188** content tokens (min 107, max 258), the four 5.5/5.1 ids +2 each. And the three arms on
turn 03's program, `heroes-next check` on
`tests/golden/full/never-rebound-near-miss-says-how-a-cell-is-declared.hero`, outputs counted on
`claude-opus-5`: **full 173, `--brief` 70, `--permissive` 173** (exit 1 all three; `unknown_name` is
not a thesis rule, so the control reads as the strict arm). R4's third arm saves 103 input tokens
per correction turn, 2.5×, which is the price of the note and the certain fix.

## 2. The prompt's identity

Same words, different shape: `count_tokens` answers the same `input_tokens` for `rendered-user` and
`system-cached` on all five ids, so the layout moves no token, and the template's bytes are unchanged
(sha `b7f06aab…`). But the template says of itself *Single turn … Any change to this file is a new
prompt sha and a new measurement series*, and the proposal makes it the turn-1 row of a five-turn,
three-arm, `system`-plus-user conversation with R5's fixed feedback sentences: **the words the model
receives after turn 1 are not in this file at all**, and a model reads a `system` block with a
different weight from a user turn, which no count can see. So:

- **it is a new prompt series, and that is free today**: metric 2 has never run (`docs/measurements/007:24-25`,
  issue 816), so no series exists to break. Amend `docs/metrics/first-try.md` before the freeze, in one
  commit: multi-turn, the `system` layout, R5's feedback sentences as named placeholders, and the
  turn-2 shape; its new sha is the series' sha. Do not run the trial on `b7f06aab…` and the held-out on
  another and call them one series.
- **`prompt sha` in provenance covers two things, named apart**: `template sha`, the sha256 of the
  template file with its placeholders in place (the words); and `layout sha`, the sha256 of the request
  body's skeleton with `{{SPEC}}`, `{{TASK}}` and the feedback placeholders unsubstituted, roles,
  `cache_control`, `system`/`messages` structure included (the shape). A run differing in either is a
  different series; the Part 11 rule *never diff runs with different compiler shas unless flagged*
  extends to both. The rendered bytes per request are not a third sha: they differ per sample by
  construction and are what the transcript holds.

This is §1.6's own lesson one level up: *the number carries its instrument*. A count without the
shape that produced it is the 998-token error again, this time on the prompt rather than the spec.

## 3. §1.2's formula and the ledger

`real cost = program tokens × (1 + rewrite rate)`; §1.6 at lines 385 to 387: *only the first is now
measured. The rewrite rate needs a model in the loop, so until metric 2 runs the formula remains the
design rule it always was and stops being an audited one (panel 011).*

**How the columns map.**

- **program tokens** = the extracted program of turn 1, counted through `count_tokens` on the pinned
  id (free, 732 for a 55-line program today), **never `usage.output_tokens`**: thinking is billed as
  output and cannot be switched off on three of the four models (`00-shared.md`), so the API's output
  figure is program plus thinking and measures §1.2's first factor on nothing. The instrument counts
  the code block apart and records both. The proposal's provenance line lists *usage* and does not say
  this; it must.
- **rewrite rate** = mean(turns-to-green − 1) over the samples that converge, per arm, the
  non-converged reported apart (censored at 5, so the mean is a lower bound, and the control arm is
  where censoring bites, R2). In §1.2's own unit it is also *tokens-to-green over turn-1 program
  tokens, minus one*: the template asks for the whole program each turn, so a rewrite is a whole
  program and the two readings should agree. Report both; a gap between them is a model that did not
  rewrite the whole program, which is a format slip (R5's bucket) and not a rewrite.
- **§1.2's constant**, *a correction round-trip costs 500 to 2,000 tokens*, becomes a column for the
  first time: feedback in (173 full, 70 brief, the run sentence) plus the rewrite out (about 740) plus
  the re-sent transcript, whose spec part is a cache read. My prediction above is that column's
  median. The *re-reads a file* half of §1.2's sentence is the cached head: 10,360 tokens per turn at
  the cache-read price, which is why the spec's size stops being the cost (§ 4).

**Does the panel-011 sentence come out?** Yes, **amended, not deleted**, on the day the first run's
measurement file lands: *…until metric 2 runs the formula remains the design rule it always was and
stops being an audited one (panel 011). **Metric 2 first ran on YYYY-MM-DD (measurement NNN): the
rewrite rate read X on Sonnet 5.5 strict and Y control, the round-trip's median Z tokens; the
formula is audited from that day.*** That is a Part 1 change, which CLAUDE.md § 4 sends to a panel;
this sitting is the panel, and should pre-authorise the sentence's shape with its numbers left to the
landing, so that the record is not false for the weeks between the run and the next sitting.

**The ledger owes no refresh**: today's real count is 10,286 = `REAL_TOKENS` (above). The brief's
*+6* compared the wrapper-inclusive answer with the wrapper-exclusive pin. The proposal spends no spec
token, so no ledger row is owed either; `heroes measure` would read STALE only under the seed-built
compiler, and gates judge with stage 2. One observation for the record, dated: on 2026-10-20 the five
live ids count this spec's content identically (10,286), where panel 123 found 967 between two
generations in 2026-09; the binding id stays `claude-opus-5` by rule (panel 123 R3) and nothing moves.

## 4. The budget arithmetic

`work/budget.py`, on the content counts above and `00-shared.md`'s prices, **every one the
`claude-api` skill's cached figure of 2026-10-06, a document and not a measurement**; the skill
itself is under no path this seat may read, so two of its facts stay **unread** and are carried as
marked constants: Haiku's cache-read price (assumed 0.1×) and whether a prior turn's thinking blocks
are billed as input (both ways computed). Other marked assumptions: one cache write per model per
phase (sequential pacing inside the 5-minute TTL), the run-mismatch feedback 100 tokens, a program
740, the three arms forking from one turn-1 generation, every arm to the same depth; the 100K Haiku
tier is never reached (turn 5 with thinking billed is about 32K of input). Per model, USD:

| phase | answers 1,500, 5 turns | 1,500, 3 turns | 6,000 (thinking), 5 turns | 6,000, 3 turns |
|---|---|---|---|---|
| trial 10×3, Haiku + Sonnet + Opus 5.5 | 25 to 29 | 12 to 14 | 79 to 107 | 41 to 50 |
| held-out 40×3, four models | **257 to 301** | 127 to 140 | **822 to 1,127** | 431 to 522 |
| of which Fable 5.1 | 157 to 184 | 77 to 86 | 508 to 697 | 266 to 323 |
| canary, `none` context, turn 1, 40×3×4 | 15 | 15 | 59 | 59 |
| variant no-productions, 10×3, Haiku + Sonnet | 9 to 10 | 4 to 5 | 27 to 37 | 14 to 17 |
| variant +diagnostic, 10×3, Haiku + Sonnet | 9 to 10 | 4 to 5 | 27 to 37 | 14 to 17 |
| **total** | **315 to 366** | **163 to 178** | **1,014 to 1,367** | **559 to 666** |

The range in each cell is thinking not billed to billed as input. One held-out conversation to turn
5 is 13 requests: 134,680 cached-in, 29,920 fresh-in, 19,500 out at 1,500; the held-out run is 6,240
requests. Three things the arithmetic says that the proposal's text does not:

- **The spec is not the cost.** Cached, its 10,360-token head costs 0.027 USD per Sonnet conversation
  against 0.06 fresh input and 0.195 output: under 10% at 1,500 output tokens, under 4% at 6,000.
  §1.6's budget is a comprehension discipline, not a budget of money; a raise argued from this run's
  cost would be arguing from the wrong column.
- **Output is, and Fable 5.1 is 61% of the held-out run** for a column the proposal itself calls
  exploratory (everything but the Sonnet primary). 508 to 697 USD at 6,000 output tokens buys no
  primary. § 6 says what to do with the word *ceiling*.
- **The canary in `none` on Fable costs 9 to 36 USD** for 120 turn-1 answers whose purpose is the rule
  of three; Haiku and Sonnet give the same canary for under 10. Not this seat's call, priced here so
  the ergonomist's is.

## 5. §1.6's consolidation sentence

The sentence (design.md lines 291 to 295, `grep -n "consolidation block, spend at most"`): *the rows from M-buildable-structs to
M-compatibility-promise, the consolidation block, spend at most **1,500** real tokens before the
thesis harness has run, and an addition past them waits for it; and the next raise rests on what
that harness measures a token of specification to buy, an ablation by section, never on the document
having approached the ceiling.*

**Measured today**: the block has spent **238** of 1,500 (10,286 − 10,048, the figure the day the
number was set; `docs/ROADMAP.md:23` says 238 and my count agrees). Rows 66 to 69 are done; row 70
is the harness itself and spends 0 by this proposal; rows 71 to 74 come after it has run.

**The first run changes the sentence in two ways, and only the second needs this sitting.**

1. *Before the thesis harness has run* is an event clause, the JEP-12 shape §1.6 itself prefers to a
   clock. When M-thesis-harness closes it is **discharged**, and the record should say so under the
   sentence, dated: *the harness first ran on YYYY-MM-DD with the block at N of 1,500*. It does
   **not** release the 1,262 unspent as headroom: the payment rule stays unconditional at every
   level, and *an addition past them waits for it* becomes *an addition past them rests on what it
   measured*. Nothing to decide here beyond writing the date when it comes.
2. *An ablation by section* **is not what this run makes.** The proposal's variants (R6) are two:
   the spec without its productions (1,717 tokens, panel 133's prediction 3) and the spec with one
   worked diagnostic (+178, issue 1005), on ten tasks, Haiku and Sonnet. That is two points on the
   curve of what a token of specification buys, not thirteen sections' worth, and a per-section cut
   is confounded anyway: a task that needs § 6 cannot be solved without § 6, so a section-wise
   ablation needs a task suite designed per section, a design problem and not a 30-to-120 USD one.
   **Amend the phrase** to *the ablations that harness runs, the productions' first (panel 133),
   then by section as a task suite allows*, so the sentence promises what the instrument delivers
   and the next raise waits for a curve with more than two points. Left as written, the first raise
   request after the run will cite an ablation nobody made, which is this seat's whole history.

No raise is on the table, and this seat would refuse one: 6,098 free, output and not the spec is
the cost, and the first curve has two points.

## 6. Principle 0

The instrument enters by **§1.1**, *LLM comprehension is the objective; operationally, the percentage
of programs that compile on the first attempt*, and by Part 0, *build the measurement harness
early*. Its burden under CLAUDE.md § 2 is a measured Part 11 effect, and it is the thing that
measures them: every *provably serves the thesis* admission since panel 012 rests on an instrument
that has not run, six ledger rows were bought by predictions naming metric 2 and none has been
collected (§1.6), and panel 133's 3 and 4 and issue 1005 are open for the same reason. **The
amendment carries its own burden by that record**: without it the second branch of Principle 0 is
unreachable and the registered prediction, the payment branch that has produced nothing, cannot
start producing.

What I would strike or amend in the proposal's paragraph as **measured by nothing**:

- *Fable 5.1 **as ceiling***: *ceiling* is a claim the run tests, not a fact it rests on; strike the
  two words. The model stays a column, exploratory, and the sitting should weigh 61% of the held-out
  run against one sample or turn 1 alone for it (§ 4).
- *what the most common permissive language does*: *most common* is a claim about the world; a row
  names its language (*as C does*, *as Python does*) or says *a permissive language*, CLAUDE.md
  § 11's narrowing rule. The compiler-engineer's table, this seat's words.
- *three outcomes named before the run (confirmed, falsified, inconclusive)*: named, not numbered.
  The threshold and the test's level live in the freeze; the paragraph must say the freeze is hashed
  into `suite sha`, or the pre-registration is a sentence nobody can check.
- *40 author-written held-out tasks … 3 samples each* beside *n from the trial's measured
  discordance*: two sources of n, one of them false on the day. Write *at least 40 × 3, raised by
  the trial's discordance and ICC, never lowered*.
- *a non-Anthropic model through its API when a key exists*: a conditional with no instrument
  today; keep it as a provision outside the primary, or strike it until a key exists.
- Add, since §1.2 is otherwise still unmeasured after the run: *the program's own tokens counted
  through `count_tokens` on the pinned id, apart from the output tokens billed* (§ 3).

## Files

- this report: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/thesis/docs/panel/226-reports/spec-warden.md`
- the work, in the seat's copy and nowhere in the lane:
  `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/7e9f826e-648d-462d-93c0-58de3463ac70/scratchpad/226-spec-warden/work/`
  (`count.sh`, `bodies.py`, `task.md`, `budget.py`, `counts.tsv`, every `*.json` body and `*.txt`
  text, `removed-productions.txt`, `full.out`, `brief.out`, `perm.out`, `build.log` one level up).
