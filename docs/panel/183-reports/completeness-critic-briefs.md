# Panel 183, completeness critic, first pass: the briefs

Written 2026-09-30, 05:04 to 05:30, by the completeness critic, before any seat
starts. Every fact of `00-shared.md`, `compiler-engineer.md`,
`llm-ergonomist.md`, `spec-warden.md` and `historian.md` was checked by a
command of my own, and each finding below gives the command and what it
printed. `<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
My directory is `<scratchpad>/183-critic/`, made by `git -C
/Users/joseph/Temp/heroes/heroes-lang archive 171e8c45 | tar -x -C <it>`,
`rm -rf build`, and `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`
(`real 6.51`, `user 6.32`, `sys 0.12`, on a shared machine). Everything else I
built or wrote is under `183-critic/_critic/`. I wrote nothing in
`183-llm-ergonomist/`; I read its files to compare them.

## The three compilers used here

| name | built from | what makes it that compiler |
|---|---|---|
| `NEW` | 171e8c45's seed | `cmp` silent against `<scratchpad>/p183/heroes-judge` and `<scratchpad>/instrument/heroes-e5cc73eb`; `git rev-parse e5cc73eb:seed/heroes.c 171e8c45:seed/heroes.c` prints `ec87ff06e81f6158...` twice |
| `OLD` | c85bccb8's seed, `_critic/c85/` | `cmp` silent against `<scratchpad>/instrument/tree/heroes`, the frozen lexer; `diff -rq instrument/tree _critic/c85` (build and binary left out) prints nothing |
| `RULE` | 41807577's `selfhost/`, built by `OLD` | `git log -1 --format=%P 41807577` prints c85bccb8, so this is the rule and nothing else; the build read `real 66.80`, `user 54.65`, `sys 7.42` |

## 1. False as written: repair before the seats start

### 1.1 The blind seat is told something false about output S

`llm-ergonomist.md`: *"Each output is what a compiler for this language prints
on that program; for each program there are two outputs"*. For Task 2 this is
false: `task2-output-S.txt` is 00-shared.md's hand-made prediction under (b),
and no compiler prints it. I stripped the `exit N` line from the four files in
`<scratchpad>/p183/ab/` and compared the seat's four outputs with `cmp`:

```
task1-output-P.txt equals (exit line stripped): task1.OLD
task1-output-Q.txt equals (exit line stripped): task1.NEW
task2-output-R.txt equals (exit line stripped): task2.OLD task2.NEW
task2-output-S.txt equals (exit line stripped): NONE
171e8c45 does not print S
c85bccb8 does not print S
```

S is byte-identical to three renderings by `NEW` of one diagnostic each
(`unclosed_bracket` 5:19, `expected_end_of_line` 10:25, `expected_expression`
11:18), each provoked by a program of its own, joined by blank lines (`diff
composed.txt task2-output-S.txt` silent). So it is a faithful rendering of a
prediction, and still not an output. 00-shared.md adds to it: *"The
llm-ergonomist gets these programs and the outputs label-stripped"*, which a
later reader takes as four real outputs. It matters because task 3 asks which
output the seat fixed in fewer turns: if the prototype of (b) prints anything
else, the reading scores a message no compiler will print.

**Repair, most robust first**: launch the llm-ergonomist after the
compiler-engineer's prototype of (b) has run Task 2, and give it that real
output as S. The alternative keeps S and says in both briefs what it is (for
the seat, without saying which: *one of the four outputs is not yet printed by
any compiler, it is what a proposed compiler is predicted to print, assembled
from messages today's compiler prints*), which costs the seat some of its
blindness. I recommend the first.

### 1.2 "The six cases batch 2 added": batch 2 added seven

00-shared.md: *"every one in the six cases batch 2 added under
`tests/golden/check/fixedbugs-130-*` and `fixedbugs-131-*`"*. compiler-engineer.md
task 2: *"and the six cases batch 2 added. Run each."*

`git diff --diff-filter=A --name-only 4f316b9e e5cc73eb -- 'tests/golden/*.hero'`
printed seven:

```
fixedbugs-130-a-declaration-past-a-bracket-never-closed.hero
fixedbugs-130-a-line-going-on-below-a-bracket-never-closed.hero
fixedbugs-130-an-orphan-under-a-whole-arm-holding-its-own-mistake.hero
fixedbugs-131-a-brace-habit-never-closed-is-told-once.hero
fixedbugs-131-a-group-a-call-or-an-index-the-lexer-found-open.hero
fixedbugs-131-a-head-that-left-a-bracket-open.hero
fixedbugs-131-a-literal-that-never-closed-takes-its-line.hero
```

The 55 lines are in six of them, the six repair cases that `e5cc73eb`'s body
names (*"6 files moved in each, the six new repair cases"*). The seventh, the
orphan case (04b51f08, *"a case added by the M9 verification"*), holds none of
the 55 and did not move in my census (§ 3). Batch 2 also added
`selfhost/doc_comments.hero`, which is how 1350 became 1358 (`git ls-tree -r
--name-only <commit> | grep -c '\.hero$'`: 1350 at c85bccb8, 1358 at e5cc73eb).
**Repair**: name the six files in 00-shared.md and in the compiler-engineer's
task 2, and say *six of the seven `.hero` cases batch 2 added; the seventh,
04b51f08's, holds none*.

### 1.3 "Nothing here reaches the C boundary" is a negative nobody ran

00-shared.md: *"The ffi-pragmatist does not sit: nothing here reaches the C
boundary"*. Rule (a)'s own word list holds `extern`, and the members of an
`extern` group sit at column 4, where neither (a) nor (b) ends a reach. I ran a
group whose first member lost its `)`, with a declaration and a mistake below:

```
extern "math.h"
    function cos(x: f64 -> f64
    function sin(x: f64) -> f64
    function tan(x: f64) -> f64 junk

function main()
    print(1 +)
```

`check --brief`: `OLD` and `RULE` alike print `2:17 unclosed_bracket` and `2:25
expected_params_close` (found `->`), and say nothing of `junk` on line 4 or of
line 7; `NEW` prints `2:17 unclosed_bracket`, `4:33 expected_extern_signature`
(found `junk`) and `7:14 expected_expression`. With the file ending inside the
bracket (no `function main()` below), `OLD` and `RULE` print the same two
lines, `NEW` prints `2:17` and `4:33`. So batch 2 changed what the author of a
binding sees, through its parser repairs and not through rule (a), and a member
line is a line inside brackets that neither word list names. The corpus holds
such lines, only in refused files: `function` opens 9 lines inside brackets past
column 1, all 9 in files that exit 1 (for instance
`tests/golden/check/fixedbugs-131-the-brace-habit-is-one-mistake.hero:42:5`,
`function sqrt(x: f64) -> f64`, depth 1). Whether this is the C boundary (it is
the FFI's surface, not the emitted C or the ABI) is the sitting's call.
**Repair**: write the sentence as what was run and leave the rest a question:
*the ffi-pragmatist does not sit; whether a member line of an `extern` group
(column 4, `function`, `constant` or `record`) should end a reach is the
compiler-engineer's to answer, with the program above among its edges, and the
sitting widens if the answer touches a binding.*

### 1.4 "clang's recovery at `}`" is not what clang did here

historian.md task 1: *"one C compiler (clang's recovery at `}`)"*. Apple clang
21.0.0, `clang -fsyntax-only` on an unclosed `f(3` in `main`, with a mistake in
a later function (`a.c` keeps the `;`, `b.c` drops it):

```
a.c:3:16: error: expected ')'
a.c:3:14: note: to match this '('
a.c:7:27: error: expected expression
b.c:4:5: error: expected ')'
b.c:3:14: note: to match this '('
b.c:7:27: error: expected expression
```

It reported where its parser stopped, at the `;` and at the next line's `if`,
with a note citing the opener, and went on to the next function's mistake.
Neither run shows a recovery at `}`. **Repair**: *one C compiler (clang: where
its parser stops, what it skips to, and where it reports)*, the answer left to
the seat's source. `b.c` is (b)'s shape in C, a statement keyword ending an
unclosed `(` inside one body; the historian may want it.

## 2. True, but a seat cannot check it from its brief, or it lacks what the seat needs

### 2.1 The measurement's arguments are not named

00-shared.md gives `python3 scratchpad/p183/reach.py <frozen lexer> <judge>
<repo> <out>` and names none of the four. My brief says 00-shared.md names the
frozen lexer as `<scratchpad>/instrument/tree/heroes`; it does not, it says
*"the `c85bccb8` snapshot's compiler"*. The judge is
`<scratchpad>/p183/heroes-judge`. The script lists files with `git -C <repo>
ls-files`, which a `git archive` cannot answer, so `<repo>` was a checkout
rather than the frozen commit; my own method read the commit (§ 3) and got the
same numbers, so it made no difference this time. **Repair**: write the
invocation in full, and give the route a seat can rebuild without the
coordinator's scratchpad: `git archive c85bccb8`, then the seed build, whose
binary is byte-identical to the snapshot's.

### 2.2 "All 15 files named above exit 1": true, and `reach.py` measured 12

`reach.py` calls `verdict()` only for `stmt_files`, so `reach.txt` prints
`check exit 1` for the 12 statement files; the 3 files that hold declaration
lines only have no verdict there. My run over all 15, from each file's
directory, from the tree's root, with `OLD`, and with `NEW --permissive`,
printed `1 | 1 | 1 | 1` for every one. **Repair**: cite the command that
measured the 15.

### 2.3 The source of (b) exists only in the coordinator's scratchpad

*"Lane recovery-b2's report raised it"*: nothing at 171e8c45 records it
(`grep -rn -E "panel 183|b2's report|u2_p|u2_q|u4_k|u5_i" docs/` finds only
panel 175's unrelated `u2_ptr_reuse`). The record is
`<scratchpad>/lane-recovery-b3-items.md:19`, *"B2 (b2's report): a stray closer
below an unclosed bracket, with no declaration between, pairs with it and the
lines between are read inside the bracket (b2's u2_p, u2_q, u4_k, u5_i)"*, and
the programs are `<scratchpad>/lane-recovery-b2/shapes/u2_p_stray_inside_body.hero`,
`u2_q_body_then_stray.hero`, `u4_k_next_mistake.hero`, `u5_i_stray_in_body.hero`.
I ran all four: `OLD` and `NEW` print the same single line on each
(`3:14 expected_separator`, `3:5 expected_group_close` found `if`, `3:5
expected_args_close` found `y`, `2:5 expected_params_close` found `print`),
and no `unclosed_bracket` in any. The claim is true today. The scratchpad goes
away with the session (`.claude/skills/panel/SKILL.md` § Procedure 2).
**Repair**: copy the four programs into `docs/panel/183-briefs/` or inline them
in 00-shared.md, and hand them to the compiler-engineer as (b)'s neighbours
beside Task 2.

### 2.4 The spec-warden is given the numbers before batch 2, not after

spec-warden.md task 2 cites `<scratchpad>/instrument/baseline-c85bccb8/report.md`;
its figures are there as quoted (`bracket-open`, *"a closing `)` or `]` left
out"*, 28 / 164; `wrong-closer`, *"a closer of the wrong kind"*, 68 / 159; 641
programs; 13594 is the `all` row's `n`). The same instrument, over the same 641
programs and 13594 mutants, finished on e5cc73eb's compiler at 05:10:19, eight
minutes after the brief (`<scratchpad>/instrument/run-e5cc73eb.log`, *"done:
this invocation took 18.8 minutes"*). Read from the two reports' § Pairs,
column *as the first, parse-stage seconds: HIDDEN / pairs* (read, not re-run by
me):

| first mistake | c85bccb8 | e5cc73eb |
|---|---|---|
| `wrong-closer` | 68 / 159 | 68 / 159 |
| `string-open`, a string left unterminated | 41 / 170 | 1 / 170 |
| `bracket-open` | 28 / 164 | 8 / 164 |
| all, normal arm, parse-stage seconds | 153 of 14685 | 91 of 14685 |

Three things the brief leaves out: `string-open`, the column's second-largest
count at c85bccb8 (sorted: 68, 41, 28, then 3 or fewer); that `wrong-closer`
did not move with batch 2, the number (b) would be judged against if it bears
at all; and that the rate counts parse-stage seconds only, where the brief says
*"a second mistake"*. **Repair**: add the e5cc73eb column and the `string-open`
row with the report's path, and write *a second mistake of the parse stage*.

### 2.5 "Before the rule, after it" compares six commits, not one

00-shared.md: *"`c85bccb8` is the compiler before the rule, `e5cc73eb` after
it"*. e5cc73eb carries all of batch 2 (`git log --oneline 4f316b9e..e5cc73eb`).
For the two tasks the difference is the rule's alone: `RULE` prints
`task1.NEW.txt` and `task2.NEW.txt` byte for byte (`cmp` silent). For other
shapes it is not. Nested openers:

```
function main()
    xs = [f(1, (2
    print(1)

function f(a: i64, b: i64) -> i64
    return a +
```

`OLD` and `RULE` print the three `unclosed_bracket` (2:10, 2:12, 2:16), then
`3:5 expected_group_close` and `7:1 expected_args_close` (*found end of file*),
so `return a +` is never told as itself; `NEW` prints the three, then `7:1
expected_expression` (*found the end of the block*). With 1.3's `extern` group,
`RULE` again prints `OLD`'s output. So beyond Task 1's shape, *"that line is
read as the declaration"* is the work of one or more of batch 2's five later
commits (I did not bisect which) reading the lexer's `closes`. **Repair**: say
that question (a) judges the lexer rule together with the parser's use of
`closes`, and give the compiler-engineer `RULE`'s route (41807577's `selfhost/`
built by c85bccb8's seed compiler, about a minute) to separate the two.

### 2.6 Two predicates the brief's map does not name

- `selfhost/parse/headless.hero:185`, a second `opens_a_declaration`, over token
  kinds, with another list (`function`, `constant`, `record`, `variant`; no
  `use`, `extern`, `test`), for the shape of a block below a failed line. A
  seat that greps the name finds two.
- `selfhost/next_line.hero:240`, `starts_afresh`, called at
  `selfhost/open_line.hero:209` for panel 181's depth-zero rule. Its statement
  words are `return` `break` `continue` `assert` `for` `while` `else`, and it
  answers false for `if` and `match`, the two expressions. (b)'s list takes `if`
  and `match` in and leaves `break` and `continue` out. By the frozen lexer,
  `break` and `continue` open none of the 11,043 lines inside brackets in the
  1358 tracked `.hero` files (tabulated by first token; the spec's fences not
  in that table, and `reach.py` found no statement word in them), so the
  difference concerns programs outside the tree only. **Repair**: name both in § Where the compiler
  does it, and say why (b)'s list is not `starts_afresh`'s.

Minor: `LexOutput` is also built with `closes` at `selfhost/lexer.hero:215` and
`220`, beside the five lines listed.

### 2.7 The ceiling's measure, in numbers

The brief gives `wc -l` and says the ceiling is `suite_layout.hero`'s measure.
That measure is `code_lines` (non-blank lines outside `test` blocks), held to
`CEILING` 300 unless the file is in `DECIDED`, and none of the named files is
(`DECIDED` is lines 435 to 476 of `suite_layout.hero`, 18 rows, and a grep of
them for `lexer|layout|next_line|unclosed|headless|state.hero|cursor` finds only
`selfhost/resolve/state.hero 330`). By
my Python port of `code_lines`, which is a port and not the instrument:
`layout.hero` 201, `next_line.hero` 207, `parse/unclosed.hero` 241, `lexer.hero`
210, `parse/headless.hero` 226. **Repair**: give the harness's own numbers, so
the cost task starts from the room each file has.

### 2.8 "Reported where a line at column 0 opens a declaration"

Both compilers print the diagnostic at the opener (`task1.hero:10:10`, the
caret under `[`); what moved is where the lexer detects it and where the reach
ends. design.md's *"reported at end of file, citing the opener"* uses the word
the same way, but the spec-warden's task 3 replaces that sentence, and "reported
where" reads as a printed position. **Repair**: *detected, and the reach ended,
where a line at column 0 opens a declaration; in both compilers the diagnostic
cites the opener's own line and column.*

### 2.9 Two small things in the blind seat's brief

- *"Write your report to `183-llm-ergonomist/report.md` in your directory"*
  reads as `<dir>/183-llm-ergonomist/report.md`; the seat has Write only and
  needs one absolute path, `<scratchpad>/183-llm-ergonomist/report.md`.
- *"Each program holds a bracket that is opened and never closed, and other
  mistakes after it"* is true, and it hands the seat one mistake of each program
  before task 1 asks it to predict them and before task 2 asks for a fix
  "after reading that output alone". A design point, not a falsehood; *each
  program holds mistakes* would leave the seat to find them.

### 2.10 The spec's silence: true, with one neighbour

The grep in both briefs returns nothing (exit 1); no version of the spec ever
matched (`git log -i -G 'unclosed|never closed' 171e8c45 -- spec/heroes-spec.md`
prints nothing); my full read of its 429 lines and a wider grep
(`unterminated|unbalanced|opener|end of file|never close|left open|bracket`)
find no sentence about an opener never closed. The neighbour: § 1 says *"a
string is one line"* (spec line 36), a bound on the reach of a quote never
closed, which batch 2's 6795bedd repaired, and § 2 says an f-string's hole
*"ends at the `}` that closes it"* (line 51). **Repair**: one line in the
spec-warden's brief, as the shape beside the one asked about.

## 3. Verified, with the command

- **The commits.** `git log -1 HEAD` is 171e8c45, 2026-09-30 04:51:11; `git
  reflog show --date=iso main`: `e5cc73eb main@{2026-09-30 04:46:19 +0200}:
  merge lane-recovery-b2: Fast-forward`, then 171e8c45 at 04:51:11.
  41807577's subject is as quoted and it is in batch 2; c85bccb8 is its parent.
  `git diff --stat e5cc73eb 171e8c45 -- seed/` is empty; the two commits differ
  in `docs/ROADMAP.md` and `docs/work/DEFECTS.md` only. design.md is untouched
  from c85bccb8 to 171e8c45.
- **design.md.** `grep -n "reported at end of file"` prints line 1944; the
  quoted sentence is verbatim over lines 1943 to 1945, in the bullet
  *Continuation lines: inside brackets only.* under § 4.15 (line 1911), Part 4
  (line 812). Part 11 is line 3904.
- **Rule (a)'s words** are `next_line.opens_a_declaration`
  (`selfhost/next_line.hero:220`): the seven words, `function` only when spaces
  and then a letter or `_` follow; `layout.hero:94` calls it only when `spaces
  == 0`. On the nested program of 2.5, `NEW` names all three openers, as *"reports
  every opener still open there"* says.
- **The counts, by my own method** (`_critic/mine.py`): the list from `git
  ls-tree -r --name-only 171e8c45`, the contents from my archive, the fences
  paired line by line, the plain dump (`lex --dump-tokens`, not `--json`), a
  line's first token found at the first non-blank column of the source text, the
  depth there the bracket balance of every token before it floored at zero, and
  every empty dump counted rather than skipped. It printed: 1358 files; 8
  fences (spec lines 87-105, 123-127, 165-174, 177-183, 219-223, 281-287,
  322-325, 344-350); 915946 tokens; no empty dump (the lexer exits 1 on 108
  sources and prints their tokens anyway); the column-1 table exactly as the
  brief's; 55 declaration lines in 6 files, by the next token and by
  `opens_a_declaration`'s text rule alike; 34 statement lines in the same 12
  files at the same line numbers. The 44th column-1 `function` is
  `fixedbugs-130-a-declaration-past-a-bracket-never-closed.hero:18`,
  `function(i64) -> i64)) -> i64`, a function type. My alignment skipped 22
  source lines (a tab or a raw carriage return before the first token), in 4
  files that exit 1, none of them opening with one of the fourteen words.
- **The census**, which 00-shared.md's premise *"nothing that compiles changes"*
  rests on: `check --brief`, both arms, `OLD` and `NEW`, all 1358 files, each
  from its own directory (`_critic/census_one.sh` under `xargs -P 6`, 44 s of
  wall): 5432 results; the outputs differ for 6 files in both arms, the six
  repair cases; no exit code differs; `NEW`'s normal arm reads 651 at exit 0 and
  707 at exit 1. On the tracked tree nothing that compiles moved.
- **The two programs.** 00-shared.md's two blocks equal `p183/ab/task1.hero` and
  `task2.hero` (`cmp`); my `OLD` and `NEW` runs reproduce the four `ab/*.txt`
  byte for byte; fixing exactly the named mistakes gives exit 0 on both
  (`task1.fixed.hero`, `task2.fixed.hero`); `print(1))` gets `2:13
  expected_end_of_line` on both compilers.
- **The code map.** `close_brackets` at `layout.hero:177`,
  `opens_a_declaration` at `next_line.hero:220`, `closes` at `lexer.hero` 46,
  119, 124, 131, 150 and `state.hero:95`, `never_closed` at
  `parse/unclosed.hero:47`; `wc -l` 304, 289, 328, 712.
- **The blind seat's directory.** Eight files, the seven the brief names and
  `brief.md`; `spec.md` equals the spec at 171e8c45 and `brief.md` the
  repository's brief (`cmp`); the two programs equal `ab/`; P, Q, R and S as in
  1.1.
- **The rebuild.** SKILL.md's § Two lanes is line 11 and its 2026-09-24 figure
  (`real 60.97 s`) lines 118 to 121; my build of 41807577's `selfhost/` read
  `real 66.80`, `user 54.65`, `sys 7.42`, so "about a minute" holds.
- **The spec's count.** `./heroes measure spec/heroes-spec.md` printed 6716,
  6838, 6838, spread 122, `real 9060 claude-opus-5, 2026-09-28`, headroom 1180
  against 10240, 60 mortgaged by the FFI floor, 9120 measured against the
  ceiling. The pin (`SPEC_REAL_TOKENS` 9060, `SPEC_REAL_TAKEN` `"2026-09-28"`,
  `SPEC_DIGEST` `"2178dab476a99fa6"`, `selfhost/measure/pinned.hero`) landed in
  4c453f5a, the spec's last change (`git log -1 -- spec/heroes-spec.md`:
  2026-09-28 08:22:13), and `measure` printed no stale line, so the date is the
  spec's own. `.env` exists, and sourcing it sets `ANTHROPIC_API_KEY` to 108
  characters (length only). Panel 012 is `012-spec-budget-3000.md`: *"a named
  removal or a pre-registered falsifiable prediction"*.
- **The historian's framing.** CPython: *What's New In Python 3.10*
  (https://docs.python.org/3/whatsnew/3.10.html) shows `SyntaxError: '{' was
  never closed`; locally 3.9.6 prints `invalid syntax` and `unexpected EOF while
  parsing`, 3.13.15 and 3.14.7 print `'[' was never closed` at the opener.
  rustc 1.90.0 on a missing `}`: `error: this file contains an unclosed
  delimiter` at 10:3, the end of the file, with `unclosed delimiter` at the
  opener and `this delimiter might not be properly closed...` / `...as it
  matches this but it has different indentation`. *Panic mode* and
  *synchronizing tokens* are the textbooks' terms: a web search returns course
  notes using both (https://teaching.idallen.com/cst8152/98w/panic_mode.html,
  https://www.cs.clemson.edu/course/cpsc827/material/LLk/LL%20Error%20Recovery.pdf),
  which I did not open. The language's own rule (a line inside brackets goes on
  at any column, outside them indentation is structure) is spec lines 11 to 19
  and 33 to 34, and design.md § 4.15.

## 4. The repairs, in the order I recommend them

1. Output S (1.1): the ergonomist after the prototype, with a real S; or both
   briefs saying what S is.
2. Six of seven (1.2): name the six cases.
3. The C boundary (1.3): the negative becomes a question, and the `extern`
   member slip joins the compiler-engineer's edges.
4. The spec-warden's figures (2.4): the e5cc73eb column, `string-open`,
   *parse-stage*.
5. The probes of (b) (2.3): into the sitting's records and the
   compiler-engineer's hands.
6. The comparison (2.5): the rule alone against the batch, with `RULE`'s route.
7. clang (1.4), the two predicates (2.6), the measure's numbers (2.7),
   "reported" (2.8), the blind seat's path and its hint (2.9), the spec's
   neighbour (2.10), the script's arguments (2.1) and the command behind the 15
   (2.2).
