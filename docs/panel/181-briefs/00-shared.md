# Panel 181, shared brief: a line that ends outside brackets where no token can end it

Written 2026-09-28 between 01:24 and 01:35 by the coordinator, for every seat.
Every number and path below names the command that produced it, run while this
brief was written. The compiler measured is the trunk's at `dfcac362`, built
from its seed with CLAUDE.md § Commands' first line; the tree the seats copy is
named in each seat's brief with the `git log -1` read before the briefs went
out.

## The sitting and why

**Defect 116** (`docs/work/DEFECTS.md`, the item whose first field is `116`),
found by lane B's agent on 2026-09-27: a statement continued on the next line
at its own margin, after a token that cannot end a line, compiles.

**Defect 118**, filed while this brief was written: the formatter accuses
itself (exit 2, *this is a compiler bug*) on one of those shapes.

**What the source of truth says.** design.md §4.15, `docs/design/design.md`
lines 1939 to 1948 (`sed -n 1939,1948p`):

> **Continuation lines: inside brackets only.** Within `(` `[` `{`, leading
> whitespace is not structural [...]. At bracket depth zero every line's
> indentation is structural: a long expression is broken inside parentheses
> or not at all. [...] Trailing-operator continuation at depth zero (Nim's
> rule) was considered and deferred: it enters only if the measurement
> baseline shows models actually produce that break shape. (Panel 007,
> predictions on record.)

**Panel 007** (`docs/panel/007-terminator-enders.md`, 91 lines, read in full
for this brief) was put an Amendment B whose depth-zero clause read *"At
bracket depth 0, a line that emitted no terminator continues onto the next
line"*. The compiler-engineer rejected it as unsound (four block headers end
in non-enders and would have their body's indent suppressed), and the author
**RATIFIED** on 2026-08-03 the resolution quoted above: continuation inside
brackets only; at depth zero, parenthesise. The panel's item 3 defers Nim's
rule to a sitting 007-bis *"with the explicit continuator set and the spec
sentence as one package"*; `ls docs/panel | grep -i 007` returns only
`007-terminator-enders.md`, so that sitting never sat.

**What the spec says.** `spec/heroes-spec.md` lines 5 to 17 (`sed -n 5,17p`),
§ 0: *"`NEWLINE`, `INDENT` and `DEDENT` come from the indentation. NEWLINE ends
a statement, and a statement whose last part is a block ends with that block
instead. Inside `(` `[` `{` a NEWLINE never ends a statement. A line there
keeps its NEWLINE when it ends with a literal, `?`, `???`, a closing bracket,
or a name or keyword other than `function` and `fail`; that NEWLINE may stand
only before a closing bracket or a `,`, or where a production writes it, and
any other line goes on below, at any column, so a long expression breaks after
an operator."* The last-token rule is stated for brackets only; what a line
OUTSIDE brackets does when it ends with an operator the spec does not say in
so many words. Whether a reader infers that it continues, or that it ends, is
the llm-ergonomist's question and is not answered here.

## What the compiler does today, measured

