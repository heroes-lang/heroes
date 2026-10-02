# Panel 187, the spec-warden's report

Written 2026-10-02 from 22:14 (clock from `date`), as it went; this text is its
second state, from 22:36, after § 6's probes withdrew a reading of mine (kept
there, with what refuted it). My copy is `<scratchpad>/187-spec-warden/` (`git
archive 07ccb72a | tar -x`), its compiler built inside it from the seed, `clang
-I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0, sha256 prefix
`60cc49e98b56ddc0`, the seed's `568bce290b6ed3bb`, 38,648,442 bytes: the
sitting's, by `00-shared.md` § Measured on the sitting's head. Work files are
under `<scratchpad>/187-spec-warden/_warden/`. Every command below ran in that
copy with that compiler unless it says otherwise; the trunk was read only
through `git -C <trunk> log` and `show` at or before `07ccb72a`. No paid run, no
`--refresh`. In quoted compiler text the em dash it prints is written ` -- `.

## The seat's verdict

- **verdict**: **approve** a resolution that adds no spec text, writes the
  promise per message and the unit of one mistake in design.md §4.17, and
  writes when a recovery item closes in `.claude/rules/verification.md`;
  **veto**, if proposed, any spec sentence about how a mistake is told (S1a,
  S1b) and the one spec change Q2 could reach (S3).
- **section**: design.md §1.6 (lines 255 to 256, 311 to 322), §1.2 (190 to
  204), §1.0 (117 to 126); §4.17 (2079 to 2145, its sentences at 2090, 2111 to
  2112, 2114 to 2120, 2126 to 2127); Part 11's metric 4 (3980 to 3981); §4.15
  (1961 to 1968).
- **spec_token_delta**: **0**, measured. `spec/heroes-spec.md` on `07ccb72a`:
  6923 vendored maximum, 9164 real (pinned, claude-opus-5, 2026-10-02), 432
  lines. The refused sentences, priced on the vendored tables: S1a +16, S1b
  +28, S3 +9.
- **removal**: nothing, and nothing is owed: the resolution adds no spec text.
- **needed_for_self_hosting**: no.
- **argument**: § 0 (119 words).
- **prediction**: § 8.
- **condition**: § 9.

## 0. The argument

§4.17 promises per message (2090, 2111: fix *it*) and measures per program,
comparatively (2126; Part 11 metric 4, never run). The per-program promise every
lane cites is CLAUDE.md § 8's paraphrase since `113b1019`, and the staged design
makes it false by construction. So the promise blocks; the costs are counted.
Measured: class (a) adds 78 to 311 tokens; class (b) cost no turn in 20 of 20
sessions at 31 to 105 lines (panel 183). Neither is a broken promise. Where: the
promise and its unit in §4.17 (a diagnostic class, a panel's), closing in
verification.md (the author's). The spec says nothing of how a mistake is told
(0 hits) and owes nothing: no route changes an accepted program.

## 1. Measured first

### 1.1 The ceiling, by grep, today

`grep -n` over `docs/design/design.md` in my copy: §1.6 at line 253; lines 255
to 256, *"must fit in 10240 tokens, measured by `claude-opus-5` through `POST
/v1/messages/count_tokens`"*; line 278, raised to 10240 by author decision
2026-09-14; lines 311 to 314, the payment rule *"unconditional. Every amendment
owes a named removal or a registered prediction naming an instrument that exists
today"*; lines 316 to 322 (panel 046), a prediction pays only if the instrument
that scores it *"exists on the day of registration"*, and otherwise *"is still
registered as an observation; it simply pays nothing"*.

### 1.2 The spec on `07ccb72a`

`./heroes measure spec/heroes-spec.md`: claude-legacy 6804, cl100k_base 6923,
maximum 6923, spread 119, **real 9164** (claude-opus-5, 2026-10-02); headroom
1076, the FFI floor mortgaging 60, so 9224 against 10240. `wc -l`: 432. I read
the spec whole in my copy first.

### 1.3 What the spec says of diagnostics

`grep -c -i` over the 432 lines: `recover` 0, `cascad` 0, `one mistake` 0, `one
message` 0, `second` 0, `diagnostic` 0, `reported` 0, `never closed` 0,
`unclosed` 0, `message` 0, `certain` 0; `compile error` 6, `an error` 8. The spec
says what is a compile error and never how one is told. Its one sentence about
what the compiler reports is § 12's hole (lines 328 to 330, *"It is not an
error: the compiler reports what belongs there"*), a form whose behaviour is its
report.

### 1.4 Where the instrument's rule is written, and where it is not

The rule, in the instrument's docstring (`<scratchpad>/instrument/tool/recovery.py`
line 5, grepped): *"their rule is design.md §4.17's: one mistake costs one
message, none is hidden, and a `certain` fix repairs the mistake it names"*.

- **design.md**: `one message` 0, `second message` 0, `cascad` 0, `none hidden`
  0, `fix the program` 0. `one mistake` 1, line 1967, §4.15's continuation
  class (panel 181): *"are one mistake, refused by one diagnostic at the line
  break, `continuation_outside_brackets`"*; and line 1261, an inline arm's
  declaration, *"one diagnostic instead of the unused-binding message plus a
  second one"*. So design.md writes *one mistake, one diagnostic* twice, each
  for one class and by a panel, and never as a general rule.
- **The third clause is written**, in process:
  `.claude/rules/diagnostics-and-goldens.md` lines 22 to 23, *"A `certain` fix
  repairs the defect the diagnostic names"* (`/decide` 2026-09-08); its
  inverse, a fix that changes meaning, is §4.17 lines 2114 to 2118.
- **The first two clauses are written nowhere as a rule.** Their negations are
  the content of `adjacent`, `.claude/rules/verification.md` § Bounded
  discovery, line 421: *"a second message for one mistake, a mistake told only
  after the first is fixed"*. So the contract counts them as defects, which
  block a tag only once aged two tags.
- **The instrument itself is in the process, and outside the tree**:
  `.claude/rules/verification.md` lines 337 to 338 (author instruction
  2026-10-02) run *"the recovery instrument ... once, at the round's gate"*, and
  `git ls-tree -r --name-only 07ccb72a` holds no `recovery.py` or `judge.py` (0
  paths). Today's gate depends on a scratchpad file a reboot empties.
- **The per-program reading has been in the contract since its first day.**
  CLAUDE.md § 8 (lines 215 to 217) and its home,
  `.claude/rules/diagnostics-and-goldens.md` lines 19 to 20: *"a diagnostic
  carries everything needed to fix the program without opening another file
  (design.md §4.17)"*. `git log 07ccb72a -S'everything needed to fix the
  program' -- CLAUDE.md` finds it entering at `113b1019` (2026-08-03 19:02, *M0
  step 5: operating contract*); `git show 113b1019:design.md` read on that day
  *"Every error carries all the context needed to fix it"* (its line 1180) and
  *"in one turn"* (1202). The contract has paraphrased §4.17's *it* as *the
  program* for two months, and lane recovery-b2's *"a model fixes the program
  in one turn"* is that paraphrase made a criterion.
