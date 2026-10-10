# Panel 208, compiler-engineer

Read `00-shared.md` beside this file first. Your directory:
`.claude/worktrees/scratch-b15/208-compiler-engineer/` (its `tree/` is
`391628b6`). Your ground is implementation cost, soundness and core-against-sugar
(design.md §1.1, §1.7, Part 5); you hold a veto on soundness.

1. **Explain the measured asymmetry first**: why the program's own call of
   `twice` from `dep.h` warns and its call of `sprintf` from `stdio.h` does
   not. Read `selfhost/emit/deprecation.hero`, `emit/macro_guard.hero`,
   `emit/header_region.hero` and the emitted unit (`--emit-c`, or the
   `build/tu-*/` C) of both programs. If the answer is clang's system-header
   rule, say which flag or include route makes `dep.h` a non-system header
   and whether a package's header is one or the other.
2. **Build the two routes that need no new form**, (R) and (S), in your copy:
   the smallest change that makes each true over every shape in the shared
   brief's list, with their cases run. For (R): where the verdict is read
   (clang's diagnostic recovered at the `.hero` line, as
   `.claude/rules/c-boundary.md`'s six members are, or a probe asking the
   header), the message, and whether it is exact on all shapes. For (S):
   which pragma or flag, where, and that nothing else is silenced with it
   (a header's `-Wall` region is panel 205's R2; it must not widen).
3. **Price (M) and (F)** without building them: lines, modules, the grammar
   and the formatter's walk for (M); the CLI's stopping rule for (F).
4. Cost of each built route: instructions retired of a cold `heroes build`
   of the reproducer and of `heroes check selfhost/main.hero`, before and
   after, `/usr/bin/time -l`, `real` against `user`+`sys` read.
5. Report to `docs/panel/208-reports/compiler-engineer.md`: the asymmetry's
   cause, each route's diff size (files, lines), cases and verdicts per shape
   (a table), cost, your verdict per route and your recommendation, and one
   prediction someone can score at the landing.
