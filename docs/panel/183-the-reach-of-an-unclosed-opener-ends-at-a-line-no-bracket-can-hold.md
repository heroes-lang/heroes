# Panel 183: the reach of an unclosed opener ends at a line no bracket can hold, at its opener's margin

2026-09-30, the recovery cluster of defects 130 to 133, at the trunk `171e8c45`,
which every seat copied by `git archive`. **The full panel**: the question is
what the lexer does with a mistake, where a diagnostic lands, and a sentence of
design.md Part 4. Seats: compiler-engineer, llm-ergonomist, spec-warden,
historian; the ffi-pragmatist joined when the compiler-engineer measured that
the question touches what the author of a binding sees (the sitting widened as
its shared brief said it would). Briefs: `docs/panel/183-briefs/`, with the
probes the sitting ran in `probes/` and the blind seat's two clean briefs in
`blind/`. Reports: `docs/panel/183-reports/`.

**The sitting is owed after the fact, and says so.** Batch 2 of the recovery
cluster landed `41807577` on 2026-09-30 and the trunk fast-forwarded to it at
04:46 (`git reflog`); it makes an unclosed opener be found closed at a line
that opens a declaration at column 0, which falsified design.md §4.15's
*reported at end of file*. The coordinator let it land on a measurement that
nothing that compiles changes, without the sitting CLAUDE.md § 4 asks before a
change to what the lexer does and to design.md Part 4. This sitting judged the
landed rule on its merits and could have refused it; it is not a retro-record.

**Six errors in the briefs, found by the completeness critic before any seat
started** (`183-reports/completeness-critic-briefs.md`), each repaired before
the seats were launched:
- the blind seat's brief said both outputs of Task 2 were run, and one was the
  coordinator's prediction: Task 2 was held back for a second reading with the
  prototype's real output;
- *the six cases batch 2 added*: seven, the 55 lines in six of them;
- *nothing here reaches the C boundary* was never run: an `extern` group whose
  first member lost its `)` shows batch 2 changed what a binding's author sees,
  which widened the sitting;
- *clang's recovery at `}`*: Apple clang 21 reports at the next token that
  cannot go on, with a note at the opener;
- the spec-warden's instrument figures predated batch 2 (`bracket-open` hidden
  28 of 164 then, 8 of 164 after);
- the comparison of `c85bccb8` with `e5cc73eb` spans all of batch 2, the rule
  and five later commits, and the rule alone differs from `e5cc73eb` on nested
  openers and on the `extern` group.
And one more, the coordinator's own, found while writing: a pair count carried
from an agent's message (85 of 164) where the file the brief cited says 28.

**The blind seat was not blind, and the sitting repaired it.** The
llm-ergonomist's first reading answered `context: yes`: a subagent of the
coordinator's session is handed the repository's `CLAUDE.md`, the empty memory
index and a git status whose commit subjects name this sitting's area (*told
once*), whatever its brief says, so by its brief the reading is void
(`183-reports/llm-ergonomist-first-reading-void.md`, verbatim). The sitting
re-ran the seat twice as fresh `claude -p` sessions started in folders outside
the repository and outside any git tree, with no `CLAUDE.md` in or above them,
the seat's method and output structure without the project's names, the spec
at `171e8c45`, one task each, and the Read and Write tools only; both answered
that nothing about the language reached them from outside their folder
(`llm-ergonomist-clean.md`, `llm-ergonomist-clean-task2.md`). **Whether the
panel skill should run the blind seat that way from now on is a process
question for the author**, and it is in this sitting's ratification item.

## The proposal

**(a)** Ratify, amend or refuse the rule as landed at `41807577`: *the reach of
an unclosed opener ends at the first line at column 0 whose first token is
`record`, `variant`, `constant`, `use`, `extern`, `test`, or `function` followed
by a name; the lexer reports every opener still open there, `unclosed_bracket`,
citing the opener, and lays the line out at depth zero*; and the words that
replace §4.15's sentence.

**(b)** Whether the reach ALSO ends at a line inside brackets whose first token
is `if`, `while`, `for`, `match`, `return`, `assert` or `else`, at any column.


