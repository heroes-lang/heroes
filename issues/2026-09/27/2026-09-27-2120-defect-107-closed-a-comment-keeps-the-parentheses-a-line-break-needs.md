---
kind: defect
area: print
milestone: none
filed: 2026-09-27
commit: b02855a922d08319f4be4097d49505d24b6c3eb8
github: none
---

# Defect 107 closed: a comment the author's parentheses hold is printed inside parentheses, wherever no other bracket holds it

2026-09-27, M-agreed-retention step 23, in lane B (`ec483127`), merged `335b6895`, with
defect 108. Found by panel 180's compiler-engineer, widened by its critic.

- [x] **107 — `heroes fmt` refuses a program with a comment after `.` or `::` where the only bracket is the author's parentheses, because it drops them and the line break lands at depth 0** | `y = (xs.  # c` / `len())` parses, and `fmt` exits 2 with *produced source that does not parse* (`expected_field_name`), the file untouched; the same for `(p.  # c` / `x)`, `(Point::  # c` / `x)`, `(a + (b.  # c` / `c))`, and for a variant case's leading `.`, a binary's operand, a unary's operand, a `return` value and an `if` condition (`indentation_jump`); inside any other enclosing bracket the same comment formats | `selfhost/print/breaks.hero` (`restored`) · `selfhost/print/around.hero` · `selfhost/print/groups.hero` · **closed 2026-09-27**

    **Origin:** panel 180's compiler-engineer, 2026-09-27, reproducers in
    `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-180/compiler-engineer/map/fmtdefect/`,
    widened by the critic (five more members), re-run by the coordinator on
    the trunk's compiler at `29ed5601`: `parse` exit 0 and `fmt` exit 2 on
    ten shapes (`expected_field_name` on seven, `expected_field_name_after_colons`,
    `expected_case_name`, `indentation_jump`), `fmt` exit 0 on the same
    comment inside a call, an array, an index and a type. The neighbour of lane g's fifth round (defect 101's record, F1 to
    F3), which kept the author's parentheses around a comment inside an
    index and did not reach these.

    **Why it is a defect.** A correct program has no canonical form (design.md
    §4.15, *exactly one correct way to write any program*); the guard stops
    the corruption, so it is a refusal and not a silent move.

## What it was, seen from beside it

