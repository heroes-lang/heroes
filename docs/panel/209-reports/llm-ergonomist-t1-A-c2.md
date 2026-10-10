# Panel 209, the blind seat, t1-A-c2 (clean reading, second)

Copied by the coordinator on 2026-10-10 at 16:54 from `.claude/worktrees/scratch-b15/209-blind/t1-A-c2/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. **The folder is its own git repository** (`git init`, one commit `inputs` holding only the inputs), made after the first twelve sessions reported the project's git status in their context: this session's `context` section says what reached it. Its inputs, byte-identical (`cmp`) to folder `t1-A`'s: `brief.md`, `spec.md` (variant A: A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:53:24, ended 16:54:28, exit 0; the CLI's own estimate `total_cost_usd` 0.292 over 5 turns.

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

Line numbers below count from 1 at `function longest`.

# sentences

How the value that changes during the loop is declared and changed (lines 2 and 5):

- "`v: i64 @ 0         # mutable declaration" and, in the same line, "the type is REQUIRED`" (the spec joins the two with a dash) govern line 2, `best: str @ words[0]`: the type `str` is written because it is required.
- "`v @ v + 1          # mutation; only a declared @ name can be mutated`" governs line 5, `best @ w`.
- "`=` binds once, forever. `@` declares a mutable cell and re-binds it, or a field or element inside one." governs lines 2 and 5: `best` is declared with `@` so it can be re-bound in the loop; `=` would make line 5 an error.
- "Binding   = ( "=" | ":" Type ( "@" | "=" ) ) Expression NEWLINE ." governs line 2.
- "Simple    = "_" Binding | Place "@" Expression NEWLINE" and "Place     = ident { "." ident | "[" Expression "]" } ." govern line 5 (the place is the plain ident `best`).

Other sentences:

- "One file is one module; the file you compile holds `function main()`, which takes nothing and produces nothing." governs line 8.
- "Declaration = ... | "function" ident [ Generics ] Params [ "->" Type ] Block" and "Param       = [ "@" ] ident ":" Type ." govern lines 1 and 8.
- "Signatures are always explicit; inference is local only." governs line 1 (`words: [str]` and `-> str` written out).
- "Indentation is significant and rigid: exactly 4 spaces per level; a tab is a compile error. No braces, no semicolons; a condition needs no parentheses." governs the layout of every line, and line 4's unparenthesised condition.
- "`[T]` | dynamic array, indices from 0" governs `words[0]` on line 2.
- "Postfix    = Primary { "." ident [ Args ] | "::" ident | Args | "[" Expression "]" | "?" } ." governs `words[0]` (line 2) and `w.len()`, `best.len()` (line 4).
- "An out-of-bounds index or slice aborts" governs line 2: the brief guarantees a non-empty array, so `words[0]` is in bounds. The spec's own example `return ok(xs[0])` in a function returning `str?` shows that indexing a `[str]` yields a plain `str`, which line 2 relies on.
- "Loops: `while cond` and `for x in xs`, over an array or a `range` (section 11)." and "For     = "for" ident "in" Expression Block ." govern line 3.
- "`if` and `match` are expressions and may stand as statements; there is no ternary." and "If      = "if" Expression Block { "else" "if" Expression Block } [ "else" Block ] ." govern line 4.
- "`if cond` / `else if` / `else` take only `bool`" (followed in the spec by "there is no truthiness.") governs line 4: a comparison is a `bool`.
- "`x.f(y)` is sugar for `f(x, y)` (UFCS)." with the built-in `len` and the spec's own `xs.len() == 0` govern `w.len()` and `best.len()` on line 4.
- "`str` | immutable UTF-8 string, indexed and measured in bytes" governs line 4: length is compared in bytes, which equals characters for these ASCII inputs.
- "comparison   == != < <= > >=    (`< <= > >=`: a number only)" governs the `>` on line 4.
- "A function with a `->` must `return` on every path that reaches its end" governs line 6.
- "An unused binding or parameter is a compile error; a read is a use and a write is not" governs lines 1 to 6: `words` is read on lines 2 and 3, `w` on lines 4 and 5, `best` on lines 4 and 6.
- "Shadowing is a compile error" governs the choice of names `best` and `w`, which collide with nothing.
- "A container literal separates elements by newline across lines and by comma on one" governs the array literal on line 9.
- "A literal takes the type its context asks for" governs the string literals on line 9 (each is a `str`).
- "When two parameters in a signature share a type, named arguments are mandatory at the call site" governs line 9: `longest` has one parameter, so the argument is unnamed.
- "`print` writes its values with no separator and exactly one trailing newline, and takes the types this language renders as text: a number, `str` or `bool`." governs line 9: the argument is a `str`.
- "A line that computes a value must use it: bind it, or discard it on purpose with `_ = f(x)`, which a `()` line refuses: it stands alone." governs line 9: `print` returns nothing, so the call stands alone; and lines 4 to 5: the `if` without `else`, whose body is a mutation, produces no value to discard.

# choice_points

- Strict `>` versus `>=` on line 4. I chose `>` so a later word of equal length never replaces an earlier one, giving the first of equally long words. `>=` would return the last of them, which violates the brief, though not for this input, where "abcd" is the unique longest; the output would still be `abcd`.
- Iterating the whole array including `words[0]` versus starting at index 1 with `for i in range(from: 1, to: words.len())` and `words[i]`. I chose the plain `for w in words`: comparing `words[0]` with itself under `>` changes nothing, and it avoids an index variable. The other choice produces the same result with more code and one more place (the `range` call) to get wrong.
- Mutable `best: str @ ...` versus a functional `fold`. I chose the mutable cell and loop. A `fold` would need a separate top-level function whose two parameters share the type `str` and must be named `acc` and `item`, which makes named parameters part of the function type; more ways to fail the checker, same output.
- Initialising `best` with `words[0]` versus `""`. `words[0]` is correct for any non-empty array; `""` would also work (every word is longer than 0 bytes, and an all-empty array would still return `""`), but `words[0]` reads more directly as "the first one".
- Method syntax `w.len()` versus `len(w)`. Both are equivalent by UFCS; I followed the spec's own example `xs.len()`.
- Writing `main` before or after `longest`: declaration order never matters, so either compiles identically.

# confidence

High, about 90 percent. The line I am least sure of is line 4, `if w.len() > best.len()`: the spec never writes the signature of `len` on a `str` (it only shows `xs.len()` on an array and says `str` is "measured in bytes"), so I assume `len` accepts a `str` and returns an `i64`. Second least sure is lines 4 to 5 as a statement: the spec says an `if` may stand as a statement but does not spell out that an `if` with no `else` whose body is only a mutation counts as producing no value; I read it that way.

# context

Only `brief.md` and `spec.md` from this directory were read. Beyond them, my context held the harness's automatic environment information (working directory path, git status listing untracked `run.err` and `run.json`, a recent commit message "inputs", the git user name and an account email), none of which I used. I did not open `run.err`, `run.json`, or any file outside this directory, and I ran nothing.
