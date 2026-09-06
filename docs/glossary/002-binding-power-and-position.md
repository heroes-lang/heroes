# 002 — Binding power and position: the half of the precedence table nobody writes down

**Origin.** Debrief of 2026-08-04, on M-syntax-tree's formatter. The question was why
`heroes fmt` turns `2 + (3 * 4)` into `2 + 3 * 4` but leaves `a - (b - c)`
exactly as written. The rule "parentheses survive when the inner operator
binds looser than the outer one" explains the first case and gets the second
one *wrong* — and getting it wrong deletes two characters and changes what
the program computes.

## The two facts an operator carries

Every table of the kind design.md §4.14 publishes states one fact per
operator and leaves a second one implicit.

**Binding power — how tight.** Published, and it is the whole content of the
table: `* / %` above `+ -` above comparisons above `&&` above `||`. This is
what makes `2 + 3 * 4` mean `2 + (3 * 4)`.

**Associativity — which side wins between equals.** Not published, because
in Heroes it is uniform (every binary operator is left-associative), and
uniform facts are the easiest ones to forget. This is what makes
`1 - 2 - 3` mean `(1 - 2) - 3` and not `1 - (2 - 3)`.

Power alone cannot tell those two apart: both operators are `-`, both have
power 4. Only *position* can — left child or right child.

## What the parser does with it

Precedence climbing needs one line for the whole of associativity
(`archive/bootstrap-rs/heroes/src/syntax/expr.rs`):

```rust
let right = binary(cur, ast, src, power + 1);
```

Asking the right operand for `power + 1` — strictly tighter — is what stops
it from swallowing the next `-` at the same power. Left-associative is
`power + 1`; right-associative would be `power`. One character apart, and
that character is the whole difference between `(1 - 2) - 3` and
`1 - (2 - 3)`.

## What the printer does with it

The formatter runs the same asymmetry backwards
(`archive/bootstrap-rs/heroes/src/printer/fmt_expr.rs`):

```rust
format!(
    "{} {} {}",
    wrapped(ast, src, *left, mine),      // needs `mine`
    binary_op(*op),
    wrapped(ast, src, *right, mine + 1), // needs `mine + 1`
)
```

with `wrapped` printing parentheses exactly when `power(child) < needed`. So
the complete rule, in one sentence:

> Parentheses survive when the inner operator binds **looser** than the outer
> one, **or** binds *equally* and sits on the **right** of it.

| written | tree | printed | why |
|---|---|---|---|
| `2 + (3 * 4)` | `+(2, *(3,4))` | `2 + 3 * 4` | `*` (5) ≥ needed 5 → no parens |
| `(2 + 3) * 4` | `*(+(2,3), 4)` | `(2 + 3) * 4` | `+` (4) < needed 5 → parens |
| `(a - b) - c` | `-(-(a,b), c)` | `a - b - c` | left child, needed 4, power 4 → none |
| `a - (b - c)` | `-(a, -(b,c))` | `a - (b - c)` | **right** child, needed 5, power 4 → parens |

The last row is the one a power-only rule gets wrong, and it is not a corner
case: it is every subtraction, division and remainder whose right operand is
another of the same.

## Why it generalises

The same shape — *a rule stated over constructs is wrong; state it over
positions* — turned up twice more in the same milestone.

- **The terminator.** §4.15's rule is not "record and variant headers have no
  terminator". It is "a terminator is planted when the line's **last token**
  can end a statement". In the pre-018 shape, `MAX = constant: int` and
  `test "3-4-5"` ended in a type name and a string, so they got one;
  `Point = record` ended in a keyword, so it did not. Stated over constructs,
  the rule needs a list and the list will be wrong; stated over the last
  token, it is one predicate (`is_line_ender`) and the parser never has to
  ask which declaration it is looking at — it skips terminators and asks only
  for the `Indent`. Coda, 2026-08-04: panel 018 inverted the declaration
  shape (`record Point` — every header now ends in an ender) and the
  position-stated rule absorbed the change with **zero lexer edits**, while
  the construct-stated patch built on the old gap (`at_line_start`, span
  arithmetic in recovery) was deleted outright. The lesson, demonstrated on
  itself.
- **The place of a mutation.** `v @ v + 1` is not recognised by lookahead at
  all, because "what may be mutated" is a property of a *position* (a name, or
  a field or index path rooted at one), not of a token. The parser reads the
  expression first and then asks whether it is a place.

## Prior art worth knowing

Precedence climbing with a `power + 1` for the right operand is the
formulation in Richard Bornat's and later Andy Chu's writing on Pratt
parsing; the equivalent in a grammar is the difference between
`expr := expr '-' term` (left) and `expr := term '-' expr` (right). C's
notorious `a - b - c` interview question exists because power alone is the
part everybody remembers.

## The rule this entry defends

When a rule is written down, write down what it ranges over. Heroes' surface
is small enough that most rules range over **tokens and positions**, not over
constructs — and a rule stated one level too high is not a simplification,
it is a rule that is wrong in exactly one case.
