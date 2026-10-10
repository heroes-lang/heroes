# Panel 209, the blind seat, t1-B-c1 (clean reading, first)

Copied by the coordinator on 2026-10-10 at 16:53 from `.claude/worktrees/scratch-b15/209-blind/t1-B-c1/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. **The folder is its own git repository** (`git init`, one commit `inputs` holding only the inputs), made after the first twelve sessions reported the project's git status in their context (commit subjects naming panels and defects, the untracked `docs/panel/209-briefs/`): this session's `context` section says what reached it. Its inputs, byte-identical (`cmp`) to folder `t1-B`'s: `brief.md`, `spec.md` (variant B: A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:51:58, ended 16:52:35, exit 0; the CLI's own estimate `total_cost_usd` 0.193 over 4 turns.

---

# Report

## program

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

## sentences

Line numbers refer to the program above (line 1 is `function longest...`).

- "Every top-level line starts with its kind." and the production `"function" ident [ Generics ] Params [ "->" Type ] Block`: lines 1 and 8.
- "Signatures are always explicit; inference is local only.": line 1 (`words: [str]`, `-> str`).
- "Indentation is significant and rigid: exactly 4 spaces per level": all body lines (4, 8 and 12 spaces).
- "`v @= 0             # mutable cell, type inferred the same way`": line 2. This is the sentence for how a value that changes during the loop is declared: `best @= words[0]`, type `str` inferred from the initialiser.
- "`=` binds once, forever. `@=` declares a mutable cell, and `@` re-binds it, or a field or element inside one.": lines 2 and 5.
- "`v @ v + 1          # mutation; only a cell declared with @= can be mutated`" and the production `Place "@" Expression NEWLINE`: line 5. This is the sentence for how the value is changed inside the loop: `best @ w`.
- "A cell nothing re-binds is a compile error: write `=`.": line 2 is legal because line 5 re-binds `best`.
- "All bindings are initialised.": line 2 has an initialiser.
- "An unused binding or parameter is a compile error; a read is a use and a write is not": `words` is read on lines 2 and 3, `w` on lines 4 and 5, `best` on lines 4 and 6.
- "`[T]` | dynamic array, indices from 0" and "An out-of-bounds index or slice aborts": line 2, `words[0]` is the first element and yields a plain `str` (only `m[k]` on a map is described as a `V?`). The array is non-empty by the task's precondition.
- "Loops: `while cond` and `for x in xs`, over an array or a `range`" and `For = "for" ident "in" Expression Block .`: line 3.
- "`x.f(y)` is sugar for `f(x, y)` (UFCS)." together with the built-in `len` and the example `if xs.len() == 0`: line 4.
- "`str` | immutable UTF-8 string, indexed and measured in bytes": line 4, lengths compared in bytes (all inputs are ASCII, so bytes equal characters).
- "comparison   == != < <= > >=    (`< <= > >=`: a number only)": line 4, `>` on two `len` results.
- "`if cond` / `else if` / `else` take only `bool`" and "`if` and `match` are expressions and may stand as statements": line 4.
- "A function with a `->` must `return` on every path that reaches its end": line 6.
- "the file you compile holds `function main()`, which takes nothing and produces nothing.": line 8.
- "`print` writes its values with no separator and exactly one trailing newline, and takes ... a number, `str` or `bool`.": line 9.
- "A line that computes a value must use it: bind it, or discard it on purpose with `_ = f(x)`, which a `()` line refuses: it stands alone.": line 9, `print(...)` returns `()` so it stands alone.
- "A container literal separates elements by newline across lines and by comma on one": line 9, the array literal.
- "A literal without the `f` is an error where its braces, read with an `f`, would be a hole": line 9, the string literals hold no braces.
- "When two parameters in a signature share a type, named arguments are mandatory": does not apply; `longest` has one parameter, so line 9 calls it positionally.

## choice_points

- Loop form: `for w in words` over the whole array versus `for i in range(from: 1, to: words.len())` with indexing. I chose the plain `for`; it compares the first element with itself once, harmlessly. The `range` form would avoid that comparison but adds an index binding and named arguments, with the same result.
- Comparison operator: `>` versus `>=`. I chose `>` so a later word of equal length does not replace the earlier one, which the task requires. `>=` would return the last of the equally long words.
- Initial value of the cell: `words[0]` versus `""`. I chose `words[0]`; with `""` the result is identical for a non-empty array, but it would hide the non-empty precondition.
- Length call: `w.len()` (UFCS) versus `len(w)`. Equivalent; I chose UFCS to match the spec's example.
- Statement versus helper value: the `if` stands as a statement with no `else`. An alternative is `best @ if ... else ...`, which would mutate on every iteration; same output.
- Comments: none added. A comment above `longest` would document it and change nothing for the checker.

## confidence

High, about 90%. The line I am least sure of is line 4/5, the `if` without `else` standing as a statement whose block ends in a mutation: the spec says `if` is an expression and "a block's value is its last expression", and "A line that computes a value must use it". I read a mutation as not an expression, so the `if` yields `()` and may stand alone, but the spec never shows an `else`-less `if` whose body is a mutation. Second least sure: that `words[0]` on an array is a plain `str` and not a `str?`; the spec states the `V?` result only for maps.

## context

Only this directory's files reached my context: `brief.md` and `spec.md`. I read no other file and ran nothing. The harness also supplied environment metadata (git status listing `run.err` and `run.json`, a recent commit message, the working directory path); I did not open those files and they did not inform the program.