Seventeen programs, each `function main()` with one statement broken across
two lines. Run with the trunk's compiler: `heroes check` for the exit, `heroes
run` for the output where it compiles, `heroes fmt` for the formatter's answer.
The files are reproduced in full in the appendix below so that nobody needs the
coordinator's scratchpad.

| # | the break, the second line at the SAME margin unless said | check | run prints | fmt |
|---|---|---|---|---|
| s01 | `y = a +` / `1` | 0 | 6 | exit 0, `y = a + 1` |
| s02 | `y = xs.` / `len()` | 0 | 2 | exit 0, **`y = xs.len(` / `)`** |
| s03 | `y = a +` / `1` one level DEEPER | 1, `expected_expression` *found an indented block* | | |
| s04 | `y = xs.` / `len()` one level DEEPER | 1, `expected_field_name` *found an indented block* | | |
| s05 | `y = a` / `+ 1` (the operator leads the second line) | 1, `expected_expression` *found `+`* | | |
| s06 | `if a &&` / `b`, then the body one level deeper | 0 | 2 (the condition read as `a && b`) | exit 0, `if a && b` |
| s07 | `return a +` / `1` | 0 | 6 | |
| s08 | `y =` / `a` | 0 | 5 | |
| s09 | `function f(a: i64) ->` / `i64` at column 0 | 0 | 5 | exit 0, one line |
| s10 | `x @ x +` / `1` | 0 | 6 | |
| s11 | `y = a ==` / `5` | 0 | true | |
| s12 | a `constant`'s value `1 +` / `2` | 0 | 3 | exit 0, `1 + 2` |
| s13 | a `match` arm `.dot => a +` / `1` at the arm's margin | 0 | 6 | **exit 2, *`fmt` is not a fixpoint on its own output* / *this is a compiler bug*** (defect 118) |
| s14 | `y:` / `i64 = 5` | 0 | 5 | |
| s15 | `while i <` / `3`, then the body deeper | 0 | 3 | |
| s16 | `y = a +` inside an `if` body, `1` one level SHALLOWER | 1, `expected_expression` *found the end of the block* | | |
| s17 | a constant's `f() +` / `1` | 1, `constant_body` (a call in a constant; the break itself accepted) | | |

**So the compiler implements the clause panel 007 rejected, at the same margin
only.** A line that plants no terminator runs on into the next line when that
line starts at the same column; a deeper line opens a block and a shallower one
closes one, and both are then refused by the parser with a message about a
block rather than about the line end. The form the compiler admits is the one
where the second line LOOKS like a new statement; the form a reader of Python's
backslash-free continuations or of Nim would reach for, the deeper line, is the
one refused.

## Where the compiler does it

- `selfhost/layout.hero` (205 lines, `wc -l`): `is_line_ender` at line 40, the
  61-kind match that decides which tokens end a line; `line_start` at line 61,
  which returns before any indent logic when `l.open_brackets.len() > 0` (line
  82) and otherwise emits `indent` for one level deeper, `dedent`s for
  shallower, and **nothing for the same margin**; `maybe_terminator` at line
  131, which plants a `terminator` only after a line ender.
- `selfhost/scan.hero:57-62` (`sed -n 57,62p`): at `\n` the scanner calls
  `maybe_terminator` and then resets `l.last_significant` to a failure, so the
  next line's `line_start` cannot see how the previous line ended. A same-margin
  check therefore has to read the last token pushed (`l.tokens`), which is what
  the instrument below does.
- `selfhost/lexer.hero:135` and `:145`: the per-line loop and the end-of-file
  terminator.
- The parser already reasons about a missing terminator in one recovery path:
  `tests/golden/check/fixedbugs-use-refusal-eats-the-next-line.hero` (lines 11
  to 30, read) records that `use shapes/` ends in punctuation, plants no
  terminator, and a recovery that scanned to the next terminator ate the
  declaration below; its repair compares line numbers.

## How often the shape occurs, measured

An instrumented copy of the trunk's compiler (the coordinator's; `line_start`
pushes a diagnostic when a depth-zero line starts at the same margin and the
last token pushed is not a `terminator`, an `indent`, a `dedent` or a
comment, skipping trailing comments; built from `selfhost/` and checked to fire
on s01, s06, s09 and s12 before it was trusted, a first version having fired on
nothing because of the reset above) was run as `heroes lex` over every `.hero`
file of the tree outside `archive/`, `build/` and `site/node_modules/`: **1164
files** (`find . -name "*.hero" ... | wc -l`).

**21 hits in 10 files, every one under `tests/golden/`** (`wc -l` and `cut |
sort | uniq -c` of the hits). **Zero** in `selfhost/` (272 files), the
examples, `tests/harness/` (32) and the programs under `docs/panel/` (66).

- **16 of the 21** follow a line whose last token is the lexer's own `error`
  token, in goldens that are refused already: the malformed literals `0X10`,
  `0b12`, `0o8`, `0x`, `1__0`, `0700`, `00`, `0_7`
  (`check/literal-bases.hero` 5, `check/leading-zero.hero` 3,
  `check/base-prefix-fix.hero` 1), an unterminated string or hole and a
  two-character char literal (`check/unterminated.hero` 2,
  `check/unterminated-hole.hero` 1), and an invisible or unexpected character
  (`surface-fixtures/json102/unseen.hero` 3, `control.hero` 1). An `error`
  token is not a line ender (`is_line_ender`'s last arm), so a malformed token
  at a line's end glues the next line to its statement; whether that produces
  a second, cascading diagnostic in any of those goldens is **not measured
  here** and is a question for the compiler-engineer.
- **4 of the 21** follow a `use` whose path ends in punctuation, `use lex/..`
  and `use shapes/`, refused already (`module_path_climbs`,
  `module_path_wants_a_part`), two each in `check/use-has-a-path.hero` and
  `check/fixedbugs-use-refusal-eats-the-next-line.hero`; the line glued on is
  the next declaration, across a blank line, which is the defect that golden
  is named after and whose repair compares line numbers.
- **1 of the 21 is the shape on purpose**: `tests/golden/surface-fixtures/
  comments107/margin.hero:13-14`, `c = xs.  # at the statement's own margin` /
  `len()`, the fixture lane B wrote for defect 107, the one that found 116.

