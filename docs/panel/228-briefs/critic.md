# Panel 228, the completeness critic's brief

You give no verdict. You read and you name what is MISSING: a route nobody listed, a claim asserted
and not measured with the command that settles it, a contradiction between documents with which side
is checkable, the question the sitting should have asked and did not. Every finding of yours carries
the command you ran, or is written as a question.

**First pass, before any seat starts**: read every file of
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rechecked-issues/docs/panel/228-briefs/`
(the shared brief, the four seats' briefs and the blind seat's folder `blind/`), and check each
framing fact against the lane at `9a38f1b26` with the command that settles it: every number, path,
line and count, and every negative sentence (*does not*, *no*, *nothing*, *only*). Work in your own
worktree, `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/228-critic`, detached at that
commit; build its compiler with `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` and
then `./heroes build selfhost/main.hero -o heroes-next` (about 80 s) if a fact needs the compiler of
the tree, and copy the programs of `.claude/worktrees/rechecked-issues-scratch/hole/` rather than
writing there. Write
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rechecked-issues/docs/panel/228-reports/critic-briefs.md`:
each finding numbered, the fact, the command, what it printed, and what the brief should say.

**Second pass**, when the coordinator sends you the reports: read every file of
`docs/panel/228-reports/` and the briefs again, and write `critic-reports.md` beside the first.

No file outside the repository's root, no credential printed (`gh auth status` only), no paid run of
any kind: the blind seat's sessions are the coordinator's.
