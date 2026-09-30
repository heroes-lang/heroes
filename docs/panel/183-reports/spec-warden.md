# Panel 183, spec-warden

Written 2026-09-30 from 05:37, as the sitting went. The tree is a `git archive`
of `171e8c45` in `scratchpad/183-spec-warden/` (the session scratchpad), with
its compiler built from that commit's seed (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, `real 4.24`). Every number below names the
command or script that produced it, run in this sitting. Everything I built or
wrote is under `183-spec-warden/_warden/`. Nothing was built or run in the
repository's working tree. This file is the one thing I wrote there.

## Verdict

- `verdict`: **approve (a) as landed**, with the design.md sentence in Task 3
  and no spec sentence. **Object to (b) as posed.** **Approve (b) amended** to
  the margin rule below, with `else` only when shallower, provided the
  compiler-engineer's own prototype reproduces these numbers. The counts are
  measured, not estimated, so none of this is provisional.
- `section`: design.md §1.6 (the budget), §1.2 (the rewrite rate), §1.0 with
  design.md:590 (what Principle 0 binds), §4.15 (the sentence) and §4.17 (a
  diagnostic carries what fixes the program).
- `spec_token_delta`: **0**, measured on `claude-opus-5`: 9060 before, 9060
  after, re-read today by `--refresh`. The two sentences I refuse would cost
  +35 (9095) and +71 (9131).
- `removal`: nothing, and none is owed, since the delta is 0.
- `needed_for_self_hosting`: **no**. `selfhost/main.hero`, the compiler's root
  module, checks clean (exit 0, no output) in both arms under `c85bccb8`'s
  compiler, which predates rule (a), under `171e8c45`'s, and under all four of
  my prototypes of (b). No tracked file changes exit code between any two of
  them (census below).
- `argument`, `prediction`, `condition`: at the end of this file.

## The ceiling, grepped

`grep -n "^### 1.6\|10240" docs/design/design.md` gives design.md:255: *must
fit in 10240 tokens, measured by `claude-opus-5` through `POST
/v1/messages/count_tokens`*. The payment rule is unconditional
(design.md:311-314).

## The count, measured

`./heroes measure spec/heroes-spec.md`, exit 0: `claude-legacy` 6716,
`cl100k_base` 6838, `maximum` 6838, spread 122, `real` 9060 (`claude-opus-5,
2026-09-28`). Headroom is 1180 against 10240; the FFI floor mortgages 60, so
9120 is what is measured against the ceiling. The coordinator's figures
reproduce exactly.

`--refresh`, with the repository's `.env` sourced (`${#ANTHROPIC_API_KEY}`
printed 108), gave `SPEC_REAL_TOKENS 9060`, `SPEC_REAL_TAKEN "2026-09-30"`,
`SPEC_DIGEST "2178dab476a99fa6"`. So the binding number is the pinned one,
re-read on the reader's tokeniser today.

## Task 1. Is a spec sentence owed? No

**Neither rule changes what compiles, by the grammar.** I checked this myself.

- All fourteen words are reserved (`selfhost/keywords.hero`: *the closed list
  ... these can never be identifiers*), so none of them can be a name inside
  an expression.
- (a)'s words head only `Use`, `Declaration` and `Member` (spec § 1, § 4,
  § 13), and none of those stands inside brackets. `function` also opens a
  function type, but there `(` follows it (spec § 3, `"(" "function" TypeArgs`,
  with `TypeArgs` opening on `(`), never a name.
- (b)'s words head statements and the `If` and `Match` primaries. Both need a
  `Block`, which needs `INDENT`, and inside brackets the lexer emits none
  (design.md:1939-1941). Four programs of mine put `if` or `match` inside `(`
  or `[`, on the opener's line and on a line of their own
  (`_warden/probes/g1` to `g4`). All four exit 1 on the missing body or arms.

**The census agrees.** Every tracked `.hero` file was checked with `check
--brief`, both arms, each from its own directory (`census_a.sh`, `census_one.sh`
under `xargs`):

