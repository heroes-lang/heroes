# Panel 207, compiler-engineer's brief

Read `00-shared.md` first. Build in your copy an exact evaluator of a
constant's written body (the forms `constant_body.hero` admits: literals,
other constants, bindings and cells, `if`, `match`, `while`, `for`, arrays,
maps, strings, records), stepping each operator at its recorded width, with a
bound on its steps, and run it on `p207/`, on panel 206's cases
(`tests/golden/check/fixedbugs-577-*` in this tree and the critic's
`.claude/worktrees/scratch-b15/critic-206b/shapes/`) and over every tracked
`.hero` file (which file moves, at which exit code, and every tracked
constant's body evaluated within the bound or not): lines (`layout`'s unit),
instructions on `check selfhost/main.hero`, what the bound should be and what
the compiler's own constants need. A route you recommend is built and run
before you report.