## The verdict table

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **object**: ratify (a) amended into one predicate; refuse (b) as posed; adopt (b) narrowed, `PE` | §4.15 (line 1944), §4.17, §1.1, §1.7, Part 5 | lexer only: `next_line.hero` 207 to 214, `layout.hero` 201 to 202 by `suite_layout`'s measure, ceiling 300 | at the landing batch: `bracket-open` told with one message in 110 or more of 156 single mistakes (89 at `171e8c45`); 16,227 or fewer diagnostics over the one-mistake programs (16,339); `wrong-closer` unchanged at 68 of 159 | drops the margin bound if a plausible program in a bracket never closed is told worse by `PE` than by (b) by more than (b)'s 78 brace mutants; refuses `PE` if a compiling program's tokens move, an operator's EXTRA or SILENT rises, or the landing's census moves a file |
| spec-warden | **approve** (a) as landed; **object** to (b) as posed; **approve** (b) amended, `proto-b4` | §1.6, §1.2, §1.0, §4.15, §4.17 | **0** spec tokens: `real` 9060 re-read with `--refresh` (digest `2178dab476a99fa6`), 9120 against 10240; two refused sentences priced at +35 and +71 | with (b) amended landed: `bracket-open` hidden 7 of 164 (8), `wrong-closer` 68 of 159, `bracket-open` EXTRA 46 (67); no single mistake gains an `unclosed_bracket`; the spec at 9060 real | amended (b) only if the compiler-engineer's prototype shows 0 changed outputs on the tree in both arms, `check` at 284 and 0, 0 new `unclosed_bracket` over the singles, and Task 2's three lines; objects to (a) if a compiling program changes; a spec sentence only if a blind reading shows a model misreading the reach |
| historian | **approve** (advisory): (a) as landed; (b) on the grammar's answer | precedent | none | none measurable; watches Swift's July 2026 pitch to lift SE-0380's limit | for (b): if a compiling program can open a line inside brackets with `if`, `match` or `else`, keep only `while`, `for`, `return`, `assert`, or copy F#'s condition for `else` |
| ffi-pragmatist | **approve** (a)'s `extern` half; recommends `PEL`, the label narrowing; no veto | §1.11, §4.19, §4.17, §4.15 | 363 `extern` files 0 changes, 272 emitted C files byte-identical, seven ladder programs' output unchanged | at the landing batch: `examples/sqlite/main.hero` emits C byte-identical to `171e8c45`'s; §4.19's sqlite group with the x2 mistakes draws two diagnostics in one `check`; with `PEL`, `L3_col4.hero` and `E_label_col0.hero` each one `expected_parameter` | vetoes if the landing's census changes the tokens, emitted C or clang verdict of a tracked `extern` program or refuses a binding that compiles; objects (the label test becomes a condition) if a §1.11 library header names a parameter with one of the seven words: **met**, the critic found `variant` in libsodium's `utils.h` |
| llm-ergonomist, first reading | **void**: `context: yes` | | | | |
| llm-ergonomist, clean, Task 1 | **approve Q**, the landed rule's output | locality, one turn | | under P 5 to 8 of 20 edits to line 13 and 2 to 4 silent wrong edits, under Q at most 1; P at least four times Q | neither, if the text between the opener and the next declaration holds its own mistake and Q reports only the bracket |
| llm-ergonomist, clean, Task 2 | **approve S**, (b)'s output | one turn, a caret on a correct line | | on one opener and a later closer of its shape: one-turn repairs about 8.5 in 10 under S, 1.5 under R | neither, if S's close manufactures false errors on more than about 1 file in 5 of a genuinely multi-line call |

## What the sitting measured

- **(a) changes nothing that compiles.** By the grammar (spec lines 39, 80, 111 to 116, 421 to 427: a declaration word heads only a declaration, and a declaration never stands inside brackets) and by every tracked file's tokens: the coordinator's `reach.py` at `171e8c45` found the 55 column-0 declaration lines inside brackets all in six of batch 2's seven new refused cases, and 34 statement-word lines in 12 files, all 12 refused; the compiler-engineer's census, `c85bccb8` against `e5cc73eb` over 1,358 files in both arms, moved 6 outputs and no exit.
- **(b) as posed breaks what the author closed.** The brace habit and an `if` or `match` expression as an argument put statement words inside brackets the author did close: 8 `check` goldens red, 103 diagnostics to 153 in 9 files, 18 false *never closed* (the compiler-engineer); 25 of the instrument's single mistakes, 9 of 9 written habit shapes, the `check` form at 276 and 8 (the spec-warden).
- **Three narrowings, one on (b)'s class.** The critic planted (b)'s class, a closer left out and one too many below, 591 times: `PE`, `PEL` and `proto-b4` each take its hidden from 190 to 127, `PE` and `PEL` differing on no pair. Over 164 probe programs from every seat: `PE` adds 17 true *never closed* and 8 false, `PEL` 17 and 4 and removes 2 false the landed rule prints (libsodium's `variant` among them), `proto-b4` 8 and 7. Cost in the harness's unit: `PE` +8, `PEL` +19, `proto-b4` +23. `proto-b4`'s reference margin, the line the outermost bracket opened on, is itself one false shape (`margin_ref`: five messages for one mistake).
- **A route no seat listed** (the critic, § 5.1): spec lines 11 to 15 let a kept NEWLINE inside brackets stand only before a closer or a `,` or where a production writes one, and only `[` and `{` write one; so inside a `(` a line after a kept NEWLINE that opens with a name is refused by the spec whatever the name. `PELNi`, `PEL` plus that clause for a name-led line: over the 1,358 tracked files 0 outputs, 0 exits and 0 token streams moved; 20 true and 5 false added on the probes; (b)'s class hidden 66 against `PEL`'s 127; it reaches `u4_k`, which no word list can.
- **The blind readings**, run clean (the preface): both approve the output the rule produces. The critic's 80 further `claude -p` sessions (80 sessions, 12.62 USD by the CLI's own report) found both readings' predictions false by their own clauses at these sizes: every session compiled in one turn under every output; the gain is in what the messages say, not in turns.
- **Precedent** (the historian, each with its source): Swift's parser ended an open list at a start-of-line declaration by 2016 (the 2.2 and 3.0 tags) and at a start-of-line statement by 2017 (3.1), safe for `if` only because SE-0380 keeps `if` and `switch` expressions out of argument position; F#, Elm and GHC end a bracket by the offside rule; CPython reads to the end of the file and reports at the opener; rustc exits on an unmatched delimiter since 1.69. Run by the critic on Task 2's shape: CPython 3.13 and 3.14 never name the opener, clang pairs the stray `)` with it, rustc cascades into 7 errors, Go reports all three mistakes.

