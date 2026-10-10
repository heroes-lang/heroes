# Panel 209, the blind seat, t1-B-c2 (clean reading, second)

Copied by the coordinator on 2026-10-10 at 16:54 from `.claude/worktrees/scratch-b15/209-blind/t1-B-c2/report.md`, verbatim below the rule. How it was run: one fresh `claude -p` session from that folder by `209-blind/run.sh` (the panel skill's command of 2026-10-01: `--restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools Read and Write only, `--max-budget-usd 0.8`), the environment stripped of `ANTHROPIC_API_KEY` (`env -u`), the CLI logged in as `authMethod: claude.ai`, `subscriptionType: team`. **The folder is its own git repository** (`git init`, one commit `inputs` holding only the inputs), made after the first twelve sessions reported the project's git status in their context: this session's `context` section says what reached it. Its inputs, byte-identical (`cmp`) to folder `t1-B`'s: `brief.md`, `spec.md` (variant B: A is the trunk's spec at 87794631, B is draft B2) and, for t3, `main.hero`. Started 16:53:24, ended 16:54:00, exit 0; the CLI's own estimate `total_cost_usd` 0.191 over 4 turns.

---

# program

```
function longest(words: [str]) -> str
    best @= words[0]
    for w in words
        if w.len() > best.len()
            best @ w
    return best

function main()
    print(longest(["ab", "abcd", "xyz"]))
```

Line numbers below count from 1 at `function longest`.

# sentences

Mutable value in the loop (lines 2 and 5):

- "`v @= 0             # mutable cell, type inferred the same way`" governs line 2, `best @= words[0]`.
- "`v @ v + 1          # mutation; only a cell declared with @= can be mutated`" governs line 5, `best @ w`.
- "`=` binds once, forever. `@=` declares a mutable cell, and `@` re-binds it, or a field or element inside one." governs lines 2 and 5: `best` has to be a cell because line 5 re-binds it.
- "A cell nothing re-binds is a compile error: write `=`." governs line 2: line 5 re-binds `best`, so `@=` is required and allowed.
- "Binding   = [ ":" Type ] ( "=" | "@=" ) Expression NEWLINE ." and "Statement = ident Binding | Simple ." govern line 2.
- "Simple    = "_" Binding | Place "@" Expression NEWLINE" and "Place     = ident { "." ident | "[" Expression "]" } ." govern line 5.
- "Signatures are always explicit; inference is local only." governs line 2 (the type of `best` is inferred as `str`) and line 1 (explicit signature).

Declarations and layout:

- "`| "function" ident [ Generics ] Params [ "->" Type ] Block`" governs lines 1 and 8.
- "the file you compile holds `function main()`, which takes nothing and produces nothing." governs line 8.
- "Indentation is significant and rigid: exactly 4 spaces per level; a tab is a compile error." governs every indented line.
- "No braces, no semicolons; a condition needs no parentheses." governs line 4.
- "Declaration order never matters" governs the placement of `main` after `longest`.

Indexing, loop, condition:

- "`[T]` | dynamic array, indices from 0" governs line 2, `words[0]` is a `str` (not a `str?`; only `m[k]` on a map is said to be a `V?`).
- "An out-of-bounds index or slice aborts" governs line 2: indexing an array yields the element, aborting instead of failing, and the brief guarantees a non-empty array.
- "Loops: `while cond` and `for x in xs`, over an array or a `range` (section 11)." and "For     = "for" ident "in" Expression Block ." govern line 3.
- "`x.f(y)` is sugar for `f(x, y)` (UFCS)." with the built-in "`len`" govern line 4, `w.len()` and `best.len()`.
- "`str` | immutable UTF-8 string, indexed and measured in bytes" governs line 4: length is in bytes, which is the same as characters for these ASCII inputs.
- "comparison   == != < <= > >=    (`< <= > >=`: a number only)" governs line 4: `>` compares two lengths, both numbers of one type.
- "`if` and `match` are expressions and may stand as statements" and "If      = "if" Expression Block { "else" "if" Expression Block } [ "else" Block ] ." govern line 4 (an `if` with no `else`).
- "`if cond` / `else if` / `else` take only `bool`" governs line 4.
- "A function with a `->` must `return` on every path that reaches its end" governs line 6.
- "An unused binding or parameter is a compile error; a read is a use and a write is not" governs lines 1, 2, 3: `words` is read on lines 2 and 3, `best` on lines 4 and 6, `w` on lines 4 and 5.

Main:

- "`print` writes its values with no separator and exactly one trailing newline, and takes the types this language renders as text: a number, `str` or `bool`." governs line 9.
- "A line that computes a value must use it: bind it, or discard it on purpose with `_ = f(x)`, which a `()` line refuses: it stands alone." governs line 9: `print` yields `()`, so the line stands alone.
- "A container literal separates elements by newline across lines and by comma on one" governs the array literal on line 9.
- "A literal takes the type its context asks for" governs the string literals on line 9, typed `str` by the parameter `[str]`.
- "When two parameters in a signature share a type, named arguments are mandatory at the call site" governs line 9: `longest` has one parameter, so the argument is positional.

# choice_points

- Loop form: `for w in words` over all elements, including the first. Alternative: `for i in range(from: 1, to: words.len())` with `words[i]`, which skips the redundant self comparison but adds an index binding and a `range` call; same output.
- Comparison: strict `>` so a later equally long string does not replace the earlier one (tie goes to the first, as the brief requires). `>=` would return the last of the longest; with this input both give `"abcd"`, but `>=` breaks the tie rule.
- Tracking the best: a `str` cell `best`. Alternative: track an index cell `best_i @= 0` and return `words[best_i]`; same result, one more indexing.
- Initial value: `words[0]`. Alternative: `""`, which would also work for non-empty arrays since every element is at least as long, but with strict `>` an all-empty array still returns `""` correctly; `words[0]` states the non-empty precondition more directly.
- No type annotation on `best` (`best: str @= words[0]` would also be accepted and is equivalent).
- `w.len()` via UFCS rather than `len(w)`; equivalent by the UFCS sentence.
- Output: the program prints `abcd`.

# confidence

High, about 90 percent. Least sure of line 4, `if w.len() > best.len()`: the specification lists `len` as a built-in without giving its signature or result type. I assume it takes a `str` and returns one numeric type (presumably `i64`) for both calls, so `>` compares like with like. Second least sure of line 2: that indexing an array with `words[0]` yields a plain `str` and not a `str?`, which I infer from the map case being singled out as `V?` and from out-of-bounds indexing being said to abort.

# context

Nothing other than this directory's files reached my context for the task: I read only `brief.md` and `spec.md`. The harness supplied environment metadata (working directory path, git status listing untracked `run.err` and `run.json`, the user's email and git name), none of which I opened or used.