- **And the items carry it**: item 130 (`docs/work/DEFECTS.md` line 38),
  *"design.md §4.17 asks that a model fix a program in one turn"*; item 131
  (line 353), *"design.md §4.17: one mistake, one message"*; the seven rulings'
  table (its line 16), *"seven readings of design.md §4.17 (one mistake costs
  one message, at the mistake, none hidden)"*.

CLAUDE.md § 1: *"Any asserted design rule cites its design.md section; an
uncitable rule is a guess."* Seven batches were judged by a rule that cites a
section which does not hold it.

## 2. Q5: which reading the contract holds, and where a finished recovery is written

### 2.1 Per message for the promise, per program for the measure

- §4.17's promise is made of one error: 2090 *"Every error carries all the
  context needed to fix it"*; 2111 to 2112 *"The model fixes it **in one
  turn**, without opening anything"*, *it* being the type mismatch of the
  example above it; 2114 to 2120, the `certain` fix.
- Its measure is per program and comparative: 2126 to 2127, *"count the number
  of exchanges needed to make a broken program compile, before and after"*,
  which Part 11 makes metric 4 (3980 to 3981): *"Turns-to-green. Given a broken
  program, how many exchanges to make it compile -- capped at 5"*. **It has
  never run**: `docs/work/milestones/M-thesis-harness.md` line 6, *"Part 11,
  whose metrics 2 and 4 have never run"*, the milestone `scheduled`
  (`docs/ROADMAP.md` line 162, row 71). `grep -rli 'turns-to-green'` over the
  copy's `.hero`, `.md` and `.py` files finds prose only (design.md, panels 003
  and 135, two milestone files), no runner.