The filing names the author's parentheses and the tokens `.` and `::`. The
shapes beside it say what it is: **a line `fmt` breaks for a comment,
printed where no bracket of the page is open**. Inside any bracket the
margin is no structure; at a statement's depth it is, so the line after
the break read as an indented block (`expected_field_name`) or as a block
one level too deep (`indentation_jump`). The bracket-holding printers were
already safe: a value's own call, array or map bracket opened, a group
`fmt` opens for a comment after an operator, the author's pair with a
comment just inside it (lane g's fifth round). What was not: a break in the
one-line text `print/breaks.hero`'s `lead` prints, outside every bracket of
that text, after a `.`, a `::`, a variant case's `.`, and the break between
a receiver and its `.name(` when the arguments are opened too. Those breaks
are safe inside a bracket the page holds open (a call's argument, an array's
element), and that is the one fact the printer did not carry.

## The repair

- **The printer carries it** (`print/around.hero`, `Place.enclosed`): a
  value is enclosed where a bracket of the page is open around it, an
  opened bracket's element, a map's key, the inside of the author's pair
  kept for a comment, a part inside the parentheses `render` prints; a
  statement's value, a condition, a `return`, the parts of a value printed
  beside it on its line (a binary's operands, a unary's, a base, a
  receiver) are not.
- **The rule** (`print/bare.hero`, new, asked by `brackets.spread`): a
  value no bracket of the page holds, whose printing would break a line
  for a comment where no bracket it prints is open, is printed as a group,
  inside the author's innermost pair of parentheses around it, or inside
  `fmt`'s own where the author wrote none. The pairs outside the innermost
  bind nothing and are dropped, as `fmt` drops every such pair; no comment
  stands between them, since one there opens its own pair first
  (`around.just_inside`). A part printed by a spread of its own asks the
  question there, so the pair kept is the nearest the comment:
  `(a + (b.  # c` over `c))` keeps the pair around `b.c`, and
  `(xs.  # c` over `len() + 1)` the pair around the sum, since the operand
  has none.
- **The question mirrors `spread`'s dispatch**, branch for branch, and says
  so in its header, with the two ways a disagreement shows, both loud: a
  group `fmt` did not need, which the tree-identity run shows, or the break
  at the statement's depth, which the output guard refuses. Where the
  one-line text breaks, it asks `breaks.loose`, which reads the same plan
  `lead` prints from (`breaks.plan`, factored out of `pieces`, with the
  author's parentheses `restored` puts back) and the depth of the text's
  own brackets after each break.
- **`brackets.hero` stays under §11's 300** (295 code lines, 291 before):
  the kept pair's group, built the same way at two sites, is
  `elements.kept`.

## What it changes in the canonical form

Nothing that formatted before (the identity run below): the rule fires
only where the trunk refused. What it prints where it fires:
`y = (` / `xs.  # c` / `len()` / `)`, the group one element opened, the
break inside it four columns further in, as `f(` / `xs.  # c` / `len()` /
`)` already was; in a block's head the group's lines are deepened as every
head printed across lines is (`margins.deepened`).

## The neighbours, and where the enumeration came from

The contexts are every place `print/fmt.hero` prints a value from with no
bracket open, read from its callers of `brackets.broken` and
`brackets.led` (`plain_valued`, `block_head`, `prefix`): a binding's,
a declaration's and a mutation's value, `return`, `assert`, a discarded
value, an arm's inline value, `if`, `else if`, `while`, `for`, `match`, a
condition wrapped whole, the part before a control form; and inside a
value, the parts `brackets.part` spreads: a binary's operands, a unary's,
a field's, an index's and a `?`'s base, a receiver. Crossed with sixteen
breaks (after the dot of a method, a field, a field's name, a case with and
without its fields, a seam with arguments after it, two dots, a chain, a
call's field, a binary's either operand, a unary's, a pair inside a sum,
two and three pairs, a `?`'s base) and three comment styles (trailing, on
a line of its own, both), with the enclosed contexts beside them (an
argument, an array's element, a call), a generated set of 849 programs
(`gen107.py` in the lane's scratch), each judged by the lane's `fmt`, the
trunk's, and the second skeptic seat's reader (`rd.py`: owner, neighbours,
fixpoint, tree):

| context | programs | trunk `fmt` | lane `fmt` |
|---|---|---|---|
| a binding's, a declaration's and a mutation's value, a discarded value | 48 each | 2 | 0, each a fixpoint, no move |
| an arm's inline value, the part before a control form | 39 each | 2 | 0 |
| `if`, `else if`, `while`, a condition wrapped whole | 39 each | 2 | 0 |
| a binary's left or right operand, a unary's operand | 39 each | 30 at 2, 9 at 0 | 0; the 9 byte for byte the trunk's |
| a value a member is read from | 39 | 27 at 2, 12 at 0 | 0; the 12 the trunk's |
| a case matched after | 6 | 2 | 0 |
| an argument, an array's element, a call's, an index's, a `range` bound, a `print`'s concatenation, an `assert` with a comment in each operand | 246 | 0 | 0, byte for byte the trunk's |
| a line continued at the statement's own margin, no parentheses, the comment trailing the break | 5 | 2 | 0, inside `fmt`'s own parentheses |
| the same, the comment on a line of its own, or both | 10 | 2 | 2, *would move the comment* (reported below) |

849 programs, 554 the trunk refused and the lane formats, 285 the trunk
formatted and the lane prints the same bytes, 10 refused by both; the
reader found no problem in any of the 839 the lane formats.

## The pins

`tests/golden/surface-fixtures/comments107/` (README and four files:
`statements.hero`, `heads.hero`, `operands.hero`, `nesting.hero`), each a
program `check` accepts, each refused by the trunk's `fmt` at exit 2, each
formatted by the lane's with the same output when run, and four `fmt` rows
of `tests/harness/suite_surface.hero`.

## Found beside it, not repaired here

**A statement continued at its own margin.** design.md §4.15 defers
*trailing-operator continuation at depth zero* (panel 007), and the lexer
accepts it where the next line keeps the statement's indentation: a line
ending in `.`, `+` or `|` plants no terminator, and the same margin plants
no indent, so `y = a +` over `1` at one column is `y = a + 1`, and
`y = xs.  # c` over `len()` parses. Searched `docs/work/DEFECTS.md`,
`docs/records/` and `docs/panel/` for *same indentation*, *depth zero* and
*Nim's rule*: panel 007's deferral and panel 180's reports, which quote it,
and nothing that says the lexer accepts it. `fmt` joins such a line where
no comment is in it; with a comment trailing the break it prints its own
parentheses now (`nesting.hero`); with one on a line of its own it refuses
at exit 2, since the owner rule (`print/owners.hero`) reads the continued
line as a new logical line and the comment as a block's; and a type, a
place or a result's releaser set continued that way, which no parentheses
can hold, is refused at exit 2 as on the trunk. A question for the
coordinator: whether the lexer should refuse the break, as §4.15 says.

## The measurements

Lane g's generators (`judge3.py` with `rd.py` and `rd2.py`, copied into the
lane's scratch), run by the coordinator on the lane's final compiler under the
memory guard with two workers, 2026-09-27: the continuation-column family
over the 47 fixtures of `comments101/`, `comments107/` and `releaser108/`,
2112 variants, 1557 at exit 0, 555 that do not parse, 0 refused; the
bracket-break family over the 32 new fixtures and the sitting's reproducers,
2547 variants, 1749 at exit 0, 798 that do not parse, 0 refused; the 32
files themselves, 30 at exit 0 and 2 that do not parse. The readers' flags
are the documented classes only: 72 of the owner model's, each the
ascending-run rule (panel 179's ruling c); 25 of the alignment's, each a
comment kept on the same code while parentheses only one file has were
dropped or reduced around it (ruling b), read by hand on four variants and
classified by the tokens crossed for all 25. One printing seen on the way
is the trunk's as well: a comment after `+` in a call's argument is printed
inside a pair of `fmt`'s own parentheses.

## The gate

In the lane, on the final source (the coordinator's, after the lane's
agent stopped at the account's weekly limit): the seed regenerated from it,
the compiler rebuilt, emitted again and compared, `cmp` silent (sha256
`5cda930d88f790f9`); the compiler's 764 tests and the net's own 171; layout
4, surface 177, canonical 2, annotations 191, fixes 25, check 150, records
24 and lines 207, each 0 failed. The agent's own gate, on the source before
its last edit (a line of `print/headline.hero` `layout/concat` flagged,
since spelled in pieces), read run 206, emission 628, determinism 236,
corpus 55 and warnings 267, 0 failed, and the full net 2576 and 0.

On the trunk after the merge, `335b6895`: the one real conflict was the
surface suite's count, lane A's 102 -> 121 and this lane's 102 -> 112 on
the same base, joined as 131 with both histories; the seed emitted again by
the trunk's compiler, fixpoint by `cmp` (`d2751877386f8382`); the
compiler's 785 tests, the net's own 172; the full net **2752 passed and 0
failed** (1464.51 s real, 833.51 user). The Windows box on the merged
tree, sent twice truncated and a third time whole by its checksum: the compiler's 785 tests, surface 306, canonical 2,
annotations 199, check 153, fixes 26, layout 4, lines 205, run 204, grammar
9 and spec 20, each 0 failed.
