# Panel 209, the blind seat, t3-B-c2 (clean reading, second)

Copied by the coordinator on 2026-10-10 at 16:54 from `.claude/worktrees/scratch-b15/209-blind/t3-B-c2/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. **The folder is its own git repository** (`git init`, one commit `inputs` holding only the inputs), made after the first twelve sessions reported the project's git status in their context: this session's `context` section says what reached it. Its inputs, byte-identical (`cmp`) to folder `t3-B`'s: `brief.md`, `spec.md` (variant B: A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:53:24, ended 16:54:07, exit 0; the CLI's own estimate `total_cost_usd` 0.216 over 5 turns.

---

# verdict

Refused, on lines 4, 2 and 8.

- Line 4, `totl @ total + x`: `@` re-binds a cell, and only a cell declared with `@=` can be mutated. No `totl` exists anywhere in `total_of`, so there is nothing for line 4 to re-bind. `@` never declares a name. This is a misspelling of `total`.
- Line 2, `total @= 0`: line 4 does not touch `total`, so no statement re-binds this cell, and "A cell nothing re-binds is a compile error: write `=`." This error follows from the typo on line 4. Fixing line 4 removes it.
- Line 8, `limit @= 10`: nothing in `main` re-binds `limit`, so the same rule refuses it on its own terms.

Smallest change that keeps the evident meaning:
- line 4: `totl` becomes `total` (one inserted letter), so `total @ total + x`;
- line 8: `limit @= 10` becomes `limit = 10` (one character removed).

With those two edits the program is accepted and prints:

```
34
```

(`total_of([3, 4, 5])` is 12, `sum` ends at 12, `limit` is 10, and 12 + 12 + 10 = 34.)

# lines

| line | text | kind | words of the specification relied on |
|---|---|---|---|
| 2 | `total @= 0` | new name | "`@=` declares a mutable cell"; `v @= 0  # mutable cell, type inferred the same way`; `Statement = ident Binding`, `Binding = [ ":" Type ] ( "=" \| "@=" ) Expression NEWLINE` |
| 4 | `totl @ total + x` | existing name (the line says it changes an existing `totl`. None exists, so the line is refused) | "`@` re-binds it, or a field or element inside one"; `v @ v + 1  # mutation; only a cell declared with @= can be mutated`; `Simple = ... \| Place "@" Expression NEWLINE` |
| 8 | `limit @= 10` | new name | "`@=` declares a mutable cell"; `Binding = ... ( "=" \| "@=" ) ...` |
| 9 | `scores = [3, 4, 5]` | new name | "`=` binds once, forever"; `x = 5  # immutable binding, type inferred`; `Statement = ident Binding` |
| 10 | `sum @= 0` | new name | "`@=` declares a mutable cell"; `Binding = ... ( "=" \| "@=" ) ...` |
| 12 | `sum @ sum + s` | existing name | "`@` re-binds it"; "only a cell declared with @= can be mutated"; `Place "@" Expression NEWLINE` |

The token after the name is enough to tell the two apart. `=` or `@=` (the `Binding` production) brings a name into existence. A bare `@` followed by an expression (the `Place "@" Expression` alternative) changes a name that must already exist as an `@=` cell. No other line holds `=` or `@`: line 1's `->` holds neither, and line 13 holds no `=` or `@`.

# sentences

- "`=` binds once, forever. `@=` declares a mutable cell, and `@` re-binds it, or a field or element inside one."
- "A cell nothing re-binds is a compile error: write `=`."
- "`v @ v + 1          # mutation; only a cell declared with @= can be mutated`"
- "`x = 5              # immutable binding, type inferred`"
- "`v @= 0             # mutable cell, type inferred the same way`"
- "An unused binding or parameter is a compile error; a read is a use and a write is not, except through an `@` parameter." (This is how I checked that `total`, `x`, `scores`, `sum`, `s` and `limit` are all read, so none of them is unused.)
- "`Statement = ident Binding | Simple .`", "`Binding = [ ":" Type ] ( "=" | "@=" ) Expression NEWLINE .`", "`Simple = "_" Binding | Place "@" Expression NEWLINE ...`", "`Place = ident { "." ident | "[" Expression "]" } .`"
- "A literal takes the type its context asks for ... otherwise `i64`." (This is why `scores` is `[i64]` and matches `xs: [i64]`, and why `0` and `10` are `i64`.)
- "No implicit conversions, widths included" (all the arithmetic is `i64` with `i64`).
- "`print` writes its values with no separator and exactly one trailing newline, and takes the types this language renders as text: a number, `str` or `bool`."
- "Loops: `while cond` and `for x in xs`, over an array or a `range` (section 11)."
- "A function with a `->` must `return` on every path that reaches its end" (`total_of` ends in `return total`).
- "Shadowing is a compile error" (I checked it: no name is bound twice, and `sum` is not one of the built-ins listed in section 11).

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

Prints `34`.

# choice_points

- **Fixing line 4.** I renamed `totl` to `total`. The other option is to declare `totl` (for example `totl @= 0` before the loop). That would leave `total` as a cell nothing re-binds (still refused), and `total_of` would return 0, which is not what the function evidently means. Renaming is both smaller and faithful to the intent.
- **Fixing line 8.** I changed `@=` to `=`, as the spec itself prescribes ("write `=`"). The other option is to add a mutation of `limit` somewhere. That is a larger change and would alter the printed value.
- **Whether line 2 counts as a separate refusal.** I listed it, because once line 4 names something else, `total` really is never re-bound. A checker that stops at the first error, or that reports only the unknown name `totl`, would mention line 4 alone. Either way, the same fix to line 4 clears it.
- **How to classify line 4 in `lines`.** I called it "existing name" because the spec's syntax (`Place "@" Expression`) and the rule that `@` only re-binds make it a mutation by form, whatever the name. The alternative reading, that it creates `totl`, is excluded because the spec gives `@` no declaring role.
- **Whether `sum` collides with a built-in.** Section 11 lists no `sum`, and `Sum` appears only as a grammar production name, so I treated it as free. If some hidden prelude defined `sum`, line 10 would be shadowing.

# confidence

High that line 4 is refused (undeclared `totl`) and that line 8 is refused (a cell never re-bound). I am high on the corrected program and on the output `34`. I am least sure of the exact set of lines a real checker would report: whether it also reports line 2, and in what order. That depends on error-recovery behaviour the specification does not describe.

# context

Only `brief.md`, `spec.md` and `main.hero` from this directory were read. The harness also supplied environment metadata: a git status listing untracked `run.err` and `run.json`, a commit message, a git user name and an email address. I did not open those files, and that metadata played no part in the verdict.