- **The per-program promise cannot be the contract**: `heroes check` runs its
  stages in order, each only if the one before said nothing
  (`docs/records/log/2026-08-04-0026-runs-three-stages-in-order-lex-parse-resolve-check.md`),
  so a program with a parse mistake and a type mistake costs two exchanges by
  design, and the instrument itself sets 1,038 later-stage hidden pairs aside
  *by design* (`00-shared.md`). *"A model fixes the program in one turn"* is
  false by construction for every such program. The reading that survives the
  design is per stage, and that restriction is written nowhere either.

So classes (c), (d) and (e) break the promise, which is what the author's
`blocking` already says. Class (a) breaks it only where a message is untrue of
the program as written, and then it is (c). Class (b) breaks nothing at 2111
and adds an exchange to the measure at 2126.

### 2.2 What each class costs, under §1.2

§1.2 (lines 192 to 199): *real cost = program tokens x (1 + rewrite rate)*, and
*"a single correction round-trip ... costs 500-2000 tokens"* (the design's
figure; I did not measure it).

- **Class (a), measured**: the messages after the first, `heroes measure`
  vendored maximum on `check`'s plain output split at its second `error[`
  (`_warden/token-table.txt`; the case paths in each `at` line inflate the
  counts a little): 131-33a **311**, 41a **223**, 54a **133**, 54b **157**, 55a
  **78**, 55b **148**, 56a **79**, 56b **85**, g1/a11 **159**. So 78 to 311
  tokens, read once, inside the round trip the mistake already costs; no
  rewrite, unless a message is untrue. The programs themselves are 14 to 43
  tokens.
- **Class (b)**: a round trip, times the chance that the reader misses the
  hidden mistake. Measured once, by panel 183's critic
  (`docs/panel/183-reports/completeness-critic.md` § 6, lines 434 to 448): five
  (b)-class mutants of 31 to 105 lines in which the compiler hid the stray
  closer, four sessions each under that output, *"20 of 20 compile"*, one
  silent wrong edit in 20 (0 in 20 with the closer told). By the rule of three
  the miss rate is at most 0.15 at 95%, so the expected cost of (b) at those
  sizes is at most 75 to 300 tokens, the order of (a)'s measured 78 to 311.
  **Unmeasured at 300 lines and more**, the experiment that report says is
  still owed.

Neither class measurably dominates, and neither is a broken promise. A
definition of done that requires every row of both closed spends compiler work
on a cost no measurement has seen.

### 2.3 Where it is written: two halves, two homes

**(i) The promise and the unit of one mistake: design.md §4.17, a panel's.** It
says what the compiler owes a broken program's author, which CLAUDE.md § 4 gives
the panel (*"a diagnostic class"*, *"what the lexer, grammar and checker DO"*);
the instrument, the rulings and both items already cite §4.17 for it; and
design.md already holds the per-class form, §4.15 lines 1967 to 1968, a panel's.
Drafted, to stand after line 2129 and before *"Specific errors to include"*
(`_warden/drafts/D1-design-4.17.md`, 181 words, 228 tokens vendored maximum;
design.md has no budget):

