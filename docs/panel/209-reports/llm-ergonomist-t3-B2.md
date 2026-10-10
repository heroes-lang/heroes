# Panel 209, the blind seat, t3-B2 (second reading)

Copied by the coordinator on 2026-10-10 at 16:51 from `.claude/worktrees/scratch-b15/209-blind/t3-B2/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. Its inputs were `brief.md`, `spec.md` (variant t3-B: t3-B is the first reading's folder copied byte for byte; A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:49:58, ended 16:50:47, exit 0; the CLI's own estimate `total_cost_usd` 0.232 over 5 turns.

---

# report

## verdict

Refused, on lines 4, 2 and 8.

- **Line 4** `totl @ total + x`: this is a mutation (`Place "@" Expression`) of the name `totl`, which nothing declares. Only a cell declared with `@=` can be mutated, and no `totl` exists, so the checker refuses the line. Given `total @= 0` on line 2 and `return total` on line 5, the line is a misspelling of `total`.
- **Line 2** `total @= 0`: because line 4 mutates `totl` and not `total`, nothing re-binds the cell `total`. "A cell nothing re-binds is a compile error: write `=`." This error follows from line 4's typo and goes away once line 4 is fixed.
- **Line 8** `limit @= 10`: nothing in `main` ever writes `limit @ ...`, so this cell is never re-bound either. Same rule, so it is refused. This error does not depend on line 4.

Lines 10 and 12 (`sum @= 0`, then `sum @ sum + s` in the loop) are correct. Every binding and parameter is read somewhere (`xs`, `total`, `x`, `limit`, `scores`, `sum`, `s`), so there are no unused-binding errors.

Smallest change that gets the program accepted while it still does what it evidently means (two edits):

- line 4: `totl @ total + x` becomes `total @ total + x`
- line 8: `limit @= 10` becomes `limit = 10`

The fixed program prints:

```
34
```

`total_of([3, 4, 5])` is 12, `sum` is 12 and `limit` is 10, so the sum is 34. All values are `i64`, and `print` of an `i64` writes its digits and one newline.

## lines

| line | text | kind | words of the specification relied on |
|---|---|---|---|
| 2 | `total @= 0` | new name | "`v @= 0  # mutable cell, type inferred the same way`"; "`@=` declares a mutable cell"; grammar `Statement = ident Binding` with `Binding = [ ":" Type ] ( "=" \| "@=" ) Expression NEWLINE` |
| 4 | `totl @ total + x` | existing name (claimed; `totl` does not exist, so the line is refused) | "`v @ v + 1  # mutation; only a cell declared with @= can be mutated`"; "`@` re-binds it"; grammar `Simple = ... \| Place "@" Expression NEWLINE` |
| 8 | `limit @= 10` | new name | "`@=` declares a mutable cell"; `Binding = ... ( "=" \| "@=" ) ...` |
| 9 | `scores = [3, 4, 5]` | new name | "`x = 5  # immutable binding, type inferred`"; "`=` binds once, forever" |
| 10 | `sum @= 0` | new name | "`@=` declares a mutable cell" |
| 12 | `sum @ sum + s` | existing name | "`v @ v + 1  # mutation`"; "`@` re-binds it"; `Place "@" Expression` |

On every line, the operator token alone tells the two apart. A bare `=` (binding) or `@=` (cell declaration) brings a name into existence. A bare `@` that is not followed by `=` changes a name that already exists. You never need to look up whether the name is already in scope to classify the line. You need that lookup only to decide whether the line is legal: a mutation of an undeclared name is refused (line 4), and a binding of a name already in scope would be refused as shadowing.

## sentences

- "`=` binds once, forever. `@=` declares a mutable cell, and `@` re-binds it, or a field or element inside one."
- "A cell nothing re-binds is a compile error: write `=`."
- The section 5 example comments: "`x = 5              # immutable binding, type inferred`", "`v @= 0             # mutable cell, type inferred the same way`", "`v @ v + 1          # mutation; only a cell declared with @= can be mutated`".
- "An unused binding or parameter is a compile error; a read is a use and a write is not, except through an `@` parameter."
- "Shadowing is a compile error: a `use` binds its name for the whole file, so nothing else in the file may take it."
- Grammar: "`Statement = ident Binding | Simple .`", "`Binding   = [ \":\" Type ] ( \"=\" | \"@=\" ) Expression NEWLINE .`", "`Simple    = \"_\" Binding | Place \"@\" Expression NEWLINE ...`", "`Place     = ident { \".\" ident | \"[\" Expression \"]\" } .`"
- "A literal takes the type its context asks for ... otherwise `i64`."
- "Loops: `while cond` and `for x in xs`, over an array or a `range` (section 11)."
- "`print` writes its values with no separator and exactly one trailing newline, and takes the types this language renders as text: a number, `str` or `bool`."
- "the file you compile holds `function main()`, which takes nothing and produces nothing."
- "A function with a `->` must `return` on every path that reaches its end" (line 5 satisfies this for `total_of`).

## program

```
function total_of(xs: [i64]) -> i64
    total @= 0
    for x in xs
        total @ total + x
    return total

function main()
    limit = 10
    scores = [3, 4, 5]
    sum @= 0
    for s in scores
        sum @ sum + s
    print(total_of(scores) + sum + limit)
```

## choice_points

- **What line 4 means.** I read `totl` as a typo for `total`. The other reading is that `totl` was meant as a new name, written `totl @= total + x`. That would still leave `total` never re-bound (line 2 still refused). It would also make `totl` an unused binding, and `total_of` would return 0. Only the `total` reading matches the function's name and its `return total`.
- **How to fix line 8.** I changed `@=` to `=`, as the spec's own error text says ("write `=`"). The other choice is to keep the cell and add a pointless `limit @ limit`. That is a bigger change and adds a meaningless mutation, though the output would be the same.
- **Which lines to report.** The spec does not say whether the checker stops at the first error or reports them all. I listed all three lines. A checker that stops at the first error might name only one of them, most likely line 2 or line 4. Line 2's error disappears with the line 4 fix, and line 8's needs its own fix either way.
- **Exactly how line 4 is refused.** The spec says only a cell declared with `@=` can be mutated. It gives no separate "unknown name" rule, and I took an undeclared name to fail that requirement. A checker could word it as "unknown name `totl`" instead. Either way the line is refused.
- **Literal types.** `0`, `10` and `[3, 4, 5]` have no annotation and no context asking for a type, so I typed them `i64` ("otherwise `i64`"). This makes `total_of(scores) + sum + limit` a sum of three `i64`s, which is legal.

## confidence

High that the program as written is refused, and high on lines 4 and 8: each breaks a sentence the spec states directly. High that the fixed program is accepted and prints `34`.

Least sure of which subset of the three lines a real checker would name. The spec does not say whether it reports every error or stops at the first, nor how it words a mutation of an undeclared name.

## context

Besides `brief.md`, `spec.md` and `main.hero`, my context held harness-supplied material: the user's email address, a git status snapshot (branch `main`, untracked `docs/panel/209-briefs/` and `docs/panel/209-reports/`, and recent commit subjects about panels 205 to 208 and defects 582 to 589), and general environment details. None of it says anything about this program or this language's rules, and none of it was used. I read no file outside this directory.
