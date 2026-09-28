# Panel 181, compiler-engineer: a line that ends outside brackets where no token can end it

Seat: compiler-engineer (design.md §1.1, §1.7, Part 5). Written 2026-09-28.

**The tree.** `0fc98107`, extracted with `git -C <trunk> archive 0fc98107 | tar -x`
into `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-181/compiler-engineer/base/`,
`build/` removed, compiler built from the seed (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, 3.70 s user), plus a `clang -O2` seed build
(`heroes-o2`) that built every prototype from `selfhost/`. Each route is a full
copy beside `base/`: `a4/` (route (a), final; `a2/` and `a3/` are its two
earlier forms, kept because §2 cites what each one measured), `a1/` (route (a)
as the brief words it), `b/`, `c/`, `p/` and `p2/` (a route no brief named, below), `m/`
(spec pricing only). The scripts that made, ran and counted the probes (`shapes.py`,
`genmap.py`, `applyfix.py`, `census.py`, `census_indent.py`, `codelines.py`,
`suites.sh`, `timing.sh`), every map run's recorded rows (`maps/`), the suite
summaries (`suites/`) and the hand-written probes (`p17/`, `inc/`, `adv/`) are
in `instruments/` in the same directory. Nothing was built, run or edited in the trunk but
this file. Every number below was produced by a command run in this session;
where a sentence infers, it says so.

**The baseline**, the seven suites of the brief plus `layout` on `base/`: surface
315/0, canonical 2/0, check 153/0, annotations 199/0, fixes 26/0, grammar 9/0,
run 210/0; the compiler's own tests 788/0.

## The verdict

- **verdict**: approve route (a), the lexer refusal that hands the parser the one
  line the author broke, in the final form below; object to (b), to (c), to
  (a) as the brief words it, and to P; record P2 as the §1.7 alternative the
  author may prefer. No veto: no route adds a core construct or breaches §1.1's
  ceiling ((b) breaches CLAUDE.md §11's per-file ceiling on `cursor.hero`, a
  module-shape rule, which is an objection and not a veto).
- **section**: design.md §1.7 (*"does it ... remove a special case from the
  compiler?"*: every route adds one, in the frontend) and Part 5 (none reaches
  the seven constructs, the checker, lowering, descriptors, ownership or the
  emitter); §1.1 for the ceiling; §4.15 for the rule, which is already ratified
  (panel 007); §4.17 for one diagnostic per mistake.
- **implementation_cost**: route (a), final: `selfhost/layout.hero` 117 to 272
  code lines by `tests/harness/suite_layout.hero`'s own rule (`code_lines`: tests
  and blank lines excluded), 115 statements and 40 comments, 205 to 389 lines raw,
  under its ceiling of 300; `selfhost/state.hero` 101 to 108 (two fields);
  `selfhost/lexer.hero` +56 test lines (six tests) and one transcribed assertion
  changed. Zero lines in the parser, the checker, the IR, the descriptor or
  ownership passes, the emitter, the runtime. Route (b): `selfhost/cursor.hero`
  279 to 355 code lines, and `layout` goes red (*"355 lines of code, past §11's
  300"*). Route (c): route (a) plus 17 code lines, +68 spec tokens on the
  reader's tokeniser, and a formatter repair nobody has priced.
- **needed_for_self_hosting**: no. Zero occurrences in `selfhost/` (272 files,
  my census below); the compiler never writes the form, and refusing it costs
  the fixpoint nothing: route (a)'s compiler builds itself and passes its own
  794 tests.
- **argument**: Nothing here is core: every route lives in the frontend, so
  §1.7 prices it as one lexer special case, and the ceiling holds for all. The
  question is which special case buys the most robust result. Route (a) is 155
  code lines in one lexer module; it refuses 241 of the map's 242 probes (the
  last is legal), 235 with exactly one diagnostic, whose `certain` fix, applied,
  compiles 81 of 81 and prints the joined program's output 74 of 74. Route (b)
  costs as much, covers one margin, and breaks `cursor.hero`'s ceiling. Route (c)
  costs more in lexer, spec and formatter, keeps defect 118, and puts a
  continued `if`'s `else` in another column than its `if`.
- **prediction**: at M-agreed-retention's close, if a refusal lands: (1) no
  module under `selfhost/parse/`, `selfhost/check/`, `selfhost/ir/` or
  `selfhost/emit/`, and none of `cursor.hero`, `grammar_expr.hero`, `parse.hero`,
  grows in code lines by `suite_layout.hero`'s rule for this repair; (2) the
  lexer modules it touches (`layout.hero`, `state.hero`, `scan.hero`,
  `lexer.hero`, or a new lexer module) grow by at most 180 code lines together
  over `0fc98107` (the prototype: 162); (3) none of the ten `.expected` files that hold the 16
  `error`-token hits and the 4 `use` hits changes. Any one falsifies it; `git
  diff 0fc98107` and the counting rule settle it.
- **condition**: I move to object to (a) on one measured shape where its
  `certain` fix, applied, fails to compile or prints other than the joined
  program (0 of 81 and 0 of 74 today), or on a change to any existing
  `.expected` but `check/depth-zero-continuation.expected` (§ 7). I move to approve (c) on the baseline §4.15 asks for (models
  producing the deeper break at a measured rate), together with a priced
  formatter repair for the three arm shapes that exit 2 under it and a layout
  for a control form in a continuation that keeps `else` under its `if`.

## 1. The map at depth zero

**Where the lists came from.**

- *The token kinds*: `selfhost/layout.hero:46-54`, the arms of `is_line_ender`
  that answer `false`: 52 of the 69 kinds (`awk` over `variant TokenKind` in
  `selfhost/token.hero` counts 69; the `true` arms name 17). Five are layout
  (`comment`, `terminator`, `indent`, `dedent`, `eof`) and never become a line's
  last significant token (its writers are `state.emit`, `error_token` and
  `error_token_at`, `state.hero:78, 107, 115`, and the reset at a line end,
  `scan.hero:61`; a comment bypasses `emit`, `scan.hero:85-88`). `error` is § 3. The three openers raise the bracket depth,
  so the line after them is not at depth zero (probed: `open_paren`,
  `open_bracket`, `open_brace` below, unchanged by every route). **43 kinds
  remain, and every one of them is in the map.**
