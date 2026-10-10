# Panel 209, the blind seat, t1-B

Copied by the coordinator on 2026-10-10 at 16:50 from `.claude/worktrees/scratch-b15/209-blind/t1-B/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. Its inputs were `brief.md`, `spec.md` (variant B: A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:48:30, ended 16:49:11, exit 0; the CLI's own estimate `total_cost_usd` 0.217 over 4 turns.

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

Expected output: `abcd`.

# sentences

Line 1, `function longest(words: [str]) -> str`:
- "Every top-level line starts with its kind." (the line starts with `function`)
- `Declaration = ... | "function" ident [ Generics ] Params [ "->" Type ] Block` and `Param = [ "@" ] ident ":" Type .`
- "Signatures are always explicit; inference is local only."
- "`[T]` | dynamic array, indices from 0" and "`str` | immutable UTF-8 string, indexed and measured in bytes"

Line 2, `best @= words[0]` (how the value that changes in the loop is declared):
- "`v @= 0             # mutable cell, type inferred the same way`"
- "`=` binds once, forever. `@=` declares a mutable cell, and `@` re-binds it, or a field or element inside one."
- "A cell nothing re-binds is a compile error: write `=`." (line 5 re-binds it, so `@=` is required, not merely allowed)
- `Statement = ident Binding | Simple .` and `Binding = [ ":" Type ] ( "=" | "@=" ) Expression NEWLINE .`
- "All bindings are initialised." (so it starts from an actual element, `words[0]`)
- "`[T]` | dynamic array, indices from 0" and "An out-of-bounds index or slice aborts" (the array is non-empty by the task, so index 0 is safe; indexing an array gives the element itself, unlike `m[k]`, which the spec singles out as a `V?`)

Line 3, `for w in words`:
- "Loops: `while cond` and `for x in xs`, over an array or a `range` (section 11)."
- `For = "for" ident "in" Expression Block .`
- "a condition needs no parentheses."

Line 4, `if w.len() > best.len()`:
- "`x.f(y)` is sugar for `f(x, y)` (UFCS)." with `len` listed among the built-ins, and "UFCS does not apply when the first parameter is `@`." (it is not here)
- "comparison   == != < <= > >=    (`< <= > >=`: a number only)" (`len` yields a number)
- "`if cond` / `else if` / `else` take only `bool` — there is no truthiness."
- "`if` and `match` are expressions and may stand as statements"
- `If = "if" Expression Block { "else" "if" Expression Block } [ "else" Block ] .` (the `else` is optional)
- The strict `>` is what makes the first of two equally long strings win: a later string of equal length does not replace it.

Line 5, `best @ w` (how the value is changed):
- "`v @ v + 1          # mutation; only a cell declared with @= can be mutated`"
- "`@` re-binds it"
- `Simple = ... | Place "@" Expression NEWLINE` and `Place = ident { "." ident | "[" Expression "]" } .`
- "a read is a use and a write is not" (`best` is still used, because it is read on lines 4 and 6)

Line 6, `return best`:
- "A function with a `->` must `return` on every path that reaches its end"
- `Simple = ... | "return" [ Expression ] NEWLINE`

Lines 1 to 6, use of names:
- "An unused binding or parameter is a compile error; a read is a use and a write is not" (`words` is read on lines 2 and 3, `w` on lines 4 and 5, `best` on lines 4 and 6)
- "Shadowing is a compile error" (no name is bound twice)
- "exactly 4 spaces per level; a tab is a compile error."

Line 8, `function main()`:
- "the file you compile holds `function main()`, which takes nothing and produces nothing."
- "`()` | nothing: what a function with no `->` returns"

Line 9, `print(longest(["ab", "abcd", "xyz"]))`:
- "`print` writes its values with no separator and exactly one trailing newline, and takes the types this language renders as text: a number, `str` or `bool`."
- "A container literal separates elements by newline across lines and by comma on one"
- `Primary = ... | "[" [ Expression { Sep Expression } ] "]"`
- "A line that computes a value must use it: bind it, or discard it on purpose with `_ = f(x)`, which a `()` line refuses: it stands alone." (`print` gives `()`, so the line stands alone)
- "When two parameters in a signature share a type, named arguments are mandatory at the call site" (`longest` has one parameter, so no name is needed)
- "A literal without the `f` is an error where its braces, read with an `f`, would be a hole" (none of the strings has braces)

# choice_points

1. Mutable cell versus another shape. I used an `@=` cell re-bound with `@`. The alternative is `words.fold(...)` with a top-level helper function (no anonymous functions exist). That would also work but needs a second helper, which the task's "two functions" rules out, and `fold`'s names (`acc`, `item`) add risk.
2. Initial value of `best`. I used `words[0]`. Starting from `""` would also give the correct answer here (every word is longer than 0 bytes, and with strict `>` the first wins), but if all words were empty it would still return `""`, which is fine; I kept `words[0]` because it is the obvious first element and needs no type annotation question.
3. Loop over every element versus from index 1. I loop over all of `words`, so the first iteration compares `words[0]` with itself, harmless. The alternative `for i in range(from: 1, to: words.len())` with `words[i]` avoids that comparison but adds an index and a call with named arguments.
4. `>` versus `>=`. I chose `>`, so a tie keeps the earlier word, as the task asks. `>=` would return the last of equally long words (for this input it would still print `abcd`, but it would violate the specification of `longest`).
5. `w.len()` versus `len(w)`. UFCS makes them the same; I chose the method form, as the spec's own example `xs.len()` does.
6. Type annotation on `best`. I let it be inferred (`best @= words[0]` is a `str`); writing `best: str @= words[0]` is also allowed by `Binding` and would produce the same program.
7. `if` without `else`. The grammar makes `else` optional, and an `if` may stand as a statement. Adding `else` with `_ = 0` would be noise.

# confidence

High, about 90 percent. The line I am least sure of is line 4 together with line 5, `if w.len() > best.len()` followed by `best @ w`: the spec says `if` is an expression standing as a statement, and does not say outright that an `if` with no `else` whose body is a mutation is accepted as a statement without its value being used. I read "may stand as statements" as covering it. A smaller doubt is line 2, that `words[0]` on a `[str]` is a plain `str` and not a `str?`; the spec makes only `m[k]` on a map fallible and says an out-of-bounds index aborts, which implies a plain value.

# context

Only `brief.md` and `spec.md` from this directory were read. The harness also placed into my context, outside these files, the working directory path, the git status of the repository (branch `main`, untracked `docs/panel/209-briefs/` and `docs/panel/209-reports/`, and five recent commit subjects mentioning panels 205 to 208 and defects), the git user name, and an email address. None of it concerned the language's rules, and I did not use it in writing the program.
