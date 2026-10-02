# Panel 187, the completeness critic's second pass, over the reports

Written as it goes, 2026-10-02 from 23:53 to 2026-10-03 00:13 (clock from
`date`). No verdict. No paid run; at most three of my processes at once.
The frozen tree is the trunk at `07ccb72a`. My copy is
`<scratchpad>/187-critic-head/` (`git archive 07ccb72a | tar -x`), its
compiler built inside it from the seed, exit 0, sha256 prefix
`60cc49e98b56ddc0`, the seed's `568bce290b6ed3bb`, 38,648,442 bytes: the
sitting's, as `00-shared.md` § Measured on the sitting's head gives them.
Every command below ran in my copy, or in another folder of mine named
beside it, unless it says otherwise. I read the seats' reports in the trunk
and the briefs beside their `-before-the-critic` texts; I read, and did not
run anything in, the files other seats left in their folders where a claim
could only be checked there, and I say so each time. `<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.

## 1. The repairs to the briefs, item by item of my first pass's § 7

Read: each repaired brief beside its `-before-the-critic` text, the blind
folder's five instantiated briefs, and the trunk at `07ccb72a`.

| § 7 item | repaired? | checked by |
|---|---|---|
| 1. the seats' compiler is R; T's label | **yes**, and re-based: `00-shared.md` lines 28 to 32, and the head's own hashes in § Measured | my build: compiler `60cc49e98b56ddc0`, seed `568bce290b6ed3bb`, 38,648,442 bytes, as stated |
| 2. item 130 damaged | **yes, at the source**: `50644159` restored it | item 130's span at `07ccb72a` against `d4fd12ef`'s, by script: 317 lines each, 0 differ |
| 3. positions, spec length, `git log` count, ruling 6's sentence | **yes** in `00-shared.md` (432 lines; *read the items by number*; ruling 6 *"true when written"*); `00-facts.md` itself unchanged, with `00-shared.md` saying its § Measured wins | `git log --oneline --grep='^Defect 130' 07ccb72a`: 30; `^Defect 131`: 21, as stated |
| 4. 166's title shapes on the frozen tree | **yes**, lines 53 to 60, and on the new head | my 147 files, `62d65e48`'s compiler against the head's (`187-critic-head/rerun-head-critic.txt`): 132 the same, the 15 that move all probes of 165 and 166; the coordinator's figures exactly |
| 5. Q1: (1b)'s basis; (1f), (1g), (1h) | **yes**, lines 67 to 96 and 115 to 147; the instrument run on the head added | the round3 run's totals, re-read with the coordinator's reader in my folder: output byte-identical; HIDDEN parse-stage absent (0) of 14,684, later-stage 1,038, control-arm hidden 893 (897 on the older runs) |
| 6. Q2's reworded route, Q3's local route, Q5's quotation | **yes**, lines 149 to 197 | read |
| 7. `g2/r09` as a question | **yes**, Q4 and the blind `p3` | section 4 below |
| 8. the blind seat | **mostly**: one session per program; `p3` replaced by `g2/r09`'s shape; the priming clause of `reading` removed; the cost sentence corrected. **Not repaired**: no class (b) program whose hidden mistake is off the message's line, which the brief now says (`p2`, *"its easiest case"*). **Introduced by the repair**: `docs/panel/187-briefs/blind/p3.messages.txt` still holds the OLD `p3`'s message (*"after `record Point`, found `)`"*), `cmp`-identical to `p3-before-the-critic.messages.txt`; it does not match `p3.hero.txt`. The session itself was given the right one: `rb3-brief.md`'s quoted messages equal what my head compiler prints for that program as `p.hero` | script over `rb1-brief.md` to `rb5-brief.md`: each program equals `p<n>.hero.txt`, each quoted block equals the head compiler's stderr, each session folder's `brief.md` equals the trunk's copy; `p3.messages.txt` the one mismatch |
| 9. the historian's premises | **yes**: every precedent written as a claim to verify | read; the historian found three of them false as the first brief framed them (`-Z treat-err-as-bug` is a debugging flag, its § 1.4; `go vet` bears on no recovery, § 3.4; the include chains are context, § 4.4) and left one unverified (Elm's *"one error at a time, by design"*, § 5.2); Go's 10 and `-e`, GCC's and Clang's options held |
| 10. `git` for the seats | **yes**, lines 266 to 270 | the spec-warden used it (`git log -S`, `git show 113b1019`) |

## 2. Claims of the reports re-run in my copy, and holding

| claim (seat) | my command | reads |
|---|---|---|
| the front end, the `use` closure from `selfhost/parse.hero` in `suite_layout`'s unit: 40 modules and 8,407 lines at `0338b598`, 109 and 19,201 at `07ccb72a`; `parse/` 10 and 2,403, then 54 and 8,696; `selfhost/` 380 files, 72,050 (compiler-engineer § 0) | `frontend-critic.py`, my own `code_lines` written from `suite_layout.hero` lines 697 to 712, each module read by `git show` | **the same, every number** |
| that growth is new code, not modules changing hands | the same script, split | of the head's 109, 68 are new files (9,885 lines), 40 were in the old closure (8,407 to 9,263), 1 joined (`stable`, 53); 0 left |
| +15,559 raw lines over 87 commits, +11,749 of them in subjects *Defect 130* to *133* (compiler-engineer § 0) | `git log --no-merges --numstat 0338b598..07ccb72a` over the 109 files | **the same**; the +929 group is *"Defects 133, 130 and 131"* (`29ebf21f`) |
| the recovery files' sizes, `opening.hero` 300 and `grammar_expr.hero` 1085 among them (compiler-engineer § 0) | my `code_lines` | the same for the eight I counted |
| the instrument is 3,618 code lines of Python, ten modules, 4,228 by `wc -l`, `ops.py` 1,647; `heroes mutate` 1,366 in seven modules (compiler-engineer § Q1) | `wc -l`; non-blank non-comment lines; my `code_lines` | **the same** |
| of the 16 open audit rows, 2 carry a `certain` fix (compiler-engineer, (1g)) | my 147-file re-run on the head, each row's own case | **2**, 131-56a and 56b, `trailing_colon[C]`, each applied clean |
| V1's full net, 4,752 and 0 over 26 suites, each suite equal to the gate's last run (compiler-engineer § Q3) | `git show -s 6bec7c8c` against the report's 26 counts | the gate's 26 counts sum to 4,752 and each equals the report's; **the run itself is the seat's, not re-run** |
| Q3's local route moves 166's four of the 147 files and nothing else (compiler-engineer) | **my own build** of the one-condition variant at the head (`187-critic-head-p/heroes-p`, sha256 prefix `3843fe6652ac37a2`, exit 0) and the 147 files | **4 of 147**, `n1`, `n2`, `n3`, `n9`, each two `expected_pattern` to one, exits and `--apply` unchanged |
| 109 `missing_body` lines in the goldens, 47 *"a `function` needs"*, 15 of them at the next declaration's first word in 8 cases (compiler-engineer § Q4); 9 say *found `function`* (spec-warden § 6) | a script over `tests/golden/**/*.expected` | **109, 47, 9**; at column 1 of an unindented line, 17 lines in 10 cases, of which 2 are a `{` below a head, so **15 in 8**; one golden says *(found the end of the block)* at the next declaration (`fixedbugs-131-a-head-cut-short`, 26:1), `g2/r09`'s own wording |
| the per-program wording entered the contract at `113b1019`, 2026-08-03 19:02 (spec-warden § 1.4) | `git log 07ccb72a -S'everything needed to fix the program' -- CLAUDE.md`; `git show 113b1019:design.md` | **the same**: `113b1019`, and design.md then read *"in one turn"* at its line 1202 |
| panel 183's critic: under the output that hid the closer, 20 of 20 compiled, one silent wrong edit (spec-warden § 2.2) | `docs/panel/183-reports/completeness-critic.md` lines 434 to 441, read | **quoted accurately** (section 3 for what it does not carry) |
| the five blind corrections, five one-turn repairs, 0.851 USD (llm-ergonomist) | each `rb<n>/c.hero` copied into my folder (each `cmp`-identical to the coordinator's `rb-measure/<n>/c.hero`), `check` and `run` on my compiler; `total_cost_usd` summed over the five `run.json` | **each checks clean and prints `0 1 2`, `7`, `3`, `2`, `3`**; the five cost 0.162, 0.152, 0.158, 0.193 and 0.187, **0.851**, each `success` |
| Q3's local route moves 0 of 1,834 tracked files in either arm (compiler-engineer) | **my own census** (`census-critic.py`): every tracked `.hero` outside `archive/` at `07ccb72a`, `check --brief` and `--permissive`, head compiler against my variant, three at a time, 00:05:23 to 00:07:19 | **1,834 files, 0 move in either arm**; the head's exits 895 at 0, 938 at 1 and 1 at -6 in the normal arm, 1,010, 823 and 1 in the control arm, the compiler-engineer's figures; the -6 is defect 169's |

## 3. Claims the reports assert and their own command does not settle

1. **The spec-warden's class (b) cost is carried, not of this sitting's
   rows.** *"class (b) cost no turn in 20 of 20 sessions at 31 to 105 lines
   (panel 183)"* is quoted accurately (panel 183's critic, lines 434 to
   441), and § 2.2 says it was measured once by that critic; § 0 calls it
   *"Measured"* in the same breath as the warden's own 78 to 311 tokens. It
   was five mutants of panel 183's own plan, on that sitting's compiler,
   none of them this sitting's rows. What it does not carry: the one silent
   wrong edit stood on the hiding side only (1 of 20, 0 of 20 with the
   closer told), the one outcome worse than a turn; the rule of three bounds
   the miss rate of 0 in 20 (0.15; exact two-sided 0.168), and 1 in 20 has
   an exact two-sided upper bound of **0.249** (computed here). Panel 183's
   critic called it *"too few to weigh"*; the warden's *"neither class
   measurably dominates"* rests on it.
2. **The compiler-engineer's *"a ratchet on ONE and EXTRA rewards adding
   recovery code"*** is an inference from § 0's growth, not a measurement:
   the instrument's history shows code added and EXTRA falling together
   (1,326 to 439), which is a correlation. Two commands bear on it: the
   instrument's run on V1, a deletion (section 5), and section 4's estimate,
   which shows a ratchet would reward a suppression rule as readily as added
   code.
3. **The compiler-engineer's (1g) objection tests a narrower loop than (1g)
   names.** Q1 says *"a mechanical loop that applies what the first message
   says and re-checks ... or ... the blind seat's one-turn rate"*. The
   measurement, 2 of 16 rows carry a `certain` fix (reproduced, section 2),
   is the loop that applies what the compiler certifies. It does not reach
   the other two readings; the instrument's own run already measures the
   certified loop over 13,594 mutants (`--apply` restored the original on
   3,506 at the head's run, `insttotals-round3-critic.txt`), a number no
   seat cites.
4. **The historian's one inference is now measured by another seat.** *"That
   a recovery skipping 'to the end of the line' then skips joined lines with
   it is my inference, unmeasured"* (§ 8, Q3): the compiler-engineer's V1
   does exactly that and reads 4 of 147 files and 0 of 1,834 moved (both
   reproduced here with my own build).
5. **The historian could not read design.md's historical appendix**, which
   its mandate names (`.claude/agents/historian.md`): the repaired brief
   allows it only `git log` and `show` in the trunk, and it has no shell to
   make a copy, so a precedent row the appendix already holds, or departs
   from, was out of its reach. Answered here from my copy: the appendix
   (design.md lines 4006 to 4090) names *"Rust / Elm -- the demonstration
   that error message quality transforms the experience"* in one line, and
   none of the recovery bounds the historian sourced (`grep -c` for
   `ferror-limit`, `fmax-errors`, `known-bug`, `panic mode`, `Pennello`,
   `Diekmann`, `Ripley`, `cascad`, `too many errors` over design.md: 0
   each). Whatever the sitting adopts from them is a new row, and no row is
   departed from in silence. The historian's brief could have given it the
   appendix as a file, as the blind seat is given the spec.
6. **`missing_body`'s placement at the next line is a convention, measured,
   and its reading by a model is measured once.** Both seats that ran
   `g2/r09` read the message as true (section 4); the blind `p3` session
   read it as a cascade and repaired in one turn without touching `main`
   (n = 1). The spec-warden's condition, *"a blind reading in which models
   edit `main` for it"*, is the run that would settle it; it is unrun.
7. **The front end's growth is real** (section 2), but the compiler-engineer's
   *"Pascal-P4, about 4,000 lines"* scale is the seat's definition
   (`.claude/agents/compiler-engineer.md` line 25, *"Pascal-P4 is ~4000
   lines; that is the scale"*), not a measurement of this tree; read as a
   yardstick it is fair, read as a ceiling it is nobody's ruling.
8. **The compiler-engineer's second prediction is false by its own clause**,
   in the direction it hoped: *"every total of both arms within 3 mutants of
   this head's run"*; the normal arm moved 4 (EXTRA 439 to 435, ONE 12,838 to
   12,842) and the control arm 10 (EXTRA 424 to 414, ONE 12,560 to 12,570)
   (section 5). Its stated condition for Q3 (a new parse-stage HIDDEN pair,
   or EXTRA above 439) is not met, so its Q3 verdict stands.

## 4. The contradictions between seats, and what a command settles

### 4.1 (1b): the compiler-engineer objects, the spec-warden approves on four conditions, the historian approves a differential and objects to a ratchet

**Settled by a command:**

- *"A ratchet on ONE and EXTRA rewards adding recovery code"* (the
  compiler-engineer): **refuted as a general claim by its own V1.** V1
  deletes `drop_line` (12 lines out of `opening.hero`) and moves the totals
  the way a ratchet asks: normal EXTRA 439 to 435 and ONE 12,838 to 12,842,
  control EXTRA 424 to 414, nothing hidden (section 5). A ratchet would have
  accepted the deletion.
- **But a ratchet on this instrument would also reward a suppression rule it
  cannot judge.** From the head's run data alone (`goline-critic.py` over
  `inst-187/round3/singles.jsonl` and `pairs.jsonl`, the instrument's own
  classes reproduced first, ONE 12,838, EXTRA 439, ELSEWHERE 14): keeping
  only the first message on each line would take EXTRA from **439 to 257**,
  181 mutants to ONE and 1 to ELSEWHERE (all 52 `c-for`, 47 `slice-type-go`,
  24 `interp-js`, 16 `nullish`, 14 `option-type`, 12 `range-dots`, 8
  `map-type-generic`, 7 `interp-swift`, 1 `string-open`, 1 `forget-f`), and
  hide **none** of the 14,684 parse-stage seconds, because **no pair of the
  15,800 puts its two mistakes on a shared line**: the site lists (lines, as
  `judge.py`'s `position` reads them) of every pair are disjoint. On V1's
  run the same filter reads 435 to 253, the same 182 mutants. With no pair
  on one line, and the filter hiding none of them, the instrument cannot see
  the cost of a per-line rule, so a ratchet on it would score Go's rule as a
  41 per cent cut of EXTRA at no cost.
- **The cost it cannot see is on the audit rows** (`goline-rows-critic.py`,
  the head's messages in report order, hidden positions from `00-facts.md`
  F2 and F3): the same rule leaves one message per mistake on 7 of the 9
  class (a) rows, but **on four (131-33a, 41a, 54a, 54b) the message it keeps
  is the lexer's `;` or the wrong opener** (`unexpected_character@2:15`,
  `unclosed_bracket@1:11`), and the message naming the repair
  (`for_missing_in`'s *"`for x in xs`"*, the function type's
  `expected_function_type_params_close@1:29`) is the one dropped; on 131-55b
  it drops `expected_end_of_line@6:10`, the stray `)` told today, turning an
  (a) row into a (b); and **5 of the 8 class (b) rows (131-16a, 16b, 53a, 53b,
  `g4/ti`) hide their mistake on the line their message stands on**, so under
  such a rule that mistake could never be told in the same run, whatever a
  later repair does. A post-filter is not a parser change; it estimates the
  rule's reach, not its whole effect.
- **"May not rise" against the record** (the spec-warden's D2, *"read at
  each round's gate and may not rise"*): item 130's gate lines at
  `07ccb72a` show the totals moving between recovery batches with other work.
  Batch 3's gate read EXTRA 1,326 (line 184) and batch 4's base EXTRA 1,336
  (line 207); batch 4 closed at ONE 12,448 and batch 5's base read ONE 12,427
  (line 232). Each run planted the plan copied from the run before it
  (`00-facts.md` F4), so the moves are compilers', merged between batches.
  A gate that may not rise would have gone red twice in those four days on
  work that was not the recovery's. Read from the record, not re-run.
- **The differential form works, and this sitting ran one.** V1 against the
  head's run on the same frozen plan is the historian's *"base and head
  compiler on the same frozen corpus ..., a moved pair being the finding"*
  and the compiler-engineer's *"a measurement at a recovery batch's gate"*:
  it named every moved mutant (28 singles and 72 pairs with fewer messages,
  0 with more, 0 seconds newly hidden), which a ratchet's single number does
  not.

**Not settled by a command**: whether a rise blocks (a ratchet) or is read
(a differential). The measurements above say what each would have done.

### 4.2 (1d): the compiler-engineer objects, the spec-warden approves it as a means, the historian approves it

They judge different things. The compiler-engineer judged *"one
resynchronisation rule per statement, or the parser told where the lexer
joined lines"* and found the second already built (`parse/apart.hero`); the
historian judged *"a rule of Go's shape (after a failed construct, nothing
more is told until the next statement or line)"*, on Go's *"only one syntax
error per line"* since Go 1. **Nobody built the historian's.** Section 4.1's
estimate is the nearest measurement: a large cut of EXTRA, invisible to the
instrument, with a cost on the rows (four kept messages worse, one told
mistake hidden, five hidden mistakes locked). **The historian's P1** (*"EXTRA
falls below 439 and HIDDEN parse-stage rises above 0"*) **cannot fail on this
instrument**, which is the historian's own question answered: no pair puts
both mistakes on one line, so the HIDDEN half cannot rise for a per-line
rule. A per-statement rule could reach the 701 pairs whose second lies
inside the first's construct (`relation: inside`, 652 of them eligible in
the normal arm); that is unmeasured.

### 4.3 (1g): the compiler-engineer objects, the spec-warden objects as a gate and approves as an observation, the historian approves as the measure

What is measured: the certified loop moves 2 of 16 open rows (reproduced),
and over the instrument's 13,594 mutants `--apply` restores the original on
3,506 in one exchange (the head's run); a model in the loop repaired 5 of 5
blind programs of 3 to 8 lines in one turn (reproduced), and panel 183's 20
of 20 at 31 to 105 lines. What is not: Part 11's metric 4 (never run, the
spec-warden's search), and any size of 300 lines or more. The three verdicts
agree on what exists and disagree on whether a measure with no baseline can
define done; no command settles that.

### 4.4 `g2/r09`: the compiler-engineer and the spec-warden both read the message as true

**Settled as far as a command reaches.** The placement is the convention:
109 `missing_body` lines in the goldens, 47 of them *"a `function` needs"*,
15 of those at the next declaration's first word in 8 cases, and one with
`g2/r09`'s own *"(found the end of the block)"* at the next declaration
(`fixedbugs-131-a-head-cut-short`, 26:1); the spec-warden's `t1` and `t4`
show the same place for an ordinary forgotten body and a bodiless `while`.
Read as false, every one of those would be false too. And the one blind
session given this shape read the second message as a cascade and repaired
in one turn without touching `main` (n = 1). **My first pass's (c) reading
is not supported by any measurement.** Open: whether models misread the
placement at a rate, which only the spec-warden's named blind run would
measure. The compiler-engineer's V5, which names the function (24 goldens
moved in words only, its measurement), is a repair of the `adjacent` kind
and was not re-run by me.

### 4.5 A contradiction no seat names: the spec-warden's drafted text holds the ratchet the other two refuse

The spec-warden's D1 for design.md §4.17 writes the counted costs *"held no
higher than its last count"* and its D2 for verification.md *"may not rise"*;
the historian objects to *"a ratchet on raw totals"* and the
compiler-engineer to *"a ratchet on ONE and EXTRA"*. A synthesis that adopts
D1 and D2 as written adopts the ratchet. Section 4.1 is what is checkable
about it.

## 5. The instrument's run on V1

`<scratchpad>/inst-187/v1/`, started 23:52:57, `exit 0` at 00:09:42 by its
log, 16.7 minutes of wall, compiler sha1 `244efccc95800462` (sha256 prefix
`45ca8514744beb95`, the same as `<scratchpad>/187-compiler-engineer-v1/heroes`,
which I hashed in that seat's folder and ran nothing in: the compiler-engineer's
V1 built from its regenerated seed, as its report says), lane recovery-b6's
gate plan, 13,594 mutants, 0 errors. Read with the coordinator's reader, V1 added as a fifth run
(`insttotals-v1-critic.txt`), and each `counts.json` read directly:

| | the head (`round3`) | V1 |
|---|---|---|
| ONE / EXTRA / ELSEWHERE / SILENT / LEGAL, normal arm | 12,838 / 439 / 14 / 23 / 280 | **12,842 / 435** / 14 / 23 / 280 |
| the same, control arm | 12,560 / 424 / 33 / 297 / 280 | **12,570 / 414** / 33 / 297 / 280 |
| HIDDEN parse-stage, normal arm | 0 of 14,684 | 0 of 14,684 |
| HIDDEN later-stage (by design) / control arm | 1,038 / 893 | 1,038 / 893 |
| APPLY-NEW / APPLY-OTHER / APPLY-SAME | 19 / 20 / 31 | 19 / 20 / 31 |
| `arm-colon` / `case` EXTRA | 10 / 6 | 8 / 4 |

Mutant by mutant (`compare-v1-critic.py`, and the same comparison with each
message's per-run file name taken out): in the normal arm 4 singles change
class, all EXTRA to ONE (`arm-colon` 2, `case` 2), and **28 singles and 72
pairs get fewer messages, none more**; in the control arm 10 singles change
class (EXTRA to ONE: `arm-thin-arrow` 6, `arm-colon` 2, `case` 2), 46 singles
and 95 pairs get fewer messages, none more; **no pair's second changes from
told to hidden in either arm**; no APPLY flag moves. The coordinator's
reading of the totals is the same as mine.

So on the instrument V1 only removes messages, and no pair's second changes
its told status (hidden, the same code, another code) in either arm. The five
true messages the compiler-engineer's own probes saw V1 drop under
`--permissive` (`h01`, `h05`, `h14`, `h16`, `h17`, a second mistake in the
body of a joined arm whose pattern failed) do not show in it: either the
plan holds no such pair, or V1 does not hide it there. Either way the
instrument did not see what the probes saw.

## 6. What is missing

1. **The instrument cannot plant two mistakes on one line, nor a mistake in
   the body of an arm whose pattern failed** (sections 4.1 and 5). Every
   route that would gate on it, (1b) and the spec-warden's D1 and D2, is
   blind to the cost every precedent the historian sourced pays; the rows
   show that cost is real here (5 of 8 (b) rows hide on the message's line).
   No seat names it.
2. **No class (b) measurement at the project's sizes.** The blind set's two
   (b) programs are 3 and 5 lines, `p2`'s hidden `)` on the message's line;
   panel 183's 20 of 20 was 31 to 105 lines. The spec-warden's prediction 2
   names the run (131-32 and `g2/r12`, ten sessions, about 4 to 5 USD); the
   300-line form panel 183's critic left owed is still owed.
3. **The control arm has no home in any definition of done.** No golden form
   runs `--permissive` (the compiler-engineer); (1f)'s pins cannot hold
   131-22's open half; V1 drops five true control-arm messages on hand-made
   probes; design.md Part 11 makes the control arm the thesis's measuring
   arm. Whether done includes it is unasked.
4. **Defects 177 to 182, filed as the cluster's kind, were run by no seat**
   (the compiler-engineer: *"unmeasured by me"*; the spec-warden: *"I read
   their item lines, not their outputs"*). Q4 files them unread.
5. **Defect 169 is a crash on a tracked file**: the census's one exit -6 in
   both arms, under every compiler run here, `docs/panel/184-briefs/blind/task3a.hero`,
   filed `blocking`. Not the recovery's, but the one class (e) row any census
   of this sitting shows.
6. **The blind record has a stale file**: `blind/p3.messages.txt` (section 1,
   item 8).
7. **The historian lacked design.md's appendix** (section 3, item 5).

## 7. The question the sitting should have asked and did not

**What can the measure we are about to make the definition of done not
see, and what would a gate on it reward?** Every route that defines done by
a count, (1b), (1g) and the spec-warden's drafted text, rests on the
instrument or on a loop like it, and the sitting asked how to hold its
totals, never what its plan plants. Answered here from its own data: no pair
on one line, no mistake inside a failed arm's body, a frozen corpus of
mutants whose totals still move with unrelated compiler work, and a
per-line suppression it would score as a 41 per cent improvement while the
audit rows show it locking five hidden mistakes. A sitting that adopts a
count should first write down, beside it, the shapes the count cannot see,
as rustc's `known-bug` pins write down what a test cannot yet judge.

## 8. Corrections to my first pass (appended, not rewritten)

- **The per-program reading is older than lane recovery-b2.** My first pass
  (§ 4, Q5) called *"a model fixes the program in one turn"* lane
  recovery-b2's paraphrase, and the repaired `00-shared.md` Q5 repeats it.
  The spec-warden found it in the contract itself: CLAUDE.md § 8, *"a
  diagnostic carries everything needed to fix the program without opening
  another file (design.md §4.17)"*, since `113b1019` (2026-08-03 19:02),
  reproduced here with `git log -S`. Lane recovery-b2 made the contract's
  own paraphrase a criterion.
- **My `g2/r09` (c) reading** is answered (section 4.4): not supported.