- *The contexts*: the spec's productions (§ 1 `Use`; § 4 `Declaration`,
  `Params`, `Fields`, `Case`; § 5 `Statement`, `Place`; § 7 `Expression` to
  `Primary`; § 8 `While`, `For`, `If`, `Match`, `Arm`, `Inline`, `Pattern`; § 9
  `Generics`; § 13 `Extern`, `Member`), read against the parser's dispatchers,
  `selfhost/parse/decl.hero:50-73` (`file`), `selfhost/grammar_expr.hero:767-799`
  (`statement_kind`), `:967` (`if_expr`), `:1009` (`match_expr`), `:1063`
  (`arm`). `Args` and `CParam` sit inside `(`, so they are panel 180's. Not
  probed, by choice: a `-` ending a line inside a `Pattern` (no production puts
  a line end there that joins to a valid arm).
- *The margins*: the second line at the head's margin, one level deeper, one
  shallower (where a shallower one exists), and for `y = a +` / `1` and `y =
  xs.` / `len()` also a blank line between, a comment-only line between, and CRLF
  endings. Every shape carries its joined one-line program as a control, and
  every control but the two trailing-comment ones compiles.

**87 shapes, 340 programs**: 83 breaks, the `?` control and the three openers,
each as its joined control plus its margins, which makes 253 breaks, 242 of them
neither a control nor an opener (the count every total below uses). Today
(`base/`): **at the same margin 82 of the 83 compile**, and every one of the 73 whose output can be
compared prints exactly what its joined control prints (the rest are `test` and
`extern` shapes with no `main` output, and the two trailing-comment shapes whose
joined control is not a program); the one refused is `use sub/` over `m2`, by the `use` parser's own
adjacency rule (`selfhost/parse/use_line.hero:114`, and its comment at `:96`,
*a path with a space in it is not a path*). One level deeper, 81 of 82 are refused and the one accepted is
the legal block arm `.dot =>` over a deeper body. Shallower, 70 of 70 refused.
So the brief's finding holds on the whole map: the compiler implements panel
007's rejected clause at the same margin, for every non-ender kind, in every
depth-zero context.

**Found beyond the brief's seventeen** (each `check` 0 today): `else` over `if a == 5` at the same margin is an
`else if`; `function` over `main()`, and `constant`, `record`, `variant`, `test`,
`extern`, `use` each split from what follows; generics broken after `<` and after
`,` (`function first<A,` over `B>(...)`, which panel 180's shape 116 probed only
two columns deeper); `->` in an `extern` member; `|` joining patterns in an arm
and joining releasers after `acquires`; `.dot =>` over an inline body at the
arm's own margin; `:` in a record field, a variant case's field and an `extern`
constant; `fail` over `(code: ..., msg: ...)`; `P::` over `x`; the trailing
comment after the operator (`y = a +  # note` over `1`).

**One of them is not the lexer's.** `.dot =>` over `1` at the arm's margin
compiles even when a terminator is planted after `=>`, because `arm()` skips
terminators after the arrow (`selfhost/grammar_expr.hero:1082`). Measured with
route P below: every other same-margin shape is refused there, and this one runs
at exit 0. A repair that works by planting terminators alone leaves it silent.

