# Panel 209, the blind seat, t2-A

Copied by the coordinator on 2026-10-10 at 16:50 from `.claude/worktrees/scratch-b15/209-blind/t2-A/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. Its inputs were `brief.md`, `spec.md` (variant A: A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:48:30, ended 16:49:17, exit 0; the CLI's own estimate `total_cost_usd` 0.223 over 4 turns.

---

# report

## program

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

Lines are numbered 1 to 9 below, the blank line being line 6.

## sentences

Line 1, `function tally(words: [str]) -> {str: i64}`:
- "Declaration = ... | "function" ident [ Generics ] Params [ "->" Type ] Block" (section 4).
- "Signatures are always explicit; inference is local only." (section 5)
- "| `[T]` | dynamic array, indices from 0 |" and "| `{K: V}` | map |" (section 3), with the productions `"[" Type "]"` and `"{" Type ":" Type "}"`.
- "Indentation is significant and rigid: exactly 4 spaces per level; a tab is a compile error." (section 1; this governs every indented line)

Line 2, `counts: {str: i64} @ {}`, which declares the empty container and the value that changes:
- "A container literal separates elements by newline across lines and by comma on one, and an empty one needs an annotation: `xs: [i64] = []` · `m: {str: i64} @ {}`." (section 10)
- "`v: i64 @ 0         # mutable declaration — the type is REQUIRED`" (section 5)
- "`=` binds once, forever. `@` declares a mutable cell and re-binds it, or a field or element inside one." (section 5)
- "Binding = ( "=" | ":" Type ( "@" | "=" ) ) Expression NEWLINE ." (section 5)

Line 3, `for w in words`:
- "Loops: `while cond` and `for x in xs`, over an array or a `range` (section 11)." (section 8)
- "For = "for" ident "in" Expression Block ." (section 8)

Line 4, `counts[w] @ counts[w].default(0) + 1`, which changes the value:
- "`v @ v + 1          # mutation; only a declared @ name can be mutated`" (section 5)
- "`@` declares a mutable cell and re-binds it, or a field or element inside one." (section 5)
- "Simple = "_" Binding | Place "@" Expression NEWLINE" and "Place = ident { "." ident | "[" Expression "]" } ." (section 5)
- "`m[k]` is a `V?` with code `missing_key`; `m[k] @ v` inserts or replaces" (section 10)
- "| `.default(v)` | extract or fall back |" (section 6)
- "Precedence, strongest first: call, `.` and `::` → unary ..." (section 7), so `counts[w].default(0)` is computed before `+ 1`.
- "arithmetic   + - * / %          (both sides one numeric type — never mixed)" (section 7) and "A literal takes the type its context asks for ... otherwise `i64`." (section 2), so `0` and `1` are `i64` like `V`.
- The example in section 10: "`m: {str: i64} @ {}` / `m["a"] @ 1` / `print(m["b"].default(0))`".

Line 5, `return counts`:
- "Simple = ... | "return" [ Expression ] NEWLINE" (section 5)
- "A function with a `->` must `return` on every path that reaches its end" (section 8)
- "An unused binding or parameter is a compile error; a read is a use and a write is not" (section 5). This return is the read of `counts`; line 4 reads `w`; line 3 reads `words`.

Line 7, `function main()`:
- "the file you compile holds `function main()`, which takes nothing and produces nothing." (section 1)

Line 8, `t = tally(["a", "b", "a"])`:
- "`x = 5              # immutable binding, type inferred`" (section 5)
- "Primary = ... | "[" [ Expression { Sep Expression } ] "]"" and "Sep = "," | NEWLINE ." (section 7)
- "A container literal separates elements ... by comma on one" (section 10)

Line 9, `print(t["a"].default(0))`:
- "`m[k]` is a `V?` with code `missing_key`" (section 10) and "| `.default(v)` | extract or fall back |" (section 6).
- "`print` writes its values with no separator and exactly one trailing newline, and takes the types this language renders as text: a number, `str` or `bool`." (section 11)
- "A line that computes a value must use it: bind it, or discard it on purpose with `_ = f(x)`, which a `()` line refuses: it stands alone." (section 5). `print` returns `()`, so it stands alone. This line also reads `t`, so `t` is used.

## choice_points

1. How to build the map in `tally`. Chosen: a mutable cell `counts: {str: i64} @ {}` updated in a `for` loop. The other choice is `fold` with a helper `function add(acc: {str: i64}, item: str) -> {str: i64}`; it needs a second top-level function, which the brief's "two functions" excludes, and it would still need a mutable cell or a rebuilt map inside the helper.
2. How to declare the empty map. Chosen: `@`. Writing `counts: {str: i64} = {}` gives an immutable binding, and then line 4 is refused because "only a declared @ name can be mutated". Leaving out the annotation (`counts @ {}`) does not parse as a Binding, and the spec says an empty container "needs an annotation".
3. How to read the old count. Chosen: `counts[w].default(0)`. The other choice is a `match` on `.ok n` / `.err _`, which is longer and gives the same value. `.must()` would abort on the first sight of each word.
4. How to read the result in `main`. The brief fixed it as `t["a"].default(0)`. `t["a"].must()` would also print `2` here.
5. Names: `counts` and `w`. Any other name works as long as it shadows nothing. There are no `use` lines, so nothing is reserved.
6. Layout: one blank line between the two declarations, as in section 4's example. No comments. A comment would change nothing the checker checks.

## confidence

High, about 90 percent. The line I am least sure of is line 4, `counts[w] @ counts[w].default(0) + 1`. It joins an indexed Place mutation of a map with a read of the same key as a `V?`, and the literal `0` must take `i64` from `.default`'s context. Each part is backed by a sentence or an example: section 10 shows `m["a"] @ 1` and `m["b"].default(0)`. But the spec never shows the two together on one line, and it does not say whether a missing key counts as "an element inside" the cell before it is inserted. Section 10's "`m[k] @ v` inserts or replaces" says it does.

## context

Only this directory's files reached my context: `brief.md` and `spec.md`. The harness also added a system note with git status (branch `main`, recent commit titles naming panels and defects, untracked `docs/panel/209-briefs/` and `docs/panel/209-reports/`), the user's email address and the working directory path. I did not use any of it, and I read no file outside this directory.
