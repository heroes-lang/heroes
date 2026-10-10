# Panel 209, the blind seat, t2-A-c2 (clean reading, second)

Copied by the coordinator on 2026-10-10 at 16:54 from `.claude/worktrees/scratch-b15/209-blind/t2-A-c2/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. **The folder is its own git repository** (`git init`, one commit `inputs` holding only the inputs), made after the first twelve sessions reported the project's git status in their context: this session's `context` section says what reached it. Its inputs, byte-identical (`cmp`) to folder `t2-A`'s: `brief.md`, `spec.md` (variant A: A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:53:24, ended 16:53:59, exit 0; the CLI's own estimate `total_cost_usd` 0.186 over 4 turns.

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

# sentences

Line numbers refer to the program above (line 1 is `function tally...`).

- "the file you compile holds `function main()`, which takes nothing and produces nothing." Governs line 7.
- "Signatures are always explicit; inference is local only." Governs line 1 (parameter and result types written) and line 7.
- `Declaration = ... | "function" ident [ Generics ] Params [ "->" Type ] Block` and `Prefix = ... | "[" Type "]" | "{" Type ":" Type "}"`. Govern line 1, the types `[str]` and `{str: i64}`.
- "Indentation is significant and rigid: exactly 4 spaces per level". Governs every indented line.
- How an empty container is declared: "A container literal ... an empty one needs an annotation: `xs: [i64] = []` · `m: {str: i64} @ {}`." Governs line 2.
- How a value that changes is declared: "`v: i64 @ 0         # mutable declaration — the type is REQUIRED`" and "`@` declares a mutable cell and re-binds it, or a field or element inside one." Govern line 2.
- How it is changed: "`v @ v + 1          # mutation; only a declared @ name can be mutated`", "`m[k] @ v` inserts or replaces", and the productions `Simple = ... | Place "@" Expression NEWLINE` and `Place = ident { "." ident | "[" Expression "]" }`. Govern line 4.
- "`m[k]` is a `V?` with code `missing_key`" and "`.default(v)` | extract or fall back". Govern line 4 (`counts[w].default(0)`) and line 9 (`t["a"].default(0)`).
- The spec's own example "`print(m["b"].default(0))`". Governs line 9.
- "Precedence, strongest first: call, `.` and `::` → unary ... → `* / %` → `+ -`". Governs line 4: `counts[w].default(0)` is evaluated before `+ 1`.
- "arithmetic + - * / % (both sides one numeric type — never mixed)" and "A literal takes the type its context asks for ... otherwise `i64`." Govern line 4: `default(0)` yields `i64`, and `1` becomes `i64`.
- "Loops: `while cond` and `for x in xs`, over an array" and `For = "for" ident "in" Expression Block`. Govern line 3.
- "An unused binding or parameter is a compile error; a read is a use and a write is not". Governs lines 2 to 5 and 8: `words` is read on line 3, `w` on line 4, `counts` on lines 4 and 5, `t` on line 9.
- "A function with a `->` must `return` on every path that reaches its end". Governs line 5.
- "`=` binds once, forever." and "`x = 5 # immutable binding, type inferred`". Govern line 8.
- "`print(...)` ... takes the types this language renders as text: a number, `str` or `bool`." Governs line 9 (an `i64`).
- "A line that computes a value must use it ... which a `()` line refuses: it stands alone." Governs line 9: `print` returns `()`, so the line stands alone without `_ =`.
- "A container literal separates elements ... by comma on one" and `Primary = ... | "[" [ Expression { Sep Expression } ] "]"`. Govern line 8, the literal `["a", "b", "a"]`, non-empty so no annotation is needed.
- "Shadowing is a compile error". Governs naming: no name (`words`, `counts`, `w`, `t`) is bound twice.

# choice_points

- Name of the map: I chose `counts`. Any other non-shadowing name behaves the same.
- Counting step: I chose `counts[w] @ counts[w].default(0) + 1`. The alternative was a `match counts[w]` with `.ok n => counts[w] @ n + 1` and `.err _ => counts[w] @ 1`; same result, more lines, and it adds a question about whether an inline arm may hold a `@` statement (the spec says an inline arm's declaration or `=` may name only `_`, which leaves mutation allowed but less obviously).
- Loop form: `for w in words` versus a `while` over an index with a mutable `i: i64 @ 0`. The `while` form adds a second mutable cell and index reads; same output.
- Map annotation with `@` versus `=`: the map must change, so `@` is required; `counts: {str: i64} = {}` would be immutable and line 4 would be refused ("only a declared @ name can be mutated").
- Reading in `main`: `.default(0)` as the brief demands; `.must()` would also print 2 here but aborts on a missing key.
- Binding `t` with `=` rather than `@`: `t` never changes; `t: {str: i64} @ ...` would also compile but is needless.

# confidence

High, about 90 percent. The line I am least sure of is line 4, `counts[w] @ counts[w].default(0) + 1`: it reads and writes the same map element in one statement and the parser must take a statement that begins with an ident followed by `[` as a `Place "@"` rather than an `ident Binding`. The grammar and the spec's own `m["a"] @ 1` example support it, but it is the one line combining the most rules.

# context

Only `brief.md` and `spec.md` from this directory were read. The harness also supplied environment details (working directory path, git status listing `run.err` and `run.json` as untracked, a git user name and an email address); none of it bore on the program. No other file and no knowledge of any Heroes implementation was used.
