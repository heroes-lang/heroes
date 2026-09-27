# Panel 180, compiler-engineer: where a line inside brackets may break

Every number below was run in this seat's directory on 2026-09-27, on the trunk
frozen at `29ed5601` (`git archive 29ed5601`), with compilers built there and
nowhere else. The trunk copy is `tree/` (compiler from the seed, 3.98 s). Each
prototype is its own fresh copy, `tree-b/`, `tree-c/`, `tree-ii/`, `tree-i/`,
`tree-bii/`, `tree-e/`, its compiler built from its own edited `selfhost/` with
`heroes build selfhost/main.hero` (98 to 107 s each). The net was built once
(`heroes build tests/harness/main.hero -o harness-bin`, 16.8 s) and run from
each copy as `./harness-bin <that copy's compiler> <suite>`, one heroes child
at a time. The scripts and every probe file are under `map/`.

**Baseline, the trunk copy:** own tests 756 passed (95.2 s); `grammar` 9/0,
`surface` 167/0, `canonical` 2/0, `annotations` 191/0, `check` 150/0.

## verdict

**veto** on route (c). **object** to (a) alone, to (i), and to (e). **approve**
(b) together with the refusal (ii): that pair is the resolution this seat puts
forward, measured green on every gate run, `layout` and `corpus` included.

## section

design.md §1.7 and Part 5 for the cost: **nothing here is core and nothing is
even sugar.** Every route is parser-only; none reaches the checker, the
lowering, the descriptors, the ownership pass or the emitter, and none touches
the lexer. The veto does NOT stand on §1.7, because (c) adds no core
construct. It stands on design.md's thesis ("The measurable thesis", line 87:
*every plausible LLM mistake is a compile error*) and §1.4 (line 223: *one
wrong token produces a program that is different but valid*), because (c)
turns a compile error today into a wrong answer at exit 0 (measured below).
design.md does not cover a break BEFORE a token inside brackets at all: §4.15
(line 1939) covers indentation inside brackets and defers trailing-operator
continuation at depth 0 (line 1946), and §4.9 (line 1450) covers the newline
separator. Where this report rules on the gap it says so.

## implementation_cost

Code lines are `layout`'s unit (outside tests, not blank), measured with a
port of `tests/harness/suite_layout.hero:604` `code_lines` (`map/codelines.py`).
Trunk: `selfhost/grammar_expr.hero` 1622 lines, 1051 code (its DECIDED
ceiling 1085, `suite_layout.hero:439`); `selfhost/parse/type.hero` 535, 280;
`selfhost/cursor.hero` 566, 279; `selfhost/parse/members.hero` 575, 299.

| route | where it lands | lines | code lines after | gates run |
|---|---|---|---|---|
| (a) spec states today's rule | nowhere | 0 | unchanged | baseline green |
| (b) uniform | `grammar_expr.hero` (index, `separator`), `parse/type.hero` (5 closers) | +9 -1 | 1053, 285 | own 756/0; grammar, check, surface, canonical, annotations, layout all 0 failed |
| (c) spec sentence made true | `cursor.hero` (a crossing mode in `bump`, `peek`), `grammar_expr.hero`, `parse/members.hero`, `parse/type.hero` | +55 -5 | cursor **301**, members **303**, grammar_expr 1065, type 288 | own 756/0; the five suites 0 failed; **layout 2/1** |
| (ii) refuse `- ` opening an element line | `grammar_expr.hero` (`separator`, one function) | +29 | 1078 | own 756/0; the five, corpus 55/0, layout 3/0 |
| (i) refuse `-` `(` `[` opening an element line | `grammar_expr.hero` | +20 | 1070 | own 756/0; **surface 165/2, canonical 2/1, corpus 54/1** |
| **(b)+(ii)**, recommended | `grammar_expr.hero`, `parse/type.hero` | +38 -1 | 1080, 285 | own 756/0; grammar 9/0, check 150/0, surface 167/0, canonical 2/0, annotations 191/0, corpus 55/0, layout 3/0 |
| (e)+(ii), a route the brief does not name | as (c), plus a token predicate | +121 -5 | cursor **334**, grammar_expr **1091** (over 1085), members **303** | own 756/0; the five and corpus 0 failed; **layout 2/1** |

