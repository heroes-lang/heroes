# Panel 184, compiler-engineer report

Written as it goes, 2026-09-30 from 17:03. Seat directory:
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/184-compiler-engineer/`
(below `<ce>`), holding `<ce>/tree`, `git archive a294a6ff`, with `build`
removed, and `<ce>/tree/heroes` built from its seed with plain `clang` at 17:03
(`real 10.87 user 7.10`, load average 22.00 by `uptime`).

Status: in progress.

Stopped at 17:09 by an API session limit (HTTP 429, the coordinator's
message), resumed 17:23; nothing of mine was running in between.

## The base timing, taken first while the machine was quiet

`uptime` read load 2.60 to 2.73 over these runs, against 22.00 at 17:03.
In `<ce>/tree`, with `<ce>/tree/heroes` (the seed's build):

| command | real | user | sys |
|---|---|---|---|
| `./heroes check selfhost/main.hero`, three runs | 4.34, 4.29, 4.36 | 4.25, 4.22, 4.29 | 0.02 each |
| `rm -rf build`, then `./heroes build selfhost/main.hero -o heroes-next` (cold) | 80.99 | 70.55 | 8.59 |
| the same build again (warm) | 71.52 | 69.85 | 0.45 |

`real` is within `user + sys` in every row, so none of them waited. The two
builds' binaries differ at byte 1449 (`cmp`), which is the link's own and not
the compiler's output.

## Question 1: the forgotten `f`

### Where each route lives

Read in `<ce>/tree` (the commit). A plain literal is one `str_lit` token,
`selfhost/literals.hero:95` `string`, and the parser makes it a childless
`.str_lit` node (`selfhost/grammar_expr.hero:467`); nothing after the lexer
looks inside it. An `f` literal is pieces and ordinary tokens,
`selfhost/lex_interp.hero` (171 lines), parsed by `interpolation`
(`grammar_expr.hero:438`) into `.interp`, resolved hole by hole
(`selfhost/resolve/walk.hero:117`) and typed by `check/interp.hero`.

- **(1a)** needs the lexer's and the parser's reading of the literal's text
  with an `f` before it. It cannot live in `literals.hero` or in the parser's
  knot: `lex_interp` uses `literals`, `grammar_expr` uses `lex_interp`, and
  Heroes refuses module cycles (`grammar_expr.hero:7-14`). So it is a module
  that uses both, called after the parse: the front door
  (`selfhost/parse.hero`, 172 code lines of 300) or the resolver.
- **(1b)** is (1a) plus a scope question, so it lives where scope is: the
  resolver's walk, `.str_lit` arm (`resolve/walk.hero:115`), which already
  `use`s `parse`; it asks `state.lookup_local`, `state.top_visible`,
  `inventory.index_of` and `resolved.is_used_module`, the order
  `resolve/names.hero:29` `bare_name` uses.
- **(1c)** lives in the unused sweep, `resolve/state.hero:354`
  `report_unused`, a file at 325 of its decided 330 by the layout measure,
  plus the message in `resolve/errors.hero:133` (260 of 300).
- **(1d)** costs nothing.

None of them is core (design.md Part 5, §1.7): a diagnostic, erased nowhere
because it adds no construct; no checker type, no lowering, no IR, no
emitter, no runtime line.

### The census that decides (1a) against (1b)

A census tool, `<ce>/tree/selfhost/zz_braces.hero` (never part of the
compiler), lexes every tracked `.hero` file at `a294a6ff` (1,389, `git
ls-tree -r --name-only a294a6ff | grep '\.hero$'`) with `parse.parse`, and
for every `str_lit` token holding a `{` re-lexes `f` + the literal with
`lexer.lex_one` (the real `lex_interp`) and parses each hole's tokens with
`grammar_expr.parse_expr`: a hole counts when it lexes and parses as one
expression with no diagnostic. Run over all 1,389 in 2.12 s:

| plain literals holding a `{` | count |
|---|---|
| no complete hole when read with an `f` (`{{`, a brace never closed, `\"` in a hole) | 201 |
| at least one hole, none parses (`{K: V}`, `{}`, `{str: i64}`) | 140 |
| **at least one hole that parses: what (1a) refuses** | **34** |

All 34 stand where an expression does (none is a test name, a header or a
pattern). **None is a forgotten `f`.** By reading each in its file:

| what it is | count | where |
|---|---|---|
| template data, a template engine's input | 20 | `examples/template/main.hero:145, 146, 150, 153, 159, 163, 164, 165, 166, 178, 186, 200, 201, 202, 210, 218, 222, 223, 234, 235` |
| C text the emitter writes, C's `{0}` initialiser | 6 | `selfhost/emit/assert_spelling.hero:188, 192, 277, 278`, `selfhost/emit/body.hero:182`, `selfhost/emit/extern_record.hero:175` |
| Heroes source, or its expected output, inside the compiler's and the harness's own tests | 7 | `selfhost/escape_readings.hero:306` (two), `:307`, `selfhost/lex_interp.hero:148`, `selfhost/lexer.hero:427`, `selfhost/next_line.hero:261`, `tests/harness/suite_surface.hero:367` |
| a golden fixture with a brace in a plain literal on purpose | 1 | `tests/golden/check/fixedbugs-135-a-backslash-before-a-bracketed-expression.hero:33` |
| a forgotten `f` | 0 | |

**(1b)**, the same shape only where at least one name is read and every name
is bound where the literal stands, refuses **none of the 34**, measured with
the prototype below: the template's names (`name`, `role`, `nobody`,
`anything`) are map keys and no binding, the emitter's `{0}` names nothing,
and the tests' `{x}`, `{n}`, `{a}`, `{b + 1}` name nothing their test binds.
**A (1b) that reads "every name bound" without "at least one" refuses the
emitter's six `{0}`**: a hole that names nothing is vacuously bound, so the
route as the brief words it needs that clause.

### The prototype, route (1b)

In `<ce>/tree`, measured by `tests/harness/suite_layout.hero`'s `code_lines`
(mirrored in `<ce>/code_lines.py`):

| file | code lines | what |
|---|---|---|
| `selfhost/plain_holes.hero` (new) | 109 | the holes a plain literal holds when read with an `f`, by `lexer.lex_one` and `grammar_expr.parse_expr`; the fix; the diagnostic |
| `selfhost/resolve/plain_literal.hero` (new) | 40 | (1b)'s scope question, and the hole's names counted as read so `unused_binding` does not fire for the same mistake |
| `selfhost/resolve/walk.hero` | 254 to 256 of 300 | the `.str_lit` arm calls it, one `use` |
| `selfhost/diag.hero` | 127 to 128 | `hole_without_f` joins `is_thesis_rule`, so `--permissive` drops it |

152 code lines, all frontend. (1a) is the same code without the scope
question (one condition), so about 125; (1c) was not built.

`hole_without_f` points at the hole's braces, and its fix, a **`guess`**,
rewrites the literal as an `f` literal doubling every brace that is not a
hole: `print("{n} and {K: V}")` gets `f"{n} and {{K: V}"`. **Why `guess`**,
by `.claude/rules/diagnostics-and-goldens.md:22`: the defect the message
names is a hole-shaped brace in a plain literal, and two repairs remove it,
writing the `f` (the program then prints the value) or writing the brace as
text in an `f` literal (it prints what it printed before). Nothing in the
program says which was meant; writing the `f` changes the output, so `check
--apply` must not choose. **The rule reaches a literal inside a hole of an
`f` literal**: the resolver's `.interp` arm walks each hole, and a `str_lit`
there reaches the same arm; measured on `print(f"{"{n}"} nested plain")`,
refused at the inner `{n}`.

**The brief's (1a) wording is wrong about the language** (measured): in an
`f` literal only the opening brace doubles. `print(f"{{n}")` prints `{n}`;
`print(f"{{n}}")` prints `{n}}` (`<ce>/q1/braces-text.hero`, built and run
with `<ce>/heroes-base`). So text braces are `{{`...`}`, not `{{`...`}}`, and
the spelling Python, C# and Rust teach (both doubled) is itself silent here,
exit 0 with a stray `}`: a shape beside the forgotten `f` that none of the
four routes touches.

### The census of `check --brief`, every tracked file, both arms

`<ce>/census.py` runs `check --brief` (and `--brief --permissive`) on each of
the 1,389 files from `<ce>/corpus`, a second untouched extraction of
`a294a6ff` (a first run from `<ce>/tree` read my own edited `walk.hero` and
is discarded), six at a time, and `<ce>/cmp_census.py` compares exit and full
output file by file.

| compiler | normal arm: files moved | permissive arm: files moved |
|---|---|---|
| base (`a294a6ff`'s seed), exits 653 at 0, 736 at 1 | | |
| (1b) prototype | **0** | **0** |
| (1a) variant | **22**, every one exit 0 to 1 | **0** |

The 22 under (1a), each for the literals above: `examples/template/main.hero`
(its 20); `selfhost/main.hero` and the nineteen other `selfhost/` files that
check alone and reach the offending modules (`checker`, `emit`,
`escape_readings`, `escape_report`, `grammar_expr`, `ir`, `layout`,
`lex_interp`, `lexer`, `line_above`, `literals`, `modules`, `next_line`,
`open_line`, `parse`, `resolve`, `scan`, and `main`), for the 6 `{0}` and
the 6 in the compiler's tests; `tests/harness/main.hero`,
`suite_probe.hero` and `suite_surface.hero`, for `suite_surface.hero:367`.
**The compiler would stop compiling itself under (1a)** until those 12
literals are respelled, which Principle 0 makes the first cost. The 34th
literal, the `fixedbugs-135` golden, does not move under a resolver placement
because its file stops at the lexer's `unknown_escape`; a parse-stage (1a)
reaches it and changes that golden's expected output.

Stopped a second time around 18:20 by the stream watchdog (the machine at
load 150 beside three lanes' gates, the coordinator's message), resumed
19:54 at load 19.5. The finding below was measured at 18:03 and is written
here first.

## A defect under Question 2, found before any prototype: `match` counts as leaving whatever its arms do

**Measured with `<ce>/heroes-base`** (the seed of `a294a6ff`), programs in
`<ce>/q2/`:

```
variant Color
    red
    blue

function f(c: Color) -> i64
    match c
        .red => print(1)
        .blue => print(2)
    print(3)
```

`check --brief match-then-falls.hero` exits **0**, silent. `build` exits
**2**: `internal error: compiling the generated C failed: ... error: non-void
function 'h_matchthenfalls_f' should return a value [-Wreturn-mismatch]`,
at the `return;` the lowering writes on the fall-through edge
(`selfhost/ir/lower.hero:180-189`, `close`). The same with the `match` as
the body's only statement (`match-prints.hero`): check 0, build 2.

**Why.** `missing_return` is `!got.jumps && result != unit` over the body's
`walk.block` (`selfhost/check/decls.hero:159-166`), and `block` sets `jumps`
when any statement jumps (`selfhost/check/walk.hero:690-720`). For a `match`
in statement position, `expression_statement` computes `all_jump =
branches.len() > 0 && !produced` (`walk.hero:898-904`), where `produced`
asks whether an arm's `value` is present; and `arms` gives an arm that
jumps and an arm that falls through the same absent value (`walk.hero:1041-1050`:
a one-statement arm maps `got.jumps` to `fail("jumps")` and a fall-through to
its `got.value`, absent for a statement; a block arm never reads
`got.jumps`). So **every `match` statement with an arm counts as leaving**,
and `missing_return` goes silent for any function whose body holds one at its
top level, whatever follows it.

**What it means for Question 2.** The brief's premise is right that
`Outcome.jumps` is already the one predicate both rules would read; it is
wrong about `match` today. A (2a) built on it as it stands refuses the
statement after **every** `match` statement in the tree as unreachable. So
the predicate is repaired first (a `jumps` beside `value` in `join.Branch`,
`selfhost/check/join.hero:33`, set in `arms` and read by the statement
`match`), in any route including (2c). The coordinator may want it filed as
a defect whatever the sitting decides; it predates this sitting.

## Question 1, continued: the instrument's `forget-f` against the prototypes

`python3 <scratchpad>/instrument/tool/recovery.py --compiler <ce>/heroes-<n>
--out <ce>/inst-ff-<n> --only forget-f --no-pairs --jobs 2 --dear 20`, the
instrument's own tree (`c85bccb8`). **`--dear 20`** so that no file leaves the
corpus for its time: the base run's census (at load up to 150, 18:29) read 647
accepted and 3 `other`, three timeouts; the (1b) repaired run's (19:57, load
about 13) read 650 and 0. **The 25 sites are the same 25 in every run**
(compared by key over `singles.jsonl`).

| compiler | sites | SILENT | ONE | EXTRA | ELSEWHERE | control arm (`--permissive`) |
|---|---|---|---|---|---|---|
| base | 25 | 17 | 1 | 4 | 3 | 23 SILENT, 1 ONE, 1 EXTRA |
| (1a) variant | **21** | 0 | 18 | 3 | 0 | 21 SILENT |
| (1b) first build | 25 | 0 | 21 | 4 | 0 | 23, 1, 1 |
| **(1b) repaired, `<ce>/heroes-1b2`** | **25** | **0** | **24** | **1** | **0** | **23, 1, 1** |

The base row reproduces 00-shared.md's counts exactly. **(1a) plants only
21**: its own census refuses 18 more files than base (448 against 430), and
four of the 25 sites are in them (`tests/golden/surface-fixtures/comments101/holes.hero`,
all four), so (1a) is measured on a smaller corpus and cannot be compared
site for site. The first (1b) build's three EXTRA were my prototype's gap,
not the route's: it counted as read only the names of the hole it reported,
so a binding read by a later hole of the same literal (`word`, `of`, `star`,
`flag`) was still `unused_binding`. Repaired (every hole's bound names are
read), those three are ONE.

**Under (1b), 23 of the 25 are told at the literal by `hole_without_f`**,
which is every site whose literal stays one plain token without its `f`. The
other two are `f"{")"}"` and `f"{"("}"` in the `comments101/holes.hero`
fixture: without the `f` the inner quote ends the literal, so there is no
plain literal to judge; the parser says `unclosed_bracket` (ONE) or
`expected_end_of_line` plus `expected_expression` (EXTRA), as it does
today. No literal rule reaches them.

The census for `<ce>/heroes-1b2`, both arms, all 1,389 files: **0 moved**.

**The six sites 00-shared.md reports only by `unused_binding` at the
binding** are now told at the literal, and `unused_binding` no longer fires
beside it, because the prototype counts the hole's names as read. That is
(1c)'s whole reach, obtained as a side effect of (1b): **(1c) is dominated**,
it reaches those six and not the seventeen.

## Question 2: a statement after a jump, and `missing_return` with it

### Where it lives, and what is core

`missing_return` is `!got.jumps && result != unit` over the body's
`walk.block` (`selfhost/check/decls.hero:159-166`); `block` sets `jumps` when
any statement's `Outcome.jumps` is set (`selfhost/check/walk.hero:690-720`).
So **the one predicate already exists** and both rules can read it from one
walk, by construction. What (2a) adds is two things, one in each half:

- **the refusal**: in `block`, the first statement after one that left is
  told, once per block. Checker only; a refused program never reaches the
  lowering.
- **the widening**: `exit(code:)`, `assert false` and `while true` with no
  `break` of its own set `jumps`. This half **reaches the lowering**: the
  lowering's `close` (`selfhost/ir/lower.hero:180-189`) writes a valueless
  `return` on a body's fall-through edge and trusts `missing_return` that a
  non-unit function never gets there. Once `missing_return` stops demanding the
  `return` after `exit`, that edge is reached in the IR (a call is not a
  terminator, and `while true`'s exit edge exists), so `close` must write
  `.unreachable` there. **No new construct**: the IR already has
  `.unreachable` (`selfhost/ir/build.hero:184`, used for a join every arm
  jumped past) and the emitter already prints it as `hero_unreachable();`, a
  `_Noreturn` abort that says it is a compiler bug (`runtime/parts/panic.c:45`).
  Part 5's table does not move: this is the checker telling the lowering a
  fact it already acts on for joins.

### The prototype, route (2a), in `<ce>/tree2` (a second untouched extraction)

| file | code lines, before to after | what |
|---|---|---|
| `selfhost/check/leaves.hero` (new) | 0 to 43 | `exit` by its resolved declaration (the library's, a name no program may redeclare: `builtin_name_taken`, measured), `assert false`, `while true` unbroken; `ROUTE`, the variant switch |
| `selfhost/check/walk.hero` | 1773 to 1799 of a decided 1870 | the refusal in `block`; `jumps` in `arms` and in the statement `match`; `break` marks its loop; `while`, `for`, `assert`, expression statement read `leaves` |
| `selfhost/check/join.hero` | 65 to 68 | `jumps` beside `value` in `Branch` |
| `selfhost/check/state.hero` | 200 to 207 | `broke`, the open loops' breaks; `leaves`, the bodies that always leave |
| `selfhost/check/decls.hero` | 219 to 222 | a body that always leaves is recorded for the lowering |
| `selfhost/ir/lower.hero` | 176 to 184 | `close` writes `.unreachable` for such a body, non-unit result, reached block |
| `selfhost/flow_errors.hero` | 150 to 161 | `unreachable_statement` |

(The before and after numbers are `<ce>/code_lines.py` on `<ce>/corpus` and
`<ce>/tree2`; the table is re-measured below with the variants.)

**The `.unreachable` close is held to where no program's IR changes today**:
only a non-unit result (a `()` function keeps its valid `return;`, which is
what the blessed emissions in `tests/emission/` hold), and only a block an
edge reaches or the entry (a join every branch returned past keeps its
`return`, which the emitter drops, so no `tests/golden/ir/` dump moves). For a
non-unit function the edge is reached today only through the `match` defect
above.

### Every probe, `check --brief`, base against (2a)

| program | base | (2a) | what (2a) says |
|---|---|---|---|
| `after-return.hero` | 0 | 1 | `unreachable_statement` at 3:5, *the one at line 2 always leaves the block before it* |
| `after-bare-return.hero` | 0 | 1 | the same at 4:5 |
| `after-break.hero` | 0 | 1 | the same at 7:9 |
| `after-continue.hero` | 0 | 1 | the same at 7:9 |
| `after-if-else-returns.hero` | 0 | 1 | the same at 6:5, naming line 2, the `if` |
| `exit-last.hero` | 1 | 0 | `missing_return` no longer demands the `return` |
| `assert-false-last.hero` | 1 | 0 | the same |
| `while-true-last.hero` | 1 | 0 | the same |
| `exit-then-return.hero` | 0 | 1 | the `return 0` it used to demand is `unreachable_statement`: one predicate, no function without a legal spelling |
| `after-match-returns.hero` (mine) | 0 | 1 | after a `match` whose arms all return |
| `while-true-then-stmt.hero` (mine) | 0 | 1 | after a `while true` left only by `return` |
| `while-true-inner-break.hero` (mine) | 1 | 0 | a `break` in a nested `for` leaves the `for`; the `while true` is still endless |
| `while-true-arm-break.hero` (mine) | 0 | 0 | a `break` in a `match` arm leaves the loop, so the `return` after it is reached |
| `match-prints.hero`, `match-then-falls.hero` (mine) | **0** | **1** | `missing_return`: the repaired `match` |
| `exit-in-main.hero`, `assert-false-arm.hero`, `must-on-fail.hero` (mine) | 0 | 0 | |

Built and run with `<ce>/heroes-2a` (`HEROES_RUNTIME=<ce>/corpus/runtime`):
`exit-last` prints `1`, and with `f(n: 0)` exits **3**; `assert-false-last`
prints `1`, and with `f(n: 0)` aborts **134**, `assert failed: false`;
`while-true-last` prints `2`; `while-true-inner-break` prints `3`. The C of
`exit-last`'s `f` ends `h_library_exit(t5); hero_unreachable();` where the
base compiler could only have written `return;`.

### The census under (2a), every tracked file

`<ce>/census.py` with `<ce>/heroes-2a` over `<ce>/corpus`, 4 jobs; compared
with the base by `<ce>/cmp_census.py` (`<ce>/q2/cmp-2a-normal.txt`).

**51 files move, every one from exit 0 to 1, in both arms**, with **142
distinct diagnostics, every one `unreachable_statement`** (the roots
`selfhost/main.hero`, `tests/harness/main.hero` and their siblings each
repeat the modules they reach). No `missing_return` is new: the `match`
repair refuses no program in the tree. Classified mechanically by the
statement that left and the one refused (`<ce>/q2/classes-2a.txt`), then read:

| what the refused statement is | count | a defect of the program, or a reason against the rule |
|---|---|---|
| a `return` after a `match` every arm of which returns | 133 | dead code, never a defect in behaviour; several say so in their own text (`return fail(code: "none", msg: "unreached")`, `selfhost/check/consuming.hero:100`). **Not demanded by today's compiler**, which counts every `match` statement as leaving |
| a `return` after a `while true` left only by `return` | 3 | the return **today's `missing_return` demands** (`examples/interpreter/run/eval.hero:136`, and two in `selfhost/`): (2a) refuses the statement the old rule required |
| a fallback value declared after an all-returning `match` or a `while true` | 4 | dead code (`selfhost/emit/externs.hero:48`, `emit/types.hero:49` and `:59`, `emit/unread.hero:170`) |
| `print("after")` after `exit(3)` | 1 | **a reason against the rule's `exit` half**: `tests/golden/run/exit-status.hero` exists to show that nothing after `exit` runs, and (2a) makes that program unwritable; the golden must hide the `exit` behind a call |
| `seen @ seen + 1` after a `match` whose arms `break` and `continue` | 1 | **a golden that pins the lowering the rule would outlaw**: `tests/golden/ir/adversarial-diverging-arms.hero` exists for the join no arm reaches; the seal stays reachable (a loop body ending in such a `match`), so the golden is rewritten by hand, never regenerated (`tests/golden/ir/` forbids it) |
| a statement meant to run (the over-indented kind) | 0 | |

**What (2a) costs the compiler itself**: `selfhost/` stops compiling itself
until about 136 dead statements in its own modules are deleted, a mechanical
migration (every one is the last statement of a function after a statement
that leaves), which Principle 0 puts before the rule lands. The fix the
diagnostic can offer is **deletion, and it is a `guess`**, by
`.claude/rules/diagnostics-and-goldens.md:22`: for the 140 dead fallbacks
deleting repairs what the message names and preserves behaviour, but for the
over-indented statement (the instrument's shape, below) deleting keeps the real
defect, a loop that no longer advances, so `check --apply` must not do it.

**The permissive arm moved too (51) because the prototype did not register
`unreachable_statement` in `is_thesis_rule`**; without the rule a program still
has a meaning (the statement never runs), so by `selfhost/diag.hero:93-110`'s
definition it is a thesis rule, like `unused_binding`. Re-measured registered,
below.

## Question 3: how deep a source may nest

### What the tracked tree holds today, measured

A census tool, `<ce>/tree/selfhost/zz_depth.hero` (never part of the
compiler), parses each of the 1,389 tracked files with `parse.parse` and walks
every declaration's body: the tree's depth in expression and statement nodes;
the openers open at once, from the token stream (`(`, `[`, `{`, an indent, a
hole); and on the way down the three chains. Checked first on `depth.py`'s
shapes at 10, each read as built (a 10-deep `+` chain: binary 10, openers 2;
ten nested parentheses: openers 11 but tree 3, because grouping builds no
node). Over all 1,389 files, 2.71 s, none unread:

| kind | deepest in the tree | where | the compiler's smallest abort on this Mac (00-shared.md) |
|---|---|---|---|
| tree depth, every node kind | **28** | `selfhost/emit/extern_field.hero` | `g(g(...))` 100 (`build` aborts at 100, 90 builds) |
| openers at once (brackets, blocks, holes) | **20** | `tests/golden/check/fixedbugs-a-nesting-bound-was-a-silence.hero` | `if` nested 200, `[[...]]` 250, `(` 600 |
| one-operand chains (`- -`, `!!`) | **1** | many | 250 |
| binary nesting (`+`, `&&`, mixed) | **23** | `selfhost/emit/extern_field.hero` | 250 |
| suffix chains (calls, fields, indexes, `?`) | **13** | `tests/harness/suite_surface.hero` | 200 |

**The margin is 3.5 at its thinnest** (28 against `build`'s 100 on calls), and
it holds only at 8 MB of main-thread stack.

### Why the compiler aborts where it does: the frames

`clang -I runtime -fstack-usage -c seed/heroes.c` (the seed of `a294a6ff`, at
the flags the seed is built with, plain `clang`, so `-O0`) reports each
function's frame, `<ce>/q3/heroes.su`:

| function | bytes | function | bytes |
|---|---|---|---|
| `checkwalk.synth` (`check/walk.hero:96`) | **37,104** | `grammarexpr.if_expr` | 7,248 |
| `checkwalk.check` (`:381`) | 26,336 | `grammarexpr.primary` | 4,608 |
| `checkwalk.statement` (`:722`) | 26,304 | `grammarexpr.call_args` | 4,448 |
| `resolvenames.bare_name` | 18,688 | `grammarexpr.postfix` | 4,432 |
| `resolvewalk.expr` (`resolve/walk.hero:107`) | 14,176 | `checkwalk.block` | 3,632 |
| `checkwalk.expression_statement` | 11,888 | `grammarexpr.binary` | 3,136 |
| `resolvewalk.statement` | 10,432 | `grammarexpr.block` | 2,752 |
| `resolvequalified.qualified` | 9,920 | `checklower.named` | 2,432 |
| `checkwalk.arms` | 8,016 | `grammarexpr.array_literal` | 2,016 |
| `checkwalk.valued_expression` | 6,384 | `grammarexpr.unary` | 1,616 |
| `checktable.ty_key` | 5,072 | `grammarexpr.statement` | 1,456 |
| `cursor.pass_comments` | 672 | `grammarexpr.group` | 336 |
| | | `grammarexpr.parse_expr` | 176 |

At `-O0` every temporary of a function has its own slot, so the knots' large
`match` functions carry frames of tens of kilobytes. **Checked against the
table**: a level of `((1))` costs the parser `parse_expr`, `binary`, `unary`,
`postfix`, `primary`, `group`, 14,304 bytes, and 8,372,224 bytes (Darwin's main
thread, `runtime/parts/spawn.c`'s own figure) over 14,304 is 585, against the
measured abort at 600. A level of a `+` chain costs the resolver one
`resolvewalk.expr` (14,176, so about 590: the table's *`resolvewalk.expr`
from 600*) and the checker at least one `synth` (37,104, about 225: the
table's 250). The depths are the stack over the frames; **the frames change
with the optimisation level, the clang and the architecture**, and the stack
with `ulimit -s` and the thread, which is why 00-shared.md found the abort
moving between builds.

### Threads

**The compiler parses and checks on the main thread only.** Its one
`hero_thread_spawn` is in a test (`selfhost/cli/link.hero:208-219`, six
concurrent C builds of a trivial program); no pass runs on a thread. A thread
the runtime starts never gets less stack than the main thread
(`runtime/parts/spawn.c`, panel 115's floor), and on Windows every thread gets
the executable's `/STACK:67108864` (`selfhost/cli/flags.hero:194`). So the
smallest stack the compiler runs on is the main thread's: `ulimit -s` on
Darwin and Linux, 64 MiB on Windows.

### The variants, each a build of `<ce>/tree2` with `leaves.ROUTE` set

| build | what it is | normal arm: files moved | permissive arm |
|---|---|---|---|
| `<ce>/heroes-2a2` | (2a), `unreachable_statement` registered in `is_thesis_rule` | 51 (the same 142 diagnostics as above) | **0** |
| `<ce>/heroes-2b` | (2b): the refusal after `return`, `break`, `continue` only; `missing_return` unchanged but for the `match` repair | **0** | 0 |
| `<ce>/heroes-2r` | the `match` repair alone, which every route carries | **0** | 0 |

So **no program in the tree has a statement after `return`, `break` or
`continue` in the same block**, and the repair of `match` refuses no program in
the tree either: both land at zero migration. Everything (2a) costs the tree
is its widening to `match`, `if`/`else`, `while true` and `exit`: the 142
statements above.

### The instrument's `over-indent`, 2,000 mutants (the brief's 150, times 13)

`recovery.py --only over-indent --no-pairs --per-op 2000 --jobs 2 --dear 20`,
the instrument's tree. **Base and (2b) mutate the same 2,000** (the same
census, 650 accepted, 17,382 sites, every key equal). **(2a) mutates a
different sample**: its census refuses the same 51 files as mine, leaving 599
accepted and 10,206 sites, so it shares **1,532** mutants with base, and only
those are compared (identical mutant, a base both compilers accept).

| build | mutants | SILENT | ONE | EXTRA | ELSEWHERE | control arm moved |
|---|---|---|---|---|---|---|
| base | 2,000 | 68 | 1,812 | 36 | 84 | |
| (2b), `<ce>/heroes-2b` | the same 2,000 | **49** | 1,831 | 56 | 64 | 0 |
| base, on the 1,532 shared | 1,532 | 46 | 1,394 | 24 | 68 | |
| (2a), `<ce>/heroes-2a2`, on the 1,532 shared | 1,532 | **29** | 1,411 | 42 | 50 | 0 |

**What moved, every mutant** (`<ce>/q2/oi-compare.txt`): under (2b), **19
SILENT become ONE**, each told at the moved line by `unreachable_statement`
(among them `selfhost/cursor.hero:269`, the brief's `refused_since` loop that
no longer ends, and `tests/harness/cases.hero:45`); and **20 ELSEWHERE become
EXTRA**: a function's last `return` moved under the `return` of the block
above, which today is only `missing_return` at the signature and is now that
plus `unreachable_statement` at the site. **On the 1,532 shared mutants (2a)
and (2b) give every mutant the same class**: the widening to `match`,
`if`/`else`, `while true`, `exit` and `assert false` catches no over-indented
statement the three jump words do not.

**The 20 EXTRA want one line more, not a different rule**: where a function
holds an `unreachable_statement`, hold back its `missing_return`, as the
driver already holds `missing_return` back for a hole (`selfhost/checker.hero:127`);
the statement moved back fixes both. Unbuilt; by the count above it turns the
20 into ONE, which is an inference until run.

**Where the message points, and the fix.** At the first statement that never
runs, naming the line of the one that left: the jump is usually right and the
statement is usually the mistake (all 19 above, and the 142 of the census, are
the refused statement's fault, never the jump's). An over-indented statement
earns a fix of its own, **a `guess`**: move the statement, and those after it
in the block, one level out, which is well defined only where the block is
nested in another; deleting it is the other repair, and neither is certain,
since for the 19 the move is right and for the 142 the deletion is. Unbuilt:
it is about fifteen lines in `flow_errors.hero` beside the message.

### The prototype, in `<ce>/tree3` (a third untouched extraction): the limit, then the thread

**The limit, `selfhost/parse/depth.hero` (new, 179 code lines), `LIMIT` 256,
held twice by the parser.** *Before it recurses*: the parser recurses once or
more per bracket, block and hole open around a token and per one-operand
operator written in a row before it (`grammar_expr.hero:257`, `unary` calls
itself), and for nothing else, since `binary` (`:231`) and `postfix` (`:302`)
climb a chain in a loop; so a token pass counts exactly those and a file where
one token has more than 256 around it is refused before the parser reads it.
*After it builds*: a chain climbed in a loop builds a tree as deep as the chain
is long, and every later pass recurses per level of it (resolver, checker,
lowering, emitter, formatter), so the built tree is walked by a function that
stops at 257 and so never recurses deeper itself. `check` says nothing after a
parse diagnostic (measured: `<ce>/q3/parse-then-name.hero` reports its
`unclosed_bracket` and not the `unknown_name` beside it), so a refusal there
protects every later pass. Told as `nesting_too_deep`, exit 1, in both arms:
it is not a thesis rule (without it the compiler aborts).
`selfhost/parse.hero` 172 to 202 code lines.

**The thread, so the number holds on every machine.** `selfhost/main.hero`
(217 to 229) runs the whole command on a thread of 256 MiB and exits with its
result; `runtime/parts/spawn.c` (341 to 357 physical lines, 23 added, 7
changed) gains `hero_thread_spawn_sized`, the existing spawn with an asked-for
size per slot: POSIX asks `pthread_attr_setstacksize` for the larger of it and
the floor, and the existing check after the thread exists (`hero_spawn_enter`,
*what the OS gave, never what it answered*) now compares against that size;
Windows passes it to `CreateThread` with `STACK_SIZE_PARAM_IS_A_RESERVATION`,
the flag whose absence panel 115's historian cited as libuv's defect.
`runtime/hero_os.h` +2. **The stack guard stays sound**: `hero_spawn_enter`
already installs it on every runtime thread (`hero_stack_guard_enter`, panel
107's door), so a thread that still ran out would say `panic: stack exhausted
in <function>` as `main` does today. A reservation is address space, not
memory: pages are committed as they are touched.

**The limit alone is not enough, measured** (`<ce>/q3/table.py`, `check
--brief` over `depth.py`'s programs at 90, 100, 200, 250, 255, 256, 257, 300,
400, 500, 600, 700, 800, 1000, 2000 and 10000, `<ce>/q3/check-base.json`
against `<ce>/q3/check-3c.json`): past the limit every shape is exit 1
`nesting_too_deep` where base aborts, but **under it, at 8 MB of main-thread
stack, `+`, `&&`, `- -`, `!!`, `[[`, `f"{` still abort at 250, and calls at
200** (`checkwalk.synth`, `checktable.ty_key`): 256 levels at `-O0` do not
fit in 8 MB, which is what the frames above predict (one `synth` alone is
37,104 bytes). The base column reproduces 00-shared.md's table cell for cell.

**00-shared.md's table re-measured under the prototype, limit and thread**
(`<ce>/heroes-3t`, `<ce>/q3/check-3t.json`; `check --brief`, this Mac, the
default `ulimit -s` 8176). A cell is the exit, `nesting_too_deep` where 1:

| shape | 90 | 100 | 200 | 250 | 255 | 256 | 257 | 300 to 10,000 | base aborts from |
|---|---|---|---|---|---|---|---|---|---|
| `x = ((((1))))` | 0 | 0 | 0 | 0 | 0 | **1** | 1 | 1 | 600 |
| the same, never closed | 1 `unclosed_bracket` | 1 | 1 | 1 | **1** | 1 | 1 | 1 | 600 |
| `x = [[[[1]]]]` | 0 | 0 | 0 | 0 | **1** | 1 | 1 | 1 | 250 |
| `x = g(g(g(1)))` | 0 | 0 | 0 | 0 | **1** | 1 | 1 | 1 | 200 |
| `x = - - - 1` | 0 | 0 | 0 | 0 | **1** | 1 | 1 | 1 | 250 |
| `b = !!!true` | 0 | 0 | 0 | 0 | **1** | 1 | 1 | 1 | 250 |
| `x = 1 + 1 + ... + 1` | 0 | 0 | 0 | 0 | **1** | 1 | 1 | 1 | 250 |
| `if true` nested | 0 | 0 | **1** | 1 | 1 | 1 | 1 | 1 | 200 |
| `f"{f"{...}"}"` | 0 | 0 | 0 | 0 | **1** | 1 | 1 | 1 | 250 |
| `if`, n-1 `else if`, `else` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | none |

**No cell aborts.** (The `&&` chain, the method chain and the block of `print`s
were not in this grid; the `+` chain and the `else if` chain stand for the flat
kinds, a method chain for which is unrun here.) The limit counts nodes, so it
lands a little before 256 for a chain inside a statement and at 128 levels of
`if`, which costs two nodes a level; the tree's deepest program today is 28.

**It holds under any `ulimit -s`** (measured, `sh -c "ulimit -s N; ..."`):

| `ulimit -s` | program | base | prototype |
|---|---|---|---|
| 8176 | `+` chain 250 | 134, `checkwalk.synth` | 0 |
| 1024 | `+` chain 90, calls 90, `if` 100, `(` 250 | 134 on all four | 0 on all four |
| 512 | the same four | 134 on all four | 0 on all four |
| 512 | **`check selfhost/main.hero`, the compiler on itself** | **134, `checktable.ty_key`** | **0** |

The last row is panel 107's recorded hole (the compiler checking itself exits
134 at `ulimit -s` 512, 640 and 768), closed by the thread and by nothing else.

**`build`, and the built program run** (`<ce>/q3/table.py ... build`,
`<ce>/q3/build-base.json` against `<ce>/q3/build-3t.json`):

| program | base | prototype |
|---|---|---|
| `g(g(...))` 90 | builds, runs, prints 1 | builds, runs |
| `g(g(...))` 100, 200, 250 | **134, `checklower.named`** | **builds, runs, prints 1** |
| `+` chain 250 | 134, `checkwalk.synth` | builds, runs, prints 251 |
| `[[...]]`, `- -`, `!!`, `f"{...}"` at 250 | 134, `checkwalk.synth` | build, run |
| `(` 255 | builds, runs | builds, runs |
| `if` nested 200 | 134 | 1, `nesting_too_deep` |
| `else if` chain 2,000 | builds, runs | builds, runs |

### A fourth kind of deep, which no parser counter can see: types

Measured with base, `<ce>/q3/types/rec-<n>.hero`: `n` records, each holding
the one before, every declaration flat, reached by one `xs: [R<n>] = []`.
**No line of the source nests anything.**

| records | base `check` | base `build` | prototype `build` |
|---|---|---|---|
| 100 to 700 | 0 | builds, runs | builds, runs |
| 1,000 | 0 | **134, `emitsynth.collect`** (11,296 bytes a frame; 8,372,224 / 11,296 is 741) | builds, runs |
| 3,000 | 0 | 134 | builds, runs |
| 10,000 | 0 | 134 | **2, `internal error: compiling the generated C failed`: clang itself crashes, `Illegal instruction: 4`**, on 10,000 nested struct types |

So the type kind has **three** boundaries: the compiler's stack (the thread
moves it), and clang's own recursion over the C it is given, which nothing in
this compiler controls; and the last one is an exit 2 that blames the tool,
the shape `.claude/rules/cli-surface.md`'s contract and CLAUDE.md § 7 exist to
refuse. **A bound on how deep a type may hold types by value** (a record in a
record, an element in an array) is therefore owed beside the source's, in the
checker where a record's fields are lowered (`selfhost/check/decls.hero`,
`.record_decl`), total by the set-of-ids walk defect 046's golden already
teaches (`tests/golden/check/fixedbugs-a-nesting-bound-was-a-silence.hero`:
a bound that gives up is a silence; this one must refuse aloud). Unbuilt; the
number is for the sitting, and 256 sits well under the 3,000 clang handled
here, which is one clang on one machine.

### A route nobody listed, (2d): the refusal of (2b), the widening of (2a)

00-shared.md's invariant is that no rule may refuse the statement
`missing_return` demands, or a function has no legal spelling. **That needs
the refusal's predicate to be contained in what `missing_return` counts as
leaving, not equal to it**: equality is (2a)'s choice, and the containment
holds just as well with a narrower refusal. So **(2d)**: refuse the statement
after `return`, `break` or `continue` (as (2b)), and count `exit(code:)`,
`assert false` and an unbroken `while true` as leaving for `missing_return`
(as (2a)), with the `match` repair and the lowering's `.unreachable` close.
Built as `<ce>/heroes-2d` (`leaves.ROUTE` `"2d"`), measured:

| | (2d) |
|---|---|
| the five jump probes | refused, as (2a) |
| `exit-last`, `assert-false-last`, `while-true-last` | exit 0, as (2a); built and run above |
| `exit-then-return` (the `return 0` after `exit`) | exit 0: **both spellings legal**, the one with the dead `return` and the one without |
| `after-if-else-returns`, after a `match` or `while true` that leaves | exit 0, as today |
| the `match` programs | `missing_return`, repaired |
| census, all 1,389 files | **0 moved, normal and permissive** |
| `over-indent`, the same 2,000 as base | **49 SILENT**, 1,831 ONE, 56 EXTRA, 64 ELSEWHERE: every mutant the class (2b) gives it; control arm unmoved |

What (2d) gives up against (2a) is the refusal after an `if`/`else`, a
`match`, `exit`, `assert false` or `while true` that leaves, which in the tree
is 140 dead fallbacks and two goldens and in the instrument is no mutant at
all. What it keeps is (2a)'s one real gain, the three shapes the brief names
compile without a statement nobody can reach.

## Cost and speed

### Lines, by `tests/harness/suite_layout.hero`'s measure

The numbers are `<ce>/code_lines.py`, a mirror of the suite's `code_lines`;
**the suite itself was run in each prototype tree** (`./heroes run
tests/harness/main.hero -- ./heroes layout` in `<ce>/tree`, `<ce>/tree2`,
`<ce>/tree3`): **4 passed, 0 failed** in all three, so no touched file passes
its ceiling.

| route | files, code lines before to after (ceiling) | total |
|---|---|---|
| (1b) | `selfhost/plain_holes.hero` new 109 (300); `selfhost/resolve/plain_literal.hero` new 40 (300); `selfhost/resolve/walk.hero` 254 to 256 (300); `selfhost/diag.hero` 127 to 128 (300) | **152**, frontend only |
| (1a) | the same without the scope question | about 125, frontend only |
| (2a), (2d), (2b) | `selfhost/check/leaves.hero` new 43; `selfhost/check/walk.hero` 1773 to 1799 (decided 1870); `check/join.hero` 65 to 68; `check/state.hero` 200 to 207; `check/decls.hero` 219 to 222; `ir/lower.hero` 176 to 184; `flow_errors.hero` 150 to 161; `diag.hero` +1 | **about 80**, checker and one lowering function; (2b) needs neither `leaves.hero`'s widening nor the lowering (about 45) |
| the `match` repair alone | `check/join.hero` +3, `check/walk.hero` about +6 | about 9 |
| (3c) plus the thread | `selfhost/parse/depth.hero` new 179 (300); `selfhost/parse.hero` 172 to 202 (300); `selfhost/main.hero` 217 to 229 (300); `runtime/parts/spawn.c` 341 to 357 physical lines; `runtime/hero_os.h` +2 | **221** Heroes, **18** net lines of C |
| the type bound | unbuilt | owed |

**None adds a core construct** (design.md Part 5): no new type, no new IR
instruction or terminator (`.unreachable` exists), no emitter line, no
descriptor. The runtime's one change is a size parameter on a function it has.

### Speed: `check selfhost/main.hero`, interleaved, one process at a time

Run at 21:14 from `<ce>/corpus`, **load average 8.2 falling to 6.6** (other
sessions' work; the machine was not still). `real` is within 2 to 4% of
`user + sys` in every run, so none waited, but `user` itself reads about 15%
above the quiet run at 17:23 (4.22 to 4.29), which is what a loaded machine does
to caches and clocks:

| compiler | user, two runs | against base |
|---|---|---|
| base | 4.86, 4.90 | |
| (1b), `<ce>/heroes-1b2` | 4.95, 4.94 | +1 to +2% |
| (2d), `<ce>/heroes-2d` | 4.94, 5.12 | +1 to +5% |
| (3c) plus thread, `<ce>/heroes-3t` | 5.00, 5.21 | +2 to +6% |

**Indicative, not a verdict**: the spread of one compiler between its own two
runs (up to 0.21 s) is as large as the differences. **The still-machine timing
of both `check` and `./heroes build selfhost/main.hero` for each prototype is
unrun**, because the machine did not stand still after 17:26; the base's own
still numbers are at the top of this report.

## Question 2, the two shapes the checker cannot know

- **`.must()` on a value the checker knows is a failure.** It knows one only
  at a literal `fail(...)`: the checker types it `.failure`
  (`selfhost/check/table.hero:90`) until it joins a `T?`. So
  `fail(code: "x", msg: "y").must()` is knowable and nothing else is: bound
  first, `x: i64? = fail(...)` then `x.must()`, the failure is a value and not
  a type, and `<ce>/q2/must-on-fail.hero` checks at exit 0 today and under
  every prototype. A regular-expression count over the 1,389 tracked files
  (`fail\([^()]*\)\.must\(\)`, an upper bound for the literal shape) finds
  **0**. Not worth a rule.
- **A call to a function every path of which leaves.** Not without making a
  line's legality depend on another function's body: the checker walks
  declarations in order with no fixpoint over calls, and even with one, editing
  the callee's body (adding a path that returns) would turn a caller's
  `return 0` from refused into demanded, in another function and possibly
  another module. That is the construct design.md §1.3 names, *meaning that
  lives elsewhere*. The known answer is a signature that says it (Rust's `!`,
  Kotlin's `Nothing`, which §4.7 cites and Heroes does not have); that is a new
  surface form and Principle 0's to admit, not this sitting's. `exit` is
  knowable only because it is one library declaration no program may redeclare
  (`builtin_name_taken`, measured on `<ce>/q2/exit-redeclared.hero`), asked by
  the resolved declaration, never by the name.

## Verdicts

No veto: no route adds a core construct (design.md Part 5), and the one route
that would breach the ceiling (§1.1), an explicit stack in every walk, is
refused below rather than vetoed, because nobody proposes adopting it.

### Question 1

- **verdict**: **(1b) approve, amended**: at least one name and every name
  bound where the literal stands (without *at least one*, the emitter's six
  `{0}` are refused); the hole's names counted as read, so `unused_binding`
  does not fire beside it; registered in `is_thesis_rule`; the fix a `guess`;
  its note teaches `{{` alone, since `}}` writes two braces. **(1a) refuse.**
  **(1c) refuse**, dominated by (1b). **(1d) refuse.**
- **section**: design.md §1.2 (a false alarm is a correction round),
  §4.17 (one mistake, one message, at the site; `certain` against `guess`),
  §1.0 (the compiler must go on compiling itself), §1.3 for the objection
  answered below.
- **implementation_cost**: 152 code lines, frontend only:
  `selfhost/plain_holes.hero` 109 (new), `selfhost/resolve/plain_literal.hero`
  40 (new), `selfhost/resolve/walk.hero` 254 to 256, `selfhost/diag.hero` 127
  to 128; layout suite 4 passed, 0 failed. (1a) about 125 and 34 literals to
  respell, 12 of them in the compiler and its harness.
- **needed_for_self_hosting**: no.
- **argument**: (1a) refuses 34 literals in the tree and none is a forgotten
  `f`: templates, C's `{0}`, Heroes source in tests; 22 files stop checking,
  the compiler among them. (1b) refuses none of them, and in the instrument
  turns 17 SILENT into 0 and 24 of 25 into ONE, 23 told at the literal, the
  control arm unmoved. My strongest objection to (1b) is §1.3: a literal's
  admissibility reads the scope, so a new local can refuse a distant template.
  It is the scope `unknown_name` already reads; the literal's meaning stays
  local, and the tree holds no case.
- **prediction**: (1b) landed as prototyped gives, on the instrument's
  `c85bccb8` tree, `forget-f` SILENT **0** and ONE **at least 23** by
  `hole_without_f`, control arm **23** SILENT, and a census of `check --brief`
  over the tracked tree moving **0** files: checkable at the batch gate that
  lands it.
- **condition**: a tracked or blind-seat program where (1b) refuses a
  literal meant as text (a placeholder naming a binding in scope), or a
  measured forgotten `f` whose hole names nothing bound (which (1a) catches and
  (1b) does not), at a rate above (1a)'s 34 false alarms.

### Question 2

- **verdict**: **the `match` repair, approve, owed in every route** (a
  defect today: `check` 0, `build` 2 with an internal error). **(2d),
  a route nobody listed, approve**: the refusal after `return`, `break`,
  `continue`; `exit(code:)`, `assert false` and an unbroken `while true` count
  as leaving for `missing_return`; the lowering closes such a body with
  `.unreachable`. **(2b) approve** as its lesser half. **(2a) object**: its
  wider refusal costs the tree 142 statements and buys no mutant. **(2c)
  refuse.** Add the hold-back of `missing_return` where a function holds an
  `unreachable_statement` (unbuilt).
- **section**: design.md §4.7 (a jump is an admissible arm body; a `match`
  every arm of which jumps produces no value), §4.17, Part 5 (no construct:
  `.unreachable` exists), §1.0.
- **implementation_cost**: about 80 code lines for (2d) or (2a), checker plus
  `ir/lower.hero`'s `close` (176 to 184); `check/walk.hero` 1773 to 1799 of a
  decided 1870; layout suite 4 passed, 0 failed. (2b) is a subset, about 45 by
  subtracting the widening's lines, an inference. The repair alone about 9.
- **needed_for_self_hosting**: no; the repair moves no tracked file.
- **argument**: the brief's invariant is containment, not equality: a refusal
  narrower than what `missing_return` counts as leaving leaves every function
  spellable. (2a) refuses 142 statements in 51 files (140 dead fallbacks, two
  goldens written for the shape) and on the 1,532 mutants it shares with base
  gives every one the class (2b) gives. (2d) moves 0 files in both arms, cuts
  `over-indent` SILENT from 68 to 49, and still frees the three shapes from a
  dead `return`.
- **prediction**: (2d) landed gives, on the `c85bccb8` tree with `--per-op
  2000`, `over-indent` SILENT **49** (from 68) with the census moving **0**
  files; with the hold-back, at least 18 of the 20 ELSEWHERE-to-EXTRA become
  ONE: checkable at its batch gate.
- **condition**: a statement meant to run found after an `if`/`else`, a
  `match`, `exit`, `assert false` or `while true` that leaves (0 of 142 today),
  or a blind-seat task where a model writes one: then (2a)'s wider refusal
  earns its 142-statement migration.

### Question 3

- **verdict**: **(3c) approve, amended, with (3b)'s thread**: one number,
  256, held by the parser on its tokens (brackets, blocks, holes, and
  one-operand runs, before it recurses) and on the tree it builds (after, by a
  walk that stops at the limit); every pass on a thread of a stack the
  compiler chooses (256 MiB in the prototype); **and a bound on how deep a type
  holds types by value, owed**, because no source counter sees it and clang
  crashes past it. **(3a) refuse** alone: it does not see the chains (the `+`
  chain, the method chain) nor types. **(3b) refuse** its explicit stack and its
  grown stack (every recursive knot rewritten: `check/walk.hero` alone is 1,799
  code lines; no portable split stack under clang), **approve** its thread.
  **(3d) refuse**: exit 134 on input breaks `.claude/rules/cli-surface.md:40-42`.
- **section**: design.md §1.12 (the tool must not die on its input: a
  diagnostic, not an abort), §1.1 (the ceiling, against (3b)'s rewrite);
  panel 107's ruling does not bind here, since both the frames and the stack are
  the compiler's own, which makes the number the compiler's too.
- **implementation_cost**: 221 Heroes code lines (`selfhost/parse/depth.hero`
  179 new, `selfhost/parse.hero` 172 to 202, `selfhost/main.hero` 217 to 229)
  and 18 net lines of C (`runtime/parts/spawn.c` 341 to 357, `hero_os.h` +2);
  layout suite 4 passed, 0 failed; the type bound unbuilt.
- **needed_for_self_hosting**: not at the default stack; **yes under a small
  one**: at `ulimit -s` 512 the compiler aborts checking itself today (134,
  measured) and checks itself under the prototype (0, measured).
- **argument**: the abort depth is the stack over the frames, and the frames
  are the compiler's: `checkwalk.synth` is 37,104 bytes at `-O0` on arm64,
  23,272 on x86-64. A counter alone leaves 256 levels that 8 MB cannot hold
  (measured: aborts at 200 to 250 under the limit); the thread alone leaves the
  number to the stack. Together: no cell of the table aborts to 10,000, `build`
  runs every accepted shape, `ulimit -s` stops mattering, and the census moves 0.
  The tree's deepest program is 28.
- **prediction**: limit and thread landed give, for `depth.py`'s programs at
  90 to 10,000, **no exit 134** from `check` or `build` on this Mac, on Linux
  x86-64 and on the Windows box, and `ulimit -s 512 ./heroes check
  selfhost/main.hero` **exit 0** on Darwin and Linux: checkable at the batch
  gate's platform legs.
- **condition**: a platform where the 256 MiB reservation is refused or
  delivers less (the runtime's own check would then panic at start, measured
  nowhere yet), or a build whose frames cost more than 1 MiB per level, or a real
  program nested past a quarter of the limit.

## What was not done, and why

- **Unbuilt**: the bound on type depth (Question 3's fourth kind); the
  hold-back of `missing_return` beside an `unreachable_statement`; the
  over-indent fix that moves a statement one level out; (1c). Each is named
  above with the file it would land in and what it would move; every effect
  claimed for them is an inference until built.
- **The (1a) instrument row** is the first (1a) build, which carried the same
  read-counting gap as the first (1b) build; its 3 EXTRA probably become ONE
  once repaired, which is an inference, unrun.
- **Unrun on the other platforms**: every measurement here is this Mac, arm64,
  `ulimit -s` 8176 unless a row says otherwise. The x86-64 frames are clang's
  static report for `x86_64-apple-macos13`, not a Linux run. The Linux and
  Windows legs belong to the batch gate that would land any of this.
- **Unrun on a still machine**: the prototypes' `check` and `build` timings
  (the machine held load 6.6 to 30 after 17:26); only the base's still timing
  stands.
- **The method chain, the `&&` chain and the block of `print`s** were not in
  my re-measured grid; 00-shared.md measured them on base.
- **No paid run**, no file written in the repository but this report, no
  process of another seat or lane touched.

## The one finding the synthesis should not miss

**`match` counts as leaving whatever its arms do**, so `missing_return` is
silent on any function whose body holds a `match` statement at its top level,
and such a program checks at exit 0 and then stops `build` with `internal
error: compiling the generated C failed ... non-void function ... should return
a value`, exit 2 (`<ce>/q2/match-then-falls.hero`, `<ce>/q2/match-prints.hero`,
measured with the seed of `a294a6ff`). The cause is `selfhost/check/walk.hero:898-904`
reading "no arm produced a value" as "every arm jumps", which `arms`
(`:1041-1050`) makes indistinguishable. It predates the sitting, it is the
predicate Question 2's premise rests on, a (2a) built on it as it stands would
refuse the statement after every `match` in the tree, and its repair (about 9
lines, `jumps` in `check/join.hero`'s `Branch`) moves no tracked file.

Status, 21:18 (by `date`): complete. (The *in progress* line at the top is the state the
file was started in, left as written.)
