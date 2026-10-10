# Panel 209, the blind seat, t3-A-c1 (clean reading, first)

Copied by the coordinator on 2026-10-10 at 16:53 from `.claude/worktrees/scratch-b15/209-blind/t3-A-c1/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. **The folder is its own git repository** (`git init`, one commit `inputs` holding only the inputs), made after the first twelve sessions reported the project's git status in their context (commit subjects naming panels and defects, the untracked `docs/panel/209-briefs/`): this session's `context` section says what reached it. Its inputs, byte-identical (`cmp`) to folder `t3-A`'s: `brief.md`, `spec.md` (variant A: A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:51:58, ended 16:52:42, exit 0; the CLI's own estimate `total_cost_usd` 0.219 over 5 turns.

---

# verdict

Refused, on line 4.

Line 4, `totl @ total + x`, has no type after the name, so it cannot be a declaration. The grammar reads it as a mutation (`Place "@" Expression`). But `totl` is not declared anywhere: line 2 declares `total`, not `totl`. The specification says "only a declared @ name can be mutated", so the checker refuses this line. The other lines are fine. `total` is read on lines 4 and 5, `x` is read on line 4, and every name in `main` is read.

The smallest change is to fix the typo on line 4 to `total @ total + x`. With that change the program is accepted and prints:

```
34
```

How it gets 34: `total_of([3, 4, 5])` is 12, `sum` ends at 12, and `limit` is 10, so the sum is 12 + 12 + 10 = 34.

# lines

| line | kind | words of the specification relied on |
|---|---|---|
| 2 `total: i64 @ 0` | new name | "`v: i64 @ 0`" commented "mutable declaration", "the type is REQUIRED"; grammar `Binding = ( "=" \| ":" Type ( "@" \| "=" ) ) Expression NEWLINE`. The `: Type` before `@` marks a declaration. |
| 4 `totl @ total + x` | existing name (the line is a mutation, so it asks for an existing name; none exists, which is the error) | "`v @ v + 1`" commented "mutation; only a declared @ name can be mutated"; grammar `Simple = ... \| Place "@" Expression NEWLINE`. `Binding` needs `=` or `:` after the name, so a bare `ident @` can only be a `Place "@"` mutation. |
| 8 `limit: i64 @ 10` | new name | same as line 2: "mutable declaration", "the type is REQUIRED", `":" Type ( "@" \| "=" )` |
| 9 `scores = [3, 4, 5]` | new name | "`x = 5`" commented "immutable binding, type inferred"; "`=` binds once, forever."; `Binding = ( "=" \| ...)` |
| 10 `sum: i64 @ 0` | new name | same as line 2 |
| 12 `sum @ sum + s` | existing name | same as line 4: "mutation; only a declared @ name can be mutated", `Place "@" Expression` |

The rule you can apply on one line alone: if the name is followed by `=`, or by `:` and a type, the line brings in a new name. If the name is followed directly by `@`, the line changes an existing name. Whether that name really exists cannot be told from the line by itself. On line 4 it does not exist.

# sentences

- "`=` binds once, forever. `@` declares a mutable cell and re-binds it, or a field or element inside one."
- "x = 5              # immutable binding, type inferred"
- "v: i64 @ 0         # mutable declaration" with "the type is REQUIRED" (the comment in the spec joins these two with a dash)
- "v @ v + 1          # mutation; only a declared @ name can be mutated"
- "Statement = ident Binding | Simple ."
- "Binding   = ( "=" | ":" Type ( "@" | "=" ) ) Expression NEWLINE ."
- "Simple    = "_" Binding | Place "@" Expression NEWLINE"
- "Place     = ident { "." ident | "[" Expression "]" } ."
- "An unused binding or parameter is a compile error; a read is a use and a write is not, except through an `@` parameter." (I checked this for the fixed program: every binding is read.)
- "A literal takes the type its context asks for" and "otherwise `i64`." (so the array literal on line 9 is `[i64]`, which matches `total_of`)
- "`print` writes its values with no separator and exactly one trailing newline"
- "Loops: `while cond` and `for x in xs`, over an array or a `range` (section 11)."

# program

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

# choice_points

- **Line 4, mutation or implicit declaration.** I read `totl @ ...` as a mutation of an undeclared name, which is refused. The other reading would let `@` without a type declare a new cell. The spec rules that out ("the type is REQUIRED", and `Binding` has no `ident "@"` form), so the other choice would wrongly accept the line.
- **Line 4, how to fix it.** I renamed `totl` to `total`, since the function evidently means to sum `xs`. The other fix, declaring `totl: i64 @ 0` inside the loop, would make a new cell on every pass and leave `total` at 0. The program would then print 22 instead of 34. It would also likely be refused, because `totl` is written but never read ("a write is not" a use).
- **Line 8, a mutable cell that is never mutated.** `limit` is declared with `@` but never re-bound. The spec only says that an unused binding is an error, and `limit` is read on line 13, so I took it as accepted. If the checker also demanded that every `@` cell be mutated, line 8 would be refused and would have to become `limit = 10`. The output would stay 34.
- **The name `sum`.** It is not among the listed built-ins, so I took it as free and not shadowing anything. If it were a built-in, line 10 would be refused as shadowing.
- **Whether the checker reports more than one error.** I assumed that refusing line 4 does not produce follow-on errors, for example about `x` being unused. On my reading `x` is read on line 4 whatever the target is, so there should be none.

# confidence

High that line 4 is refused and why, and high on the output 34 for the fixed program. I am least sure of two side points the spec does not settle: whether a never-mutated `@` cell (line 8) draws a complaint, and whether `sum` collides with anything built in.

# context

Beyond this directory's files (brief.md, spec.md, main.hero), my context also held an automatic environment note. It gave the working directory path, a git status showing untracked `run.err` and `run.json` with one commit "inputs", the git user name, and an account email address. I did not read `run.err` or `run.json` or any other file, and none of that note affected the verdict.