All 21 were printed with the line above them and read before this
classification was written.

## The price instruments

- The spec: `heroes measure spec/heroes-spec.md` read **8999 real**
  (claude-opus-5, pinned 2026-09-27), 6672 claude-legacy, 6794 cl100k_base;
  headroom 1241 against the 10240 ceiling, of which the FFI floor mortgages 60.
- `.env` holds the key `heroes measure --refresh` needs; a seat that prices a
  sentence sources it from the trunk (`. /Users/joseph/Temp/heroes/heroes-lang/.env`)
  and never prints it.

## What the sitting decides

1. **Refuse or admit.** Either the compiler is brought to the ratified rule
   (a depth-zero line that ends where no token can end it is refused), or the
   deferred Nim rule is admitted with its continuator set, which is the
   package panel 007 said 007-bis would need. Or a route this brief does not
   name.
2. **If refused: where and how.** In the lexer (plant a terminator at every
   depth-zero line end, or report there) or in the parser; which diagnostic,
   with which message and which `Fix`, `certain` or `guess`
   (`.claude/rules/diagnostics-and-goldens.md`: *a `certain` fix repairs the
   defect the diagnostic names*); what happens to the deeper and shallower
   shapes (s03, s04, s16), which are refused today with a message about a
   block.
3. **The shapes beside it**: the `error` token at a line's end (19 hits), the
   block headers panel 007 named, a comment between the two lines, CRLF, a
   line ending in `,` or `=>` at depth zero, a `test` or `extern` header.
4. **The spec sentence**, if any, priced on the reader's tokeniser.
5. **Defect 118** under each route.

## Appendix: the seventeen programs, verbatim

```
== s01_op_same.hero
function main()
    a: i64 = 5
    y = a +
    1
    print(y)
== s02_dot_same.hero
function main()
    xs: [i64] = [1, 2]
    y = xs.
    len()
    print(y)
== s03_op_deeper.hero
function main()
    a: i64 = 5
    y = a +
        1
    print(y)
== s04_dot_deeper.hero
function main()
    xs: [i64] = [1, 2]
    y = xs.
        len()
    print(y)
== s05_before_op.hero
function main()
    a: i64 = 5
    y = a
    + 1
    print(y)
== s06_if_and.hero
function main()
    a: bool = true
    b: bool = false
    if a &&
    b
        print(1)
    print(2)
== s07_return.hero
function f(a: i64) -> i64
    return a +
    1

function main()
    print(f(a: 5))
== s08_after_eq.hero
function main()
    a: i64 = 5
    y =
    a
    print(y)
== s09_arrow.hero
function f(a: i64) ->
i64
    return a

function main()
    print(f(a: 5))
== s10_assign_at.hero
function main()
    x: i64 @ 5
    x @ x +
    1
    print(x)
== s11_eqeq.hero
function main()
    a: i64 = 5
    y = a ==
    5
    print(y)
== s12_constant.hero
constant C: i64
    1 +
    2

function main()
    print(C)
== s13_arm.hero
variant S
    dot
    sq

function main()
    a: i64 = 5
    s: S = .dot
    y = match s
        .dot => a +
        1
        .sq => 0
    print(y)
== s14_colon_ty.hero
function main()
    y:
    i64 = 5
    print(y)
== s15_while.hero
function main()
    i: i64 @ 0
    while i <
    3
        i @ i + 1
    print(i)
== s16_op_dedent.hero
function main()
    a: i64 = 5
    if true
        y = a +
    1
        print(y)
    print(0)
== s17_amp_col0.hero
function f() -> i64
    return 1

constant D: i64
    f() +
    1

function main()
    print(D)
```