> **One turn is promised per message, and the rest is counted** (panel 187).
> Every message is true of the program as written, and a `certain` fix repairs
> the mistake its message names and writes nothing else; a message that breaks
> either is a defect that blocks. One mistake is one edit its author owes: a
> habit carried down a run of lines is one, and each braced block is its own.
> What a broken program costs beyond that is counted rather than promised: a
> second message for one mistake, a mistake of the parse stage told only once
> another is fixed, and a message off its mistake's lines, each over a frozen
> corpus of planted mistakes and held no higher than its last count. A later
> stage's mistakes wait for the earlier stage's by design, since `heroes check`
> runs a stage only when the one before said nothing. Part 11's metric 4 counts
> the exchanges themselves once it runs; where it measures a hidden mistake of
> the parse stage costing one, telling that mistake in the same run joins the
> promise.

Four things it does that the sitting should see before adopting it:
- *one edit its author owes ... each braced block is its own* is rulings 5, 1
  and 4 of 2026-10-01 made one sentence: adopting it is route (1h);
- *a `certain` fix repairs the mistake its message names* moves the clause of
  `/decide` 2026-09-08 from the process file into design.md, so the author's
  rule becomes a panel's; the process file keeps its reasoning and cites §4.17,
  one rule in one place;
- its last clause names the measurement that would make its refusal wrong
  (CLAUDE.md § 12, CL-005);
- **its counted half has no instrument in the tree** until (1b)'s first
  condition is met: until then the count is the gate's scratchpad run, which
  a reboot can lose. That is this route's weakest joint, and (1b)'s condition 1
  is its repair, not an option.

Its first state said *"Every message is true of the line its excerpt shows, or
names the line it is about"* (`_warden/drafts/D1-first-withdrawn.md`, 161
words, 203 tokens). § 6's probes show that test makes the ordinary forgotten
body a broken promise, so it was replaced by *true of the program as written*.

**(ii) When a recovery item closes: `.claude/rules/verification.md` § Bounded
discovery, the author's**, after *When a round ends*
(`_warden/drafts/D2-verification.md`, 91 words, 137 tokens):

> **A recovery item closes** when each of its rows is repaired, filed
> `adjacent` apart one item per cause, or pinned as a known cost in a
> `tests/golden/check/` case whose comment gives the reason; never a row that
> breaks design.md §4.17's promise (a false message, a `certain` fix that
> writes another meaning, an exit 2), which is `blocking`. The counts §4.17
> names are read at each round's gate and may not rise; a pinned row is not an
> open item and does not age, and its golden moving, either way, is read.

The promise cannot then be weakened by a process amendment, and a gate does not
need a panel to move; each text cites the other once.

**What the stronger promise would be, recorded for the synthesis.** D1 with its
second counted cost moved into the promise: *no mistake of the parse stage is
told only once another is fixed*, blocking. It is reachable on the instrument's
corpus, where HIDDEN parse-stage reads 0 of 14,684 on this head, and it would
make the hand-found (b) rows `blocking` at once (131-16a, 16b, 32, 53a, 53b,
`g2/r12`, `g4/ti`, and 130-34a by reading (A)), for a cost the one measurement
put at no turn in 20 of 20 (§ 2.2). I do not take it: it restarts the loop the
author's *D1a D2a D3a* ended, on a premise no instrument has scored. D1's last
clause is where it enters, the day one does.

**(iii) Corrections the author owes, both process**: CLAUDE.md § 8 line 216 and
`.claude/rules/diagnostics-and-goldens.md` lines 19 to 20, *"to fix the
program"* to *"to fix the mistake it names"*: CLAUDE.md 6155 to **6157** on the
vendored maximum (`_warden/drafts/CLAUDE-s8.md`), its `real` row 8167 of
2026-09-29 against 12288, the new figure unmeasured. And dated corrections
appended under item 130's line 38, item 131's line 353, and a log entry beside
the seven rulings (`docs/records/log/` is append-only), saying the sentence they
cite is §4.17's per-message promise and that the per-program counts are the new
paragraph's. The instrument's docstring is the scratchpad's, not a record.

