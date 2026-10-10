# Panel 209, the blind seat, t3-B-c1 (clean reading, first)

Copied by the coordinator on 2026-10-10 at 16:53 from `.claude/worktrees/scratch-b15/209-blind/t3-B-c1/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. **The folder is its own git repository** (`git init`, one commit `inputs` holding only the inputs), made after the first twelve sessions reported the project's git status in their context (commit subjects naming panels and defects, the untracked `docs/panel/209-briefs/`): this session's `context` section says what reached it. Its inputs, byte-identical (`cmp`) to folder `t3-B`'s: `brief.md`, `spec.md` (variant B: A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:51:58, ended 16:52:41, exit 0; the CLI's own estimate `total_cost_usd` 0.21 over 5 turns.

---

# report

## verdict

Refused. The checker refuses lines 4, 2 and 8.

- Line 4, `totl @ total + x`: `@` re-binds a cell that already exists, and no `totl` was ever declared. The cell declared on line 2 is `total`; `totl` is a misspelling of it. Only a cell declared with `@=` can be mutated, so mutating a name that was never declared is an error.
- Line 2, `total @= 0`: because line 4 names `totl`, nothing ever re-binds `total`. "A cell nothing re-binds is a compile error: write `=`." This error goes away once line 4 is fixed.
- Line 8, `limit @= 10`: `limit` is only read (line 13) and never re-bound, so it is a cell nothing re-binds. That is a compile error.

Smallest change that keeps what the program evidently means:
1. Line 4: `totl @ total + x` becomes `total @ total + x`.
2. Line 8: `limit @= 10` becomes `limit = 10`.

With both changes the program is accepted. It prints `34`: `total_of(scores)` is 3+4+5 = 12, `sum` is 12, `limit` is 10, and 12 + 12 + 10 = 34.

## lines

| line | text | new name or existing name | words of the specification relied on |
|---|---|---|---|
| 2 | `total @= 0` | new name (`total`) | "`@=` declares a mutable cell"; `Statement = ident Binding`, `Binding = [ ":" Type ] ( "=" \| "@=" ) Expression NEWLINE` |
| 4 | `totl @ total + x` | existing name (it is written as a change to `totl`, and no `totl` exists, which is the error) | "`@` re-binds it, or a field or element inside one"; "only a cell declared with @= can be mutated"; `Simple = ... \| Place "@" Expression NEWLINE` |
| 8 | `limit @= 10` | new name (`limit`) | "`@=` declares a mutable cell"; `Binding = ... ( "=" \| "@=" ) ...` |
| 9 | `scores = [3, 4, 5]` | new name (`scores`) | "`=` binds once, forever"; `x = 5  # immutable binding, type inferred` |
| 10 | `sum @= 0` | new name (`sum`) | "`@=` declares a mutable cell" |
| 12 | `sum @ sum + s` | existing name (`sum`, from line 10) | "`@` re-binds it"; `v @ v + 1  # mutation; only a cell declared with @= can be mutated`; `Place "@" Expression` |

On each line the operator token decides it: `=` or `@=` (the `Binding` production) brings a name into existence, and a bare `@` after a `Place` changes one that already exists. The spelling of the operator is enough on its own; line 4 shows that the line alone does not tell you whether the name it changes actually exists.

## sentences

- "`=` binds once, forever. `@=` declares a mutable cell, and `@` re-binds it, or a field or element inside one."
- "A cell nothing re-binds is a compile error: write `=`."
- "v @ v + 1          # mutation; only a cell declared with @= can be mutated"
- "x = 5              # immutable binding, type inferred"
- "v @= 0             # mutable cell, type inferred the same way"
- "Statement = ident Binding | Simple ."
- "Binding   = [ \":\" Type ] ( \"=\" | \"@=\" ) Expression NEWLINE ."
- "Simple    = \"_\" Binding | Place \"@\" Expression NEWLINE"
- "An unused binding or parameter is a compile error; a read is a use and a write is not, except through an `@` parameter."
- "Shadowing is a compile error"
- "A literal takes the type its context asks for ... otherwise `i64`."
- "Loops: `while cond` and `for x in xs`, over an array or a `range` (section 11)."
- "`print` writes its values with no separator and exactly one trailing newline, and takes the types this language renders as text: a number, `str` or `bool`."
- "A function with a `->` must `return` on every path that reaches its end"
- "One file is one module; the file you compile holds `function main()`, which takes nothing and produces nothing."

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

Prints:

```
34
```

## choice_points

1. **Line 4: rename the target or declare `totl`.** I changed `totl` to `total`, because the loop is evidently meant to add into `total`, which line 5 returns. The other choice, declaring `totl` somewhere, would leave `total` never re-bound (line 2 still refused), and `total_of` would return 0 even if the program were accepted.
2. **Line 8: write `=` or add a mutation.** I wrote `limit = 10`, as the spec prescribes ("write `=`"). Adding some re-binding of `limit` would also satisfy the checker, but it is a larger change and could change the printed value.
3. **Whether line 2 counts as a separate refusal.** The spec does not say how many errors the checker reports, or in what order. I list line 2 because it is an error as written, but it goes away with the line 4 fix and needs no edit of its own. A checker that stops at the first error might report only one of lines 2, 4, 8.
4. **Whether `sum` clashes with a built-in.** Section 11 lists no `sum` built-in, so I treated `sum` as a free name and line 10 as no shadowing. If `sum` were an implicit built-in, line 10 would be shadowing and need renaming. Nothing in the spec suggests that.
5. **Literal types.** The `0` on line 2 and the `0` on line 10 have no annotation. I took them as `i64` ("otherwise `i64`"), which matches `xs: [i64]` and the `-> i64` return type. Reading them as typed from later use gives the same result.

## confidence

High that the program is refused, and that line 8 and the `totl` misspelling on line 4 are the causes. High that the corrected program prints `34`. Least sure of exactly which lines a real checker would report: whether it names line 2 as well as line 4, and whether it reports every error or stops at the first. The spec does not define error reporting.

## context

Only this directory's files reached my context: `brief.md`, `spec.md` and `main.hero`, plus the automatic environment note (git status listing untracked `run.err` and `run.json`, and the latest commit "inputs"). I did not read `run.err` or `run.json`, or any file outside this directory.