## Disagreements, unsmoothed

- **The word list.** The compiler-engineer's `PE` takes `next_line.starts_afresh`'s fourteen words with `if` and `match`, `function` only before a name; the spec-warden's `proto-b4` takes `if`, `while`, `for`, `match`, `return`, `assert` and `else`; the historian would drop `if`, `match` and `else` if a compiling program could open a line inside brackets with them, and the compiler-engineer answered that none can (a block needs an indent and brackets never give one). **Measured for `PE`**: `proto-b4` misses nine true shapes `PE` reaches (the `extern` stray, `break`, a declaration at column 4).
- **The margin.** `proto-b4` bounds by the line the outermost bracket opened on, `PE` by the margin of the statement the brackets opened in; `margin_ref` is the case that separates them, and `PE`'s answer is the true one. The compiler-engineer's own design.md sentence names `proto-b4`'s margin, not `PE`'s code (the critic, § 4): the sentence adopted below names the statement's.
- **The label test.** The ffi-pragmatist's narrowing, a keyword directly before `:` is a label and ends no reach, was a recommendation and not a condition, until the critic found its condition met in libsodium.
- **What the blind readings predicted.** Both expected a turn saved; the critic's 80 sessions measured none at these sizes. The readings' approval of what the messages say stands; their turn predictions are scored false below.

## The resolution: `provisional, author ratification pending`

**R1. The reach of the brackets still open ends at a line whose first word no bracket holds in a program that compiles**, the compiler-engineer's `PE` with the ffi-pragmatist's label test (`PEL`): `next_line.starts_afresh`'s words, `if` and `match`, `function` only before a name, and none of them directly before a `:`, at a margin no deeper than the margin of the statement the outermost bracket opened in, and for `else` a margin strictly shallower; the lexer names every opener still open there, `unclosed_bracket` at the opener's own position as today, and lays the line out as what it begins. **(a) as landed is ratified as this rule's column-0 case**, and the trunk's rule is amended into it, since it misfires on libsodium's `variant` today.

