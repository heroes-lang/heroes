# Panel 181, brief for the compiler-engineer

Read `00-shared.md` first, in this directory. Your seat judges implementation
cost and core-versus-sugar (design.md §1.1, §1.7, Part 5), with a veto on
soundness.

**Your directory is `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-181/compiler-engineer/`,
and it is yours alone.** Make it with `git -C /Users/joseph/Temp/heroes/heroes-lang
archive 0fc98107 | tar -x -C <your directory>`, then `rm -rf build` inside it,
and build your own compiler there from the seed: `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, about 3 s. A rebuild from `selfhost/` is about 61 s
from that binary, or about 32 s from one built with `clang -O2` (19 s to build),
measured 2026-09-24, so prototyping in `selfhost/` is affordable. Never build,
run or edit anything in the trunk, `/Users/joseph/Temp/heroes/heroes-lang`, or
in another seat's directory, and kill only processes you started, by PID.
Set `HEROES_RUNTIME=<your directory>/runtime` when a binary outside the tree's
root runs `run` or `build`.

## What you decide

1. **The whole map at depth zero, not the brief's sample.** Every token kind
   that is not a line ender (`selfhost/layout.hero:40`, the arms that answer
   `false`), at the end of a line in every depth-zero context the grammar has
   (a statement, a block head's value after `if`, `while`, `for`, `match`, a
   function or extern header, a constant's body, a `match` arm, a `test`
   header, a record or variant member line, a `use`), against the next line at
   the same margin, one deeper, and shallower. Say where each list came from
   (the spec's productions, `grep` of the parser), because the list is a
   measurement too. The seventeen shapes in `00-shared.md` are a sample.
2. **Price the routes**, each in files and lines of `selfhost/`, prototyped in
   your copy far enough that the number is real:
   - **(a) refuse in the lexer**: at a depth-zero line start at the same or a
     shallower margin, when the previous line ended with a token that is not a
     line ender, report a diagnostic naming that token and the rule, and plant
     the terminator so the parser sees two statements;
   - **(b) refuse in the parser**, from the line numbers it already compares in
     one recovery (the `fixedbugs-use-refusal-eats-the-next-line` golden);
   - **(c) admit Nim's rule** as panel 007 said a 007-bis would have to: a line
     that ends with a member of an explicit continuator set goes on to a line
     one level DEEPER, which opens no block, and the same margin is refused;
     say what it does to the four block headers panel 007 named and to every
     header that ends in a non-ender today;
   - **any route none of these names.**
   For each prototype run the compiler's own tests and the net's `surface`,
   `canonical`, `check`, `annotations`, `fixes`, `grammar` and `run` suites in
   your copy (`./heroes run tests/harness/main.hero -- ./heroes <suite>`, one at
   a time), and say which break and why.
3. **The `error` token at a line's end**, the 16 hits of `00-shared.md`: run
   those goldens today and say whether the glued line produces a second
   diagnostic anywhere; say whether an `error` token should end a line under
   each route, and what that does to the goldens' `.expected` files. The `use`
   lines, the 4 hits, likewise.
4. **The diagnostic and its `Fix`**: the code, the message, the span, and
   whether the fix is `certain` (it repairs the defect the diagnostic names:
   `.claude/rules/diagnostics-and-goldens.md`) or a `guess`. Joining the two
   lines, and wrapping the expression in parentheses, are two candidates; say
   which one is the program the author meant in each shape of your map, and
   where the two differ (a block head, a function header, a `match` arm).
5. **The formatter under each route**, and defect 118 (`s13` in
   `00-shared.md`: exit 2 today): what `fmt` does, and what it owes.
6. **The cost in time**: if a route changes the lexer's per-line work, time
   `heroes lex` or `heroes check` on `selfhost/main.hero` before and after
   with `/usr/bin/time -p`, and report `user` and `real`; if the machine was
   busy (`real` far above `user` plus `sys`), say so.

## Prediction

Register one falsifiable prediction with an instrument that exists today,
scored at this milestone's close (M-agreed-retention).

Write your report to `<your directory>/report.md`, and copy it to
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/181-reports/compiler-engineer.md`
(the only file you write in the trunk). English, no em dashes.
