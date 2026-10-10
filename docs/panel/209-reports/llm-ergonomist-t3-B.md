# Panel 209, the blind seat, t3-B

Copied by the coordinator on 2026-10-10 at 16:50 from `.claude/worktrees/scratch-b15/209-blind/t3-B/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. Its inputs were `brief.md`, `spec.md` (variant B: A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:48:30, ended 16:49:14, exit 0; the CLI's own estimate `total_cost_usd` 0.225 over 5 turns.

---

# verdict

Refused. The refused lines are 4 and 8. Line 2 is also refused as a consequence of line 4.

- **Line 4**, `totl @ total + x`: the statement has no `=` or `@=`, so it is not a `Binding` and can only parse as `Simple = Place "@" Expression NEWLINE`. That form mutates a place that already exists, and "only a cell declared with @= can be mutated". Nothing declares `totl`. The evident intent is `total @ total + x`, and `totl` is a typo.
- **Line 2**, `total @= 0`: because line 4 names `totl` and not `total`, nothing re-binds `total`, and "A cell nothing re-binds is a compile error: write `=`." Fixing line 4 also fixes this line.
- **Line 8**, `limit @= 10`: nothing in `main` re-binds `limit`, so the same sentence refuses it. `limit` is read on line 13, so it is not unused. It just should not be a cell.

Smallest change that gets the program accepted and keeps what it evidently means:
- line 4: `totl @ total + x` becomes `total @ total + x`
- line 8: `limit @= 10` becomes `limit = 10`

With those two changes the program prints `34`. `total_of([3, 4, 5])` is 12, `sum` is 12 and `limit` is 10, so 12 + 12 + 10 = 34.

# lines

| line | text | kind | words relied on |
|---|---|---|---|
| 2 | `total @= 0` | new name | "`@=` declares a mutable cell"; `Statement = ident Binding`, `Binding = [ ":" Type ] ( "=" \| "@=" ) Expression NEWLINE`; the comment "v @= 0 # mutable cell, type inferred the same way" |
| 4 | `totl @ total + x` | existing name (as written it is meant to change an existing cell, but no cell `totl` exists, so the line is refused) | "`@` re-binds it, or a field or element inside one"; "v @ v + 1 # mutation; only a cell declared with @= can be mutated"; `Simple = ... \| Place "@" Expression NEWLINE` |
| 8 | `limit @= 10` | new name | "`@=` declares a mutable cell"; `Binding = [ ":" Type ] ( "=" \| "@=" ) Expression NEWLINE` |
| 9 | `scores = [3, 4, 5]` | new name | "`=` binds once, forever"; "x = 5 # immutable binding, type inferred"; `Binding` |
| 10 | `sum @= 0` | new name | "`@=` declares a mutable cell"; `Binding` |
| 12 | `sum @ sum + s` | existing name | "`@` re-binds it"; "v @ v + 1 # mutation"; `Place "@" Expression NEWLINE` |

How the two kinds are told apart on one line: the token after the leading name decides it. `=` or `@=` (optionally after `: Type`) is a `Binding`, which makes a new name. A bare `@` is the `Place "@" Expression` form, which changes a name that already exists. Line 4 has a bare `@`, so on that line alone it claims `totl` already exists. You only see that the claim is false by looking at the rest of the function.

# sentences

- "`=` binds once, forever. `@=` declares a mutable cell, and `@` re-binds it, or a field or element inside one."
- "A cell nothing re-binds is a compile error: write `=`."
- "v @ v + 1 # mutation; only a cell declared with @= can be mutated" (section 5 example comment)
- "x = 5 # immutable binding, type inferred" and "v @= 0 # mutable cell, type inferred the same way"
- "Statement = ident Binding | Simple ."
- "Binding = [ ":" Type ] ( "=" | "@=" ) Expression NEWLINE ."
- "Simple = "_" Binding | Place "@" Expression NEWLINE ..."
- "Place = ident { "." ident | "[" Expression "]" } ."
- "An unused binding or parameter is a compile error; a read is a use and a write is not, except through an `@` parameter." (Checked: `x`, `s`, `scores`, `sum`, `limit` and `total` are all read in the fixed program.)
- "A literal takes the type its context asks for ... otherwise `i64`." (So `total`, `sum` and `limit` are `i64`, and the `+` on line 13 has one numeric type on both sides.)
- "`print` writes its values with no separator and exactly one trailing newline, and takes the types this language renders as text: a number, `str` or `bool`."
- "Loops: `while cond` and `for x in xs`, over an array or a `range`."
- "A function with a `->` must `return` on every path that reaches its end" (`total_of` ends with `return total`).

# program

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

Output:

```
34
```

# choice_points

1. **Which lines to report for the `totl` error.** The spec does not say whether the checker reports cascading errors. I reported line 4 as the root error and line 2 as refused because of it. If the checker stopped at the first error, it would report only line 2 or only line 4, depending on whether it checks declarations or uses first. The fix is the same either way.
2. **How to repair line 4.** I took `totl` as a typo for `total`. The other option is to add `totl @= 0` and return `totl`. That changes more lines, leaves `total` as a cell nothing re-binds (still refused), and is not what the code evidently means.
3. **How to repair line 8.** I changed `@=` to `=`, as the spec itself prescribes ("write `=`"). The other option is to keep `@=` and add a mutation of `limit`. That changes what the program prints, which is not a minimal change.
4. **Whether `sum` is taken.** Section 11 lists no built-in `sum`, and shadowing covers names a `use` binds (and, implicitly, names in scope). I treated `sum` as a free local name. If the checker reserved `sum`, lines 10 and 12 would be refused and the variable would need renaming. Nothing in the spec suggests this.
5. **Whether an undeclared place on the left of `@` is refused.** The spec never says "an undeclared name is an error" in so many words. I read "only a cell declared with @= can be mutated" as covering it. The only other reading would be that `totl @ ...` implicitly declares a name, but the `Binding` production rules that out, because only `=` and `@=` declare.

# confidence

I am highly confident the program as written is refused, and that the two-line fix is accepted and prints `34`. I am least sure of exactly which lines the checker would list for the typo: line 4, line 2, or both (choice point 1). I am also somewhat less sure about the precise spec wording that refuses line 4 (choice point 5), since the spec implies that undeclared names are refused rather than stating it.

# context

Only this directory's `brief.md`, `spec.md` and `main.hero` were read. The harness also attached some workspace metadata automatically: git status, recent commit messages about "panels" and "defects", and the user's email. None of it described this program or the language's checker, and I did not rely on it.
