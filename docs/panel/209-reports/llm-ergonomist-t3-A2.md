# Panel 209, the blind seat, t3-A2 (second reading)

Copied by the coordinator on 2026-10-10 at 16:51 from `.claude/worktrees/scratch-b15/209-blind/t3-A2/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. Its inputs were `brief.md`, `spec.md` (variant t3-A: t3-A is the first reading's folder copied byte for byte; A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:49:58, ended 16:50:48, exit 0; the CLI's own estimate `total_cost_usd` 0.226 over 5 turns.

---

# report

## verdict

Refused, on line 4.

Line 4, `totl @ total + x`, has the form of a mutation: a Place (`totl`) followed by `@` and an expression, with no `:` and no type. That form can only change a mutable cell that already exists, and no name `totl` was ever declared (line 2 declares `total`; `totl` is a misspelling). The spec says "only a declared @ name can be mutated", so the checker refuses the line. It cannot be a declaration instead, because a mutable declaration requires the `: Type` part ("the type is REQUIRED").

Smallest fix: change `totl` to `total` on line 4. Then the loop adds every element of `xs` to `total`, which is what the function evidently means.

With that fix the program is accepted and prints:

```
34
```

How I got there: `total_of([3, 4, 5])` is 12, the loop in `main` makes `sum` 12, and `limit` is 10, so 12 + 12 + 10 = 34. `print` writes one value and one trailing newline.

No other line is refused. Every binding is read: `total` (lines 4 and 5), `x` (line 4), `limit` (line 13), `scores` (lines 11 and 13), `sum` (lines 12 and 13), `s` (line 12). All literals default to or are asked to be `i64`, so no types are mixed. `main` takes nothing and has no `->`. `total_of` returns on its only path.

## lines

| line | kind | words of the specification relied on |
|---|---|---|
| 2 `total: i64 @ 0` | new name | "`v: i64 @ 0` # mutable declaration ... the type is REQUIRED"; "`@` declares a mutable cell"; grammar `Statement = ident Binding`, `Binding = ( "=" \| ":" Type ( "@" \| "=" ) ) Expression NEWLINE`. The `: i64` before `@` marks a declaration. |
| 4 `totl @ total + x` | existing name (by its form; the name does not exist, so the line is refused) | "`v @ v + 1` # mutation; only a declared @ name can be mutated"; "`@` ... re-binds it"; grammar `Simple = ... \| Place "@" Expression NEWLINE`. No `:` Type before `@`, so it can only re-bind an existing cell. Telling that `totl` does not exist needs line 2 as well, not this line alone. |
| 8 `limit: i64 @ 10` | new name | same as line 2: `: Type` before `@` means "mutable declaration". |
| 9 `scores = [3, 4, 5]` | new name | "`x = 5` # immutable binding, type inferred"; "`=` binds once, forever."; grammar `Binding = ( "=" \| ... )`. A bare `=` only ever binds a name; it can never change one. |
| 10 `sum: i64 @ 0` | new name | same as line 2. |
| 12 `sum @ sum + s` | existing name | same as line 4: no `: Type` before `@`, so it is the mutation form `Place "@" Expression`; "only a declared @ name can be mutated" (`sum` is declared on line 10). |

On one line alone, the rule is: `name = e` and `name: T = e` and `name: T @ e` bring a new name into existence; `place @ e` (no type) changes an existing one.

## sentences

- "`v: i64 @ 0         # mutable declaration` ... `the type is REQUIRED`" (section 5 example; the original has a dash between the two parts)
- "`v @ v + 1          # mutation; only a declared @ name can be mutated`" (section 5 example)
- "`x = 5              # immutable binding, type inferred`" (section 5 example)
- "`=` binds once, forever."
- "`@` declares a mutable cell and re-binds it, or a field or element inside one."
- "An unused binding or parameter is a compile error; a read is a use and a write is not, except through an `@` parameter."
- "Shadowing is a compile error: a `use` binds its name for the whole file, so nothing else in the file may take it."
- `Statement = ident Binding | Simple .`
- `Binding   = ( "=" | ":" Type ( "@" | "=" ) ) Expression NEWLINE .`
- `Simple    = "_" Binding | Place "@" Expression NEWLINE | ...`
- `Place     = ident { "." ident | "[" Expression "]" } .`
- "A literal takes the type its context asks for ... otherwise `i64`." (the original has a dash and an example in place of the dots)
- "A function with a `->` must `return` on every path that reaches its end"
- "Loops: `while cond` and `for x in xs`, over an array or a `range` (section 11)."
- "`print` writes its values with no separator and exactly one trailing newline, and takes the types this language renders as text: a number, `str` or `bool`."
- "the file you compile holds `function main()`, which takes nothing and produces nothing."

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

## choice_points

1. **Is a mutable cell that is never mutated an error?** `limit` (line 8) is declared with `@` and never written again. The spec makes unused bindings an error but says nothing about a `@` cell that is never mutated. I chose: allowed (it is read on line 13, so it is used). The other choice would refuse line 8 too, and the fix would be `limit = 10`; the output would still be 34. In the original program `total` is also never mutated (line 4 writes `totl`), so under the other choice line 2 might be reported as well; that disappears with my fix.
2. **Fix for line 4: rename `totl` to `total`, or declare `totl`?** Declaring `totl` would leave `total` always 0 and contradict what the function evidently means (summing `xs`). I chose the rename. The other choice would also need a declaration with a type and would print 22 (0 + 12 + 10) if accepted at all.
3. **Is `sum` a reserved or built-in name that line 10 would shadow?** The built-ins list (section 11) does not contain `sum`, so I treated it as a free name. If it were taken, line 10 would be a shadowing error and the fix would be a rename; the output would be unchanged.
4. **How line 4 is reported.** One could read `totl @ ...` as an attempt to declare a new mutable name without a type. The grammar gives no such form (`ident Binding` needs `=` or `:` Type), so I read it as a mutation of an undeclared name. Either reading refuses the same line with the same fix.

## confidence

High that line 4 is refused and that the fixed program prints `34`. High on the new/existing classification of every line. Least sure of choice point 1: whether the checker also complains about a `@` cell that is never mutated (`limit`, and `total` in the unfixed program). The spec is silent on it, so I assumed no complaint.

## context

Besides `brief.md`, `spec.md` and `main.hero`, my context held a harness system prompt and an automatically attached note with the user's email address, the git branch, git status (untracked `docs/panel/209-briefs/` and `docs/panel/209-reports/`) and recent commit subjects about "panels" and "defects" in this language's project. None of it describes the language's rules, and I did not use it. I read no other file.
