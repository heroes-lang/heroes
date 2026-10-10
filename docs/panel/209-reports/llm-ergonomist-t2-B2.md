# Panel 209, the blind seat, t2-B2 (second reading)

Copied by the coordinator on 2026-10-10 at 16:51 from `.claude/worktrees/scratch-b15/209-blind/t2-B2/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. Its inputs were `brief.md`, `spec.md` (variant t2-B: t2-B is the first reading's folder copied byte for byte; A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:49:58, ended 16:50:38, exit 0; the CLI's own estimate `total_cost_usd` 0.201 over 4 turns.

---

# program

```
function tally(words: [str]) -> {str: i64}
    counts: {str: i64} @= {}
    for w in words
        counts[w] @ counts[w].default(0) + 1
    return counts

function main()
    t = tally(["a", "b", "a"])
    print(t["a"].default(0))
```

# sentences

Lines are numbered within the program block above (line 1 is `function tally...`).

- "the file you compile holds `function main()`, which takes nothing and produces nothing." Governs line 7.
- "Indentation is significant and rigid: exactly 4 spaces per level; a tab is a compile error." Governs every indented line (2 to 5, 8, 9).
- "Every top-level line starts with its kind." Governs lines 1 and 7.
- `"function" ident [ Generics ] Params [ "->" Type ] Block` and `Param = [ "@" ] ident ":" Type .` Govern line 1 (`words: [str]`, `-> {str: i64}`).
- "`[T]` | dynamic array" and "`{K: V}` | map", with `Prefix = ... | "[" Type "]" | "{" Type ":" Type "}"`. Govern the types on lines 1 and 2.
- Empty container: "A container literal separates elements by newline across lines and by comma on one, and an empty one needs an annotation: `xs: [i64] = []` · `m: {str: i64} @= {}`." Governs line 2, which copies the second form exactly.
- Value that changes, declared: "`v @= 0             # mutable cell, type inferred the same way`" and "`@=` declares a mutable cell, and `@` re-binds it, or a field or element inside one." Governs line 2.
- Value that changes, changed: "`v @ v + 1          # mutation; only a cell declared with @= can be mutated`", "`m[k] @ v` inserts or replaces", the example `m["a"] @ 1`, and `Simple = ... | Place "@" Expression NEWLINE .` with `Place = ident { "." ident | "[" Expression "]" } .` Govern line 4.
- "A cell nothing re-binds is a compile error: write `=`." Governs line 2: `counts` is re-bound on line 4, so `@=` is required and correct.
- "`m[k]` is a `V?` with code `missing_key`" and "`.default(v)` | extract or fall back". Govern the reads `counts[w].default(0)` on line 4 and `t["a"].default(0)` on line 9; the brief also prescribes the latter.
- "A literal takes the type its context asks for ... otherwise `i64`." Governs `0` and `1` on line 4 and `0` on line 9 (all `i64`, matching `V`).
- "arithmetic + - * / % (both sides one numeric type, never mixed)" and "Precedence, strongest first: call, `.` and `::` ... `+ -`". Govern line 4: `counts[w].default(0)` binds before `+ 1`, both sides `i64`.
- "Loops: `while cond` and `for x in xs`, over an array" and `For = "for" ident "in" Expression Block .` Govern line 3.
- "An unused binding or parameter is a compile error; a read is a use and a write is not". Governs `words` (read on line 3), `w` (read on line 4), `counts` (read on lines 4 and 5), `t` (read on line 9).
- "A function with a `->` must `return` on every path that reaches its end" and `"return" [ Expression ] NEWLINE`. Govern line 5.
- "Every value behaves as an independent copy". Governs line 5: returning `counts` hands back its value.
- "`x = 5              # immutable binding, type inferred`" and "`=` binds once, forever." Govern line 8.
- `"[" [ Expression { Sep Expression } ] "]"` with `Sep = "," | NEWLINE`, and "by comma on one". Govern the array literal on line 8; its element type `str` comes from the parameter type `[str]`.
- "`print(...)` ... takes the types this language renders as text: a number, `str` or `bool`." Governs line 9 (an `i64`).
- "A line that computes a value must use it ... which a `()` line refuses: it stands alone." Governs line 9: `print` returns `()`, so it stands alone without `_ =`.
- "Shadowing is a compile error". Governs the names chosen: `counts`, `w`, `t`, `words` shadow nothing and no built-in.

# choice_points

1. Updating the count. I chose the one-line `counts[w] @ counts[w].default(0) + 1`. The alternative is a `match counts[w]` with `.ok n => counts[w] @ n + 1` and `.err _ => counts[w] @ 1`; it should also be accepted and produce the same map, but it is longer and adds questions (whether an arm's inline `@` statement is fine, `_` payload) without benefit.
2. Where the map lives. I used a local mutable cell plus `for`. The alternative is `fold` with a helper function `(acc, item)` that returns an updated map; that needs a second named function (no anonymous functions), an `@=` copy inside it, and generic inference from `{}` with an annotation, so more risk for the same result.
3. Annotating the empty map on the binding (`counts: {str: i64} @= {}`). The spec says an empty container needs an annotation; leaving it off (`counts @= {}`) would be a compile error. Writing `=` instead of `@=` would make line 4 a compile error (only an `@=` cell can be mutated).
4. Annotating `t` on line 8. I left it inferred, since inference is local and `tally`'s signature gives `{str: i64}`. Writing `t: {str: i64} = ...` would be equally valid.
5. Return type plain `{str: i64}`, not fallible, as the brief fixes. Hence no `?` is used anywhere (it would need a fallible caller return type).
6. Default literal `0` vs `.must()`: I used `.default(0)` in `tally` because the first sight of a word is a `missing_key`; `.must()` there would abort at run time. In `main` the brief prescribes `.default(0)`.
7. Names (`counts`, `w`): free choices; any non-shadowing, non-builtin names behave the same.

The program prints `2`.

# confidence

High, about 90 percent. Least sure of line 4, `counts[w] @ counts[w].default(0) + 1`: it reads the element through the same place it writes, and the statement begins with an identifier, which the grammar can also start as `ident Binding`; the spec's own example `m["a"] @ 1` and the `Place "@" Expression` production make me fairly confident it parses and checks, but it is the one line not copied nearly verbatim from an example.

# context

Only `brief.md` and `spec.md` from this directory were read. The harness also placed into my context environment information (working directory path, git branch, recent commit messages mentioning panels and defects, the user's email); none of it describes the language and none was used for the program.