- `c85bccb8`'s compiler (the frozen `instrument/tree/heroes`) against
  `171e8c45`'s, over 1358 files: **0 exit codes differ** in either arm, 651 at
  exit 0 in the normal arm both times. Output moves in exactly 6 files, the six
  repair cases batch 2 added. This is the critic's result, re-measured.
- The control against each of my four prototypes of (b), below: 0 exit codes
  differ, in either arm, for any of them.

**So the spec is not falsified by either rule.** It is the language, and its
sentence *any other line goes on below, at any column* (spec line 15) stays
true of every program that compiles. So does design.md:1980, panel 180's copy
of it. Where the lexer finds a mistake is a diagnostic's placement. design.md
§4.17 gives the diagnostic the job of carrying the fix, and both compilers
already cite the opener (Task 1, `10:10`).

**The reader's mistake a sentence would prevent: none that I can name.** A model
does not leave a `)` out because it misjudged where the reach ends, and knowing
the reach does not help it put the `)` back. The effect of both rules arrives in
the output of a failed round, while every prompt would pay for the sentence
(design.md §1.6: *the spec is the prompt*). The silence was never a ruling
either way. Panel 007 put the diagnostic in design.md as the engineer's
condition and left the spec untouched (`docs/panel/007-terminator-enders.md`,
resolution items 2 and 4), and the critic found that no version of the spec
ever held such a sentence.

**What I would have written, priced on the reader's tokeniser, so the sitting
knows what the refusal saves.** Each draft was merged after the preamble's
*... breaks after an operator.*, applied to my copy, measured with
`--refresh`, and reverted (`cmp` silent afterwards):

| draft | text added | vendored max | real |
|---|---|---|---|
| none | | 6838 | 9060 |
| A, (a) | *A bracket still open at the end of the file, or where a line at column 0 opens a declaration, is an error.* | 6867 (+29) | **9095 (+35)** |
| B, (a) and (b) | *A bracket still open at the end of the file, where a line at column 0 opens a declaration, or where a line opens with `if`, `while`, `for`, `match`, `return`, `assert` or `else`, is an error.* | 6895 (+57) | **9131 (+71)** |

Neither is owed.

## Task 2. Principle 0, and what the recovery numbers bear on

**The burden does not apply as a burden of entry.** §1.0 is *the burden of
proof for any proposed form*, and design.md:590 says it *binds what enters the
language*. Neither rule adds a form or moves a program across the compile line
(Task 1). Each is the shape of a feature already admitted: panel 007's
unclosed-opener diagnostic (§4.15), inside §1.0's own *rich errors*
(design.md:119). The shape of an admitted feature is judged the way §1.12
judges one (design.md:590-593), which here means on §1.2's rewrite rate and
§1.1's ceiling. **If the panel holds that Principle 0 applies anyway**, a
measured argument meets it for (a) and for amended (b), and fails it for (b) as
posed, by the numbers below.

**The reports, read at their source and replayed.** `hidden.py` rebuilds every
pair that the column *as the first, parse-stage seconds: HIDDEN / pairs* counts,
from the instrument's own `plan-singles.jsonl` and `pairs.jsonl`. It reproduces
the column exactly: at `c85bccb8`, `bracket-open` 28/164, `string-open` 41/170
and `wrong-closer` 68/159; at `e5cc73eb`, 8/164, 1/170 and 68/159.
`replay.py` re-runs the instrument's mutants on any compiler by its own method:
the mutant beside its original in a mirror of the corpus, then `check --brief`
from the mirror's root. My control agrees with the instrument on **all 493
pairs** and **all 13,594 singles** (exit, number of diagnostics,
`unclosed_bracket`). That held once I had fixed a pattern of mine that could not
read a code with a digit in it, `indentation_not_multiple_of_4`.

