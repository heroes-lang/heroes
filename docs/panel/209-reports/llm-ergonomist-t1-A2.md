# Panel 209, the blind seat, t1-A2 (second reading)

Copied by the coordinator on 2026-10-10 at 16:51 from `.claude/worktrees/scratch-b15/209-blind/t1-A2/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. Its inputs were `brief.md`, `spec.md` (variant t1-A: t1-A is the first reading's folder copied byte for byte; A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:49:58, ended 16:50:49, exit 0; the CLI's own estimate `total_cost_usd` 0.257 over 6 turns.

---

# Report

## program

```
function longest(words: [str]) -> str
    best: str @ words[0]
    for w in words
        if w.len() > best.len()
            best @ w
    return best

function main()
    print(longest(["ab", "abcd", "xyz"]))
```

Expected output: `abcd`.

## sentences

Line numbers refer to the program above (1 to 9).

- "Every top-level line starts with its kind." and the production `"function" ident [ Generics ] Params [ "->" Type ] Block`: lines 1 and 8.
- "Signatures are always explicit; inference is local only.": line 1 (`words: [str]`, `-> str`).
- "One file is one module; the file you compile holds `function main()`, which takes nothing and produces nothing.": line 8.
- "Indentation is significant and rigid: exactly 4 spaces per level; a tab is a compile error.": every indented line (4, 8 and 12 spaces).
- How the changing value is declared: "`v: i64 @ 0         # mutable declaration — the type is REQUIRED`" and "`@` declares a mutable cell and re-binds it, or a field or element inside one." with the production `Binding = ( "=" | ":" Type ( "@" | "=" ) ) Expression NEWLINE .`: line 2 (`best: str @ words[0]`).
- How it is changed: "`v @ v + 1          # mutation; only a declared @ name can be mutated`" with the production `Simple = ... | Place "@" Expression NEWLINE` and `Place = ident { ... }`: line 5 (`best @ w`).
- "`=` binds once, forever.": the reason line 2 uses `@` and not `=`.
- "All bindings are initialised.": line 2 gives `best` an initial value.
- "An out-of-bounds index or slice aborts": line 2, `words[0]` on an array yields a `str` directly (only a map index is described as a `V?`), and the array is non-empty by the task's contract.
- "Loops: `while cond` and `for x in xs`, over an array or a `range` (section 11)." with `For = "for" ident "in" Expression Block .`: line 3.
- "`if cond` / `else if` / `else` take only `bool` — there is no truthiness." and "`if` and `match` are expressions and may stand as statements": line 4.
- "a condition needs no parentheses.": line 4.
- "`x.f(y)` is sugar for `f(x, y)` (UFCS)." and the built-in `len`: line 4 (`w.len()`, `best.len()`).
- "comparison   == != < <= > >=    (`< <= > >=`: a number only)" and "No implicit conversions, widths included": line 4 compares two `len` results, both the same numeric type.
- "An unused binding or parameter is a compile error; a read is a use and a write is not": `words` is read on lines 2 and 3, `best` on lines 4 and 6, `w` on lines 4 and 5.
- "A function with a `->` must `return` on every path that reaches its end": line 6.
- "`print` writes its values with no separator and exactly one trailing newline, and takes the types this language renders as text: a number, `str` or `bool`.": line 9.
- "A container literal separates elements by newline across lines and by comma on one": line 9, `["ab", "abcd", "xyz"]`.
- "A line that computes a value must use it: bind it, or discard it on purpose with `_ = f(x)`, which a `()` line refuses: it stands alone.": line 9, `print(...)` returns `()` and stands alone; line 4's `if` has a body ending in a mutation, so it yields no value to discard.
- "Shadowing is a compile error": no name is reused (`best`, `w`, `words` are distinct).

## choice_points

1. Loop form. Chose `for w in words`, iterating every element including the first. Alternative: `for i in range(from: 1, to: words.len())` with `words[i]`; it would produce the same result and skip one self-comparison, but adds a call with named arguments and an index whose type must match `len`'s.
2. Comparison operator. Chose strict `>` so a later equally long word does not replace the earlier one, giving "the first one where two are equally long". `>=` would return the last of equally long words (for the given input still `abcd`, but wrong in general).
3. Initial value. Chose `words[0]` (the array is non-empty). Alternative `best: str @ ""` would give the same result for this input and for any input, since a non-empty word replaces `""` and an all-empty array returns an equal `""`; `words[0]` states the intent directly.
4. `len` spelling. Chose UFCS `w.len()` as the spec's own example `xs.len()` does. `len(w)` is the same call.
5. Printing. Chose `print(longest(...))` directly. Alternative: bind `result = longest(...)` then `print(result)`; same output, one more binding.
6. No `else` on the `if`. The spec allows `if` as a statement without `else`; adding `else` with `_ = 0` would be legal but needless.

## confidence

High, about 90%. The line I am least sure of is line 4, `if w.len() > best.len()`: the spec never states the return type of `len` (I assume `i64`, and both sides share it in any case), and it does not say explicitly that an `if` without `else` used as a statement whose block ends in a mutation is accepted, though "may stand as statements" and section 8's rules imply it. Second least sure is line 2, that `words[0]` on an array is a plain `str` rather than a `str?`.

## context

Only this directory's files reached my context: `brief.md` and `spec.md`. The harness also attached a git status snapshot (branch name, recent commit titles about "panels" and "defects", untracked docs directories) and the user's email; I did not use any of it for the program.
