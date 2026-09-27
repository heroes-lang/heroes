# Defect 106 closed: a list refuses a subtraction it would split

2026-09-27, M-agreed-retention step 21, in lane `74203a64` (a detached worktree
at the trunk's `2b1a1f24`), merged `49197fc9`. Found by panel 180 beside the
question it sat on (the coordinator, the historian, the compiler-engineer and
the completeness critic, each running it); ruled by the sitting's R2
(provisional, author ratification pending). It closes in the same commit as
defect 104, whose record holds the gate.

- [x] **106 — inside a list whose elements a NEWLINE separates, a line that begins with `- b` is a new element, so a subtraction broken before its operator runs with one element too many** | the lexer ends the line after `a` (Go's last-token rule, design.md §4.15) and the literal's `Sep` takes that NEWLINE as a separator, so `xs = [a` / `- b]` is `[a, -b]` at exit 0; `[base * qty` / `- discount]` and `{1: 10` / `- 2: 20}` the same; PEP 8 and Black break BEFORE a binary operator, so a model's natural spelling compiles to a different program, and `heroes fmt` then prints the line as `-b`, erasing the space that showed the intent | `selfhost/grammar_expr.hero` (`separator`, `array_literal`, `map_literal`) · `docs/panel/180-a-line-inside-brackets-breaks-by-how-it-ends-and-a-list-refuses-a-subtraction-it-would-split.md` · **closed 2026-09-27**

    **Origin:** panel 180, 2026-09-27: `heroes run` printed `2` for
    `[a` over `- b]` and for `[base * qty` over `- discount]` on the trunk
    at `29ed5601`, run by four seats independently. The one exit-0 wrong
    answer in the sitting's question, and the thesis forbids it.

    **The census that priced the refusal**, the compiler-engineer's: 1447
    line-separated element lines in 68 files under `selfhost/`, `tests/` and
    `examples/`, none beginning with `-`, so the refusal edits no program in
    the tree; the gate below confirms it.

## The repair

- **Where a line end separates two elements with no `,`, the next line may
  not begin with a `-` set apart from its operand** (`spaced_minus_element`,
  `selfhost/parse/list_line.hero`, called from `grammar_expr.separator`). The
  rule is keyed on that line end, as the critic required: the first element
  and a line after a written `,` are never asked. Set apart means a space, a
  comment or a line end after the sign, and each has its own true message:
  *this line of the list begins with `- `, a minus set apart from its value
  by a space*, *… begins with `-` and a comment, a minus set apart from its
  value*, *… holds only `-`, a minus set apart from its value by a line end*,
  each going on *and a line end with no `,` in a list starts a new element, so
  it would be a negative element where the gap says a subtraction: to
  subtract, end the line above with the `-`; for a negative element, write
  the `-` against its value*.
- **Every fix, applied, gives a program the rule accepts**, which the critic
  found the prototype's did not: the join moves the `-` to the end of the
  line above and keeps a comment that trailed it, and the negative element
  writes the `-` against its operand and keeps a comment that stood between
  them, on its own line. Both `guess`, since only the gap says which was
  meant (the precedent `selfhost/number.hero`'s leading zero). **In a map the
  line says which one parses**, a case the prototype did not reach: a line
  holding the entry's `:` (`{1: 10` over `- 2: 20}`) is a negative key and
  gets that fix alone, and a line holding none (`{"k": a` over `- b}`) can
  only be a subtraction and gets the join alone, and the rest of the map is
  passed over, so the trunk's second diagnostic there, `expected_map_entry_colon`,
  is gone. `selfhost/parse.hero`'s tests apply every fix of both refusals,
  nine shapes of this one and 25 of defect 104's, one fix at a time, and parse
  the result clean.
- **The golden**: `tests/golden/check/fixedbugs-a-list-split-a-subtraction-broken-before-its-minus.hero`,
  each of the five refusals annotated and snapshotted, beside the shapes that
  stay elements (`- 1` first, `-2`, `-discount`, a line after a `,`).

## What it does not change

- **`-1` and `-fee`, the spelling `fmt` prints**, stay elements, and so does
  a column of negatives one per line.
- **The unspaced `[a` over `-b]` stays two elements**, the residual the
  sitting recorded: no style the historian found spells a continuation that
  way, and Swift, which keys the same choice on the same space, reads it as a
  sign (critic, run). So do `[ys` over `[i]]` and `[o` over `.minus]`.
- **`fmt`**: no change; it never prints a shape this refuses (the critic's
  attempt, and the `canonical` suite).

## The measurements

The compiler-engineer's map on the lane's final compiler: row 55, `[1` over
`- 2]`, from exit 0 to `spaced_minus_element`, the one shape to move that
way, and row 66, `{"a": 1` over `- 2}`, from `expected_map_entry_colon` and
`expected_expression` to `spaced_minus_element` alone. The gate and the
timing are defect 104's record.