- **They bear on (a).** 28 hidden becomes 8, but the sets do not nest (keys
  compared). Batch 2 freed **25** pairs, **20** of them with a column-0
  declaration between the two mistakes, which is the shape rule (a) names. 5
  went the other way. At `c85bccb8` each of those was told only because
  cascade landed on its lines (the instrument's `told = other`: codes such as
  `expected_declaration` and `missing_body`, never the second's own), and batch
  2 removed that cascade. Of the 8 still hidden, 2 have a declaration between,
  and in both a `)` meets an unclosed `[` (the wrong-closer shape below). The
  compiler-engineer separates the rule from the parser commits.
- **They do not bear on (b) through `wrong-closer`.** In all 68 hidden
  `wrong-closer` pairs, the bracket depth where the second mistake's line
  begins is **0**, by the lexer's own token dump (`depth.py`). No opener is
  open there. In 43 of them a column-0 declaration already stands between the
  two mistakes, and none prints `unclosed_bracket`.
- **They bear on (b) through `bracket-open` only at the edge.** By the text, 6
  of the 8 hidden pairs have a (b) word opening a line inside the reach before
  the second mistake. Rule (a) already ends the reach in 2 of those, so at most
  **4 of 164** can move. My prototype of (b) as posed frees exactly those 4
  (`growth.hero`, `strings.hero`, `lastbracket.hero`, `layout.hero`). The
  margin rule below frees 1, `layout.hero`'s. The other three have their opener
  in a function's head, and a body is always deeper than its head, so no margin
  rule reaches them. Only parameters and types stand inside a head's brackets,
  so a statement word there never continues an expression. Whether the rule
  should say so is a question for the compiler-engineer.
- **(b)'s own shape is in the sample once.** The instrument planted 6 pairs of
  an unclosed bracket followed by a closer mistake (`extra-closer`,
  `stray-closer-head`, `wrong-closer`), all past the first's extent and all
  told. One of them is Task 2's shape: `selfhost/open_line.hero`, where `left
  = l.tokens[at` lost its `]` (280), `if head.kind == .minus` stands at the
  opener's margin (284), and an extra `)` at 296 would close the `[`. The
  control never names the opener: it prints `expected_index_close` at 281 and
  `missing_body` at 285 and 290, on bodies that exist. Every margin rule below
  prints `unclosed_bracket` at 280 and `expected_end_of_line` at 296, and
  nothing else. The instrument counts the pair as told either way.
- **The singles are where (b) is decided** (next two sections).

## The strongest reason (b) is wrong: it says a closed bracket is never closed

I built (b) as posed into a copy of `171e8c45` (`_warden/proto-b/heroes-b`). It
widens `layout.hero:93-97`'s test by one predicate over the seven words, at any
column. For a clean comparison I also built the unpatched `selfhost/` by the
same route (`_warden/heroes-ctl`). The prototype prints the shared brief's
prediction for Task 2 exactly: `unclosed_bracket` 5:19,
`expected_end_of_line` 10:25, `expected_expression` 11:18.

**Then I ran the shapes beside it.** Each is a program in which a model breaks a
common expression from another language across lines, inside a bracket that it
DID close. All exit 1 under every compiler.

| program (`_warden/habits/`) | control: diagnostics / `unclosed_bracket` | (b) as posed |
|---|---|---|
| h1, a Python comprehension, `for x in xs` on its own line | 1 / 0 | 4 / 1 |
| h2, Black's conditional expression, `if` and `else` lines | 1 / 0 | 5 / 1 |
| h3, a Kotlin or Rust `if` argument | 1 / 0 | 3 / 1 |
| h4, a JS callback with `return` | 1 / 0 | 6 / 2 |
| h5, a Rust `match` argument | 1 / 0 | 3 / 1 |
| h6, a generator with a filter | 1 / 0 | 5 / 1 |

**The corpus says the same.** Over the 1358 tracked files, (b) as posed changes
the output of 9 and adds `unclosed_bracket` to 5. All 5 are false, each bracket
closed below: `impl Point { ... }` and `impl Shape { ... }`
(`fixedbugs-130-braces-under-a-line-nothing-reads.hero:33:12`,
`fixedbugs-130-declarations-under-a-refused-head.hero:19:12`), `if x == 2 {
... }` (`fixedbugs-131-a-body-in-braces-indented-inside.hero:33:15`),
`f(\n match t\n _ => 1)`
(`fixedbugs-131-arms-at-the-margin-of-their-match.hero:83:10`), and
`surface-fixtures/brackets180/shape073.hero`'s `f(if c ... 2)`.

**And so do the instrument's 13,594 planted single mistakes.** Under (b) as
posed, 180 change output (148 gain diagnostics, 32 lose some), and **25 gain
an `unclosed_bracket`: 19 `brace-else-chain` and 6 `brace-allman`.** All 25 are
false. In each, the lexer's own tokens balance: the depth returns to 0, never
goes below it, and the braces pair. I also opened two by hand
(`examples/assembler/main.hero:54`, `selfhost/ast.hero:720`): an Allman `{`
under a `match` arm, closed three and seven lines below. The control tells the
one true mistake. (b) as posed adds *`{` opened here is never closed* and three
or four `expected_pattern` or `expected_expression` lines. The brace habit
fills the first four rows of the instrument's catalogue.

**It does not reach the class it was raised for, either.** Of its four
reproducers, u2_p (`print(x) )`), u4_k (`y = 3 )`) and u5_i (`print(a) )`)
hold no (b) word, and the prototype prints on them byte for byte what the
control prints. Only u2_q moves (its `if x > 0` line), to 2 diagnostics, both
true.

## (b) amended: the margin rule, run rather than argued

What separates Task 2 from the habit shapes is visible on the line. In Task 2
and u2_q the statement word stands at the margin of the line that opened the
bracket (column 4, under `    total = double(3`); in every habit shape it
stands deeper. One word, `else`, continues an `if` begun on the opener's own
line, and the language's own layout of an `if` expression puts that `else` at
the margin. `heroes fmt` prints `x: i64 = if c`, then `1` one level deeper,
then `else` under the `x` (`_warden/probes/ifexpr_valid.hero`, exit 0). So g1,
`x: i64 = f(if c` ... `else` ... `2)`, draws a false `unclosed_bracket` from a
margin rule that keeps `else` at the margin. An `else` shallower than the
opener's line cannot belong to an `if` inside the bracket, and one planted
single needs exactly that (`selfhost/layout.hero:150`: a `)` left out on an
`if` body line at column 8, and `else` at column 4). I built three margin
rules. Each ends the reach at a line inside brackets whose first token is
`if`, `while`, `for`, `match`, `return` or `assert`, when its indentation is at
most that of the line that opened the outermost open bracket
(`l.open_brackets[0]`, the stack's bottom, `state.hero:139-145`). They differ
only on `else`: at the margin as well (`proto-b2`), never (`proto-b3`), or
**only when strictly shallower (`proto-b4`), the one I recommend**. `proto-b4`
adds 33 lines and removes 1 across `layout.hero` and `next_line.hero`, comments
and blanks included (`git diff --no-index --numstat`).

| | control | (b) as posed | margin, `else` at it | margin, no `else` | **margin, `else` shallower** |
|---|---|---|---|---|---|
| Task 2, diagnostics / `unclosed_bracket` | 2 / 0 | 3 / 1 | 3 / 1 | 3 / 1 | **3 / 1** |
| u2_q | 1 / 0 | 2 / 1 | 2 / 1 | 2 / 1 | **2 / 1** |
| h1 to h6, g3, g4 (eight habit shapes) | 1 / 0 each | 3 to 6 / 1 or 2 | 1 / 0 each | 1 / 0 each | **1 / 0 each** |
| g1, an `if` argument in the language's own layout | 1 / 0 | 4 / 1 | 4 / 1 | 1 / 0 | **1 / 0** |
| g5, g6, g7 (`if`, `match`, `return` AT the margin, closer written) | 1 / 0 | 2 to 4 / 1 or 2 | same | same | same |
| g9 (a true unclosed `while` condition, then `for`) | 3 / 1 | 2 / 1 | 2 / 1 | 2 / 1 | **2 / 1**, a false `missing_body` gone |
| tracked tree, 1358 files, both arms: outputs changed | | 18 (9 files) | 0 | 0 | **0** |
| golden `check` form | 284 passed, 0 failed | 276 passed, **8 failed** | | | **284 passed, 0 failed** |
| 13,594 singles: gain an `unclosed_bracket` | | **25** | 0 | 0 | **0** |
| 13,594 singles: output changed | | 180 | 31 | 30 | **31**, all `bracket-open` |
| of those, true diagnostic kept | | 180 | 31 | 30 | **31** |
| their diagnostics off the mistake's own lines, before and after | | **122 to 603** | 113 to 10 | 112 to 10 | **113 to 10** |
| singles turned from EXTRA to exact | | 21 | 21 | 20 | **21** |
| instrument pairs, `bracket-open` hidden (of 164) | 8 | 4 | | 7 | **7** |
| instrument pairs, `wrong-closer` hidden (of 159) | 68 | 67 | | 68 | **68** |

(`singles_judge.py` computes every singles row for every prototype by one
method. Each base program compiled clean, so any diagnostic off a single's own
lines is cascade.)

**What the table says.** On the singles it moves, (b) as posed multiplies the
cascade by about five, from 122 diagnostics to 603. It adds 25 false
`unclosed_bracket`, and it turns 8 green golden cases red. Every margin rule
does the opposite on the one operator it moves, `bracket-open`, the mistake this
sitting is about. There it leaves 10 of the 112 or 113 cascade lines, and the
removed ones include `missing_body` on bodies that exist. It keeps the true
diagnostic every time, turns 20 or 21 EXTRA outputs exact, and adds no false
diagnostic anywhere. The `else`-shallower rule matches the best of the other two
on every row. g5, g6 and g7 are the
limit of any rule that reads one line: whether a closer further down belongs to
this opener cannot be seen on the line. Each of them puts a line inside a
bracket no deeper than the statement that opened it, which none of the nine
ordinary layouts does. How often a model writes that is unmeasured. The
measured gain is modest, 1 hidden pair of 164 and 21 of 156 `bracket-open`
singles made exact. Whether that pays for 33 lines is §1.1's question, and the
compiler-engineer's to price.

## Task 3. design.md's sentence

It replaces design.md:1943-1945, the sentence *An unclosed opener is a compile
error reported at end of file, citing the opener*, together with the reason it
gives after a dash: *without that diagnostic one missing `)` would silently
swallow the rest of the file's layout*. For (a) as landed:

> A closer of any kind closes the innermost opener still open. An opener still
> open at the end of the file, or at a line with no indentation that begins
> with the keyword `record`, `variant`, `constant`, `use`, `extern` or `test`,
> or with `function` before a name, is a compile error citing the opener, and
> that line is laid out as if no bracket were open. Without that diagnostic one
> missing `)` would silently swallow the rest of the file's layout; without
> that line, the declarations below it and their mistakes (panel 183).

With (b) amended (`proto-b4`), after *before a name,* add: *or at a line that
begins with `if`, `while`, `for`, `match`, `return` or `assert` no deeper than
the line that opened the outermost of them, or with `else` shallower than that
line,*. The last sentence then ends: *...; without those lines, a stray closer
further down would close it unnamed.*

**Each clause, and the program that makes it true** (my runs, `_warden/`):

| clause | why it is there | run |
|---|---|---|
| *a closer of any kind closes the innermost opener* | without it the sentence is false of `[)`, whose `[` is never reported | `state.hero:139-145` pops the last opener for any closer; `probes/wc1` to `wc3` print no `unclosed_bracket`; u2_p's stray `)` closes its `[`; `words/fstring_hole`'s `}` closes a `(` |
| *at the end of the file* | panel 007's case | `words/eof_only`: `2:10` |
| *with no indentation* | an `extern` member at column 4 ends nothing | `sitting/extern_member_unclosed`: the reach runs to `function main()` |
| the six keywords, and *`function` before a name* | a function type opens with `function (` | `words/w_*`: all seven words report `2:10`; `next_line.hero`'s own test rejects `function(i64) -> i64)` |
| *citing the opener*, each | nested openers | `words/nested`: `2:10`, `2:12`, `2:16` |
| *laid out as if no bracket were open* | the declaration's own mistakes are told | `w_record` 6:12, `w_variant` 7:16, `w_constant` 7:1, `w_use` 5:10, `w_extern` 6:33, `w_test` 7:1, `w_function` 6:14, Task 1 14:14 |
| both arms | | census: 0 files where the arms differ on `unclosed_bracket` |

The old sentence's *reported at end of file* named when the lexer finds the
opener, not where the diagnostic prints (the critic's 2.8). The new one says
*citing the opener* and never *reported at*, so it cannot be read as a printed
position.

## Neighbours found on the way, for the synthesis

- **The wrong-closer class is the largest parse-stage hiding in the
  instrument's table, and no open item names it.** `wrong-closer` hides 68 of
  159 parse-stage seconds (43%) at both compilers. The lexer pairs any closer
  with the innermost opener (`state.hero:144-145`), so its depth is back to 0.
  Yet after the parser's first error nothing more is told:
  `_warden/probes/wc4_three_below.hero`, whose `xs = [1, 2)` is followed by
  three functions each holding a mistake, prints one diagnostic, while the same
  file with `]` prints all three. It is a question for the compiler-engineer
  where the parser's recovery goes after a closer of the wrong kind. I grepped
  `docs/work/DEFECTS.md`, `DECIDE.md` and the milestone files for `wrong
  closer`, `wrong kind`, `wrong-closer`, `mismatched closer`, `closer of the
  wrong` and `closer`, and found no item. The instrument's own findings hold it
  (`instrument/run-e5cc73eb/findings/HIDDEN/151-wrong-closer.hero` onward).
- **The same root inside an f-string hole.** In `print(f"{g(n}")` the `}`
  closes the `(`, the parser runs to `8:1 expected_hole_close` (*found end of
  file*), and the next line's `print(n +)` is never told
  (`_warden/words/fstring_hole.hero`).
- **(a)'s one measured cost: noise, not falsehood.** Python's triple-quoted
  string inside a call (`_warden/habits-a/t1`, `t2`) already had its `(` truly
  unclosed at the token level, because the author's `)` sits inside the second
  broken string. Both compilers say so. Under (a) the column-0 text lines are
  also read as declarations (`use --help` draws `expected_module_name`), and
  the output grows from 3 diagnostics to 7 and 6, the first three unchanged.
  The instrument plants no triple quote, so the frequency is unmeasured.

## Argument, prediction, condition

- `argument` (≤120 words): (a) changes no program that compiles. Its seven
  words are reserved and head only declarations, which no bracket holds (0 of
  1358 exit codes moved), so the spec owes and pays nothing (9060 real,
  re-read today) and the diagnostic carries the fix (§4.17). The design.md
  sentence must say how a closer pairs, or it is false of `[)`. (b) as posed
  says a closed bracket is never closed: 25 of 13,594 planted mistakes, 8
  golden cases red, 9 of 9 habit shapes. Where it acts it multiplies the
  cascade fivefold, and it reaches 1 of its own 4 reproducers. §1.2
  refuses that. At the opener's margin, with `else` only shallower, it adds 0
  false diagnostics and makes 21 cascades exact.
- `prediction`: with (b) amended landed, `recovery.py` over the same 641
  programs reads `bracket-open` as-the-first 7/164 hidden (from 8),
  `wrong-closer` 68/159 (unchanged), `bracket-open` singles EXTRA 46 (from 67),
  and no single gains an `unclosed_bracket`. With (b) as posed instead, 25
  `brace-*` singles gain one, and the golden `check` form reads 276 passed and
  8 failed, as it did on my prototype. The spec's real count reads 9060 at the
  sitting's ratification commit.
- `condition`: I approve amended (b) only if the compiler-engineer's own
  prototype reproduces 0 changed outputs on the tracked tree in both arms, the
  golden `check` form at 284 passed and 0 failed, 0 new `unclosed_bracket` over
  the 13,594 singles, and Task 2's three lines. I would object to (a) if a
  program that compiles were found to change: none was, by the grammar or the
  census. I would write a spec sentence, and name its removal, only if a blind
  reading (the llm-ergonomist's) showed a model misreading the reach in a way
  that the sentence repairs.