### 2.4 Does a finished recovery owe the spec a word? No (Principle 0)

1. The spec is the language, and recovery is what the compiler does with text
   outside it. The spec has 0 hits for how a mistake is told (§ 1.3).
2. No route of Q1 to Q5, as posed, is meant to change an accepted program,
   except S3 (§ 4), which would refuse one the spec accepts; each owes the
   census that shows it does not.
3. §4.17 makes a spec sentence on diagnostics self-defeating: an error that
   carries all the context needed to fix it needs nothing from the spec to be
   read.
4. §1.6's payment rule: the one Part 11 instrument that could score a recovery
   sentence is metric 4, which does not exist, so a prediction pays nothing
   (panel 046) and a named removal would be owed. No sentence of the spec is a
   candidate: each states a rule a program obeys.
5. Principle 0 (CLAUDE.md § 2, §1.0 lines 117 to 126): the compiler does not
   need such a sentence, and no measured Part 11 effect says it serves the
   thesis.
6. Precedent: panel 183's R5, *"No spec sentence (the spec-warden: 0 tokens)"*,
   for a rule that changed what the lexer does with an unclosed opener, nearer
   the language than this.

**The refused sentences, priced** (`_warden/drafts/make.py`, each anchor
asserted to match once; `heroes measure`, vendored only, against S0, the spec
unchanged, 6804 and 6923):

| draft | where | text | legacy | cl100k | max | delta |
|---|---|---|---|---|---|---|
| S1a | after line 3 | *Each mistake is reported once, at the mistake, with what fixes it.* | 6820 | 6939 | 6939 | **+16** |
| S1b | after line 3 | S1a, its period a `;`, then *names and types are checked only once the program parses.* | 6832 | 6951 | 6951 | **+28** |
| S3 | lines 16 to 17 | *Where a NEWLINE separates without a `,`, the next line stands deeper than the statement's first line and may not begin with a `-` ...* | 6813 | 6932 | 6932 | **+9** |

The binding `real` delta of each is unmeasured: it is the coordinator's one
refresh, and I ask for none, since I recommend none lands. The spec's ratio of
real to vendored, 9164 to 6923, is 1.32; applied to a delta it is an inference,
not a count.

## 3. Q1: the definition of done, route by route

| route | verdict | spec delta | why, in one line |
|---|---|---|---|
| (1a) every audit row closed | **object** | 0 | an enumeration by whoever probes (CL-057), which seven batches have not reached; it treats (a) and (b) as broken promises (§ 2.1, § 2.2), and two of its rows are (f), no defect (131-22, 131-52a by ruling 1) |
| (1b) the instrument's totals ratcheted by a suite | **approve on four conditions** | 0 | the only route holding the counts with an instrument that runs; the process already runs it at the gate (§ 1.4) |
| (1c) the classes alone | **object as the definition; approve as the filing** | 0 | refiling with the two-tag clock brings the same rows back `blocking` without a ruling on what they cost |
| (1d) a structural change | **object as the definition; approve as a means, measured** | 0 if no accepted program changes | a means to move the counts, not a statement of when they are low enough |
| (1f) pin what stays | **approve** | 0 | the tree's own shape (panel 183's known-cost golden), finite, and a change either way is seen; never for a row of (c), (d) or (e) |
| (1g) §4.17's own measure | **object as today's gate; approve as an observation for M-thesis-harness** | 0 | metric 4 does not exist (§ 2.1), and the one measurement saturated at one turn (§ 2.2) |
| (1h) the seven rulings | **approve: adopt 1, 3, 4, 5 and 7** | 0 | a definition resting on unratified rulings inherits them; 1, 4 and 5 are D1's unit, 7 is its stage order; 6 is settled by panels 184 and 185, both ratified |
| (1e) a route nobody listed | **the split of § 2.3** | 0 | the promise blocks, the costs are counted: (1b) with (1f) and (1h) under the §4.17 paragraph |

