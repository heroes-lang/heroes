# Panel 199, compiler-engineer

Read `00-shared.md` first, whole. Your directory `<scratchpad>/199-compiler-engineer/`.

1. **Where the rule can live** (check, resolve, IR): what each stage knows of "every path" (the checker's flow, `check/flow.hero`; the IR's blocks), and the shapes it must get right: a direct self-call in every arm of a `match`, a self-call behind `if` with no `else` (a path returns), a self-call through UFCS, a generic function calling its own instance, a call through a function value, mutual recursion (two functions, every path calling the other), a self-call after `?` (a path returns early), a `while true` loop that calls itself, an `abort`/`panic` path.
2. **Build route (A) or (B)** in your copy, its cases (each shape above, annotated), the compiler's own tests, `check` narrowed, the `check` census over every tracked `.hero` (no correct program refused: say how many hits and read each), and the compiler's own check's cost before and after (instructions).
3. **(C)**: what the build would need to read clang's warning back to a `.hero` line, and on which clang versions `-Winfinite-recursion` exists (18, 21, 22, 23).

Verdict: the route, its cost, the other routes' costs. Veto on soundness only.
