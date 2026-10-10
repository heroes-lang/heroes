# Panel 209, the blind seat, t1-A

Copied by the coordinator on 2026-10-10 at 16:50 from `.claude/worktrees/scratch-b15/209-blind/t1-A/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. Its inputs were `brief.md`, `spec.md` (variant A: A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:48:30, ended 16:49:33, exit 0; the CLI's own estimate `total_cost_usd` 0.304 over 5 turns.

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

Lines are numbered below as L1 to L9, counting the blank line as L7.

# sentences

Declaring and changing the value that changes during the loop (L2, L5):

- "`v: i64 @ 0         # mutable declaration` [dash] `the type is REQUIRED`" (the specification separates the two parts with an em dash, replaced here by [dash]) governs L2: `best` is declared with its type `str` written out.
- "`v @ v + 1          # mutation; only a declared @ name can be mutated`" governs L5: `best @ w` re-binds the declared cell.
- "`=` binds once, forever. `@` declares a mutable cell and re-binds it, or a field or element inside one." governs L2 and L5: `=` would forbid the update, so both use `@`.
- "`Binding   = ( "=" | ":" Type ( "@" | "=" ) ) Expression NEWLINE .`" governs L2 (the form `ident ":" Type "@" Expression`).
- "`Simple    = "_" Binding | Place "@" Expression NEWLINE`" and "`Place     = ident { "." ident | "[" Expression "]" } .`" govern L5 (the Place is the plain ident `best`).
- "All bindings are initialised." governs L2: `best` starts as `words[0]`.

Other sentences:

- "One file is one module; the file you compile holds `function main()`, which takes nothing and produces nothing." governs L8.
- "`| "function" ident [ Generics ] Params [ "->" Type ] Block`" governs L1 and L8.
- "Signatures are always explicit; inference is local only." governs L1: parameter and result types are written.
- "Indentation is significant and rigid: exactly 4 spaces per level; a tab is a compile error. No braces, no semicolons; a condition needs no parentheses." governs L2 to L6 and L9, and the unparenthesised condition on L4.
- "`[T]` | dynamic array, indices from 0" governs L2 (`words[0]` is the first element).
- "An out-of-bounds index or slice aborts" governs L2: an array index yields the element itself (it aborts rather than giving a `T?`, unlike "`m[k]` is a `V?`" for maps), so `words[0]` is a `str` and matches the declared type. The brief guarantees a non-empty array.
- "Loops: `while cond` and `for x in xs`, over an array or a `range` (section 11)." and "`For     = "for" ident "in" Expression Block .`" govern L3.
- "`if` and `match` are expressions and may stand as statements; there is no ternary." and "`If      = "if" Expression Block { "else" "if" Expression Block } [ "else" Block ] .`" govern L4 (an `if` with no `else`, standing as a statement).
- "`if cond` / `else if` / `else` take only `bool`" (followed in the specification by "there is no truthiness.") governs L4: the condition is a comparison, a `bool`.
- "`x.f(y)` is sugar for `f(x, y)` (UFCS)." and "Built-ins: `print(...)` · `len` ..." govern L4 (`w.len()`, `best.len()`).
- "`str` | immutable UTF-8 string, indexed and measured in bytes" governs L4: lengths compare in bytes, which for these ASCII words equals characters.
- "comparison   == != < <= > >=    (`< <= > >=`: a number only)" governs L4: both sides are the numbers `len` returns, of one type.
- "Every value behaves as an independent copy" governs L5: `best @ w` holds its own copy.
- "A function with a `->` must `return` on every path that reaches its end" governs L6.
- "An unused binding or parameter is a compile error; a read is a use and a write is not" governs L1 to L5: `words` is read on L2 and L3, `w` on L4 and L5, `best` on L4 and L6.
- "Shadowing is a compile error" governs the names: `words`, `best`, `w` are all distinct and none is a built-in name.
- "A container literal separates elements by newline across lines and by comma on one" and "`"[" [ Expression { Sep Expression } ] "]"`" govern L9's array literal.
- "When two parameters in a signature share a type, named arguments are mandatory at the call site" governs L9: `longest` has one parameter, so the positional argument is allowed ("`Arg      = [ ident ":" ] [ "@" ] Expression .`").
- "`print` writes its values with no separator and exactly one trailing newline, and takes the types this language renders as text: a number, `str` or `bool`." governs L9: the argument is a `str`.
- "A line that computes a value must use it: bind it, or discard it on purpose with `_ = f(x)`, which a `()` line refuses: it stands alone." governs L9: `print` returns `()`, so its line stands alone; and L4, where the `if`'s block ends in a mutation, not a value.
- "A literal without the `f` is an error where its braces, read with an `f`, would be a hole" governs L9 only vacuously: no literal holds a brace.

# choice_points

- Strict `>` versus `>=` on L4. Chosen `>`, so on a tie the earlier word stays, as the brief asks. `>=` would return the last of equally long words (for `["ab", "cd"]`, `"cd"`); for the given input both print `abcd`.
- Iterating all of `words` versus `range(from: 1, to: words.len())` with indexing. Chosen the plain `for w in words`; comparing `words[0]` with itself is harmless. The range version would need an index binding and `words[i]`, more places to err, same result.
- Initialising `best` from `words[0]` versus `""`. Chosen `words[0]`. `""` would also produce the right answer for any non-empty array, but relies less directly on the brief's guarantee; either is accepted.
- Return type `str` versus `str?` failing on an empty array. Chosen `str`, as the brief's signature says; `str?` would force `main` to handle it (`.must()` or `match`) and change the signature.
- `for` loop versus `fold`. Chosen `for`; `fold` would need a separate named top-level function with parameters `acc` and `item`, since there are no anonymous functions.
- Positional versus named argument in L9. Chosen positional; `longest(words: [...])` would also be accepted.
- Calling `w.len()` (UFCS) versus `len(w)`. Chosen UFCS; both are the same call.

# confidence

High, about 90 percent. The line I am least sure of is L4, `if w.len() > best.len()`, because the specification never states `len`'s result type; I rely on both sides being the same type whatever it is. Next least is L2, `best: str @ words[0]`, where the specification says an array index out of bounds aborts but never states in so many words that `xs[i]` has type `T` rather than `T?`.

# context

Only `brief.md` and `spec.md` from this directory were read. The harness also placed into my context, unrequested, a git status snapshot (branch `main`, recent commit titles mentioning panels and defects, untracked `docs/panel/209-briefs/` and `docs/panel/209-reports/`), the user's email address, and the environment details (working directory path, platform). None of it informed the program.