**R2. And, inside a `(`, a line after a kept NEWLINE that opens with a name** (the critic's `PELNi`): the spec's own sentence refuses it, the census moved nothing, and it halves (b)'s hidden class. Adopted on the same conditions as R1 and measured again at its landing.

**R3. Where the bracket the rule finds open is closed later in the file**, the message the rule writes is not *never closed*: the five false shapes `PELNi` keeps (a statement word at or left of the margin inside a bracket the author did close, `b_if_col0`, `g5`, `g6`, `g7`, `m1_at_arg_line`) are told as what they are, the statement inside the bracket, and the landing batch measures that it can be made true; if it cannot, the five are recorded as the rule's known cost and the author chooses.

**R4. design.md §4.15, the sentence at line 1944, becomes**: *An unclosed opener is a compile error, reported at the opener. Its reach ends at the end of the file, or earlier at the first line inside the brackets whose first word no bracket holds in a program that compiles (a declaration's first word or a statement's, `if`, `match`, `function` before a name, and none of them before a `:`) at a margin no deeper than the statement the brackets opened in, strictly shallower for `else`, or, inside a `(`, at a line that opens with a name after a line that kept its NEWLINE: there the lexer names every opener still open and lays the line out as what it begins. Without that, one missing `)` would silently swallow the rest of the file's layout, or pair with a stray closer below and never be named.* It lands in the same commit as the rule, so the design and the compiler change together.

**R5. No spec sentence** (the spec-warden: 0 tokens; the clean Task 1 reading predicted the helpful reading from the spec's *every top-level line starts with its kind*).

**R6. The landing** is the recovery cluster's next batch (lane recovery-b4's fifth item, then its gate): golden cases for every shape above, the census of every tracked file in both arms, the recovery instrument on a pinned plan, the `extern` census and the sqlite prediction, and the spec-warden's four conditions, before the commit lands.

**What conservative would have been**: (a) as landed with the label test alone, and (b) refused. **What a veto would compel**: the ffi-pragmatist's, if a binding that compiles changed, the rule's withdrawal to column 0 with the label test.

**Found beside the sitting and carried to the list, not to this resolution**: batch 3's `52b2d378`, which removed `cursor.recover_past_closer`, raises the diagnostics on 42 of 3,000 single missing closers from 101 to 253 and `missing_body` from 14 to 142, on bodies that exist (the critic, § 1; one example read, 41 counted): defect 131's class, the recovery cluster's next item. The head class, a head's own `(` left open with the stray closer in its body (165 of the critic's 591 pairs), no route here reaches: the cluster's item too.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | `bracket-open` one message in 110 or more of 156; 16,227 or fewer diagnostics; `wrong-closer` 68 of 159 unchanged | the landing; the third clause already falsified on the trunk (batch 3 made it 0 of 159); the critic measured the first two hold on the trunk with `PEL` (110 of 156, 16,216) |
| spec-warden | `bracket-open` hidden 7 of 164; `wrong-closer` 68 of 159; EXTRA 46; no single mistake gains `unclosed_bracket`; spec 9060 real | the landing, on a pinned plan; the `wrong-closer` clause already falsified |
| ffi-pragmatist | sqlite's C byte-identical and two diagnostics on §4.19's group with x2's mistakes; `L3_col4.hero` and `E_label_col0.hero` one `expected_parameter` each | the landing |
| llm-ergonomist, clean | Task 1: edits to line 13 under P at least four times Q; Task 2: one-turn repairs 8.5 in 10 under S, 1.5 under R | scored by the critic's 80 sessions: **both false** at these sizes (one turn under every output) |

## Author's verdict

**RATIFIED 2026-09-30**, on the author's answer of the afternoon to five
recommendations put to them with their reasons, meant as: *1 yes*, the first
being this sitting. **Recorded as a reading**, CLAUDE.md § 4's default, which
the author asked on 2026-09-21 to be taken for granted; not `by delegation`.

**What the yes settles**: R1 to R6 as the resolution above states them, the
landed column-0 rule ratified as R1's first case and amended with the label
test, R2's kept-NEWLINE clause on its landing conditions, R3's rule for a
bracket closed later, and R4's sentence for design.md §4.15, landing with the
rule. **What it does not settle**: the landing itself, which lane
recovery-b4 owes as its fifth repair with its own measurements and may still
refuse R2 or R3 on them; and the head class, defect 131's and the cluster's.
The regression batch 3 left in `missing_body`, which this sitting's critic
found, was repaired the same afternoon in that lane (`3a93d155`), before the
yes.

**The landing, recorded 2026-10-01** (lane recovery-b4: R1 with R4's
sentence at `e5076b57`, R2 with R4's clause at `ab36aa61`, the batch gate at
`84430015`, integrated into the trunk at `3cc3b553`): **R3 did not land, on
its own measurement.** As sat, R3 tells a bracket *closed later in the
file* as the statement inside it, and Task 2's `(` at 5:19, which the stray
`)` at 10:25 closes by the lexer's pairing, is such a bracket, while R6 asks
that this `(` be named: the two cannot both hold. So the shapes are pinned
as the rule's known cost, as R3's last clause provides, in
`tests/golden/check/panel-183-a-statement-inside-a-bracket-closed-below-is-the-rules-known-cost.hero`:
four functions, `g5`, `g6`, `b_if_col0` and R2's `m1`, 4 messages on the
trunk's compiler before R1 (`a294a6ff`, one per function) and 12 on
`3cc3b553` (2, 2, 5 and 3), each opener told *never closed* though its
closer is written, measured again by the coordinator at 00:20 on
2026-10-01; `g7`, measured by the lane and not in the case, 1 to 4. Against
it, R6's run at each landing: 1,394 tracked files, 0 outputs moved; 13,594
single mistakes, none gaining `unclosed_bracket`, 7 and then 8 to fewer
messages, none to more. **The author chooses**: `docs/work/DECIDE.md`, the
item `panel 183`.

## The critic's two passes

`183-reports/completeness-critic-briefs.md` (the briefs, before any seat) and
`183-reports/completeness-critic.md` (the reports, before this synthesis). The
second pass built eleven compilers and ran the instrument pinned to the seats'
plan, and changed this resolution three times: the label test from a
recommendation to a condition, the margin in the design.md sentence, and R2.