**(1b)'s four conditions**, each a reason it is wrong without it:

1. **In the tree, as one command** (CLAUDE.md § 10, *never a script*): the
   instrument is a Python tool in the scratchpad that the gate already depends
   on (§ 1.4). Its home is `heroes mutate`'s recovery arm (the critic's route);
   its cost in code and in the net is the compiler-engineer's to price.
2. **One corpus**: a frozen, pinned plan in the tree, or rates published with
   the corpus's hash; a ratchet on raw totals over the live tree compares
   different corpora.
3. **Its counters mapped onto §4.17's halves**: the APPLY flags are the
   promise's and block (defect 172 today); EXTRA, ELSEWHERE and HIDDEN
   parse-stage are the counted costs, ratcheted, a finding of EXTRA or
   ELSEWHERE read once for a message untrue of the program, which would block;
   SILENT is the thesis's and the language's reading (panels 184 and 185), not
   the recovery's; later-stage HIDDEN is by design and not counted.
4. **Ruling 4 adopted, or EXTRA split by operator**: 150 of EXTRA's 439 are
   `brace-else-chain`, which ruling 4 calls no defect; a ratchet on the raw
   count adopts or contradicts it in silence.

Baseline, the coordinator's run on this head (`00-shared.md`, not run by me):
ONE 12,838, EXTRA 439, ELSEWHERE 14, SILENT 23, LEGAL 280, HIDDEN parse-stage 0
of 14,684, APPLY-OTHER 20 and APPLY-NEW 19, every one operator `int` (defect
172, `blocking`). HIDDEN parse-stage already reads 0, so a ratchet can hold it
at 0.

## 4. Q2: row 130-34a

**What the spec says the program is.** Spec lines 11 to 15 keep line 2's NEWLINE
(it ends with a literal), and inside `[` a production writes it (line 210, line
214 `Sep = "," | NEWLINE`), so `print(x)` is a third element and the `)` a
closer of another kind: reading (B) is the grammar's. The author's intent is not
in the text. Measured in `_warden/q2/`: a name-led line after a kept NEWLINE
inside `[` is a legal element **at the statement's own margin**
(`margin.hero`, `x = [1, 2` over `    three()]`: `check` exit 0, `run` prints
3), and deeper (`deeper.hero`, the same). So the `[`'s reach cannot end at line
3, which is what reading (A) needs, without refusing `margin.hero`, a program
the spec accepts.

| route | verdict | spec delta | why |
|---|---|---|---|
| (A) made the parser's reading by a language change (S3) | **veto** | +9 vendored | it refuses `margin.hero`; Principle 0 unmet: the compiler does not need it, and it turns no silent mistake into an error (the row exits 1 today); a census of tracked files was not run, and the veto does not rest on it |
| (B) the only reading, as today | **object** | 0 | the message (*"between one element and the next"*) is true of the program the spec reads, but the (A) author pays a second run: the `)` deleted gets `unclosed_bracket@2:9` (F6) |
| one message reworded, naming the `[` and both edits | **approve** | 0 | no accepted program changes, and both of its conditionals are true of the program; a drafted wording of mine, for pricing only, reads 115 tokens against today's 62 (+53, vendored, `_warden/q2-reworded.plain`), against a round trip of 500 to 2000 saved for an (A) author |

## 5. Q3: defect 166's cause

design.md §4.15 lines 1967 to 1968 already say what the parser is owed: the lexer
*"hands the parser the one line the author broke, so nothing downstream moves"*.
`drop_line` (`selfhost/parse/opening.hero` line 305) stops at any token that
begins a line of the text, `margin_width(...) >= 0`, where its comment gives one
reason, *"after an `=>` or an `else` over a line whose margin holds a tab"*.

