# Panel 209, the blind seat, t2-B-c1 (clean reading, first)

Copied by the coordinator on 2026-10-10 at 16:53 from `.claude/worktrees/scratch-b15/209-blind/t2-B-c1/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. **The folder is its own git repository** (`git init`, one commit `inputs` holding only the inputs), made after the first twelve sessions reported the project's git status in their context (commit subjects naming panels and defects, the untracked `docs/panel/209-briefs/`): this session's `context` section says what reached it. Its inputs, byte-identical (`cmp`) to folder `t2-B`'s: `brief.md`, `spec.md` (variant B: A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:51:58, ended 16:53:06, exit 0; the CLI's own estimate `total_cost_usd` 0.294 over 6 turns.

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

Lines are numbered 1 to 9 below, with the blank line between the functions counted as line 6.

# sentences

Empty container declaration (line 2, `counts: {str: i64} @= {}`):
- "A container literal separates elements by newline across lines and by comma on one, and an empty one needs an annotation: `xs: [i64] = []` · `m: {str: i64} @= {}`." This governs line 2. The annotation `{str: i64}` comes before `@=`.
- "`Binding   = [ ":" Type ] ( "=" | "@=" ) Expression NEWLINE .`" This governs line 2: an ident, an optional type annotation, then `@=`.
- "| `{K: V}` | map |" This governs the type `{str: i64}` on lines 1 and 2.

How a changing value is declared and changed (lines 2 and 4):
- "`v @= 0             # mutable cell, type inferred the same way`" This governs line 2.
- "`=` binds once, forever. `@=` declares a mutable cell, and `@` re-binds it, or a field or element inside one." This governs line 2 (the declaration) and line 4 (an element write inside the cell).
- "A cell nothing re-binds is a compile error: write `=`." This governs line 2. `counts` is re-bound element-wise on line 4, so I use `@=` and not `=`.
- "`Simple    = "_" Binding | Place "@" Expression NEWLINE`" and "`Place     = ident { "." ident | "[" Expression "]" } .`" These govern line 4: `counts[w]` is a Place.
- "`m[k]` is a `V?` with code `missing_key`; `m[k] @ v` inserts or replaces;" This governs line 4: the left side inserts or replaces, and the right side reads a `i64?`.
- The spec's own example `m: {str: i64} @= {}` / `m["a"] @ 1` / `print(m["b"].default(0))` governs lines 2, 4 and 9 together.

Other sentences:
- "the file you compile holds `function main()`, which takes nothing and produces nothing." Governs line 7.
- "`Declaration = ... | "function" ident [ Generics ] Params [ "->" Type ] Block`" and "`Param       = [ "@" ] ident ":" Type .`" Govern lines 1 and 7.
- "Signatures are always explicit; inference is local only." Governs line 1 (written parameter and result types) and line 8 (the type of `t` inferred).
- "Indentation is significant and rigid: exactly 4 spaces per level" Governs the indentation of lines 2 to 5 and 8 to 9.
- "Loops: `while cond` and `for x in xs`, over an array or a `range` (section 11)." and "`For     = "for" ident "in" Expression Block .`" Govern line 3.
- "| `.default(v)` | extract or fall back |" Governs lines 4 and 9.
- "A literal takes the type its context asks for ... otherwise `i64`." Governs the `0` and `1` on line 4 and the `0` on line 9: all are `i64`, matching `V`.
- "No implicit conversions, widths included: `1 + 2.0` is an error." and, from the operator table, "arithmetic + - * / % (both sides one numeric type" (the quote stops where the table continues). These govern `+ 1` on line 4: `i64 + i64`.
- "Precedence, strongest first: call, `.` and `::` → unary ... → `+ -`" Governs line 4: `counts[w].default(0)` is evaluated before `+ 1`.
- "A function with a `->` must `return` on every path that reaches its end" Governs line 5.
- "An unused binding or parameter is a compile error; a read is a use and a write is not" Governs every binding: `words` is read on line 3, `w` on line 4, `counts` on lines 4 and 5, `t` on line 9.
- "Shadowing is a compile error" Governs the names: `words`, `counts`, `w`, `t` are all distinct and none is a built-in name.
- "`x = 5              # immutable binding, type inferred`" Governs line 8.
- "A container literal separates elements ... by comma on one" Governs `["a", "b", "a"]` on line 8, which is non-empty and so needs no annotation.
- "`print` ... takes the types this language renders as text: a number, `str` or `bool`." Governs line 9: it prints an `i64`.
- "A line that computes a value must use it: bind it, or discard it on purpose with `_ = f(x)`, which a `()` line refuses: it stands alone." Governs line 9: `print` returns `()`, so it stands alone.

# choice_points

1. Name of the counter binding in `tally` (`counts`). Any other name not taken would behave the same. Naming it `keys`, `len` or another built-in might count as shadowing.
2. Reading the current count with `counts[w].default(0)` instead of a `match` on `.ok n` / `.err _`. A `match` would also work but needs more lines, and its arm bodies would have to give one `i64` type. `.must()` would abort on the first time a word appears.
3. Building the map in place with one mutable cell and element writes, instead of a `fold` with a helper function. A `fold` would need a separate top-level function with parameters `acc` and `item`, and its result would be a new map each step.
4. No annotation on `t` in `main`. Writing `t: {str: i64} = tally(...)` is also allowed and would type-check the same.
5. Passing the array literal straight to `tally` and not binding it first. Binding it first would add a name that must then be read.
6. `print(t["a"].default(0))` was fixed by the brief. Printing `t["a"]` directly would be refused, because `print` does not take a `i64?`.

# confidence

High, about 90 percent. The line I am least sure of is line 2, `counts: {str: i64} @= {}`, together with line 4. The rule "A cell nothing re-binds is a compile error" might be read as asking for a whole-cell re-binding (`counts @ ...`), not just an element write. But the spec says `@` re-binds "it, or a field or element inside one", and its own example declares `m` with `@=` and then only writes `m["a"] @ 1`, so I read element writes as enough.

# context

Nothing outside this directory's files reached my context. I read only `brief.md` and `spec.md`. The only other input was the environment information the harness attached automatically (the working directory, the git status listing `run.err` and `run.json` as untracked, and the user's account details). I did not open those files and did not use any of this information.
