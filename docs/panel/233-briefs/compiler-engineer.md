# Panel 233, the compiler-engineer's brief

Read `00-shared.md` first; it binds you. Your worktree: `.claude/worktrees/233-compiler-engineer/`.
Your report: `docs/panel/233-reports/compiler-engineer.md`, written as you go.

Pointers into the live compiler (line counts by `wc -l` at the frozen commit): `selfhost/emit/aggregate.hero`
(351, records as C: construction, field reads), `selfhost/emit/construct.hero` (278, a composite value
coming into existence), `selfhost/emit/byte_copy.hero` (120), `selfhost/emit/typeorder.hero` (409,
`IN_ORDER_FROM`), `selfhost/emit/signature.hero` (257, a function's C signature), `selfhost/emit/ctype.hero`
(548), `selfhost/cli/flags.hero` (584, what the C is compiled under), `runtime/heroes_runtime.h:269-272`
and `:389-408`.

Your task: **build the route you judge most robust in your worktree and measure it** before you write
your verdict (the panel skill, § 3c: a route that does not build is not adopted). For the reproducer at
k = 16 and k = 18 and for the filing's other three shapes (`00-shared.md`), clang's instructions retired
and peak memory on the program's unit before and after (the command in `00-shared.md`); the program's
`-O2` output identical before and after, and its `-O0` run not made worse (at k = 16 and 18 it aborts
`stack exhausted` today, the frames `-O2` elides being ones `-O0` keeps, so a route that keeps those
frames at `-O2` must say whether it turns a correct `-O2` run into that abort); and the emission of
every program in the tree that the route changes, counted (`heroes build <program> --emit-c` with your
compiler against the frozen one's, over `examples/` and `tests/golden/run/` at least), since every
changed emission is a blessed file to read. Say what the route costs a program it does not need to help,
in instructions at run time where you can measure it, and whether it closes or opens any robustness class
(design.md §1.12): a frame larger than the stack (defect 2253) is one such record's other face.
