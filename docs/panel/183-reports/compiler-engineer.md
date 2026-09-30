# Panel 183, compiler-engineer

Written 2026-09-30 from 05:36, as I went. Every number below names the command
that produced it, run in my own directory, `<seat>` =
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/183-compiler-engineer/`,
a `git archive 171e8c45` with `build` removed. Every probe named below is a
file under `<seat>/t1` to `<seat>/t5` or `<seat>/p183`, with each compiler's
output beside it (`<probe>.hero.<COMPILER>.txt`).

## The compilers

| name | built from | check |
|---|---|---|
| `NEW` | `<seat>/heroes`, 171e8c45's seed (`clang -I runtime seed/heroes.c runtime/runtime.c`, `real 4.39`) | |
| `OLD` | `<seat>/c85/heroes`, `git archive c85bccb8`'s seed | `cmp` silent against `scratchpad/instrument/tree/heroes`, the brief's frozen lexer |
| `RULE` | `<seat>/r41/heroes-rule`, `git archive 41807577`'s `selfhost/` built by `OLD` (`real 57.55`, `user 49.48`) | 41807577's parent is c85bccb8 (`git log -1 --format='%H %P'`) |
| `PB` | `<seat>/pb/heroes-b`, question (b) exactly as posed: the seven words, any column, through the landed rule's own mechanism | built by `NEW` |
| `PN` | `<seat>/pn/heroes-n`, (b'): one predicate for (a), (b) and (c), bounded by margin | built by `NEW` |
| `PE` | `<seat>/pe/heroes-e`, (b''): (b') with `else` needing a strictly shallower margin | built by `NEW` |

On the two tasks, `RULE` prints `NEW`'s output byte for byte (`cmp` silent on
`task1` and `task2`, exit line included); `OLD` differs from `NEW` on task 1
only.

## Task 1: the grammar question

**(b) No program that compiles places `if`, `while`, `for`, `match`,
`return`, `assert` or `else` anywhere inside brackets, at the head of a line or
not.** The argument, from the spec at 171e8c45:

- `return`, `break`, `continue`, `assert`, `while` and `for` occur only in
  `Statement` (spec lines 143 to 149), `While` and `For` (237, 238) and an arm's
  `Inline` (242, 243). A `Statement` stands only in a `Block`, `INDENT {
  Statement } DEDENT` (142), and an `Inline` only in an `Arm`, inside `Match`'s
  `INDENT { Arm } DEDENT` (240).
- `if` and `match` are `Primary` (212), expressions, but `If` is `"if"
  Expression Block ...` (239) and `Match` needs `INDENT` (240). `else` occurs only
  in `If`.
- Inside `(` `[` `{` the lexer makes no `INDENT`: `layout.line_start` returns
  before the margin is read whenever `l.open_brackets.len() > 0`
  (`selfhost/layout.hero:93`), which is the spec's *"any other line goes on
  below, at any column"* (line 15).
- The parser agrees, and has no route to a block that does not refuse.
  `grammar_expr.block` (`selfhost/grammar_expr.hero:727`), which reads an `if`'s
  and a loop's body, takes `.indented`, `.braced` or `.tabbed` from
  `opening.body_follows`. `.braced` always pushes `missing_here` unless the head
  already refused that token (`selfhost/parse/opening.hero`, `braced`).
  `.tabbed` exists only where the lexer refused the margin, and the tab check
  runs before the bracket check (`layout.hero:80`), so inside brackets too. A
  `match`'s arms take `INDENT` or, in braces, `braced_arms`, which is the brace
  habit's route and reports `missing_match_arms` at the `{` (run below).

Run, not only argued: 20 programs, each placing one of the seven words (and
`break`, `continue`) at the head of a line inside brackets, including an `if`
expression, a `match` expression, a multi-line `if`'s `else`, the brace
habit's `if c { 1 } else { 2 }`, a braced `match n { ... }` on one line, on
several and in Allman style (`match_expr`'s fourth route, `braced_arms`, which
reports `missing_match_arms` at the `{`), the colon habit `if c:`, a column-0
`if`, an arm's `return` and an f-string hole (`<seat>/t1/b_*.hero`). **All 20
exit 1 under `OLD`, `NEW` and `PE`, normal arm and `--permissive` alike**; the
control `ok_if_outside.hero` (the same `if` bound outside the call) exits 0 on
all of them.

**(a) No program that compiles places `record`, `variant`, `constant`, `use`,
`extern` or `test` inside brackets at any column, nor `function` before a
name.** All 21 words of `keywords.keywords()` are reserved, *"these can never
be identifiers"* (`selfhost/keywords.hero`); the six occur only in `Declaration`,
`Use` and `Member` (spec 39, 111 to 116, 421 to 427); `function` inside brackets
occurs only in `Prefix`'s `"(" "function" TypeArgs "->" Type ")"` (80), before
`(`. Run: five compiling controls with a column-0 line inside brackets that
begins with a function type, `function (`, names that start with the words'
letters (`records`, `use_count`, `tests`, `constants`, `variants`, `externs`,
`functions`), a string and a comment at column 0, and a parameter's function
type (`<seat>/t1/ok_a_*.hero`): **all exit 0 on `OLD`, `NEW` and `PE`, both
arms.** Four refused ones (`a_*.hero`) exit 1 on all three.

The landed rule's own census, run here and not only read from the critic:
`OLD` against `NEW` over the same 1358 files, both arms, moves 6 outputs per arm
and no exit, and both read 651 at exit 0 in the normal arm.

**What the grammar says about the column**: nothing. The column-0 bound in (a)
is not what makes it sound; the word is. The column decides only how the line
is laid out once the reach ends, and, as Task 3 measured, whether the author
meant the line inside the brackets.

**The three predicates.** `next_line.opens_a_declaration` (text, column 0, the
reach), `next_line.starts_afresh` (token kind, panel 181's depth-zero rule,
`open_line.hero:209`) and `parse/headless.opens_a_declaration` (token kind,
which lines make an indented block below a head nothing reads a block of
members, `headless.hero:156`). The first two answer one question, *can this
word continue anything*, in two contexts: at depth zero `if` and `match` can
(an expression goes on a line left open), inside brackets they cannot (no
block exists there). So the reach's list is `starts_afresh`'s fourteen plus
`if` and `match`, with `function` only before a name: one predicate, which the
prototypes below build on `starts_afresh` rather than beside it. The third is
another layer (tokens after the lexer) and another question (what a block is
made of), and its list is right for it; it should stay, under a name that is
not the lexer's (`declares_a_member`), since two functions named
`opens_a_declaration` with different lists is what the critic tripped on.

**(c) The `extern` member line.** A member line (`function name`, `constant`,
`record`, column 4) is a line no bracket holds in a program that compiles, by
the same argument as (a). The landed rule does not end the reach there because
of its column-0 bound only. On the critic's probe, `extern_member_unclosed.hero`,
`OLD` and `RULE` print `2:17 unclosed_bracket`, `2:25 expected_params_close`
and nothing more (the recovery past a closer takes the rest of the file); `NEW`
prints `2:17`, `4:33 expected_extern_signature`, `7:14`, and so does every
prototype. That output is the parser's (`params` asks `never_closed`,
`selfhost/parse/members.hero:79`, ac62cde9), not the lexer rule's. Where a
member line would end a reach is a stray closer on a later member:
`x1_extern_stray.hero` and `x2_extern_spec_stray.hero` (the spec's own sqlite
group, line 4's `)` left out, line 5 given one too many). For those two
mistakes `NEW` prints one diagnostic, `expected_params_close` found `->`: its
recovery pairs the opener with the stray `)` and takes the member between
unread. Measured in `x2b_member_mistake.hero`, the same group with a third
mistake inside `sqlite3_close`'s own parameters (`consumes junk`): `NEW` prints
the first diagnostic alone, though it tells the member's mistake when that
stands alone (`x2c`, `5:44 expected_params_close`); `PE` prints all three, the
opener, the member's `junk` and the stray `)`. `PN` and `PE` print the opener
and the stray closer on `x1` and `x2`. **So the answer touches what a binding's author sees**, in a refused
group, at the parse stage; no header, type, width, mark or emitted C is
involved.

## Task 2: the landed rule at its edges, and what each part owes

Every edge below has a mistake beneath the unclosed opener, so a swallowed
declaration shows. Probes in `<seat>/t2`; outputs `OLD`, `RULE`, `NEW`.

| edge | `OLD` = `RULE` | `RULE` = `NEW` | what it shows |
|---|---|---|---|
| e01 generic head below a `[` | no | yes | the rule reads `function first<T>(...)` and its own mistake |
| e02 generic head, its `(` open | yes | no | the parser's (ac62cde9): `RULE` prints `expected_params_close` too and loses line 5 |
| e03 `(1 + 2` over a no-parameter head | yes | no | the parser's: `RULE`'s `expected_group_close` recovery walks past the declaration the lexer laid out |
| e04 `function main(` with no parameters | yes | no | all three read the body line as a parameter (`2:10 expected_parameter_type`), a second message for one `)`; a legal multi-line parameter list looks the same to the lexer |
| e05 `"function main()"` and `# function main()` at column 0 | no | yes | neither ends the reach; the declaration below does |
| e06 CRLF line ends | no | yes | as LF |
| e07 a tab before `function` | yes | yes | **the rule does not fire** (`line_start`'s tab check returns first, `layout.hero:80`); the declaration is read inside the list, `5:2 expected_expression` |
| e07b the same, and a stray `)` further down | no | yes | a later column-0 declaration still ends the reach |
| e08 the file ends inside the bracket | yes | yes | the end-of-file path, unchanged |
| e08b the file ends after a declaration head | no | yes | `missing_body` at the end, a true second mistake |
| e09 four openers deep | yes | no | the parser's: all three name the four; only `NEW` reads the next declaration's mistake |
| e10 the list's last line ends with `,` | no | yes | `4:1 expected_expression`, *found the end of the block*: a second message for the one `]`, in `RULE` and `NEW` |
| e11 `function<TAB>report` | yes | yes | **the rule does not fire**: its name scan skips spaces only (`next_line.hero:232`) |
| e12 `test` blocks | no | no | the parser's |
| e13 `use` below | no | no | the parser's |
| e14 brace habit, `}` left out | yes | no | the parser's (48a433f4) |
| e15 map literal then `record` | no | yes | the lexer's, because literals already asked `never_closed` |
| e16 `variant` | no | yes | the lexer's |
| e17 `extern` below a call | yes | no | the parser's |
| e18 `function  report` (two spaces) | no | yes | fires |

**The separation, in one sentence**: where the unclosed opener is a list or map
literal, the lexer's rule is the whole change (`RULE` = `NEW`), because at
41807577 `never_closed` with `closes` was asked by `array_literal` and
`map_literal` only; where it is a group, a call, an index, a parameter list or
a head, `RULE` still recovers past a closer, and `recover_past_closer`
(`selfhost/cursor.hero:412`) ignores `closes`, so it walks over the declaration
the lexer laid out; batch 2's later commits (de991c25, 48a433f4, 6795bedd,
ac62cde9) put a `never_closed` guard before every non-test caller of it
(`parse/members.hero:138`, `parse/type.hero:207`, `:309`, `parse/unclosed.hero:185`,
`:197`, `:208`; grepped). On the six cases: `RULE` = `NEW` on
`fixedbugs-130-a-declaration-past-a-bracket-never-closed` only; the other five
differ, so what they pin is mostly the parser's work.

**The rule does what it says, with two gaps and one cost.** Gaps: a tab before
the word (e07) and a tab between `function` and its name (e11). Both lines are
already refused by the lexer (`tab_in_indentation`, `tab_in_line`) and the rule
then lets the reach run on, so a tab and a `]` cost three messages, the third
the declaration read inside the list (`5:2` and `5:1 expected_expression`, found
`function`); the file's own third mistake is told after them. Cost:
where the declaration word is itself the mistake inside a bracket that IS
closed (`a_record_col0`, `record P` at column 0 inside `[`), `NEW` prints four
diagnostics, including *"`[` opened here is never closed"* for a `[` closed on
the next line, where `OLD` printed one. I found no plausible program of that
shape (a column-0 line inside brackets is where every indentation language puts
a declaration), so I record it and do not stand on it.

## Task 3: prototype (b), and the census

**Task 2's program under `PB`** (written to `compiler-engineer-task2.txt`, 560
bytes, all on stderr, exit 1): `unclosed_bracket` 5:19, `expected_end_of_line`
10:25, `expected_expression` 11:18. The brief's prediction, exactly. `PN` and
`PE` print the same bytes.

**Its neighbours** (`<seat>/t4`): the stray closer two statements down (n1), in
a nested block (n2), below a `match` line (n3b), below an `else` (n4), on a
`return` line (n5), after a trailing `,` (n8), and u2_q: `PB`, `PN` and `PE`
each tell every mistake once, three messages where `NEW` told two. Unchanged by
every prototype: an arm line `.b =>` (n3a: an arm opens with a pattern, not a
word), a head whose body is swallowed with no stray closer (n7, already told by
`NEW`), and **three of the four reproducers the question was raised on**: u2_p
(`print(x) )`), u4_k (`y = 3 )`) and u5_i (`print(a) )`) open their lines with a
name, so no word list reaches them. u2_p is a `)` closing a `[`: the lexer's
stack pops any opener on any closer (`selfhost/state.hero:141`), which is the
brief's `wrong-closer` row and no part of (b).

**What (b) as posed breaks.** A statement word at the head of a line inside a
bracket the author DID close is a habit, not a fantasy: an `if` or `match`
expression as an argument (spec line 233 says they are expressions), a Python
comprehension or conditional expression broken across lines, a JavaScript
function expression as an argument, and above all the brace habit, whose body
lines inside `{` open with `if`, `for`, `while`, `match`, `else`, `return` and
`assert`. On these (`<seat>/t3/h1` to `h6`, `t5/x3`, `t5/x6`, `t1/b_*`), `NEW`
tells each habit once, at its own token, and never says "never closed". `PB`
says *"`(` opened here is never closed"* for a bracket that is closed later on
18 of my closed-bracket probes (a 19th, `b_hole_if`, is not closed: its only `)`
sits inside a string that never closes, and the hole's `}` pops the `(`), adds
margin errors for continuation lines whose
column the spec leaves free (`found 9`, `found 10`, `indentation_jump`), and
takes the habit messages on h1 to h6 from 7 to 27. On the closed brace habit
(x6) it turns four `missing_body`, one per brace, into six
`expected_declaration`.

**The census** (`<seat>/census_one.sh` under `xargs -P 8`, every one of the
1358 tracked `.hero` files at 171e8c45, each from its own directory,
`check --brief` and `check --brief --permissive`, output and exit compared by
`cmp`; `NEW` reads 651 at exit 0 and 707 at exit 1 in the normal arm, the
critic's numbers):

| compiler | outputs moved, normal | exits moved, normal | outputs moved, `--permissive` | exits moved, `--permissive` |
|---|---|---|---|---|
| `PB` | 9 | 0 | 9 | 0 |
| `PN` | 0 | 0 | 0 | 0 |
| `PE` | 0 | 0 | 0 | 0 |

`PB`'s nine, every one refused before and after: `fixedbugs-130-braces-under-a-line-nothing-reads`,
`fixedbugs-130-declarations-under-a-refused-head`,
`fixedbugs-131-a-body-in-braces-indented-inside`,
`fixedbugs-131-a-brace-habit-nested-in-a-body`,
`fixedbugs-131-a-brace-on-the-line-below-its-head`,
`fixedbugs-131-a-line-going-on-past-a-closing-brace`,
`fixedbugs-131-arms-at-the-margin-of-their-match`,
`fixedbugs-131-the-brace-habit-is-one-mistake` (all `tests/golden/check/`) and
`tests/golden/surface-fixtures/brackets180/shape073.hero`: why, in each, a
statement word opens a line inside braces or parentheses the author closed,
one level under the head (brace habit) or under the call (`shape073`,
panel 180 R5's `if` inside brackets). Their diagnostics go from 103 to 153,
five of them new `unclosed_bracket`, each naming a bracket the file closes
later: `impl Point {` (closed line 36), `impl Shape {` (23), `if x == 2 {` (36),
`y = f(` (85) and `shape073`'s `y = f(if c` (12)
(`fixedbugs-131-a-body-in-braces-indented-inside`: 7 diagnostics to 26). The harness's own
`check` form: `NEW` 284 passed, 0 failed (`real 51.85`); `PB` 276 passed,
**8 failed**, those eight; `PE` 284 passed, 0 failed. Of the brief's 12
statement-word files, three do not move under `PB`. In two
(`fixedbugs-130-a-declaration-past-a-bracket-never-closed`, lines 49 and 55;
`fixedbugs-131-a-brace-habit-never-closed-is-told-once`, lines 33 and 38) a
column-0 declaration just above (`test "t"`, `function g<T>`, `function
an_if_body`, `function a_match`) already ends the reach under `NEW`, and the
brief's frozen lexer predates that rule; `PB` leaves their tokens byte-identical.
In the third (`fixedbugs-131-a-head-that-left-a-bracket-open`) `PB` does change
the tokens, and the parser's head recovery prints the same diagnostics either
way. So `PB`'s tokens differ from `NEW`'s in 10 files and its diagnostics in 9.
(The brief's instrument, run on my archive with only its file list changed,
reproduces 1358, 8, 915,946, 55, 34 and 12.)

**The narrowing that keeps (b)'s gain and drops its cost.** In every habit
above, the statement word stands deeper than the line the bracket opened on;
in every Task 2 shape, it stands at that line's margin or shallower. The lexer
knows both at `line_start`: `spaces`, and `l.level`, which is frozen at the
level of the line the outermost open bracket opened on. So:

- **`PN`, (b')**: the reach ends at a line whose first word no bracket holds in
  a program that compiles (`starts_afresh`, `if`, `match`; `function` before a
  name) and whose margin is no deeper than `l.level * 4`. Column 0 is the case
  margin 0, so (a) is inside it, and so is (c).
- **`PE`, (b'')**: the same, with `else` needing a margin strictly shallower.
  An `else` stands at its `if`'s margin, which is one level above the line the
  bracket opened on when the `if` is outside the bracket, and that line's own
  margin when it is an `if` expression inside it. `PN` said "never closed" on
  the Rust-style `x = f(if c` over `else` at the statement's margin (`b_if_same_line`,
  `b_else_line`); `PE` does not, and still ends the reach at n4's `else`.

False "never closed" on my closed-bracket probes: `PB` 18, `PN` 3, `PE` 1,
`NEW` 0. `PE`'s one is `b_if_col0.hero`, an `if` at column 0 inside a call
opened at column 4, which I do not find plausible. What `PE` gives up against
`PB`: n6, a head whose own bracket is open and whose body line opens with
`return` one level deeper (`NEW`'s one message stands there, true and at the
first mistake). What `PE` adds against `NEW`: every Task 2 shape above, `break`
and `continue` (x4), a nested `function` at column 4 (x5, four mistakes told
where `NEW` told one), and the `extern` stray (x1, x2). The compiler's own
tests under `PE`, from a full tree (a fresh archive with `PE`'s `selfhost/`):
911 tests, all passed, as `NEW`'s from `<seat>` (run from a directory holding
only `selfhost/` and `runtime/`, both fail the same three tests about the
command's surroundings, so that directory is not a test tree).

## Task 4: cost, where it lands, and whether the lexer can know it

**The instrument's own numbers.** `tests/harness/suite_layout.hero` prints a
file's `code_lines` only past its ceiling, so I ran a copy of the harness
(`<seat>/hl`, the one change `CEILING` 300 to 0) over each tree; every file
below is under 300 and none is in `DECIDED` (its 18 rows, the array of lines 435 to
455 of `suite_layout.hero`).
Non-blank lines outside `test` blocks:

| file | c85bccb8 | 41807577 | 171e8c45 | (b) `PB` | (b') `PN` | (b'') `PE` |
|---|---|---|---|---|---|---|
| `selfhost/layout.hero` | 171 | 194 | 201 | 201 | 202 | 202 |
| `selfhost/next_line.hero` | 186 | 207 | 207 | 219 | 209 | 214 |
| `selfhost/lexer.hero` | 199 | 210 | 210 | 210 | 210 | 210 |
| `selfhost/state.hero` | 166 | 173 | 177 | 177 | 177 | 177 |
| `selfhost/parse/unclosed.hero` | 87 | 102 | 241 | 241 | 241 | 241 |
| `selfhost/cursor.hero` | 300 | 277 | 277 | 277 | 277 | 277 |
| `selfhost/doc_comments.hero` | absent | 51 | 51 | 51 | 51 | 51 |
| `selfhost/parse/members.hero` | 217 | 217 | 227 | 227 | 227 | 227 |
| `selfhost/parse/opening.hero` | 281 | 281 | 284 | 284 | 284 | 284 |

- **The landed rule, lexer half (41807577)**: `layout.hero` +23,
  `next_line.hero` +21, `lexer.hero` +11, `state.hero` +7, `parse/unclosed.hero`
  +15, and the `closes` field took `cursor.hero` to its ceiling, so
  `take_docs` left it for a new 51-line `doc_comments.hero` (`cursor.hero` -23).
- **The parser's use of `closes` (de991c25, 48a433f4, 6795bedd, ac62cde9)**:
  `parse/unclosed.hero` 102 to 241, `parse/members.hero` +10,
  `parse/opening.hero` +3, besides `grammar_expr.hero`, `parse.hero`,
  `parse/braces.hero`, `parse/brace_habit.hero` and `literals.hero`, which these
  four commits also touched (`git show --stat`). This half is most of the cost
  and most of what the six cases pin.
- **(b) as posed**: `next_line.hero` +12, one line of `layout.hero` changed,
  nothing else; and 8 `check` goldens red.
- **(b')**: `next_line.hero` +2, `layout.hero` +1: the predicate replaces
  `opens_a_declaration` rather than sitting beside it.
- **(b'')**: `next_line.hero` +7, `layout.hero` +1. No parser file, no
  `state.hero` field, no new module: the parser's `never_closed`, `in_reach`
  and `past_what_was_swallowed` read offsets, so a close at column 4 costs them
  what a close at column 0 does (the census and the `check` form say so).
  What it owes beside the lines is golden cases (the Task 2 class, the `extern`
  stray, `break`, and the habits as guards that must NOT say never closed),
  design.md's sentence and a `docs/records/log/` entry.

**Core or sugar (design.md §1.7, Part 5)**: neither. No construct of Part 5's
seven is touched, nothing reaches the checker, the lowering, the descriptors,
the ownership pass or the emitter, and no accepted program changes: under
`PE`, the token stream of every one of the 1358 tracked files and of the
spec's 8 fences is byte-identical to `NEW`'s (`heroes lex --dump-tokens`,
`cmp`; under `PB`, 10 files differ, none of which compiles).

**Can the lexer know the first word for (b) where it knows it for (a)? Yes,
at the same line.** `line_start` reads the margin of every line, inside
brackets too (`layout.hero:73` to `78`), before it asks
`l.open_brackets.len() > 0` (`:93`), so at `:94` `l.pos` is the first byte of
the line's first token at any column, and `next_line.word_at` with
`keywords.keyword` names it from the text, as (a) does; all three prototypes
change only that line. The margin bound costs nothing more: `l.level` is
frozen at the level of the line the outermost open bracket opened on, because
`line_start` returns before the level moves while brackets are open. What the
lexer does not have there is the rest of the line's tokens, so a condition on
anything past the first word (an arm's `=>`) would be a text scan, of the kind
`next_line.holds_an_arrow` already is.

**Speed**: `heroes check selfhost/main.hero`, `NEW` and `PE` alternated five
times: user medians 7.00 and 6.89 s. The machine was at load 10 to 14 on 8
cores from other sessions, so by CLAUDE.md § Verification this is not a
duration and a still-machine timing is owed where it lands; the ratio (`real`
7.56 against `user` 7.30 at worst) says the runs were not waiting. The added
work is one call and one integer comparison per line inside brackets (the
prototype calls `ends_a_reach` first; the comparison can stand inline before
the call, as the landed rule's `spaces > 0` does, and should), and a word
lookup only on lines at or left of the opener's margin.

**The suites a lexer change owes, run on `PE`** in a full tree (a fresh
archive of 171e8c45 with `PE`'s `selfhost/`, its compiler built there):
`canonical` 2 and 0, `layout` 4 and 0, `order` 3 and 0, `surface` 334 and 0,
`probe` 24 and 0, `check` 284 and 0, the compiler's own tests 911, all passed.
`records` could not run on an archive (it asks git which files are the
project's) and printed the same line for `NEW` on my archive, so it is unrun.
My first attempt ran six harness builds at once in one tree and every one died
on a `build/` cache the others were writing (`suiterecords.c`, `null character
ignored`), defect 134's shape; run one at a time they are the counts above.

## The mutation instrument on (b'')

The brief's recovery instrument (the coordinator's `scratchpad/instrument/tool`,
copied to `<seat>/mtool` unchanged) planted from my `c85bccb8` archive, whose
seed binary is byte-identical to the frozen lexer and whose `.hero` files are
exactly `tracked-c85bccb8.txt` (`cmp` of the sorted lists), judged by `PE`,
`--jobs 3`, `--compare` against `run-e5cc73eb`. Its plan equals that run's:
census `{'refused': 430, 'accepted': 650, 'nested': 270}`, 9 files dearer than
0.8 s, 641 programs, 298,325 sites over 96 operators, 13,035 single mutants,
15,800 pairs (16.0 minutes of wall clock). Against `NEW`:

- **Singles**: every operator's row is unchanged but `bracket-open`'s, whose ONE
  goes from 89 to 110 (+21) and EXTRA from 67 to 46. Read mutant by mutant
  (`singles.jsonl`, keys matched): 36 singles move, all `bracket-open`; 35 draw
  fewer diagnostics, 1 the same number, none more; together 163 diagnostics under
  `NEW`, 51 under `PE`. The typical one: a `]` left out in
  `examples/interpreter/lex/token.hero:175`, where `NEW` adds `missing_body` at
  178:9 for the head the reach swallowed and `PE` says `unclosed_bracket` alone.
  The one of equal count is an `extern` record's field `i8[8` over a member line:
  its second message, `expected_array_length`, moves from 37:5 to 37:1, both
  cascades of the one `]` (the fixed array's length in a group's field asks no
  `never_closed`, a residual for the implementer).
- **Pairs, parse-stage seconds, normal arm**: HIDDEN 91 to 90; told as something
  else 19 to 13; not told as itself 110 to 103, of 14,685. `bracket-open` as the
  first: 8 to 7 of 164. **`wrong-closer` as the first: 68 of 159, unchanged**, as
  Task 3's u2_p predicted: no word rule reaches a closer of the wrong kind.
  Control arm 998 to 996 of 15,434.
- Nothing moved toward SILENT, ELSEWHERE or a new exit; no other row moved but
  one `range-dots` pair, 1 fewer hidden.

So the class (b'') reaches is mostly not the stray closer the question was
raised on but the single missing closer whose reach swallowed a statement head
and cost a cascade: 21 of the 156 `bracket-open` singles, 13%, go from several
messages to one.

What still draws more than one message under `PE`, the 46 `bracket-open`
singles by their extra code: `expected_expression` 15 (a list going on at the
line below, e10's shape), `expected_parameter_type` 12 (a head's parameter list
reading its body line as a parameter, e04's shape), `expected_separator` 7,
`expected_array_length` 5, and seven more in six kinds. Every one is the
parser reading a line inside the reach as a continuation; none is a word the
reach could end at.

## The same instrument on (b) as posed

Same copy, same inputs, judged by `PB`; the plan again equals `run-e5cc73eb`'s
(the same census, 9 dearer, 641 programs, 298,325 sites, 13,035 singles; 16.3
minutes). Against `NEW`:

- **Singles**: `brace-same-line` ONE 146 to 110 (EXTRA +36), `brace-allman` 151
  to 109 (EXTRA +42), `bracket-open` ONE 89 to 104 (+15, where `PE` gives +21).
  Mutant by mutant, 181 singles move: all 39 moved `brace-same-line` draw more
  diagnostics (43 to 319), all 43 `brace-allman` (44 to 259; six gain an
  `unclosed_bracket`), all 60 moved `brace-else-chain` (201 to 518), and of 39
  `bracket-open`, 32 fewer and **6 go from one message to several**. A typical
  brace mutant, `examples/routes/main.hero` with a body put in braces at line 56:
  `NEW` says `missing_body` at 56:54 and nothing else; `PB` adds
  `expected_expression` at 65:9 and `expected_declaration` at 67:5 and 73:1.
- **Pairs, parse stage**: HIDDEN 91 to 86, but told as something else 19 to 25,
  so not told as itself 110 to **111**, against `PE`'s 103.
- **The whole corpus of one-mistake programs**: the normal arm's diagnostics
  over all 13,594 singles read **16,339 under `NEW`, 17,059 under `PB` (+720),
  16,227 under `PE` (-112)**. `PB`'s +720 is the brace habits' +808 less
  `bracket-open`'s -88; `PE`'s -112 is `bracket-open`'s alone.

## The verdict

**(a): ratify the rule and the parser's use of `closes`, amended.** Sound: no
program that compiles holds a declaration word inside brackets, by the grammar
and by every tracked file's tokens. But the column-0 bound is not what makes it
sound, and it is what leaves out the `extern` member line (c) and a declaration
at a body's margin (x5). Amend it into the one predicate below, whose column-0
case is exactly today's rule, and make its name scan skip tabs as well as
spaces (e11). The tab before the word (e07) I leave: the lexer refuses that
margin already, and ending a reach on a line whose margin it cannot read would
need a margin it does not have.

**(b): refuse as posed, adopt narrowed (`PE`).** As posed it breaks the brace
habit and the `if`/`match`-expression argument, the two commonest shapes that
put these words inside brackets the author closed: 8 `check` goldens red, 103
diagnostics to 153 in 9 files, 18 false "never closed" on my probes, and on the
instrument 720 more diagnostics over the one-mistake corpus, 78 brace-habit
singles taken from one message to several. Narrowed, it moves nothing that
compiles and nothing in the tree, tells every Task 2 shape, and takes 112
diagnostics off the one-mistake corpus, every one a cascade of a missing
closer.

**The one predicate (`next_line.ends_a_reach`, `PE`'s):** the reach of the
brackets still open ends at a line whose first word no bracket holds in a
program that compiles, `starts_afresh`'s fourteen with `if` and `match`,
`function` only before a name, at a margin no deeper than the line the
outermost bracket opened on, and for `else` a margin strictly shallower. The
lexer names every opener still open there and lays the line out by its own
margin. `headless.opens_a_declaration` stays, renamed (`declares_a_member`).

**Not a veto.** Nothing here adds a core construct (Part 5's seven are
untouched) or breaches the ceiling (`next_line.hero` 214 and `layout.hero` 202
of 300). My refusal of (b) as posed rests on measurement, and the programs it
would make worse are named above; no program that compiles breaks under either
form.

**(c) touches what a binding's author sees**, in a refused group only (x1,
x2: a stray closer after a member told, and the member it swallowed read), at
the parse stage; no header, type, width, mark or emitted C. By the brief's own
condition the sitting widens to the ffi-pragmatist. What I would ask of that
seat is the message: under `PE` the stray `)` after `sin(x: f64)` is told as
`expected_extern_signature`, *expected a `function`, a `constant` or a `record`,
found `)`*, where a body's stray `)` gets `expected_end_of_line`.

**design.md §4.15, the sentence at line 1944, in words I would offer the
spec-warden** (theirs to shape): *An unclosed opener is a compile error,
reported at the opener. The lexer finds it at the end of the file, or earlier at
the first line inside the brackets that opens with a word no bracket holds in a
program that compiles (a declaration's or a statement's first word, `if`,
`match`, `function` before a name) at a margin no deeper than the line the
brackets opened on, shallower for `else`: there it names every opener still
open and lays the line out as what it begins. Without that, one missing `)`
would silently swallow the rest of the file's layout, or pair with a stray
closer below and never be named.*

**Outside the sitting, found on the way**: 1,000 nested `(` left open abort
`check` with `panic: stack exhausted in grammarexpr.postfix`, exit 134, under
`NEW` and `PE` alike; under `NEW`, 1,000 nested and closed abort the same way
and 100, 200 and 400 check clean (`<seat>/t6/r3_*`). I searched `docs/work/DEFECTS.md` for `stack exhausted`,
`nest`, `depth` and `recursion` and found no item; whether it is a known limit
is a question for the coordinator, not a premise.

## Verdict, in the seat's form

- `verdict`: **object**. Ratify (a) amended; refuse (b) as posed and adopt it
  narrowed (`PE`). Not a veto: no core construct, no ceiling breached.
- `section`: design.md §4.15 (line 1944, *"reported at end of file, citing the
  opener"*) and §4.17 (*"Every error carries all the context needed to fix
  it"*, the fix *"in one turn"*); the cost against §1.1 (simplicity *"sets the
  ceiling"*) and §1.7 with Part 5 (the seven core constructs). design.md has no
  sentence that a diagnostic must not assert something false of the program, or
  that one mistake costs one diagnostic; the second lives in
  `selfhost/cursor.hero`'s recovery header, which cites §4.17. I say the
  document does not cover it rather than invent the rule.
- `implementation_cost`: (b'') is lexer only, `selfhost/next_line.hero` 207 to
  214 and `selfhost/layout.hero` 201 to 202 by `suite_layout`'s `code_lines`
  (ceiling 300, neither in `DECIDED`); no parser, checker, descriptor,
  ownership or emitter line; golden cases, the design.md sentence and a log
  entry owed. (b) as posed: `next_line.hero` +12 and 8 `check` goldens red. The
  landed rule's lexer half cost `layout.hero` +23, `next_line.hero` +21,
  `lexer.hero` +11, `state.hero` +7, `parse/unclosed.hero` +15 and pushed
  `cursor.hero` (300) into a new 51-line `doc_comments.hero`; the parser's use
  of `closes` took `parse/unclosed.hero` from 102 to 241.
- `needed_for_self_hosting`: **no**. The compiler compiled itself before
  41807577, and nothing that compiles changes under any form measured here.
- `argument`: (a) is sound by the grammar (spec 39, 80, 111 to 116, 421 to 427)
  and the census, but its column-0 bound adds nothing to soundness and misses
  the `extern` member line. (b) as posed is wrong: the brace habit and
  `if`/`match`-expression arguments put statement words inside brackets the
  author closed, so it calls closed brackets never closed (18 probes), turns 8
  `check` goldens red, and adds 720 diagnostics over the instrument's 13,594
  one-mistake programs. One predicate, `starts_afresh` plus `if` and `match`,
  bounded by the opener line's margin and `else` strictly shallower, covers (a),
  (b) and (c) in 8 lexer lines, moves nothing that compiles, and removes 112
  cascades.
- `prediction`: at the batch gate that lands (b''), a re-run of the recovery
  instrument against that gate's compiler reads `bracket-open` singles at ONE
  110 or more of 156 (e5cc73eb: 89), the normal arm's diagnostics over the
  13,594 singles at 16,227 or fewer (e5cc73eb: 16,339), and `wrong-closer` as
  the first **unchanged at 68 of 159**, the class the question was raised on,
  which no word rule reaches.
- `condition`: I would take (b) without the margin bound if a plausible program
  showed a statement word opening a line deeper than its opener's line, in a
  bracket the author never closed, told worse by (b'') than by (b), by more
  than the 78 brace-habit singles (b) makes worse. I would refuse (b'') if any
  program that compiles changed its tokens under it, if any operator's EXTRA or
  SILENT rose on the instrument, or if the landing's census moved a file. I
  would drop the `extern` half of the amendment to (a) if the ffi-pragmatist
  judges x1's and x2's messages worse for a binding's author than `NEW`'s single
  `expected_params_close`.

