# Panel 209, the blind seat, t2-B

Copied by the coordinator on 2026-10-10 at 16:50 from `.claude/worktrees/scratch-b15/209-blind/t2-B/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. Its inputs were `brief.md`, `spec.md` (variant B: A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:48:30, ended 16:49:16, exit 0; the CLI's own estimate `total_cost_usd` 0.23 over 4 turns.

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

Program lines, numbered for reference below:
1. `function tally(words: [str]) -> {str: i64}`
2. `counts: {str: i64} @= {}`
3. `for w in words`
4. `counts[w] @ counts[w].default(0) + 1`
5. `return counts`
6. (blank)
7. `function main()`
8. `t = tally(["a", "b", "a"])`
9. `print(t["a"].default(0))`

# sentences

Empty container and mutable value (the ones the brief asks for in particular):

- "A container literal separates elements by newline across lines and by comma on one, and an empty one needs an annotation: `xs: [i64] = []` · `m: {str: i64} @= {}`." Governs line 2 (the annotation `{str: i64}` on the empty `{}`) and line 8 (the comma-separated one-line array literal).
- "`=` binds once, forever. `@=` declares a mutable cell, and `@` re-binds it, or a field or element inside one." Governs line 2 (`@=` declares `counts` as a cell) and line 4 (`@` re-binds an element inside it).
- "A cell nothing re-binds is a compile error: write `=`." Governs line 2 versus line 4: line 4 re-binds the cell, so `@=` is required and legal; governs line 8, where `t` is never re-bound, so it uses `=`.
- "`v @= 0             # mutable cell, type inferred the same way`" and "`v @ v + 1          # mutation; only a cell declared with @= can be mutated`". Governs lines 2 and 4.
- "`m[k]` is a `V?` with code `missing_key`; `m[k] @ v` inserts or replaces;" Governs line 4 (the read `counts[w]` is an `i64?`; the write `counts[w] @ ...` inserts the first time and replaces after) and line 9 (`t["a"]` is an `i64?`).
- The worked example in section 10: "`m: {str: i64} @= {}` / `m["a"] @ 1` / `print(m["b"].default(0))`". Line 2, 4 and 9 follow it directly.
- Grammar: "`Binding   = [ ":" Type ] ( "=" | "@=" ) Expression NEWLINE .`" Governs lines 2 and 8. "`Simple    = "_" Binding | Place "@" Expression NEWLINE`" and "`Place     = ident { "." ident | "[" Expression "]" } .`" Govern line 4.

Other sentences the program rests on:

- "One file is one module; the file you compile holds `function main()`, which takes nothing and produces nothing." Line 7.
- "Indentation is significant and rigid: exactly 4 spaces per level; a tab is a compile error." All indented lines.
- "`Declaration = ... | "function" ident [ Generics ] Params [ "->" Type ] Block`" and "`Param       = [ "@" ] ident ":" Type .`" Lines 1 and 7.
- "`Prefix   = ident [ "." ident ] | "[" Type "]" | "{" Type ":" Type "}"`" Line 1 (`[str]`, `{str: i64}`) and line 2.
- "Signatures are always explicit; inference is local only." Line 1 writes both parameter and result types.
- "An unused binding or parameter is a compile error; a read is a use and a write is not, except through an `@` parameter." `words` is read on line 3, `w` on line 4, `counts` on lines 4 and 5 (the read in line 5 is what makes it used, since line 4's write alone would not), `t` on line 9.
- "Loops: `while cond` and `for x in xs`, over an array or a `range` (section 11)." and "`For     = "for" ident "in" Expression Block .`" Line 3.
- "A function with a `->` must `return` on every path that reaches its end" Line 5.
- "| `.default(v)` | extract or fall back |" Lines 4 and 9.
- "`x.f(y)` is sugar for `f(x, y)` (UFCS)." Lines 4 and 9 call `default` in that form, as the spec's own example does.
- "A literal takes the type its context asks for ... otherwise `i64`." The `0` in `.default(0)` and the `1` on line 4 are `i64`, matching `V = i64`.
- "arithmetic   + - * / %          (both sides one numeric type — never mixed)" Line 4: `i64 + i64`.
- "Precedence, strongest first: call, `.` and `::` → unary ... → `+ -`" Line 4: `counts[w].default(0)` binds before `+ 1`.
- "Postfix    = Primary { "." ident [ Args ] | "::" ident | Args | "[" Expression "]" | "?" } ." Lines 4 and 9 (`counts[w].default(0)`, `t["a"].default(0)`).
- "`print` writes its values with no separator and exactly one trailing newline, and takes the types this language renders as text: a number, `str` or `bool`." Line 9 passes an `i64`.
- "A line that computes a value must use it: bind it, or discard it on purpose with `_ = f(x)`, which a `()` line refuses: it stands alone." Line 9: `print` yields `()`, so it stands alone as `Expression NEWLINE`.
- "Shadowing is a compile error" No name is bound twice (`counts`, `w`, `words`, `t` are distinct, none named after a built-in).
- "A literal without the `f` is an error where its braces, read with an `f`, would be a hole" The strings `"a"`, `"b"` hold no braces.

# choice_points

1. **How to read a missing key while counting (line 4).** Chose `counts[w].default(0) + 1`. The alternative is a `match` on `counts[w]` with `.ok n => n + 1` and `.err _ => 1` arms (or an `if` on `.is_err()` followed by `.must()`). Those produce the same counts with more lines and an extra binding; `.must()` alone would abort on the first occurrence of each word.
2. **Mutable map versus rebuilding.** Chose a `@=` cell mutated in place by element. The alternative of an immutable binding is impossible (nothing else can grow a map); an `@` parameter variant would need a helper function and gives the same result.
3. **Annotation placement for the empty map (line 2).** Chose the annotation on the binding, exactly as the spec's example. No other place is offered for it; omitting it is a compile error per the spec.
4. **`=` versus `@=` for `t` (line 8).** Chose `=`; `@=` would be a compile error because nothing re-binds `t`.
5. **Loop form (line 3).** Chose `for w in words`. A `while` with an index cell `i @= 0` and `words[i]` would also work but adds a cell and bound checks.
6. **Call form of `default`.** Chose UFCS `x.default(0)`, as the spec writes it; `default(x, 0)` is the desugared equivalent and should mean the same.
7. **Names.** `counts`, `w`, `t` (fixed by the brief). Any non-shadowing names would do.

The program prints `2`.

# confidence

High, about 90 percent, that the checker accepts it on the first try. Lines 2, 4 and 9 mirror the specification's own section 10 example almost verbatim.

Least sure of line 4, `counts[w] @ counts[w].default(0) + 1`: it reads the same cell on the right that it writes on the left, and the specification never shows exactly this shape (its example writes a constant). Nothing in the specification forbids it, the `Place "@" Expression` production admits it, and the right side is an `i64` as the map's value type requires, but it is the one line whose acceptance I infer rather than read off an example. A second, smaller doubt is whether line 4's element write alone counts as re-binding the cell for the "a cell nothing re-binds" rule; the sentence "`@` re-binds it, or a field or element inside one" says it does.

# context

Only this directory's files reached my context: `brief.md` and `spec.md`. The harness also attached session metadata (a git status snapshot with recent commit titles of the repository, the user's email, environment details). I did not use any of it in writing the program. I read no other file and ran nothing.