| route | verdict | spec delta | why |
|---|---|---|---|
| the critic's local repair, one condition of `drop_line` | **approve, pending the compiler-engineer's net and census** | 0, and 0 lines of design.md | it restores what §4.15 already says; the critic measured the seven doubled probes at one message, 147 files unmoved, `check` 406 and 0, the compiler's tests 1036 passed (on `62d65e48`) |
| the parser told where the lexer joined lines | **object** | 0 | a channel from lexer to parser (architecture, CLAUDE.md § 4) to undo what §4.15 says the lexer already does, for a defect the local repair closes |

A question for the compiler-engineer, unrun by me: `tabbed_line`, the function
directly above `drop_line`, already asks the stop's stated reason, so the stop
could be narrowed to it rather than removed.

## 6. Q4: what stays open, and `g2/r09`

**`g2/r09`'s second message is true and less exact than it could be:
`adjacent`.** Run in `_warden/out/b8-g2-r09.plain`: `missing_body` at 5:1, *"a
`function` needs an indented body -- one level deeper, exactly 4 spaces (found
the end of the block)"*, the excerpt line 5, `function main()`, which has its
body; the words name no function. `f` at 3:5 has no body, so the message is
true of the program, and it does not say which function it means.

**My first reading, withdrawn at 22:30, and what refuted it.** I first classed
it `blocking`, by a test I meant to propose: *a message is false when what it
says, read of the line its excerpt shows, is untrue there, and its words name no
other line*; and I called it item 131's first shape. Attacked at the shapes
beside it (CL-061, CL-078; `_warden/r09-beside/`), the test fails:
- `t1`, the ordinary forgotten body, `function f() -> i64` over `function
  main()`: `missing_body` at 3:1, *"(found `function`)"*, the excerpt on `function
  main()`;
- `t4`, a bodiless `while` before a declaration: `missing_body` at 5:1, *"a
  `while` needs an indented body ... (found the end of the block)"*, the
  excerpt on `function g()`;
- `t3`, a bodiless `if` over `print(x)` at its own margin: the excerpt on
  `print(x)`, which is the line to indent;
- and the placement is the compiler's blessed convention:
  `tests/golden/check/fixedbugs-135-p8-a-result-after-a-colon-over-no-body.expected`
  (lane recovery-b7, 2026-10-02) pins three `missing_body` at the next
  declaration's line (19:1, 21:1, 23:1), its case saying *"The missing body is
  told as before"*; 9 of the 109 `missing_body` lines in the goldens' snapshots
  say *found `function`*.

So the excerpt test would make every forgotten body before another line a broken
promise. And item 131's first shape is not this one: there (`DEFECTS.md` line
353) `missing_body` *"names a body that is there ... as missing"*, which is
untrue of the program; here `f`'s body is absent. The test that matches that
item and panel 183's *never closed* for a bracket closed later (untrue, and
reworded true by the author's choice *3b*) is **untrue of the program as
written**, the one D1 now carries. Whether `missing_body` should name its head
when its excerpt shows another line is one item for that message, reaching `t1`
and `t4` and the pinned goldens, `adjacent` by kind, and not a row of this
cluster.

| route | verdict | why |
|---|---|---|
| 131's fifteen rows and the six shapes filed `adjacent` by cause, or pinned | **approve** | (a) and (b) by kind (§ 2.1); no spec text |
| the (f) rows (131-22, 131-52a by ruling 1, `hc/h09`, `g1/b07`) | **close as no defect** | two mistakes two messages, or one and one (F3) |
| `g2/r09` | **`adjacent`**, with the `missing_body` placement filed once | above |
| items 130 and 131 close | **approve, under § 2.3 (ii)** | once each row is repaired, filed or pinned |
| 166 joins the cluster's filing | **approve, as its own item** | it has a number and a measured local repair (§ 5) |
| 177 to 182 | **`adjacent`, as filed** | the cluster's kind (`00-shared.md`); I read their item lines, not their outputs |
| 172 | **`blocking`, outside the cluster** | class (d); the APPLY flags read 0 only once it is repaired, before (1b)'s baseline counts them |

