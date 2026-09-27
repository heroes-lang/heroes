# comments107: a comment the author's parentheses hold stays inside parentheses

Defect 107 (panel 180's compiler-engineer and its completeness critic,
2026-09-27): a comment after a `.` or a `::` where the only bracket around it
is the author's parentheses. `fmt` dropped the parentheses, which bind
nothing, and broke the line after the token the comment follows, so the next
line stood at the statement's depth, where a margin is structure: `y = (xs.
# c` over `len())` was printed `y = xs.  # c` over an indented `len()`, which
does not parse, and `fmt` refused at exit 2 with the file untouched. Every
file here is refused by the trunk's compiler at `f39836a6` and formatted by
the lane's, each output a fixpoint.

A value printed where no bracket of the page is open around it, whose
printing would break a line for a comment outside every bracket it prints,
is printed as a group now (`selfhost/print/bare.hero`, asked by
`print/brackets.hero`'s `spread`): inside the author's innermost pair of
parentheses around it, or inside `fmt`'s own where the author wrote none.
The same rule `fmt` already followed for a comment after an operator, whose
group is its own (design.md §4.15, *broken inside parentheses or not at
all*).

`statements.hero`: a binding's value, a declaration's, a mutation's, a
`return`'s, an `assert`'s, a discarded one and an arm's; after the dot of a
method, a field, a field's name, and a variant case with and without its
fields; a comment trailing the dot or on a line of its own.

`heads.hero`: the value of a line that opens a block, an `if`, an `else if`,
a `while`, a `for` and a `match`, and a condition wrapped whole. There the
lexer read the dropped parentheses' next line as a block the head opened,
one level too deep (`indentation_jump`). The group is a head printed across
lines, deepened as every such head is (`print/margins.hero`).

`operands.hero`: the parentheses around a binary's operand, a unary's, a
value a member is read from, the base of a `?`, a sum's inner operand, a pair
inside a pair, a method whose arguments hold a comment too, two dots in one
value, and the part before a control form. The pair kept is the innermost
around the comment; an outer one binds nothing and is dropped, as `fmt`
drops every such pair. An element of an array holds its break already and
keeps the form the trunk printed.

`nesting.hero`: two and three pairs around one value, of which the innermost
is kept, and the same value inside a call, which holds the break already.

`margin.hero`: a line continued at the statement's own margin with no
parentheses at all, which `fmt` prints inside its own. That continuation is
one the lexer accepts and design.md §4.15 defers (*trailing-operator
continuation at depth zero*); with a comment on a line of its own there,
`fmt` refuses at exit 2, since the owner rule reads the continued line as a
new logical line and the comment as a block's, which a group cannot keep.
That shape is reported with the defect's record, apart from this one.

Every output is pinned in `tests/harness/suite_surface.hero`.
