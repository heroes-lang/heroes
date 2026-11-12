# Panel 246, the completeness critic's first pass, over the briefs

Read every file in `docs/panel/246-briefs/` of the worktree `.claude/worktrees/p246`, whole (the
`*.before-critic.md` copies are the same text, kept for the record; read the plain ones). Before any
seat starts, name every framing fact a brief states that you cannot verify with a command you run (a
line number, a path, a count, a claim about what a document, the spec or the compiler says or does
not say), each with the command you ran and what it printed; every route nobody listed that a reader
of the issues (defect 2453; features 1073, 1069, 925, 893, 1072 and 2295; panels 111, 113, 114, 232,
234 and 235's R12) would ask about; and every question the sitting should ask and does not.

Three things the briefs' author flagged as inferred and not run, which you settle or hand to a seat
with the command that would settle them:

1. **PID reuse in the staging folders**: `staging.opened` removes any folder with its own name first
   (`selfhost/fetch/staging.hero:48`) and the per-run serial restarts at 1 in every process
   (`runtime/parts/run.c:1425-1428`), so a later run handed a dead run's PID may delete that run's
   `replaced-N`, the only copy of an edited tree. Read both, and say whether a reproduction is
   possible on this Mac without waiting for the kernel to recycle a PID (for example a test door, or
   a run whose PID is chosen); if it is, run it in your folder.
2. **A killed `heroes build`** can leave a staging folder on an error path (`module/unplaced.hero:46`):
   whether part A's recovery must account for it.
3. **Feature 1069's re-check of 2026-10-25** built a working stop with a three-line header the program
   ships (`stopflag.h`): whether panel 234's R6 (*no package ships platform C of its own*) refuses
   that route for a core package and leaves it open for a program, and whether the briefs present it
   to the seats as a route.

Build a compiler at the base (`b8c8240d4`) in `.claude/worktrees/p246-scratch/critic/` from the seed
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, then `./heroes build
selfhost/main.hero -o heroes-next`) if a reproduction needs one. Work read-only in the repository;
write only under `.claude/worktrees/p246-scratch/critic/` and your report. No paid run, no
credential (`gh auth status` at most, never `gh auth token`), no GitHub write, no file outside the
repository's root, one heavy command at a time (four other lanes are building on this Mac).

Write your report to `docs/panel/246-reports/critic-briefs.md` in the p246 worktree as you go, and
end it with a numbered list: each item the fact, the command, what it printed, and the repair you
propose to the brief.