(ii)'s function uses only `cursor`, `diag` and `token`, so it can leave the
knot: then `grammar_expr.hero` grows by 3 lines, not 29 (unprototyped). The
formatter (`selfhost/print/`, 19 files, 5567 lines) needs **no change** for
(b), (ii) or (b)+(ii), measured in § 4; it owes unpriced work for (c) and (e).
One person: +37 lines is nothing against a 72,576-line compiler (261 files,
`wc -l`); (e)'s +116 plus a module split is still one person's, but it buys
the least per line.

## needed_for_self_hosting

**no.** The trunk reaches its fixpoint with none of this. (ii) enters on the
thesis: it removes a measured exit-0 wrong answer. (b) enters on a measured
simplification of the rule the spec must state (three exception classes
become none); whether that pays in tokens is the spec-warden's number.

## argument

Nothing here is core. The compiler's irregularity is accidental: no
production needs an index's `]` or a type's closer to refuse a line end, and 7
net code lines in two files flip exactly the 14 refused shapes of those
exceptions with every gate green. Making the spec's sentence true is not cheap
or safe: route (c) adds 50 lines, pushes two modules past CLAUDE.md §11's 300,
leaves 11 comment shapes fmt refuses, and makes `f(a` / `-1)` print 4 at exit
0 where today it is an error. The silent class is in the literals:
`[base * qty` / `- discount]` runs with two elements. Refusing the spaced
spelling costs 29 lines, changes 0 of 1447 existing element lines, and keeps
fmt's output legal.

## prediction

If (b) and (ii) land at M-agreed-retention as prototyped, then at the close
`layout` reads 3 passed 0 failed with `selfhost/grammar_expr.hero` at 1080
code lines or fewer (1056 or fewer if `spaced_minus` leaves the knot) and
`selfhost/parse/type.hero` at 285 (each within 2), and `git diff --stat
29ed5601..<close>` shows no byte of any EXISTING file under `examples/`,
`tests/harness/` or `tests/golden/` changed to accommodate it, only new
golden files. Falsified by a red `layout`, by `grammar_expr.hero` above 1085,
or by an existing program edited to keep compiling. If (c) lands instead,
`layout` is red on `cursor.hero` (301) and `parse/members.hero` (303) until a
module is split.

## condition

- **The veto on (c) lifts** for a (c) that keeps a comma list's missing comma
  loud. (e) is one, measured: `f(a` / `-1)` stays `expected_args_close`. The
  question is then cost, not soundness.
- **I move from (b)+(ii) to (e)+(ii)** if the ergonomist measures that models
  break BEFORE an operator inside brackets (PEP 8's and Black's shape) often
  enough to cost first-try compiles: (e) accepts it in a group, an index and a
  type, and before any operator that cannot begin an element in a list. Its
  price is measured above (+116 net, `layout` red on three files) plus 12 new
  `fmt` refusals whose repair nobody has priced.
- **I move from (ii) to a stronger refusal** if the ergonomist shows that
  `[a` / `-b]`, the unspaced spelling, is a continuation models produce: (ii)
  leaves it silent (measured), and the stronger rules cost fmt work or break
  the tree (§ 5.3).

---

## 1. The map

**Where the list of contexts came from.** The parser's opener-consuming sites,
`grep -n '\.lparen\|\.lbracket\|\.lbrace'` over `selfhost/grammar_expr.hero`
and `selfhost/parse/*.hero`, outside tests: 13 sites. Group (`grammar_expr.hero:373`),
call (`:288`), method call (`:331`), variant case (`:583`), all three through
`call_args` (`:594`); index (`:293`); array literal (`:376`); map literal
(`:379`); parameter list (`parse/members.hero:69`, the same for a function and
an `extern` member); `[T]` (`parse/type.hero:73`), `{K: V}` (`:76`), `()` and
`(function ...)` (`:79`), a function type's parameters (`:206`), `T[n]`
(`:39`). Cross-checked against every spec production holding `(` `[` `{`:
`Type`, `Prefix`, `TypeArgs`, `Params`, `Place`, `Postfix`, `Primary`, `Args`,
`Member`/`CParam`, the same set. Two more that are not bracket tokens: the
hole of `f"..."` (`selfhost/lex_interp.hero`) and generics `<...>`.

