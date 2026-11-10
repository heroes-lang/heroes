# Panel 241, the compiler-engineer's brief

Read `00-shared.md` beside this file first (rewritten on the critic's first pass). Your worktree:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/241-compiler-engineer` (detached at
87d77dec2); build your compiler there from the seed and nowhere else. Your report:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p241/docs/panel/241-reports/compiler-engineer.md`.

## Question 1 is yours, built

1. **Build route 1 in your worktree**: `heroes add core --index file://../packages -C selfhost`, then
   move the package manager's calls whose contract is `core/strings`' (`has_prefix`, `has_suffix`,
   `contains`; count them again, the scoping said 22, 6 and 5) to `use <core>/strings`, and run the
   two-stage build, the fixpoint (`cmp` of stage 2's and stage 3's `--emit-c`), the compiler's own
   tests, and the net's `order`, `probe`, `spec`, `layout`, `canonical`, `seed`, `records`, `unseen`
   and `packages`, each into its own file. Where the guard refuses a run because the trunk's
   `provenance.py` does not follow `use <name>/...`, do not try to get around it: write the patch it
   needs in your worktree, run the hooks' own tests on it there (`python3 -I -m unittest discover -s
   .claude/hooks -t .claude/hooks`), and report the lines; the runs it blocks stay unrun.
2. **The admission count** (question 1 is a new ruling): which functions of `core` 0.2.0 the
   compiler would call, how many hand copies in `selfhost/` each replaces (`grep`), and whether
   `cli/strings`' `trimmed` and `with_all` have a `core` contract (the scoping said no).
3. **The costs** named in `00-shared.md`: `heroes check selfhost/main.hero` and the warm build,
   before and after, by `heroes run tests/harness/latency.hero -- compare <before> <after> --clock
   check` (and `build` if time allows; say the machine was busy where it was); the `#line`s stage 2
   writes for the placed folder (count them), and so what each `core` version costs the seed; which
   instruments need to learn the placed tree, and the lines each needs; what the site's sizes and the
   ROADMAP's count would read.
4. **R13 literally** (a copy at `selfhost/core/`): reproduce its refusal and say whether anything
   short of editing the copy's bytes makes it compile.
5. **Question 3 from the compiler's side**: what the package manager's transport would need to move
   to an `os/exec` package (the modules, the calls, `hero_env_put`'s three callers), and whether
   question 2's route (c) (a third door hashing a `[u8]`) needs anything in the compiler beyond a
   declaration, and whether it moves `HERO_RUNTIME_ABI` and the seed's route (`seed/README.md`).

## Your verdict

Per question: approve a route, object or veto, with the cost in lines and seconds. Veto on soundness
only, naming the program or compiler fact that would make the route wrong.
