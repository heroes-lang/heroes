# Panel 209, the blind seat, t2-A2 (second reading)

Copied by the coordinator on 2026-10-10 at 16:51 from `.claude/worktrees/scratch-b15/209-blind/t2-A2/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. Its inputs were `brief.md`, `spec.md` (variant t2-A: t2-A is the first reading's folder copied byte for byte; A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:49:58, ended 16:50:54, exit 0; the CLI's own estimate `total_cost_usd` 0.282 over 6 turns.

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

Lines are numbered 1 to 9 below, blank line included as line 6.

# sentences

Two quotes below are shortened with "..." where the specification has an em dash; nothing else is changed.

- "an empty one needs an annotation: `xs: [i64] = []` · `m: {str: i64} @ {}`." (section 10). Governs line 2: the empty map is declared with its type written out.
- "`v: i64 @ 0         # mutable declaration ... the type is REQUIRED`" (section 5). Governs line 2: the map changes, so it is declared with `@` and an explicit type.
- "`=` binds once, forever. `@` declares a mutable cell and re-binds it, or a field or element inside one." (section 5). Governs lines 2 and 4: line 2 declares the cell, line 4 re-binds an element inside it.
- "`v @ v + 1          # mutation; only a declared @ name can be mutated`" (section 5). Governs line 4: `counts` is a declared `@` name, so its element may be mutated.
- "`m[k]` is a `V?` with code `missing_key`; `m[k] @ v` inserts or replaces;" (section 10). Governs line 4: the read `counts[w]` is an `i64?`, and the write `counts[w] @ ...` inserts or replaces.
- "`Place     = ident { "." ident | "[" Expression "]" } .`" and "`Simple    = ... | Place "@" Expression NEWLINE`" (section 5 grammar). Governs line 4: `counts[w]` is a Place.
- "`.default(v)` | extract or fall back" (section 6 table). Governs lines 4 and 9.
- "Precedence, strongest first: call, `.` and `::` → unary ... → `* / %` → `+ -`" (section 7). Governs line 4: `counts[w].default(0) + 1` adds 1 to the extracted value, not to the `i64?`.
- "arithmetic   + - * / %          (both sides one numeric type ... never mixed)" (section 7). Governs line 4: both sides are `i64`.
- "A literal takes the type its context asks for ... otherwise `i64`." (section 2). Governs lines 4 and 9: `0` and `1` are `i64`.
- "Loops: `while cond` and `for x in xs`, over an array or a `range` (section 11)." (section 8). Governs line 3.
- "Signatures are always explicit; inference is local only." (section 5). Governs line 1: parameter and result types are written.
- "A function with a `->` must `return` on every path that reaches its end" (section 8). Governs line 5.
- "An unused binding or parameter is a compile error; a read is a use and a write is not" (section 5). Governs lines 1, 2, 3, 8: `words` is read by the loop, `counts` is read on lines 4 and 5, `w` is read on line 4, `t` is read on line 9.
- "the file you compile holds `function main()`, which takes nothing and produces nothing." (section 1). Governs line 7.
- "`x = 5              # immutable binding, type inferred`" (section 5). Governs line 8: `t` never changes, so it is bound with `=`.
- "A container literal separates elements by newline across lines and by comma on one" (section 10). Governs line 8: `["a", "b", "a"]`.
- "`print` ... takes the types this language renders as text: a number, `str` or `bool`." (section 11). Governs line 9: the argument is an `i64`.
- "A line that computes a value must use it: bind it, or discard it on purpose" (section 5). Governs line 9: `print` returns `()`, so the line stands alone; no other line computes an unused value.
- "Indentation is significant and rigid: exactly 4 spaces per level" (section 1). Governs every indented line.

# choice_points

- Line 2, `@` versus `=` for the empty map. I chose `counts: {str: i64} @ {}`. With `counts: {str: i64} = {}` the binding is immutable and line 4 would be refused, since only a declared `@` name can be mutated.
- Line 4, reading the old count. I chose `counts[w].default(0) + 1`. A `match` on `.ok n` / `.err _` would also work but is longer and adds a binding; `.must()` would abort on the first occurrence of each word (`missing_key`); a bare `counts[w] + 1` adds an `i64?` to an `i64`, a type error.
- Line 3, loop form. I chose `for w in words`. A `while` over an index `i: i64 @ 0` with `words[i]` would also work but needs a second mutable cell.
- Line 5, return. I return the map directly; values are copies, so nothing else is needed.
- Line 8, passing the array. I passed the literal positionally; with a single parameter no name is required, though `tally(words: [...])` would also be accepted (Arg allows a name).
- Line 9, `t["a"].default(0)` as the brief dictates; `.must()` would also print 2 here, but the brief asks for `default`.
- Naming: lowercase function and binding names; SCREAMING_CASE is required only for constants, of which there are none.

# confidence

High, about 90 percent, that the checker accepts the program. Least sure of line 4, `counts[w] @ counts[w].default(0) + 1`: it is the only line combining an indexed Place on the left, a fallible map read on the right, and UFCS `.default` with an untyped literal whose type must come from the `i64?` it falls back for. Every piece appears in section 10's example (`m["a"] @ 1`, `m["b"].default(0)`), so the risk lies only in their combination.

# context

Only `brief.md` and `spec.md` from this directory were read. The harness also attached a system context (a git status snapshot of the surrounding repository, recent commit titles, a user email, and environment details); I did not use any of it for the program.