**The instrument.** 117 shapes (`map/cases.py`), each a one-line control and
the same text with a break (a newline and 8 spaces), run with `heroes parse`
(`map/run.py`). 112 controls parse; the 5 that do not are refused on one line
too, by design (a trailing comma twice, a map one-entry-per-line control, and
`if` and `match` inside brackets). Full results: `map/trunk.tsv`.

**The trunk, 68 of 117 breaks accepted.** "After X" breaks after a token that
cannot end a line; "before X" breaks after one that can.

| context | accepted | refused |
|---|---|---|
| group | after `(`, before `)`, after an operator, after unary `-` `!`, after `.`, after `::` | before an operator (`+` `*` `&&` `\|`), before `.`, before a call's `(`, before an index's `[`, before `?`, before `::` |
| call, method call, record construction, variant case | after `(`, before `)`, after `,`, **before `,`**, after `:`, after `@`, after an operator | before a named argument's `:`, before an operator, before `.` |
| index, and a Place's index left of `@` | after `[`, after an operator | **before `]`**, before an operator |
| array literal | after `[`, before `]`, after `,`, one element per line, after an operator | **before `,`**, before `+`; **silently re-read** before `-` and before `.` |
| map literal | after `{`, before `}`, after `:`, after `,`, one entry per line | before `:`, **before `,`**, before `-` in a value |
| parameter list, function and `extern` member | after `(`, before `)`, after `,`, before `,`, after `:`, after `@`, after `\|` in a releaser set | before `:`, before a mark, **after a mark word** (`counted_by` / `n`), before `lent`, before `\|` |
| type brackets | after every opener, after `:`, after `->`, after `.`, after `function` | **before every closer** (`[T]`, `{K: V}`, `T[n]`, a function type's parameter `)`, its outer `)`), before `:`, **before a function type parameter's `,`**, before `->`, before `?`, before `geom.Point`'s `.` |
| hole of `f"..."` | nothing | every break, even inside a call in the hole: `unterminated_string` |
| `if`, `match` inside brackets | nothing, on one line or several | `missing_body`, `missing_match_arms`: no INDENT exists inside brackets |
| generics `<A, B>` | not a bracket | a break is structural: `indentation_jump` |

## 2. The rule the compiler implements

In the fewest words true of the whole map:

> Inside brackets a line may end after a token that cannot end a statement.
> After one that can (a name, a literal, a closer, `?`), only before the
> closer of a group, a call, a literal or a parameter list, before a call's or
> a parameter list's `,`, or between a literal's elements, which it separates.

Its exceptions, every one measured:

1. **an index's `]`**, so also a Place's left of `@`;
2. **every type's closer**, five kinds, and a function type's `,`: the brief's
   list had the index only. Types never call `skip_terminators`
   (`parse/type.hero`, 0 calls);
3. **a break before `,` in an array literal and a map literal**: `,` is then
   read as the next element's start (`expected_expression`);
4. **the hole of `f"..."` holds no line end at all**, brackets nested in it
   included (`lex_interp.hero` `line_ends`);
5. **`if` and `match` cannot stand in brackets**, on any number of lines;
6. **a mark word is a name**, so `counted_by` / `n` and `acquires` / `a` are
   refused although spec § 13 reads them as keywords;
7. **between a literal's elements a line that opens with `-`, `(`, `[` or `.`
   is a new element**, silently where the types agree (§ 5).

Under (b) the rule becomes *"Inside brackets a line may end after a token that
cannot end a statement; after one that can, only before a closer or a `,`, or
between a literal's elements, which it separates"*, and exceptions 1 to 3 are
gone. 4 and 5 are not about line ends in brackets (a string is one line, a
block needs an indent). The spec does not state 4 today (§ 1 says a string may
hold *any UTF-8 but a raw carriage return*; `grep` for *line* in § 2 finds no
single-line rule), nor 5: a question for the warden, not this seat.

## 3. The routes

**(a)** costs 0 lines and keeps exceptions 1 to 3, which exist for no reason a
production gives: no seat has named one, and the formatter already works
around exception 1 (lane g's F1 keeps the author's parentheses because *"the
parser skips a line's end before a group's `)` but not before an index's
`]`"*, commit `574711c3`). A spec that lists an accident preserves it.

**(b)** is 8 added code lines, 1 removed: `grammar_expr.hero` gets a
`skip_terminators` before the index's `]` and `separator` reads the line end
before the comma instead of after it; `parse/type.hero` gets one before `]`,
`}`, a function type's `)`, its parameters' `,` and `)`, and `T[n]`'s `]`.
The map moves **exactly** the 14 shapes of exceptions 1 to 3 (`map/b.tsv`)
and nothing else. For "one answer for `,`" the direction is forced by the
formatter: `fmt` prints a comma on its own line after a comment in a call and
a parameter list (`a  # c` / `,` / `b`, measured), and 9 lines of two
fixtures begin with `,` (`comments101/commas.hero`, `nextvalue.hero`), so
refusing `,` after a line end everywhere would make `fmt`'s own output
unparsable. Accepting it everywhere changes no existing line.

**(c)** as prototyped keeps the lexer and gives the cursor a crossing mode:
on in a group, a call, an index, a parameter list and a type; off in a
literal, where NEWLINE separates; restored before each closer is consumed, so
the line end after the closer is judged outside. It accepts 107 of 117. What
it breaks, measured:

- **a missing comma between arguments now merges them.** `print(f(a` /
  `-1))` with `f(n: i64)`: trunk `expected_args_close`, exit 1; (c) prints
  `4`, exit 0. The same for a leading `(`, `[`, `.`: a call has no NEWLINE
  separator, so every token that can start an argument can also continue the
  one before. This is the veto;
- `layout`: `cursor.hero` 301 and `parse/members.hero` 303 code lines, past
  300 (`members.hero` is at 299 today);
- the formatter: 11 comment shapes it newly accepts are refused by `fmt`'s
  own guard, at exit 2 with the file unchanged (§ 4);
- panel 007's compiler-engineer predicted *"parser has zero depth-conditional
  layout code"*; a crossing mode is exactly that.

It does **not** break §4.9's literals or `Sep = "," | NEWLINE` (the mode is
off there), nor lane g's parentheses (they stay legal), and it does not
reverse §4.15's deferral, which is about depth 0 and a TRAILING operator;
inside brackets a trailing operator already continues (no terminator follows
an operator). (c) is a different question the design never ruled on: the
brief's `grep` found nothing, and panel 007, read in full here, rules only on
indentation inside brackets and on depth 0.

**(e)**, a route none of the brief's names: cross a line end anywhere in a
bracket holding ONE expression or type (group, index, type), and in a list
(arguments, parameters, literals) only before a token no element can begin
with (`begins_expression`, an enumerated match over all 69 kinds, in the
style of `layout.is_line_ender`). It accepts Black's `(a` / `+ b)` and
`f(a` / `+ b)` and `[a` / `+ b]` as one value, keeps `f(a` / `-1)` refused,
and keeps a literal's elements separated. 102 of 117 accepted. Its cost is
above; with (ii) folded in it is the most permissive route measured here that
keeps a missing comma loud.

## 4. What `heroes fmt` does with each accepted shape

`map/fmtmap.py`: `fmt` on every accepted break, then `parse` on its output,
then `fmt` again.

| compiler | plain breaks | with `# c` before the break |
|---|---|---|
| trunk | 68, all format, reparse, idempotent | 68 accepted, 65 format, **3 refused** |
| (b) | 82, all | 82, 3 refused, the same 3 |
| (b)+(ii) | 81, all | 81, 3 refused, the same 3 |
| (c) | 107, all | 107, **14 refused** |
| (e)+(ii) | 102, all | 102, **15 refused** |

What it prints. Groups, parameter lists, types and indexes are joined back to
one line. A call, a method call, a record construction, a variant case and a
literal the author broke stay one element per line, even with two elements,
and a literal loses its commas (`[1, 2` / `]` prints `1` and `2` on their own
lines). Two oddities, legal and idempotent: `(xs.` / `len())` prints `xs.len(`
/ `)`, an empty argument list over two lines; and the silent readings are
printed as what the parser read, `[1` / `- 2]` as `1` / `-2`,
`[base * qty` / `- discount]` as `-discount`. So **`fmt` erases the one piece
of evidence (the space after `-`) that the author meant subtraction**, which
is why (ii) must refuse before `fmt` runs rather than rely on it.

## 5. The silent class inside NEWLINE-separated lists

### 5.1 Which tokens both continue and begin

From spec § 7: a line can continue with a binary operator or a `Postfix`
opener (`.`, `::`, `(`, `[`, `?`); an element can begin with a `Primary` or a
`Unary`. The intersection is exactly **`-`, `(`, `[`, `.`**. `!` and `~`
begin but cannot continue (no binary `!`, `~`), so `[a` / `!b]` has no second
reading: two elements is the only parse, not a misreading. Every other binary
operator cannot begin, so it is loud (`[a` / `+ b]`: `expected_expression`,
measured). NEWLINE-separated lists inside brackets are the array and the map
literal and nothing else (`Sep` appears in `Primary` only). At depth 0 the
same shape is loud (a statement `- b`: `discarded_value`; an indented one:
`unexpected_block`) or the only reading (`- 1 => 20` is a `match` arm).

In realistic programs, `heroes check` and `heroes run` on the trunk
(`map/silent2/`):

| line opens with | silent, exit 0 | loud |
|---|---|---|
| `-` | `[base * qty` / `- discount]` len 2; `[a` / `-b]` len 2 | in a map value, `expected_map_entry_colon` |
| `[` | `[ys` / `[i]]` len 2 where `ys: [i64]`. A matrix's rows open lines with `[` too, legitimately (6 in the tree, § 5.2) | in a map, `type_mismatch` |
| `(` | `[a` / `(b)]` len 2, but a call on an `i64` is ill-typed, so it is the only well-typed reading | `[double` / `(3)]`, `type_mismatch` |
| `.` | `[o` / `.minus]` len 2 on a variant, where a field of a variant is ill-typed | `[p` / `.x]`, `[ys` / `.len()]`, `type_mismatch` |

**Plausible AND silent is `-`**: PEP 8 and Black break before a binary
operator, and the types always agree for numbers. `[` is silent only for a
break before an index's `[`, which no style guide produces.

### 5.2 The census

`map/census.py`: `heroes lex --dump-tokens` on all 946 `.hero` files under
`selfhost/`, `tests/`, `examples/` (32 s; 17 are fixtures with lexer
diagnostics, their tokens still counted). A `[` is taken as an index exactly
when the postfix loop takes it (the token before it can end a line and is not
`return`); an element line is one whose line end at the literal's own depth
is followed by something other than the closer.

**1447 NEWLINE-separated element lines in 68 files.** By first token: string
679, name 666, `.` 57 (`selfhost/operators.hero` 34, `selfhost/scan.hero` 11,
`examples/interpreter/syn/tree.hero` 12, all variant-case lists), integer 38,
`[` 6 (matrix rows: `examples/board/main.hero:39-42`,
`tests/harness/suite_cache.hero:392-393`), `(` 1
(`tests/golden/surface-fixtures/comments101/dots.hero:37`, a fixture pinning
the two-element reading), **`-` 0**. Cross-checked by `grep -E '^\s+-\s*[0-9A-Za-z_(]'`:
two hits, both constant bodies at depth 0 (`selfhost/print/owners.hero:41,45`).

### 5.3 The refusal routes, prototyped

**(i) refuse `-`, `(` or `[` opening an element line after a bare line end**
(`tree-i/`, +20 lines). Diagnostic `ambiguous_element_start`, landing text:
*"this line of the list begins with a token that could continue the line
above, and a line end here starts a new element; end the line above with `,`
for a new element, or with the operator to continue it"*, one fix, `guess`,
*"end the line above with `,`"*. It breaks the gate exactly where the census
said: `surface` 165/2 (`dots.hero`; `heroes mutate examples` refuses
`examples/board`), `canonical` 2/1 (`fmt` exit 1 on `examples/board/main.hero`
and `tests/harness/suite_cache.hero`), `corpus` 54/1. It refuses the
ergonomist's `deltas = [` / `1` / `-1` / `]` and `{1: 10` / `-1: 20}`, and
`fmt`, which prints lists without commas, would have to learn to add them
(unprototyped). Narrowed to `-` alone it would break nothing in the tree but
still refuse the `-1` column and still need `fmt` to print a comma.

**(ii) refuse `- `, the binary spelling, opening an element line after a bare
line end** (`tree-ii/`, `tree-bii/`, +29 lines, no lexer change: it compares
the `-` token's end with the next token's start). Diagnostic
`spaced_minus_element`, landing text: *"this line begins with `- `, but a
line end in a list starts a new element, so it would be a negative element;
to subtract, end the line above with the `-`, and for a negative element
write the `-` against its value"*. Two fixes, both `guess`, because the author
meant one of two things and nothing in the text says which beyond the space:
*"subtract: join it to the line above"* and *"a negative element: `-` against
its value"*. The precedent for two guesses is the leading-zero literal,
`selfhost/number.hero:106-109`. At landing the join fix must keep a comment
trailing the line above (the prototype's span would delete it). Measured:
`[base * qty` / `- discount]` and `{1: 10` / `- 1: 20}` refused; `-1`
columns, `-b`, `(b)`, `[0]`, `.minus` and `!b` unchanged. The 117-shape map
moves exactly one shape from accepted to refused, `[1` / `- 2]`; `{"a": 1` /
`- 2}`, refused already, gains this diagnostic first. Every gate green; `fixes`
not run. **`fmt`'s output stays legal with no change**: `fmt` spells a unary
minus against its value in every shape measured here, and every fixture and
corpus file the gates format still formats. What it leaves: `[a` / `-b]`
unspaced stays two elements, silent, and so does `[ys` / `[i]]`.

**(iii) found**:

- **(b)+(ii) together** (`tree-bii/`): the recommended pair, +38 -1, every
  gate green including `layout` (§ implementation_cost).
- **(e)+(ii)**: also refuses the missing-comma merge; makes `[a` / `+ b]` one
  element; costs `layout` on three files and 12 new `fmt` refusals.
- **A negative number only** (refuse `- b`, `-b` and `- 1`, keep `-1`):
  closes (ii)'s residual, but `fmt` prints a negated name as `-b` on its own
  line, so its output would stop parsing until `fmt` adds a comma there.
  Reasoned from the measured `fmt` output, not prototyped.
- **Comma-only lists, reversing §4.9**: the only rule that closes all four
  tokens at once. Its price is the census, 1447 element lines in 68 files
  rewritten, plus `fmt`'s list printer, spec § 7's `Sep` and § 10's sentence,
  and design.md §4.9 line 1450. Not prototyped.

## 6. Found beside the question

**`heroes fmt` refuses, at exit 2, programs the trunk's parser accepts**, and
none of this sitting's routes changes it (the same three under (b) and
(b)+(ii)):

```
function main()
    y = (xs.  # c
        len())
```

`error: fmt produced source that does not parse ... expected_field_name`. The
same for `(p.  # c` / `x)`, `(Point::  # c` / `x)`, `(a + (b.  # c` / `c))`,
and `function g(@out: Db acquires a |  # c` / `b)` in an `extern` group
(`expected_releaser`). `f(xs.  # c` / `len())`, `[xs.  # c` / `len()]` and a
qualified type after its `.` all format. The file is left untouched, so this
is not corruption, but under §4.15 (*"exactly one correct way to write any
program"*) these programs have no canonical form. Defect 101's record
(`docs/records/done/2026-09-27-1154-defect-101-...`) reports 0 refused across
its generators; these shapes were not in them. This seat did not file it: it
works only in its own directory. Reproducers in `map/fmtdefect/`.

## 7. Unrun

- the full net on any prototype, and any platform but this Mac;
- the compiler's speed: not timed, because other seats share the machine and
  CLAUDE.md § Verification forbids a clock while anything else runs. (b) adds
  one `skip_terminators` per index and closer; (c) and (e) add a check to
  every `bump` inside brackets;
- the `fixes` suite on (ii)'s new diagnostic, and the golden and source
  annotation it would owe (CLAUDE.md § 9);
- the formatter repair (c) and (e) would owe for their 11 and 12 new refusals;
- `fmt` adding commas for (i) and for the negative-number-only rule;
- comma-only lists beyond the census;
- the spec's token price of any wording: the coordinator's `measure --refresh`;
- whether models produce break-before-operator inside brackets, or `-b`
  unspaced as a continuation: the ergonomist's measurement, and the one fact
  that would move this verdict.
