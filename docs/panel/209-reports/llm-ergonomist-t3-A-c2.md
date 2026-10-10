# Panel 209, the blind seat, t3-A-c2 (clean reading, second)

Copied by the coordinator on 2026-10-10 at 16:54 from `.claude/worktrees/scratch-b15/209-blind/t3-A-c2/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. **The folder is its own git repository** (`git init`, one commit `inputs` holding only the inputs), made after the first twelve sessions reported the project's git status in their context: this session's `context` section says what reached it. Its inputs, byte-identical (`cmp`) to folder `t3-A`'s: `brief.md`, `spec.md` (variant A: A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:53:24, ended 16:54:11, exit 0; the CLI's own estimate `total_cost_usd` 0.23 over 5 turns.

---

# report

## verdict

Refused, on line 4: `totl @ total + x`.

Line 4 has no type annotation, so it cannot be a declaration. It parses only as `Place "@" Expression`, which is a mutation, and a mutation needs a name that already exists. No `totl` exists anywhere: the cell declared on line 2 is `total`. The spec says "only a declared @ name can be mutated", so the checker refuses the line. This is evidently a typo for `total`.

Smallest fix: on line 4, change `totl` to `total`.

With that fix the checker accepts the program, and it prints:

```
34
```

How that comes out: `total_of([3, 4, 5])` is 12, `sum` ends at 3 + 4 + 5 = 12, and `limit` is 10. 12 + 12 + 10 = 34, an `i64`, printed with one trailing newline.

Every other line checks. Every binding is read somewhere: `total` on lines 4 and 5, `x` on line 4, `limit` and `sum` on line 13, `scores` on lines 11 and 13, `s` on line 12. No name is shadowed. All the arithmetic is on `i64`. `total_of` returns on its only path.

## lines

| line | kind | words of the specification relied on |
|---|---|---|
| 2 `total: i64 @ 0` | new name | `ident ":" Type ( "@" \| "=" )` in `Binding`; "`v: i64 @ 0` # mutable declaration"; "the type is REQUIRED"; "`@` declares a mutable cell". The `: i64` before the `@` marks it as a declaration. |
| 4 `totl @ total + x` | existing name (as written it names one that does not exist, which is the error) | `Place "@" Expression` in `Simple`; "`v @ v + 1` # mutation; only a declared @ name can be mutated"; "`@` ... re-binds it". A bare name followed by `@` with no type is a mutation, never a declaration. |
| 8 `limit: i64 @ 10` | new name | Same as line 2: `ident ":" Type "@"`, "mutable declaration", "the type is REQUIRED". |
| 9 `scores = [3, 4, 5]` | new name | `ident "="` in `Binding`; "`x = 5` # immutable binding, type inferred"; "`=` binds once, forever"; "Shadowing is a compile error". A `=` can only bring a name into being, because it cannot rebind and cannot shadow. |
| 10 `sum: i64 @ 0` | new name | Same as line 2. |
| 12 `sum @ sum + s` | existing name | Same as line 4: `Place "@" Expression`, "mutation; only a declared @ name can be mutated". Here `sum` was declared on line 10. |

The rule you can read off a single line: `name: Type @` or `name: Type =` declares, `name =` binds a new immutable name, and `name @` with no type mutates an existing cell. Whether that existing cell actually exists (the line 4 question) depends on earlier lines, not on the line itself.

## sentences

- "`v: i64 @ 0         # mutable declaration`" and, in the same comment, "the type is REQUIRED"
- "`v @ v + 1          # mutation; only a declared @ name can be mutated`"
- "`x = 5              # immutable binding, type inferred`"
- "`=` binds once, forever. `@` declares a mutable cell and re-binds it, or a field or element inside one."
- "Statement = ident Binding | Simple ."
- "Binding   = ( \"=\" | \":\" Type ( \"@\" | \"=\" ) ) Expression NEWLINE ."
- "Simple    = \"_\" Binding | Place \"@\" Expression NEWLINE"
- "Place     = ident { \".\" ident | \"[\" Expression \"]\" } ."
- "An unused binding or parameter is a compile error; a read is a use and a write is not, except through an `@` parameter."
- "Shadowing is a compile error: a `use` binds its name for the whole file, so nothing else in the file may take it."
- "A literal takes the type its context asks for ... otherwise `i64`."
- "Loops: `while cond` and `for x in xs`, over an array or a `range` (section 11)."
- "`print` writes its values with no separator and exactly one trailing newline, and takes the types this language renders as text: a number, `str` or `bool`."
- "A function with a `->` must `return` on every path that reaches its end"

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

1. **A `@` cell that is never mutated (line 8 `limit`, and `total` in the original).** The spec does not say that declaring a cell with `@` and never writing it is an error. It only requires every binding to be used, and `limit` is read on line 13. I treated it as accepted. If the checker did refuse it, line 8 would also be refused, and the fix would be `limit = 10`. The output would still be 34.
2. **How to repair line 4.** I renamed `totl` to `total`, which matches the evident intent. The alternative, adding a declaration `totl: i64 @ 0`, would still fail: `totl` would only ever be written, and "a write is not" a use, so it would be an unused binding. Even if it passed, `total_of` would return 0 and the program would print 22.
3. **Whether line 4 could be read as an implicit declaration.** It cannot. The grammar has no `ident "@"` form inside `Binding`, and the spec says "the type is REQUIRED". So I read it only as a mutation.
4. **`sum` as a name.** `sum` does not appear in the built-in list in section 11, so I treated it as an ordinary free name that shadows nothing. If some hidden built-in `sum` existed, line 10 would be a shadowing error, but nothing in the spec says one does.
5. **Number formatting.** I assumed an `i64` prints as plain decimal `34`. The spec says how floats print but says nothing explicit about integers.

## confidence

I am highly confident that line 4 is refused and that the fixed program prints `34`. I am least sure about choice point 1, because the spec neither allows nor forbids a `@` cell that is never written. After that, I am least sure whether the checker would also report something about the original `total` (for example that it is never mutated) next to the line 4 error. That would not change the fix.

## context

Only `brief.md`, `spec.md` and `main.hero` from this directory were read. The harness also supplied environment metadata that I did not use: a git status listing untracked `run.err` and `run.json`, the user's email address, the date and the platform. I did not open `run.err` or `run.json`, and I read nothing outside this directory.