**The census, independently of the brief's instrument.** `census.py` reads
`heroes lex --dump-tokens` for every `.hero` outside `archive/`, `build/` and
`site/node_modules/` and counts two significant tokens at bracket depth zero on
different lines with no layout token between: **1164 files, 21 hits**, the
brief's 16 after `error`, 4 after a `use` path, 1 on purpose (`comments107/
margin.hero:13`); **zero** in `selfhost/` (272 files), `examples/` (120),
`tests/harness/` (32). A second count, of what precedes an INDENT with no
terminator before it, gives the block heads that end in a non-ender today:
`=>` 824 times, `else` 404, `:` 2 (both in `check/trailing-colon.hero`, a
refusal), `+` 1 (`check/depth-zero-continuation.hero`), `,` 1
(`brackets180/shape116.hero`), `error` 3.

The whole table is at the end of this report (§ 9).

## 2. The routes, priced

**The scale, for §1.1's question** (*does it still fit one person?*): by the same
counting rule `selfhost/` is 53,383 code lines in 272 files, and the eight lexer
modules (`layout`, `scan`, `state`, `lexer`, `lex_interp`, `literals`, `number`,
`keywords`) are 1,416. Route (a)'s 162 are 0.3% of the compiler and 11% of its
lexer, and 4% of Pascal-P4's ~4000; no route here moves the answer.

All counts are `code_lines` by `suite_layout.hero`'s rule, reproduced in
`codelines.py` and checked against that suite's verdicts; raw diff sizes are `git
diff --no-index --numstat` against `base/`. The suites are the brief's seven plus
`layout`, run one at a time in each copy with that copy's compiler, plus the
compiler's own tests.

| route | where | code lines | map: same / deeper / shallower (refused probes: exactly one diagnostic, total diagnostics) | suites that break |
|---|---|---|---|---|
| today | | | 82 compile / 64 of 82, 103 / 8 of 70, 161 | |
| **(a) final**: the lexer reports and hands the parser the join | `layout.hero`, `state.hero` | +155, +7 | **82 of 83, 85 / 77 of 82, 88 / 70 of 70, 70** | surface 314/1, check 152/1, annotations 197/2 (the three fixtures and the ratified golden, § 7); canonical 2/0, fixes 26/0, grammar 9/0, layout 4/0, run 210/0; units 794/0 |
| (a) as worded: report and plant the terminator | `layout.hero`, `state.hero` | as (a), +3 | 1 of 83, 211 / 2 of 82, 176 / 0 of 70, 231 | the same four failures as (a); run 210/0; units 793/1 (the test that asserts the join) |
| (b): the parser, from a bit per token | `cursor.hero`, `parse.hero` | +76 (2 changed) | 82 of 83, 85 / as today / as today | surface 314/1, **layout 3/1** (`cursor.hero` 355); check 153/0, annotations 199/0, canonical, fixes, grammar green; run 210/0; units 788/0 |
| (c): Nim's rule, one level deeper | `layout.hero`, `state.hero`, spec | (a) +17, spec +68 real | 82 of 83, 85 / **54 continuations admitted** / 70 of 70, 70 | surface 314/1, **check 152/1** (the ratified guard now parses), annotations 197/2; canonical, fixes, grammar, layout green; run 210/0; units 788/1 (the transcription (a) updates) |
| P: plant a terminator at every depth-zero line end (Python's rule), a route no brief named | `layout.hero` | 1 condition, +2 comment lines | 45 of 82, 128, **and `.dot =>` over `1` still compiles** / 67 of 82, 100 / 8 of 70, 161 | the same four failures as (a); canonical, fixes, grammar, layout green; run 210/0; units 788/2 (two tests transcribing today's rule) |
| P2: P, and an arm's body after a line end must be a block | `layout.hero`, `grammar_expr.hero` | P, +10 (1062 to 1072 of its decided 1085) | 241 of 242 refused, 127 with exactly one, 396 in all | the same four failures as (a); run 210/0; units 788/2 (two tests transcribing today's rule) |

**Route (a), final.** In `layout.maybe_terminator`, a depth-zero line whose last
significant token is not a line ender records that token (`LexState.open_line`,
`open_end`). At the next line with words on it, `layout.goes_on` reads the margin
and, without lexing it, the first word of the new line, and decides one of four:

1. **a block head**: `else` or `=>` before a deeper line opens its body, and so
   does `:` before a deeper line, which is the Python habit the parser already
   refuses with its own `certain` fix (`trailing_colon`). *Measured*: the first
   version omitted `:` and turned `check/trailing-colon.hero`'s one diagnostic
   per line into three, which is how the clause got there;
2. **a fresh line**: the new line begins with a word that begins a declaration
   or a statement and continues nothing (`constant`, `function`, `record`,
   `variant`, `test`, `extern`, `use`, `return`, `break`, `continue`, `assert`,
   `for`, `while`, `else`), or `else` stands before anything but `if`: the lexer
   plants the terminator and reports nothing, and the parser says what the first
   line lacks, in its own words;
3. **two statements**: the line left open binds or mutates (a lone `=` or an
   `@` outside brackets, read from its tokens) and so does the new one (read
   from its text): joined they cannot parse, so the line is ended there as in 2.
   *Measured*: without this clause, `total @ total +` over `count @ count + 1`
   cost two diagnostics where today costs one, and the join fix, `certain`,
   applied, did not parse (three probes, `inc1` to `inc3`); with it, one
   diagnostic each, `expected_expression`, as today's single one;
   Neither 2 nor 3 applies after `=>`: the parser reads an arm's body across a
   terminator (`grammar_expr.hero:1082`), so a line ended there goes on in
   silence. *Measured*: `.dot =>` over `return 1` at the arm's margin runs at
   exit 0 today, and ran at exit 0 under the third copy (`a3/`), whose split
   put a terminator where `arm()` skips it; the final copy reports it as 4, and
   its fix, applied, prints 1;
4. **a continuation, refused**: `continuation_outside_brackets` at the token the
   line ended with, and **the stream the parser is handed is the one line the
   author broke**: nothing at the same margin, no INDENT one deeper, no DEDENT
   shallower. The parser and everything after it read exactly what they read
   today, so the break costs one diagnostic and nothing downstream moves.

Every branch refuses the program or leaves it as the parser would have it; the
only thing the two heuristics in 2 and 3 choose is which diagnostic the author
reads, never whether the program compiles. That rests on one fact about the
parser, read rather than run: outside brackets it reads on across a terminator
after a non-ender only in `arm()`. The other depth-zero `skip_terminators` sites
(`parse/decl.hero:54, 208`, `parse/group.hero:63, 214`, `parse/members.hero:247,
294, 306`, `parse/tails.hero:278, 325`, and `grammar_expr.hero`'s `block`,
`match_expr` and their loops) are loops between lines or stand before a
required INDENT. That is why a scan of the next
line's text that a quote could fool (`holds_a_binding`) is acceptable here and
would not be in a decision about acceptance; its comment says so.

**What (a) does to the goldens and fixtures**, measured by the failures above
and nothing else: `check/depth-zero-continuation.hero` (the author's ratified
adversarial case of 2026-08-04) moves from `expected_expression` at 7:1 to
`continuation_outside_brackets` at 6:15; `surface-fixtures/brackets180/
shape116.hero` from five diagnostics to one (the second `annotations` failure is
that suite's floor, 478 annotated diagnostics against 527, and the 49 missing
are exactly the `brackets180` group's, which drop out while the group fails); `surface-fixtures/comments107/
margin.hero` stops parsing, so `suite_surface.hero:380`'s row, which expects
`fmt` at exit 0, is owed a new expectation or the fixture owed its parentheses.
The sixteen `error`-token hits and the four `use` hits: no `.expected` changes.

**Route (a) as the brief words it** (*plant the terminator so the parser sees two
statements*): built as `a1/`, measured on the map, and it doubles the cost of
every break: the lexer's diagnostic, then the parser's at the planted terminator
(*expected an expression, found the end of the line*), then whatever the second
line makes on its own (`main()` at the top level, `u` before an indented body).
Two or more diagnostics on 82 of 83 same-margin shapes. **And the net cannot
see it**: `a1/` fails exactly the four checks final (a) fails, and nothing else
(its one extra unit failure is my own test asserting the join). No suite counts
the diagnostics a break costs, so the golden a landing owes should carry one
shape per margin with its `.expected` holding exactly one line each, or a
regression to two per break would land green.

**Route (b).** `Cursor` gains a bracket depth and one bit per token (a line end
lies between it and the token before, computed once from the text in
`text_cursor`, so the cursor still never holds the text, `cursor.hero:17`), and
`bump` refuses two significant tokens read across a line end outside brackets.
It is route (a) seen from the other side of the token stream, with less to see:
between the two lines of a deeper or a shallower break there is an INDENT or a
DEDENT, so the parser's view of those is today's (*found an indented block*),
and it needs its own copy of the fresh-line word list. It lands in the one
module that every parser file calls and takes it past §11's ceiling, which
`layout` measures; a split would have to keep `bump` and the check together,
since the check runs in `bump`. The parser has no knowledge the lexer lacks
here, and the brief's *line numbers it already compares* are
`text_lines.line_col`, which walks from byte 0 (`selfhost/text_lines.hero`), so
using them per token would make the parse quadratic; I used
`newlines_between`, which walks the gap.

**Route (c).** The prototype is (a)'s plus `layout.continues`, a 69-arm match
(24 continuators: the 18 binary operators, `!`, `~`, `=`, `@`, `.`, `::`), and
three lines in `goes_on`: exactly one level deeper after a continuator goes on
silently. Measured:

- *the four block headers panel 007 named*: two no longer exist (`Point =
  record` and `Token = variant` became `record Point` and `variant Token` at
  panel 018, and `recover_to_next_decl`'s comment, `cursor.hero:419-423`, says
  so). The other two, `else` (404 in the tree) and `=>` (824), are not
  continuators, so they keep opening blocks; so does `:` (the Python habit).
  Every header that ends in a non-ender today is one of those three (the INDENT
  census above), so no legal program changes meaning;
- *54 continuation shapes admitted*; the 50 whose output can be compared print
  exactly their joined control's;
- *a control form in a continuation* is legal only with its block at the
  continuation's own column and its `else` at the statement's: `y =` over
  `    if t` over `    1` over `else` over `    0` compiles, and the nesting a
  reader expects (`if t` one level in, `1` one further, `else` under `if`) is
  refused with `indentation_jump`. The continuation opens no level, so nothing
  nested under it can sit deeper than it without breaking the rule, and a
  continued `if` puts its `else` in a different column from itself;
- *`fmt` never prints the form it admits*: it joins every admitted continuation
  back onto one line, and on `.` it prints `y = xs.len(` over `)` (as it does
  today at the same margin);
- *defect 118 survives*: `fmt` exits 2 on the three `match`-arm continuations one
  level deeper (`arm_op`, `arm_dot_case`, `arm_return_op`), as it does today at
  the same margin;
- *the author's ratified guard is inverted*: `check/depth-zero-continuation.hero`
  (2026-08-04, *"Guards panel 007: there is NO continuation at bracket depth
  zero"*) parses under (c) as `total = 1 + 2`, and `check` now reports only
  `unused_binding` there, so that golden changes from a refusal of the form to an
  admission of it, which is a ratification reversed rather than a snapshot moved;
- *the spec*: a sentence is owed, priced below. I found no silent misreading
  under (c) (every wrong join I tried is a type or parse error), which is why it
  is an objection on cost and not a veto.

**Route P**, which no brief named: `maybe_terminator` plants at every depth-zero
line end, which is Python's rule and the most literal reading of §4.15's *every
line's indentation is structural*. One condition in `layout.hero`. Every
diagnostic is the parser's existing one (*expected an expression, found the end
of the line*), which names no rule and carries no fix; 37 of the 82 same-margin
shapes it refuses cost two or more, because the second line is then parsed alone (a block
head's second half before an indented body, `main()` at the top level); and it
leaves `.dot =>` over `1` compiling, above. Two unit tests transcribe today's
rule and fail. **P2** closes that shape in the parser: after `=>` and a line end
only a block is a body (`missing_body`), +10 code lines in `grammar_expr.hero`,
and then only the legal block arm compiles. P2 is the smallest correct refusal
this sitting has, 12 code lines against (a)'s 162, and by §1.7's own test it is
the simplification: it removes a special case (at depth zero `is_line_ender`
stops mattering) where (a) adds one. **This is the strongest objection to my own
recommendation, and I record it rather than answer it away.** What the 150 lines
buy, measured on the same 242 probes: 235 single-diagnostic refusals against
127, 249 diagnostics against 396, a `certain` fix that compiles on 155 probes
against none, a message that names the rule against *found the end of the
line*, and a Part 11 control arm that can switch the rule off (§ 4), which under
P2 it cannot. §1.1 puts the ceiling above both (neither is near it), so §4.17 decides,
and it decides for (a). If the author weighs §1.7 over §4.17, P2 is the route,
and its price is this paragraph.

## 3. The `error` token at a line's end, and the `use` lines

**Today, in the sixteen hits, the glued line produces no second diagnostic
anywhere**; the ten `.expected` files hold one diagnostic per malformed token
and none on a glued line. **It does the opposite, and that is a finding**: at an
`error` token the parser stays silent (`cursor.hero:13-15`), `finish` finds no
terminator and runs `skip_line` (`grammar_expr.hero:824-832`), and `skip_line`
eats every glued line up to the next terminator, so **a mistake on the glued
line is never reported**. Measured on three programs: `a = 0X10` over `b = 1 +*
2` reports `base_prefix_case` alone; the same two mistakes one line apart
(`hide2`) report both; `b = 1 +* 2` alone reports `expected_expression`. One
correction round trip per hidden mistake, which is design.md §1.2's cost.

**So an `error` token should end a line at depth zero under every route**, and
route (a) and P both do it (in (a), `maybe_terminator` plants a terminator after
`error` at depth zero). Effect, measured: **no `.expected` file changes**
(`check` 152/1 under (a), the one failure being the depth-zero golden; the
`json102` fixtures pass in `surface` and `annotations`), and one unit test that
transcribes today's stream after an unterminated string
(`lexer.hero`, *a backslash at end of line leaves the layout intact*, one of
the cases still marked unverified and pending debrief) gains the terminator. Under (b) and (c) the same
line is owed; (b) does not have it as built.

**The four `use` hits** (`use lex/..` over `use shapes/`, and `use shapes/` over
`function main()`, twice each) are fresh lines by route (a)'s rule 2: the lexer
ends the `use` line and reports nothing, the `use` parser's own refusals stand,
and **no `.expected` changes** (measured: `check/use-has-a-path.hero` and
`check/fixedbugs-use-refusal-eats-the-next-line.hero` pass under (a), (b) and P).
With the terminator there, `use_line.skip_rest_of_line`'s line comparison
(`use_line.hero:220-228`) is no longer the only landmark; whether it can be
retired is not measured here. **The residual**: `use sub/` over `m2` costs three
diagnostics under (a) and (b) (today two), because the `use` parser refuses a
path broken across lines by its adjacency rule whatever the lexer does; the
join fix repairs it (applied, it compiles).

## 4. The diagnostic and its `Fix`

- **code**: `continuation_outside_brackets`, a new class, emitted by the lexer.
- **message**, as prototyped, one clause per margin: *a line outside brackets
  cannot end with `+`, and the next line goes on with it at the same margin,
  which reads as a new statement* (or *one level deeper, which is not a block*,
  or *at a shallower margin, which reads as the block's end*), then, after the
  dash the house style puts there, the repair: *write the statement on one
  line, or break it inside parentheses*. The wording is the
  llm-ergonomist's to judge, not mine.
- **span**: the token the line ended with (`y = a +` underlines the `+`).
- **the fix**: *write the statement on one line*, replacing everything from the
  end of that token to the first byte of the next line's words with one space
  (nothing after `.`, `::` and `/`). **`certain` at the same margin and one
  deeper**: the repaired text lexes to the token stream the parser already read,
  so the fix changes the break and nothing the compiler understood.
  **Measured**: `check --apply` on every refused probe, then `check` and `run` on
  the result: the fix compiles 81 of 81 times at the same margin and 74 of 74
  one deeper where offered, and prints the joined control's output 74 of 74 and
  68 of 68 (the rest have no `main` output to compare: `test`, `extern`).
  A statement broken twice costs one diagnostic per break, and the two fixes
  apply together (*measured*: `y = a +` over `b +` over `3`, two diagnostics,
  `--apply` gives `y = a + b + 3`, which prints 6).
  **`guess` after a shallower line**, where the author more likely closed the
  block; **none when a comment stands between the two lines**, since a join would
  carry the comment's text into the statement.
- **joining against wrapping in parentheses.** Both repair what the diagnostic
  names wherever both exist, and the join is the program `fmt` prints (today it
  prints `y = a + 1` for s01). They differ where there is nothing to wrap:
  a block head's two keywords (`else` over `if`), a function or `extern` header
  (`->` over `i64`: `(i64)` is not a `Type`, spec § 3), a declaration keyword
  split from its name, a `match` arm's patterns (`.dot |` over `.sq`), `y:` over
  its type, a `use` path. In every one of those the join is the only repair. In
  the expression shapes (a statement's value, a block head's condition, a
  `constant` body, an arm's inline body) both give the same tree, and
  parentheses keep the author's break; the lexer cannot place the `(`, since it
  does not know where the expression starts, so a parenthesising fix belongs to
  the parser and would be a second fix, not the `certain` one.
- **the thesis list** (`diag.hero`'s `is_thesis_rule`): by the ruling of
  2026-09-27 (*without the rule the program still has a meaning*), this is a
  thesis rule, since without it the program means the join. One line, built in
  a fifth copy (`a5/`) and measured: `check` exits 1 and `check --permissive`
  exits 0 on s01, s06, s09 and s13, so Part 11's control arm reads the join
  exactly as today's compiler does (`cli/check.hero:80` filters each stage's
  diagnostics, and the parse stage carries the lexer's); the unfinished line of
  rule 3 stays refused there (`expected_expression`), as it is today. It is
  also a reason to hand the parser the join, measured: under P2 and under (a)
  as worded, `check --permissive` still exits 1 on the same four, because what
  refuses them there is `expected_expression` at the planted terminator, a code
  that cannot join the list without dropping every genuine syntax error of
  that name. Under those routes Part 11's control arm cannot say what the rule
  costs or saves.

## 5. The formatter, and defect 118

**Today**, in the map: `fmt` exits 2 (*this is a compiler bug*) on five
same-margin shapes: the three `match`-arm continuations (`.dot => a +` over `1`,
which is s13; `.dot => .` over `sq`; `.dot => return a +` over `1`) and a
comment on its own line inside a continuation, after `+` and after `.`. On five
more it prints a fixpoint that is not the joined form: `y = xs.` over `len()`
becomes `y = xs.len(` over `)`, and the same for a module call and a discard;
`return fail` over `(code: ..., msg: ...)` becomes a call broken over four lines.

**Under (a), (b) and P2** the shapes stop parsing, and `fmt` refuses them at exit
1, *refusing to format a file with diagnostics*: measured on all five members
under each of the three, and on `margin.hero` under (a): both members of defect 118
close with 116, as that defect's own text foresaw, and `fmt` owes nothing in
`selfhost/print/`. The branch of `print/bare.hero` that prints `fmt`'s own
parentheses around *a line continued at the statement's own margin*
(`bare.hero:19`) loses its only input; whether any printer lines can then go is
not measured. `canonical` stays green under (a) (2/0): every file it formats
still parses under the new rule, and `fmt`'s own reparse guard says the same of
what it prints.

**Under (c)** defect 118 stays open in a new margin (the three arm shapes one
level deeper, exit 2, measured) and the formatter owes a repair of its own; and
since `fmt` joins every admitted continuation, keeping the author's break would
need a depth-zero continuation printer, with comments (defect 107's owner rule)
inside it. Neither is priced; I would not guess a line count for either.

## 6. The cost in time

Route (a) adds per line one store in `maybe_terminator` and one comparison in
`line_start`; its other work runs only after a line left open. Measured with
`/usr/bin/time -p`, today's compiler and route (a)'s both built from their
`selfhost/` by the same `heroes-o2`, alternated, after every suite of mine had
exited (another sitting's harness was running; load average 2.56 before, 3.17
after):

| input | today, `user` s | route (a), `user` s |
|---|---|---|
| `heroes lex` on all 272 `selfhost/` files concatenated (74,955 lines, 3.38 MB, no diagnostic on either) | 1.47, 1.47, 1.46, 1.46, 1.46 | 1.51, 1.48, 1.48, 1.47, 1.46 |
| `heroes check selfhost/main.hero` | 4.22, 4.21, 4.23 | 4.23, 4.23, 4.24 |

Medians: lexing 1.46 against 1.48 (+0.02 s, about 1%, inside the second
column's own spread of 0.05), checking the compiler 4.22 against 4.23 (+0.2%).
`real` matched `user` within 0.04 s on every run but one, route (a)'s first
lex (`real` 1.87 against `user` 1.51), which waited and whose `real` is
discarded. What I infer from the table, and it is an inference: the cost is at
the edge of what this machine resolves, and nothing a program would feel.

## 7. What a landing of (a) owes, beyond the lines above

- a `check/` golden for the class, `.hero` annotated at each diagnostic, with
  `.expected` and `.fixed` (the `fixes` suite applies it), holding a shape per
  margin and per rule of § 2 so that its `.expected` pins one diagnostic per
  break, which no suite measures today (§ 2, (a) as worded);
- the ratified `check/depth-zero-continuation.hero`: its annotation and
  `.expected` move to the new code, and since `tests/golden/` is append-only its
  header gains a dated correction beneath the 2026-08-04 text, which already
  describes the mechanism (*"the next line is read as part of the same
  expression"*);
- `brackets180/shape116.hero`'s annotation (five codes to one) and its header's
  *refused, exit 1*, which stays true;
- `comments107/margin.hero` and `suite_surface.hero:380`;
- defect 118 closed with 116, and a `check/` or surface row for each of its two
  members at exit 1;
- `diag.hero`'s thesis list, one line (measured in `a5/`, § 4), if the sitting
  agrees it is one;
- design.md §4.15's record of the ruling (the class, the three lines that are
  not continuations, and that an `error` token ends a depth-zero line), with this
  sitting's number; its price is not measured here, and the spec owes nothing
  (§ 8);
- the one unit test that transcribes today's stream after an unterminated
  string (§ 3);
- the module seam: `layout.hero` ends at 272 of its 300; "the line left open" is
  a concern with a name and no cycle (it uses `token`, `state`, `diag`,
  `keywords`, `bytes`; `layout` uses it), so I would land it as its own module
  and leave `layout.hero` near its 117.

## 8. Spec prices, on the reader's tokeniser

`heroes measure spec/heroes-spec.md --refresh`, in the copy `m/`, with `.env`
sourced from the trunk, the spec restored afterwards: today **8999** real
(6794 vendored maximum). Route (c)'s sentence, inserted after *"ends with that
block instead."*: *"Outside `(` `[` `{` a line that ends with an operator, `=`,
`@`, `.` or `::` goes on to the next, which stands one level deeper and opens no
block; at its own margin it is an error."*: **9067, +68 real** (+52 vendored).
Route (a) needs no sentence to be true: § 0 says *NEWLINE ends a statement* and
states a condition on keeping a NEWLINE only for a line inside brackets.
Whether a reader needs one anyway is the llm-ergonomist's; the cheapest
clarifying clause I tried, *"instead, and outside `(` `[` `{` a line may not end
where the statement cannot"*, prices **9024, +25 real** (+18 vendored).

## 9. The whole map

`today`, `route (a)` final and `route P` at each margin, and `route (c)` one
level deeper, where it differs from (a). A cell is `check`'s exit, the first
diagnostic's code and how many more followed; **0** marks a program that
compiles; `fmt2` a formatter exit 2; `cob` is `continuation_outside_brackets`,
`exp_expr` is `expected_expression`.

| shape | context | token at the line end | today: same | deeper | shallower | route (a): same | deeper | shallower | route (c): deeper | route P: same |
|---|---|---|---|---|---|---|---|---|---|---|
| `bin_or_or` | statement: binding value | `or_or` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `bin_and_and` | statement: binding value | `and_and` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `bin_eq_eq` | statement: binding value | `eq_eq` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `bin_bang_eq` | statement: binding value | `bang_eq` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `bin_lt` | statement: binding value | `lt` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `bin_le` | statement: binding value | `le` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `bin_gt` | statement: binding value | `gt` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `bin_ge` | statement: binding value | `ge` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `bin_pipe` | statement: binding value | `pipe` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `bin_caret` | statement: binding value | `caret` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `bin_amp` | statement: binding value | `amp` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `bin_shl` | statement: binding value | `shl` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `bin_shr` | statement: binding value | `shr` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `bin_plus` | statement: binding value | `plus` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `bin_plus` blank between | | | **0** | | | 1 cob | | | | 1 exp_expr |
| `bin_plus` comment line between | | | **0** fmt2 | | | 1 cob | | | | 1 exp_expr |
| `bin_plus` crlf | | | **0** | | | 1 cob | | | | 1 exp_expr |
| `bin_minus` | statement: binding value | `minus` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `bin_star` | statement: binding value | `star` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `bin_slash` | statement: binding value | `slash` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `bin_percent` | statement: binding value | `percent` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `un_minus` | statement: binding value (prefix) | `minus` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `un_bang` | statement: binding value (prefix) | `bang` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `un_tilde` | statement: binding value (prefix) | `tilde` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `bind_eq` | statement: `name =` | `eq` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `discard_eq` | statement: `_ =` | `eq` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `decl_colon` | statement: `name:` type | `colon` | **0** | 1 expected_type +1 | 1 expected_type +2 | 1 cob | 1 expected_type +1 | 1 cob | 1 expected_type +1 | 1 expected_type +1 |
| `decl_eq` | statement: annotated `=` | `eq` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `decl_at` | statement: annotated `@` | `at` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `mut_at` | statement: mutation `@` | `at` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `mut_at_op` | statement: mutation value | `plus` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `mut_idx_at` | statement: element mutation `@` | `at` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `mut_field_at` | statement: field mutation `@` | `at` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `dot_method` | statement: UFCS `.` | `dot` | **0** | 1 expected_field_name | 1 expected_field_name +1 | 1 cob | 1 cob | 1 cob | **0** | 1 expected_field_name |
| `dot_method` blank between | | | **0** | | | 1 cob | | | | 1 expected_field_name |
| `dot_method` comment line between | | | **0** fmt2 | | | 1 cob | | | | 1 expected_field_name |
| `dot_method` crlf | | | **0** | | | 1 cob | | | | 1 expected_field_name |
| `dot_field` | statement: field `.` | `dot` | **0** | 1 expected_field_name | 1 expected_field_name +1 | 1 cob | 1 cob | 1 cob | **0** | 1 expected_field_name |
| `dot_module` | statement: module `.` | `dot` | **0** | 1 expected_field_name | 1 expected_field_name +1 | 1 cob | 1 cob | 1 cob | **0** | 1 expected_field_name |
| `cc_fieldname` | statement: `::` | `colon_colon` | **0** | 1 expected_field_name_after_colons | 1 expected_field_name_after_colons +1 | 1 cob | 1 cob | 1 cob | **0** | 1 expected_field_name_after_colons |
| `dot_discard` | statement: `_ = xs.` | `dot` | **0** | 1 expected_field_name | 1 expected_field_name +1 | 1 cob | 1 cob | 1 cob | **0** | 1 expected_field_name |
| `ret_op` | statement: `return` value | `plus` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `ret_fail` | statement: `return fail` | `kw_fail` | **0** | 1 expected_end_of_line +1 | 1 expected_declaration | 1 cob | 1 cob | 1 cob | 1 cob | 1 expected_group_close |
| `bind_fail` | statement: binding `fail` | `kw_fail` | **0** | 1 expected_end_of_line +1 | 1 expected_declaration | 1 cob | 1 cob | 1 cob | 1 cob | 1 expected_group_close |
| `assert_kw` | statement: `assert` | `kw_assert` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | 1 cob | 1 exp_expr |
| `assert_op` | statement: `assert` value | `eq_eq` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `if_kw` | block head: `if` | `kw_if` | **0** | 1 exp_expr | 1 indentation_jump +3 | 1 cob | 1 cob | 1 cob | 1 cob | 1 exp_expr +1 |
| `if_and` | block head: `if` condition | `and_and` | **0** | 1 exp_expr | 1 indentation_jump +3 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr +1 |
| `if_eqeq` | block head: `if` condition | `eq_eq` | **0** | 1 exp_expr | 1 indentation_jump +3 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr +1 |
| `elseif_split` | block head: `else` then `if` | `kw_else` | **0** | 1 missing_body | 1 indentation_jump +2 | 1 cob | 1 missing_body | 1 cob | 1 missing_body | 1 missing_body |
| `elseif_cond` | block head: `else if` condition | `eq_eq` | **0** | 1 exp_expr | 1 indentation_jump +3 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr +1 |
| `while_kw` | block head: `while` | `kw_while` | **0** | 1 exp_expr | 1 indentation_jump +3 | 1 cob | 1 cob | 1 cob | 1 cob | 1 exp_expr +1 |
| `while_lt` | block head: `while` condition | `lt` | **0** | 1 exp_expr | 1 indentation_jump +3 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr +1 |
| `for_kw` | block head: `for` | `kw_for` | **0** | 1 for_missing_in +2 | 1 indentation_jump +4 | 1 cob | 1 cob | 1 cob | 1 cob | 1 for_missing_in +2 |
| `for_in` | block head: `for .. in` | `kw_in` | **0** | 1 exp_expr | 1 indentation_jump +3 | 1 cob | 1 cob | 1 cob | 1 cob | 1 exp_expr +1 |
| `for_iter_dot` | block head: `for` iterable | `dot` | **0** | 1 expected_field_name | 1 indentation_jump +3 | 1 cob | 1 cob | 1 cob | **0** | 1 expected_field_name +1 |
| `match_kw` | block head: `match` value | `kw_match` | **0** | 1 exp_expr +1 | 1 indentation_jump +3 | 1 cob | 1 cob | 1 cob | 1 cob | 1 exp_expr +1 |
| `match_stmt_kw` | block head: `match` statement | `kw_match` | **0** | 1 exp_expr +1 | 1 indentation_jump +3 | 1 cob | 1 cob | 1 cob | 1 cob | 1 exp_expr +1 |
| `match_scrut_op` | block head: `match` scrutinee | `plus` | **0** | 1 exp_expr +1 | 1 indentation_jump +3 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr +1 |
| `if_value_kw` | expression: `y = if` | `kw_if` | **0** | 1 exp_expr | 1 indentation_jump +3 | 1 cob | 1 cob | 1 cob | 1 cob | 1 exp_expr +2 |
| `arm_arrow` | match arm: `=>` | `fat_arrow` | **0** | **0** | 1 exp_expr | 1 cob | **0** | 1 cob | **0** | **0** |
| `arm_op` | match arm: inline value | `plus` | **0** fmt2 | 1 exp_expr | 1 exp_expr | 1 cob | 1 cob | 1 cob | **0** fmt2 | 1 exp_expr +1 |
| `arm_pipe` | match arm: pattern `|` | `pipe` | **0** | 1 expected_pattern | 1 expected_pattern | 1 cob | 1 cob | 1 cob | **0** | 1 expected_pattern |
| `arm_dot_case` | match arm: value `.case` | `dot` | **0** fmt2 | 1 expected_case_name | 1 expected_case_name | 1 cob | 1 cob | 1 cob | **0** fmt2 | 1 expected_case_name +1 |
| `arm_return_op` | match arm: `return` value | `plus` | **0** fmt2 | 1 exp_expr | 1 exp_expr | 1 cob | 1 cob | 1 cob | **0** fmt2 | 1 exp_expr +1 |
| `fn_kw` | header: `function` | `kw_function` | **0** | 1 expected_name | n/a | 1 cob | 1 cob | n/a | 1 cob | 1 expected_name +1 |
| `fn_arrow` | header: `->` | `arrow` | **0** | 1 expected_type | n/a | 1 cob | 1 cob | n/a | 1 cob | 1 expected_type +2 |
| `fn_generic_lt` | header: generics `<` | `lt` | **0** | 1 expected_type_parameter +2 | n/a | 1 cob | 1 cob | n/a | **0** | 1 expected_type_parameter +3 |
| `fn_generic_comma` | header: generics `,` | `comma` | **0** | 1 expected_type_parameter +2 | n/a | 1 cob | 1 cob | n/a | 1 cob | 1 expected_type_parameter +3 |
| `fn_ret_modtype` | header: result type `.` | `dot` | **0** | 1 expected_qualified_type | n/a | 1 cob | 1 cob | n/a | **0** | 1 expected_qualified_type +2 |
| `fn_ret_opt` | header: result `?` (ender, control) | `question` | **0** | **0** | n/a | **0** | **0** | n/a | **0** | **0** |
| `const_kw` | header: `constant` | `kw_constant` | **0** | 1 expected_name | n/a | 1 cob | 1 cob | n/a | 1 cob | 1 expected_name +1 |
| `const_colon` | header: constant `:` | `colon` | **0** | 1 expected_type | n/a | 1 cob | 1 expected_type | n/a | 1 expected_type | 1 expected_type +2 |
| `const_body_op` | constant body | `plus` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `record_kw` | header: `record` | `kw_record` | **0** | 1 expected_name | n/a | 1 cob | 1 cob | n/a | 1 cob | 1 expected_name +1 |
| `field_colon` | record member line `:` | `colon` | **0** | 1 expected_type +1 | 1 expected_type +1 | 1 cob | 1 expected_type +1 | 1 cob | 1 expected_type +1 | 1 expected_type +1 |
| `field_modtype` | record member type `.` | `dot` | **0** | 1 expected_qualified_type +1 | 1 expected_qualified_type +1 | 1 cob | 1 cob | 1 cob | **0** | 1 expected_qualified_type +1 |
| `variant_kw` | header: `variant` | `kw_variant` | **0** | 1 expected_name | n/a | 1 cob | 1 cob | n/a | 1 cob | 1 expected_name +1 |
| `case_field_colon` | variant case member `:` | `colon` | **0** | 1 expected_type +1 | 1 expected_type | 1 cob | 1 expected_type +1 | 1 cob | 1 expected_type +1 | 1 expected_type +1 |
| `test_kw` | header: `test` | `kw_test` | **0** | 1 expected_test_name | n/a | 1 cob | 1 cob | n/a | 1 cob | 1 expected_test_name +1 |
| `test_assert_op` | test body `assert` | `eq_eq` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `extern_kw` | header: `extern` | `kw_extern` | **0** | 1 expected_extern_header | n/a | 1 cob | 1 cob | n/a | 1 cob | 1 expected_extern_header +1 |
| `ext_fn_kw` | extern member: `function` | `kw_function` | **0** | 1 expected_name +1 | 1 expected_name +1 | 1 cob | 1 cob | 1 cob | 1 cob | 1 expected_name +1 |
| `ext_fn_arrow` | extern member: `->` | `arrow` | **0** | 1 expected_type +1 | 1 expected_type +1 | 1 cob | 1 cob | 1 cob | 1 cob | 1 expected_type +1 |
| `ext_const_colon` | extern member: constant `:` | `colon` | **0** | 1 expected_type +1 | 1 expected_type +1 | 1 cob | 1 expected_type +1 | 1 cob | 1 expected_type +1 | 1 expected_type +1 |
| `ext_acquires_pipe` | extern member: releaser `|` | `pipe` | **0** | 1 expected_releaser +1 | 1 expected_releaser +1 | 1 cob | 1 cob | 1 cob | **0** | 1 expected_releaser +1 |
| `use_kw` | `use` | `kw_use` | **0** | 1 expected_module_name +1 | n/a | 1 cob | 1 cob | n/a | 1 cob | 1 expected_module_name +1 |
| `use_slash` | `use` path `/` | `slash` | 1 module_path_wants_a_part +1 | 1 module_path_wants_a_part +1 | n/a | 1 cob +2 | 1 cob +2 | n/a | 1 module_path_wants_a_part +1 | 1 module_path_wants_a_part +1 |
| `open_paren` | opener `(` (depth 1) | `lparen` | **0** | **0** | **0** | **0** | **0** | **0** | **0** | **0** |
| `open_bracket` | opener `[` (depth 1) | `lbracket` | **0** | **0** | **0** | **0** | **0** | **0** | **0** | **0** |
| `open_brace` | opener `{` (depth 1) | `lbrace` | **0** | **0** | **0** | **0** | **0** | **0** | **0** | **0** |
| `cm_trailing_op` | trailing comment after `+` | `plus` | **0** | 1 exp_expr | 1 exp_expr +1 | 1 cob | 1 cob | 1 cob | **0** | 1 exp_expr |
| `cm_trailing_dot` | trailing comment after `.` | `dot` | **0** | 1 expected_field_name | 1 expected_field_name +1 | 1 cob | 1 cob | 1 cob | **0** | 1 expected_field_name |
