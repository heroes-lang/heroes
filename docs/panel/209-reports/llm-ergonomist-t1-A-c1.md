# Panel 209, the blind seat, t1-A-c1 (clean reading, first)

Copied by the coordinator on 2026-10-10 at 16:53 from `.claude/worktrees/scratch-b15/209-blind/t1-A-c1/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. **The folder is its own git repository** (`git init`, one commit `inputs` holding only the inputs), made after the first twelve sessions reported the project's git status in their context (commit subjects naming panels and defects, the untracked `docs/panel/209-briefs/`): this session's `context` section says what reached it. Its inputs, byte-identical (`cmp`) to folder `t1-A`'s: `brief.md`, `spec.md` (variant A: A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:51:58, ended 16:52:54, exit 0; the CLI's own estimate `total_cost_usd` 0.27 over 5 turns.

---

# program

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

Line numbers below refer to this block (line 1 is `function longest...`).

# sentences

- "Every top-level line starts with its kind." and the production `"function" ident [ Generics ] Params [ "->" Type ] Block`: lines 1 and 8.
- "Signatures are always explicit; inference is local only.": line 1, where the parameter type `[str]` and the result `str` are written out.
- "`[T]` | dynamic array, indices from 0": line 1 (`[str]`) and line 2 (`words[0]` is the first element).
- "Indentation is significant and rigid: exactly 4 spaces per level; a tab is a compile error.": every indented line (2 to 6, and 9).
- "`v: i64 @ 0         # mutable declaration`" followed in the spec by the words "the type is REQUIRED" (quoted in two parts to avoid reproducing the spec's dash), and "`@` declares a mutable cell and re-binds it": line 2, `best: str @ words[0]`, the value that changes during the loop, declared with its type written.
- The production `Binding = ( "=" | ":" Type ( "@" | "=" ) ) Expression NEWLINE .`: line 2.
- "`v @ v + 1          # mutation; only a declared @ name can be mutated`": line 5, `best @ w`, which changes the value inside the loop; `best` was declared with `@` on line 2.
- The production `Simple = ... | Place "@" Expression NEWLINE` with `Place = ident { ... }`: line 5.
- "`=` binds once, forever.": the reason line 2 uses `@` and not `=`; a `best = ...` could not be re-bound on line 5.
- "An out-of-bounds index or slice aborts": line 2; `words[0]` on an array gives a `str` directly (only `m[k]` on a map is described as a `V?`), and the brief guarantees a non-empty array.
- "Loops: `while cond` and `for x in xs`, over an array or a `range` (section 11)." and `For = "for" ident "in" Expression Block .`: line 3.
- "`if cond` / `else if` / `else` take only `bool`" together with "there is no truthiness.", and "`if` and `match` are expressions and may stand as statements": line 4, an `if` with no `else` used as a statement whose condition is a `bool`.
- "a condition needs no parentheses.": line 4.
- "`x.f(y)` is sugar for `f(x, y)` (UFCS).": line 4, `w.len()` and `best.len()` are `len(w)` and `len(best)`.
- "`str` | immutable UTF-8 string, indexed and measured in bytes": line 4, length compared in bytes, which for these ASCII words equals characters.
- "comparison   == != < <= > >=    (`< <= > >=`: a number only)": line 4, `>` between two lengths. Strict `>` keeps the first of equally long words.
- "An unused binding or parameter is a compile error; a read is a use and a write is not": `words` is read on lines 2 and 3, `w` on lines 4 and 5, `best` on lines 4 and 6.
- "Shadowing is a compile error": names `best` and `w` are new and do not reuse `words`, `longest`, `main` or a built-in.
- "A function with a `->` must `return` on every path that reaches its end": line 6.
- "the file you compile holds `function main()`, which takes nothing and produces nothing.": line 8.
- "`print(...)` ... takes the types this language renders as text: a number, `str` or `bool`." and "exactly one trailing newline": line 9.
- "A literal takes the type its context asks for" and "A container literal separates elements by newline across lines and by comma on one": line 9, `["ab", "abcd", "xyz"]` is a `[str]` as the parameter asks.
- "A line that computes a value must use it: bind it, or discard it on purpose with `_ = f(x)`, which a `()` line refuses: it stands alone.": line 9, `print` returns `()` so the line stands alone; line 5 is a mutation, not a computed value.
- "A literal without the `f` is an error where its braces, read with an `f`, would be a hole": line 9, none of the strings holds a brace.

# choice_points

- Loop form: `for w in words` over all elements, including `words[0]` again. Alternative: `for i in range(from: 1, to: words.len())` with `words[i]`; same output, more code, and an extra name `i`. Comparing the first word with itself is harmless because `>` is strict.
- Comparison operator: strict `>`. Choosing `>=` would return the last of equally long words, violating the brief ("the first one").
- Initial value: `words[0]`. Alternative `best: str @ ""` would also give `abcd` here, but `words[0]` follows the non-empty precondition directly.
- Length call style: UFCS `w.len()`. Alternative `len(w)`; identical meaning by the UFCS sentence.
- A `while` loop with `i: i64 @ 0` and `i @ i + 1` would also work but adds a second mutable cell.
- Function order: `longest` before `main`. "Declaration order never matters", so the other order produces the same program.
- Return type: plain `str`, not `str?`; the precondition (non-empty) is the caller's, and an empty array would abort on `words[0]` rather than fail. A `str?` result would force `main` to handle it with `.must()` or `match`.

Program output: `abcd`.

# confidence

High, about 90%. The line I am least sure of is line 4, `if w.len() > best.len()`: the specification lists `len` as a built-in without stating its signature, so I am assuming it accepts both `str` and `[T]` and returns `i64` (not a `T?`), and that an `if` without `else` is acceptable as a statement. Second least sure is line 2, assuming `words[0]` on an array yields `str` and not `str?`.

# context

Only `brief.md` and `spec.md` from this directory were read. The harness also supplied environment metadata (working directory path, git status listing untracked `run.err` and `run.json`, a recent commit message, and the user's email address); none of it informed the program. I did not read `run.err` or `run.json`.