## 7. Q5's verdicts, in one table

| route | verdict | spec delta |
|---|---|---|
| the per-program reading as the promise | **object**: contradicted by the staged design (§ 2.1) and absent from §4.17 | 0 |
| the per-message reading as the promise, the per-program counts as the measure | **approve** | 0 |
| a finished recovery written in design.md §4.17 alone | **object**: the closing gate would need a panel each time it moves | 0 |
| written in a process file alone | **object**: the unit of one mistake decides what the compiler must report, a diagnostic class (CLAUDE.md § 4) | 0 |
| the split, § 2.3 | **approve** | 0 |
| any spec sentence (S1a, S1b) | **veto** if proposed: §1.6's payment rule unmet, Principle 0 unmet | +16, +28 |

## 8. Predictions

1. **Tokens**: `heroes measure spec/heroes-spec.md` reads 6923 vendored and
   9164 real at the commit that lands this sitting's resolution, the same
   digest as `07ccb72a`'s spec. Falsified by any change to the spec in that
   commit.
2. **Rewrite rate of class (b)**: a clean blind reading of rows 131-32 and
   `g2/r12`, the two (b) rows whose hidden mistake stands on a line no message
   shows (`} while (1 == 1)` on line 4; `print(1 +)` inside the function among
   the cases), five sessions each, repairs or removes the hidden mistake in its
   first answer in at least 8 of 10. Falsified at 3 or more of 10 leaving it.
   Unrun: a paid run, the coordinator's to decide, ten `claude -p` sessions, at
   panel 186's recorded 0.38 to 0.48 USD a reading (its synthesis, lines 43 and
   44) about 4 to 5 USD.
3. **CLAUDE.md**: the § 8 correction moves its vendored maximum from 6155 to
   6157.

## 9. What would change my verdict

- **On the reading**: a measured (b) turn cost at the project's sizes, the
  300-line experiment panel 183's critic left owed, metric 4 when it runs, or
  prediction 2 failing, moves the parse-stage hidden count from counted to
  promised, as D1's last clause says.
- **On the spec**: a clean blind reading in which a model misreads a recovery
  message in a way a spec sentence would have prevented, by 2 in 10 or more,
  with a removal named; the veto on S1a and S1b lifts then, and not before.
- **On S3**: none at these counts; it would take a program class the compiler
  needs that the spec's *at any column* forbids, which nobody has shown.
- **On `g2/r09`**: a blind reading in which models edit `main` for it, which
  would show the placement misleads in practice; then the `missing_body` item
  is `blocking` for every shape it reaches, `t1` included, never for `r09`
  alone.
- **On where**: an author's ruling that the unit of one mistake is process;
  then I object, since it would let the gate redefine what the compiler must
  report without a panel.

## 10. What I ran, and what I did not

**Ran, in my copy**: the build and both hashes; `heroes measure` on the spec,
CLAUDE.md, three spec drafts, a CLAUDE.md draft, my drafted texts (both states
of D1, and D2), and 22 outputs split in two; `check` and `check --brief` on the
22 cases copied from `<scratchpad>/audit-130-133/cases/` (16 rows) and
`<scratchpad>/lane-recovery-b8/p1/` (six shapes), every exit 1, the message
counts as F2 gives them; three Q2 probes; six probes beside `g2/r09`; the greps
of § 1 and the golden count of § 6. **Read in the trunk's history**: `git log
-S` at `07ccb72a`, and `git show 113b1019:design.md`.

**Not run**: any paid session; `--refresh`; the instrument; a census of tracked
files for S3; the full net; the `tabbed_line` narrowing. **Not read**: any other
seat's report or folder, any worktree.
