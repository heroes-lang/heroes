# Panel 209, the blind seat, t2-A-c1 (clean reading, first)

Copied by the coordinator on 2026-10-10 at 16:53 from `.claude/worktrees/scratch-b15/209-blind/t2-A-c1/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. **The folder is its own git repository** (`git init`, one commit `inputs` holding only the inputs), made after the first twelve sessions reported the project's git status in their context (commit subjects naming panels and defects, the untracked `docs/panel/209-briefs/`): this session's `context` section says what reached it. Its inputs, byte-identical (`cmp`) to folder `t2-A`'s: `brief.md`, `spec.md` (variant A: A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:51:58, ended 16:52:37, exit 0; the CLI's own estimate `total_cost_usd` 0.197 over 4 turns.

---

# program

```
function tally(words: [str]) -> {str: i64}
    counts: {str: i64} @ {}
    for w in words
        counts[w] @ counts[w].default(0) + 1
    return counts

function main()
    t = tally(["a", "b", "a"])
    print(t["a"].default(0))
```

Line numbers below refer to this block (line 1 is `function tally...`).

# sentences

- "the file you compile holds `function main()`, which takes nothing and produces nothing." Governs line 7.
- "Indentation is significant and rigid: exactly 4 spaces per level; a tab is a compile error." Governs every indented line (2 to 5, 8, 9).
- "Every top-level line starts with its kind." and the production `"function" ident [ Generics ] Params [ "->" Type ] Block`. Govern lines 1 and 7.
- "Signatures are always explicit; inference is local only." Governs line 1 (parameter type `[str]` and result type `{str: i64}` written out).
- "`{K: V}` | map" and "`[T]` | dynamic array". Govern the types on lines 1 and 2.
- Empty container: "A container literal separates elements by newline across lines and by comma on one, and an empty one needs an annotation: `xs: [i64] = []` · `m: {str: i64} @ {}`." Governs line 2.
- Declaring a value that changes: "`v: i64 @ 0         # mutable declaration — the type is REQUIRED`" and "`@` declares a mutable cell and re-binds it, or a field or element inside one." Govern line 2.
- Changing it: "`v @ v + 1          # mutation; only a declared @ name can be mutated`", "`m[k] @ v` inserts or replaces", and the production `Place "@" Expression NEWLINE` with `Place = ident { "." ident | "[" Expression "]" }`. Govern line 4.
- "`m[k]` is a `V?` with code `missing_key`" and "`.default(v)` | extract or fall back". Govern line 4 (`counts[w].default(0)`) and line 9 (`t["a"].default(0)`).
- "A literal takes the type its context asks for ... otherwise `i64`." Governs the `0` and `1` on line 4 and the `0` on line 9, all `i64`.
- "Precedence, strongest first: call, `.` and `::` → unary ... → `+ -`". Governs line 4: `counts[w].default(0) + 1` adds 1 to the extracted value.
- "arithmetic + - * / % (both sides one numeric type — never mixed)". Governs line 4 (`i64 + i64`).
- "Loops: `while cond` and `for x in xs`, over an array or a `range`". Governs line 3.
- "A function with a `->` must `return` on every path that reaches its end". Governs line 5.
- "An unused binding or parameter is a compile error; a read is a use and a write is not". Governs `words` (read on line 3), `w` (read on line 4), `counts` (read on lines 4 and 5), `t` (read on line 9).
- "`x = 5              # immutable binding, type inferred`" and "`=` binds once, forever." Govern line 8.
- "`x.f(y)` is sugar for `f(x, y)` (UFCS)." Governs the `.default(0)` calls on lines 4 and 9.
- "A line that computes a value must use it: bind it, or discard it on purpose with `_ = f(x)`, which a `()` line refuses: it stands alone." Governs line 9: `print` returns `()`, so it stands alone.
- "`print` ... takes the types this language renders as text: a number, `str` or `bool`." Governs line 9 (an `i64`).
- "A container literal separates elements ... by comma on one". Governs the array literal on line 8.
- "Shadowing is a compile error". Governs the choice of distinct names `words`, `counts`, `w`, `t`.
- "A literal without the `f` is an error where its braces, read with an `f`, would be a hole". Checked: no string literal contains braces.

# choice_points

- Loop form: `for w in words` versus `while` with an index cell. The `while` form would need a second mutable `i: i64 @ 0`, an `i @ i + 1`, and `words[i]`; more lines, same result.
- Reading the current count: `counts[w].default(0)` versus a `match` on `.ok n` / `.err _`. The `match` would also be accepted but is longer; `.must()` would abort on the first occurrence of each word.
- Map name `counts` (any non-colliding name works). Naming it `t` would not collide with `main`'s `t` since they are in different functions, but I avoided it for clarity.
- Returning the mutable cell directly with `return counts` versus copying it into an immutable binding first. Copying adds an extra binding with no effect.
- Literal `0` in `.default(0)`: could be written `0` or another base like `0x0`; plain `0` is the obvious choice. A leading zero like `00` would be an error.
- Binding the printed value in `main` (`n = t["a"].default(0)` then `print(n)`) versus inline. Inline is what the brief asks for; both work.
- Expected output: `2`.

# confidence

High, about 90%. The line I am least sure of is line 4, `counts[w] @ counts[w].default(0) + 1`: it reads and writes the same map element in one statement. The spec allows element mutation through `@` (section 5 and the section 10 example `m["a"] @ 1`) and reading `m[k]` as a `V?`, but no example combines them on one line. A second, smaller doubt is whether the checker counts `counts` as used, since it is both written and read; it is read on lines 4 and 5, so it should be.

# context

Only `brief.md` and `spec.md` from this directory. I also saw automatic environment context: the git status (branch `main`, untracked `run.err` and `run.json`, a commit named "inputs"), the user's git name and email address, and the working directory path. None of it shaped the program. I did not open `run.err` or `run.json`.
