# Panel 209, the blind seat, t3-A

Copied by the coordinator on 2026-10-10 at 16:50 from `.claude/worktrees/scratch-b15/209-blind/t3-A/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. Its inputs were `brief.md`, `spec.md` (variant A: A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:48:30, ended 16:49:39, exit 0; the CLI's own estimate `total_cost_usd` 0.325 over 6 turns.

---

# report

## verdict

Refused, on line 4.

Line 4, `totl @ total + x`, is a mutation (the `Place "@" Expression` form). Its target `totl` was never declared anywhere: the function declares `total` on line 2, and `totl` is evidently a misspelling of it. The specification allows mutation only of a declared `@` name, so the checker refuses the line. Line 4 cannot be read as a declaration of a new name either: a mutable declaration must carry a type (`ident ":" Type "@"`), and line 4 has none.

No other line is refused. `total` is still read on lines 4 and 5, so line 2 does not also fail as an unused binding. `limit` on line 8 is declared with `@` and never mutated. The specification does not call that an error, and `limit` is read on line 13, so it is used.

Smallest change: on line 4, change `totl` to `total`. With that change the program is accepted and prints:

```
34
```

`total_of([3, 4, 5])` is 12, `sum` is 12 and `limit` is 10, so the sum is 12 + 12 + 10 = 34.

## lines

| line | text | new name or existing name | words of the specification relied on |
|---|---|---|---|
| 2 | `total: i64 @ 0` | new name (`total`, mutable) | "mutable declaration -- the type is REQUIRED"; "`@` declares a mutable cell"; grammar `Statement = ident Binding`, `Binding = ( "=" \| ":" Type ( "@" \| "=" ) ) Expression NEWLINE`. The `: Type` before `@` marks a declaration. |
| 4 | `totl @ total + x` | existing name (the form claims an existing name; `totl` is not one, so the line is refused) | "mutation; only a declared @ name can be mutated"; "`@` ... re-binds it"; grammar `Simple = ... \| Place "@" Expression NEWLINE`. A bare name followed by `@`, with no `: Type`, is a mutation. |
| 8 | `limit: i64 @ 10` | new name (`limit`, mutable) | Same as line 2: "mutable declaration -- the type is REQUIRED", `ident ":" Type ( "@" \| "=" )`. |
| 9 | `scores = [3, 4, 5]` | new name (`scores`, immutable) | "immutable binding, type inferred"; "`=` binds once, forever"; grammar `Binding = ( "=" \| ... )`. A bare `=` always makes a new name, because `=` never re-binds. |
| 10 | `sum: i64 @ 0` | new name (`sum`, mutable) | Same as line 2. |
| 12 | `sum @ sum + s` | existing name (`sum`, declared on line 10) | Same as line 4: "mutation; only a declared @ name can be mutated", `Place "@" Expression`. |

Lines 1, 3, 5, 6, 7, 11 and 13 contain neither `=` nor `@`. The `->` on line 1 is not `=`.

You can tell the two cases apart from a line alone. A name followed by `:` and a Type and then `@` or `=` creates a new name. A name followed by a bare `=` also creates a new name, since `=` "binds once, forever" and shadowing is an error. A place followed by a bare `@`, with no type, changes an existing name. Whether that name really exists is a question for the rest of the scope, and that is the question line 4 fails.

## sentences

The specification's own em dashes are written here as `--`, because this report uses no em dashes. Otherwise the quotes are exact.

- "v: i64 @ 0         # mutable declaration -- the type is REQUIRED"
- "v @ v + 1          # mutation; only a declared @ name can be mutated"
- "x = 5              # immutable binding, type inferred"
- "`=` binds once, forever. `@` declares a mutable cell and re-binds it, or a field or element inside one."
- "Statement = ident Binding | Simple ."
- "Binding   = ( \"=\" | \":\" Type ( \"@\" | \"=\" ) ) Expression NEWLINE ."
- "Simple    = \"_\" Binding | Place \"@\" Expression NEWLINE"
- "Place     = ident { \".\" ident | \"[\" Expression \"]\" } ."
- "An unused binding or parameter is a compile error; a read is a use and a write is not, except through an `@` parameter." (used to confirm that `total`, `limit`, `x` and `s` are all read, so none of them is unused)
- "Shadowing is a compile error" (used to confirm that a bare `=` can only create a name, never reuse one)
- "Loops: `while cond` and `for x in xs`, over an array or a `range` (section 11)."
- "A literal takes the type its context asks for -- `b: u8 @ 255`, and `b + 1` is a `u8` -- otherwise `i64`." (so `[3, 4, 5]` is `[i64]`, which matches `total_of`'s parameter)
- "`print` writes its values with no separator and exactly one trailing newline, and takes the types this language renders as text: a number, `str` or `bool`."
- "the file you compile holds `function main()`, which takes nothing and produces nothing."
- "A function with a `->` must `return` on every path that reaches its end" (`total_of` ends with `return total`)

## program

```
function total_of(xs: [i64]) -> i64
    total: i64 @ 0
    for x in xs
        total @ total + x
    return total

function main()
    limit: i64 @ 10
    scores = [3, 4, 5]
    sum: i64 @ 0
    for s in scores
        sum @ sum + s
    print(total_of(scores) + sum + limit)
```

Output: `34`

## choice_points

1. **Line 4: implicit declaration or mutation of an undeclared name.** I read `totl @ ...` as a mutation of an undeclared name, which is refused. The alternative would be to treat it as a new mutable name with an inferred type. The specification rules that out: the type is REQUIRED for a mutable declaration, and the grammar has no `ident "@"` declaration form. Under that alternative, line 4 would instead fail as an unused binding, `total` would stay 0, and the output would be 22.
2. **The fix.** I renamed `totl` to `total`, since that is what the code evidently means: a running sum that `return total` hands back. Another option would be to declare `totl` and return it, but that is a larger change and would leave `total` without any purpose.
3. **Line 8: `limit` is declared `@` but never mutated.** I treated this as allowed, because no sentence makes it an error and `limit` is read. If a checker refused a mutable binding that is never mutated, line 8 would also be refused, and the fix would be `limit = 10`. The output would still be 34.
4. **Whether the checker reports only line 4.** Fixing line 4 leaves `total` both mutated and read, so no further error follows from it. If a checker also complained that `total` is never mutated because of the typo, that would be a cascade from line 4, not a separate fault, and the same fix removes it.
5. **Using `sum` as a name.** `sum` is not among the listed built-ins, so I treated declaring it as legal, not as shadowing. If it were a built-in, line 10 would be refused as shadowing.

## confidence

I am highly confident that line 4 is refused, that the rename fixes it, and that the output is 34. I am least sure about choice point 3: the specification does not say whether a `@` declaration that is never mutated is accepted. Nothing in the text forbids it, but a strict checker could flag it.

## context

Besides this directory's `brief.md`, `spec.md` and `main.hero`, my context also held the harness's system prompt. It included a git status snapshot for the surrounding repository, listing recent commit subjects about "panels" and "defects" in this language project and untracked `docs/panel/209-briefs/` and `docs/panel/209-reports/` directories, plus the user's email address and git user name. I read no other file and none of that material informed the verdict.
