# Panel 209, the blind seat, t1-B2 (second reading)

Copied by the coordinator on 2026-10-10 at 16:51 from `.claude/worktrees/scratch-b15/209-blind/t1-B2/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. Its inputs were `brief.md`, `spec.md` (variant t1-B: t1-B is the first reading's folder copied byte for byte; A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:49:58, ended 16:50:34, exit 0; the CLI's own estimate `total_cost_usd` 0.19 over 4 turns.

---

# program

```
function longest(words: [str]) -> str
    best @= words[0]
    for w in words
        if w.len() > best.len()
            best @ w
    return best

function main()
    print(longest(["ab", "abcd", "xyz"]))
```

# sentences

- "the file you compile holds `function main()`, which takes nothing and produces nothing." Governs `function main()`.
- "Every top-level line starts with its kind." and the production `"function" ident [ Generics ] Params [ "->" Type ] Block`. Govern both `function` lines.
- "Signatures are always explicit; inference is local only." Governs `function longest(words: [str]) -> str`, where parameter and result types are written.
- "Indentation is significant and rigid: exactly 4 spaces per level; a tab is a compile error. No braces, no semicolons; a condition needs no parentheses." Governs the indentation of every body line and `if w.len() > best.len()` written without parentheses.
- "`v @= 0             # mutable cell, type inferred the same way`" and "`@=` declares a mutable cell, and `@` re-binds it, or a field or element inside one." Govern `best @= words[0]`: the value that changes during the loop is declared with `@=`.
- "`v @ v + 1          # mutation; only a cell declared with @= can be mutated`" and the production `Place "@" Expression NEWLINE`. Govern `best @ w`: the cell is changed with `@`, not `=`.
- "A cell nothing re-binds is a compile error: write `=`." Governs `best @= words[0]`: it is legal because `best @ w` re-binds it.
- "`=` binds once, forever." Governs why `best` cannot be declared with `=` and then reassigned.
- "An unused binding or parameter is a compile error; a read is a use and a write is not". Governs `words` (read by `words[0]` and the `for`), `w` (read in the condition and in `best @ w`), and `best` (read by `best.len()` and `return best`).
- "An out-of-bounds index or slice aborts" together with "`m[k]` is a `V?`" said of maps only. Governs `words[0]`: array indexing yields a plain `str`, not a `str?`, so no `.must()` is needed; the brief guarantees a non-empty array.
- "Loops: `while cond` and `for x in xs`, over an array or a `range`" and `For = "for" ident "in" Expression Block`. Govern `for w in words`.
- "`if` and `match` are expressions and may stand as statements" and "`if cond` / `else if` / `else` take only `bool`". Govern the `if` line standing as a statement with a `bool` condition and no `else`.
- "`x.f(y)` is sugar for `f(x, y)` (UFCS)." and the built-in list naming `len`. Govern `w.len()` and `best.len()`.
- "comparison   == != < <= > >=    (`< <= > >=`: a number only)" and "both sides one numeric type". Govern `>` between two lengths, both from `len`, so both of one type.
- "`str` | immutable UTF-8 string, indexed and measured in bytes". Governs what "longest" means: byte length, which equals character length for these ASCII inputs.
- "A function with a `->` must `return` on every path that reaches its end". Governs `return best`.
- "A literal takes the type its context asks for" and "A container literal separates elements by newline across lines and by comma on one". Govern `["ab", "abcd", "xyz"]`, a `[str]` matching the parameter.
- "`print` writes its values with no separator and exactly one trailing newline, and takes ... a number, `str` or `bool`." Governs `print(longest(...))` with a `str`.
- "A line that computes a value must use it: bind it, or discard it on purpose with `_ = f(x)`, which a `()` line refuses: it stands alone." Governs `print(...)` standing alone as a statement, since it yields `()`.
- "When two parameters in a signature share a type, named arguments are mandatory". Governs the call `longest([...])` without a name, since `longest` has one parameter.

# choice_points

- Strict `>` versus `>=` in the comparison. I chose `>`, so a later string of equal length never replaces an earlier one and the first is kept, as the brief requires. `>=` would return the last of the equally long strings.
- Loop over all of `words` versus starting at index 1 (`for i in range(from: 1, to: words.len())` with `words[i]`). I chose all elements; comparing `words[0]` with itself is harmless. The other choice gives the same result with more code, and needs `len` to return `i64` to match `range`, which the spec does not state.
- `for` versus `while` with an index cell. I chose `for`, which needs no second mutable cell. A `while` would need `i @= 1` and `i @ i + 1`, same output.
- Inferred cell type versus `best: str @= words[0]`. I chose inference, which the spec allows ("type inferred the same way"). The annotation would be equally accepted.
- `w.len()` versus `len(w)`. UFCS makes them identical; I chose the method form.
- Printing the call directly versus binding it first (`result = longest(...)` then `print(result)`). Same output either way.
- Output: the program prints `abcd` followed by a newline.

# confidence

High, about 90 percent. The line I am least sure of is `best @= words[0]`: the spec says map indexing is a `V?` but never states outright that array indexing yields a plain `T`. I infer that from "An out-of-bounds index or slice aborts", since aborting rather than failing implies no `T?`. If array indexing were fallible, this line would need `.must()`. A smaller doubt is `if` with no `else` as a statement whose body is a mutation; the spec allows `if` as a statement, so I expect it to pass.

# context

Nothing beyond this directory's files reached my context apart from the harness's own system context (a git status snapshot, recent commit titles of the repository, and the user's email and environment details), none of which describes the language. I read only `brief.md` and `spec.md`.
