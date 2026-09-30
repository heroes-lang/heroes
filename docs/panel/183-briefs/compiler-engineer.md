# Panel 183, brief for the compiler-engineer

Read `docs/panel/183-briefs/00-shared.md` first: the sitting, both questions,
and every measurement it rests on, with the commands.

**Your directory** is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/183-compiler-engineer/`:
`git -C /Users/joseph/Temp/heroes/heroes-lang archive 171e8c45 | tar -x -C <it>`,
`rm -rf build`, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`.
Never read, build or run in the repository's working tree or another seat's
directory. Rebuilding the compiler from `selfhost/` is `./heroes build
selfhost/main.hero -o heroes-next` (about a minute from the plain seed, per
`.claude/skills/panel/SKILL.md`'s re-measurement of 2026-09-24); prototype
there.

## Your tasks, in order

1. **The grammar question** 00-shared.md leaves open: can a program that
   compiles place, at the head of a line inside brackets, (a) at column 0, a
   declaration word (`record`, `variant`, `constant`, `use`, `extern`, `test`,
   `function` before a name), or (b) at any column, `if`, `while`, `for`,
   `match`, `return`, `assert`, `else`? Answer from `spec/heroes-spec.md`'s
   grammar and from `selfhost/grammar_expr.hero` and `selfhost/parse/`, with
   the productions cited: `if` and `match` are also EXPRESSIONS in this
   language (the spec's grammar has `If` and a `match` expression), so (b)'s
   `if` and `match` need an argument, not an assumption. A line inside brackets
   that opens with an `if` expression, a `match` expression or an `else` of a
   multi-line `if` expression: is any such program accepted? Build one and run
   it. Weigh `selfhost/next_line.hero:240`'s `starts_afresh` (panel 181's
   depth-zero rule), whose list says `if` and `match` do NOT start a line
   afresh while `return`, `assert`, `for`, `while` and `else` do, and
   `selfhost/parse/headless.hero:185`'s `opens_a_declaration`, whose list is
   not (a)'s: three predicates of one family, and whether the sitting should
   leave them three. And (c), the critic's: a member line of an `extern`
   group (column 4, `function`, `constant` or `record`) below a member whose
   `(` never closed, `docs/panel/183-briefs/probes/extern_member_unclosed.hero`;
   should it end a reach, and does the answer touch what a binding's author
   sees (the sitting widens to the ffi-pragmatist if it does)?
2. **Read the landed rule (a)** at `171e8c45` (00-shared.md § Where the
   compiler does it) and say whether it does what it says, at its edges: a
   generic head (`function f<T>(`), a head with no parameters, a declaration
   word inside a string or a comment at column 0, CRLF line ends, a tab before
   the word, a file that ends inside the bracket, nested brackets several
   deep, the `extern` group above, and the six cases 00-shared.md names. Run
   each. **Separate the lexer's rule from the parser's use of `closes`**: build
   `41807577`'s compiler (its `selfhost/`, from `git archive 41807577`, built
   by the `c85bccb8` seed's compiler) and say which part each shape's output
   owes to which; the critic found them different on nested openers and on
   the `extern` group.
3. **Prototype (b)** in your copy, whatever your answer to 1 says about which
   of the seven words it can safely take, run Task 2 of 00-shared.md and its
   neighbours (the stray closer two statements down, inside a nested block, a
   `match` arm line, an `else` line, and the four probes in
   `docs/panel/183-briefs/probes/`), and give the real output. **Write Task 2's
   output under your prototype to
   `docs/panel/183-reports/compiler-engineer-task2.txt`** as soon as you have
   it, exactly as `heroes check task2.hero` prints it: the llm-ergonomist's
   second reading waits for it. Then the
   census: `check --brief` and its exit over every tracked `.hero` file, your
   prototype against `171e8c45`'s compiler, both arms (`--permissive` too),
   every moved file named with why.
4. **Cost**: the lines each route adds, against `tests/harness/suite_layout.hero`'s
   measure for the files it touches; and whether the lexer can know "a line's
   first token" for (b) at the same point it knows it for (a).

Your verdict: ratify, amend or refuse (a), and adopt, narrow or refuse (b),
each with the condition that would change it; your veto is a refusal on
soundness, and says which program it would break. Write your report to
`docs/panel/183-reports/compiler-engineer.md` in the repository AS YOU GO
(the one file you write there), so a stalled run leaves what it had. English,
no em dashes.
