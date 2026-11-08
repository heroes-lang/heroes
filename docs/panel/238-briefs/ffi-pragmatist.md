# Panel 238, the ffi-pragmatist's brief

Read `00-shared.md` and `proposal.md` beside this file first. Your worktree is
`.claude/worktrees/238-ffi-pragmatist/`, detached at the briefs' commit; build your compiler there in
two stages as `00-shared.md` says. You sit because the critic's first pass falsified the briefs'
*the change touches no C boundary* (F20): the written order of `use` lines orders the C headers of
the one-file emission and the libraries on the link line, and a sorted order turned a program whose
`--emit-c` exits 0 into `ffi_one_file`, exit 1. Your mandate is design.md §1.11 and §4.19; you hold a
veto on what breaks the C ABI or makes bindings categorically harder. Compile, don't opine.

## What the sitting needs from you, each measured

1. **The class F20 opens, counted.** Read `selfhost/cli/one_file.hero` and the critic's
   `build/238/c453/` and `cside/` (copy them into your own `build/238/`). Which shapes of two or more
   modules with `extern` groups make the written order decide: whether `build --emit-c` accepts the
   program (a macro one header defines and another reads, a type one header declares and another
   needs, a `link` and a `package` group, a header included by two groups), what the one-file C
   means, and what the per-module units mean (`heroes build` and `heroes run`, which compile a unit
   per module: does any order change what a unit compiles?). Then count it over the tree: every
   `examples/` program and `tests/golden/run` case with two or more modules carrying `extern`
   groups, asked `build --emit-c` in its written order and in the order the rule would write (F5's
   key, or the compiler-engineer's prototype if it is ready; say which), and every `examples/`
   program asked `build` and `run` both ways. A correct program refused by one order and not the
   other is a `blocking` class (`.claude/rules/verification.md` § Bounded discovery): say so with its
   reproducer.
2. **The link line.** F20's link words follow the order (`"-lm" "-lc"` against `"-lc" "-lm"`). On
   this Mac (ld64) and in the Linux arm64 container (`heroes-linux-arm64` is built,
   `docs/platforms/linux/LINUX-MACHINE.md` says how to run it; mount your worktree only, never write
   outside the repository), build two modules that link two static archives where one archive needs a
   symbol of the other, in both orders, and say whether a link fails or binds another definition in
   one order and not the other (how each linker reads archives in command-line order is a question
   each manual answers: read both, then run both). Then the shared-library and `package` cases: two
   libraries defining one symbol, which one a call binds in each order.
3. **The seed and `compat`.** `--emit-c` is the seed's bootstrap (design.md §4.1, lines 936-944)
   and what `compat` asks of every release program (`suite_compat.hero:15`). Say whether the
   compiler's own `selfhost/` reformatted can change what `--emit-c` writes for the compiler in a way
   that matters to C (the compiler-engineer measures the bytes; you say whether any of its modules
   carries an `extern` group whose order could matter, `git grep -l '^extern' -- selfhost/`), and
   whether a release's programs reformatted by the rule could leave `compat`'s green.
4. **The route.** Whether the robust answer is to make the order immaterial in the compiler (the
   units, the one-file emission and the link words ordered by a key independent of the written
   order, `00-shared.md` § A) or to keep the written order as the program's own and let `fmt` move
   it; and what each costs a binding author. A binding whose meaning depends on the order its modules
   are named in is the thing §4.19's header verification exists to refuse: say whether it is refused
   today in every shape you found.

Your report: `docs/panel/238-reports/ffi-pragmatist.md` in your worktree, written as you go, ending
as `00-shared.md` § Rules says, in the structure of `.claude/agents/ffi-pragmatist.md`.
