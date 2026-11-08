# Panel 239, the completeness critic's brief

You are not a judge and give no verdict. You read and you run; you name what is MISSING. Your
worktree is `.claude/worktrees/239-critic/`, detached at the sitting's briefs commit on `p239`; build
your compiler there as `00-shared.md` says if a check needs one.

## First pass, before any seat starts: the briefs

Read every file of `docs/panel/239-briefs/` in lane `p239`
(`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p239/docs/panel/239-briefs/`):
`00-shared.md`, `proposal.md`, `compiler-engineer.md`, `ffi-pragmatist.md`, `spec-warden.md`,
`historian.md`; and the reproducers and outputs it cites under
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p239-scratch/` (`repro/`, `repro/mac.sh`,
`repro/mac.out`, `linux/repro.sh`, `linux/repro.out`, `linux/pthread.out`, `stage2.log`, `linux/build.out`),
read only: copy what you run into your own worktree. Then return, each item with the command you
ran:

1. **Every framing fact you could not verify with a command**: a number, a path, a line, a count, a
   quoted message, a program's output. Re-run what the brief ran; a fact that does not reproduce is a
   finding.
2. **Every negative sentence** (*no check compares one record's layout in two units*, *the compiler
   starts no symbol reader*, *no golden case builds a library*, *the spec says nothing of the
   libraries' order*, *GNU ld's trace names only the definition taken*): run it, or say it stands as
   a question.
3. **A route nobody listed** for either defect, beyond question 1's A to D and question 4's A to E:
   what would have to be true for it to exist, and whether it does.
4. **A question the sitting should ask and does not**, a seat handed a question its input cannot
   answer, and a shape beside either defect at depth one that no brief names.
5. **Whether the sitting's lane is right**: a full panel without the blind seat, for lack of the
   author's yes to a paid run; what that loses, and what a blind task would have to be to measure it.

Write it to `docs/panel/239-reports/critic-briefs.md` in your worktree as you go.

## Second pass, after the seats: the reports

The coordinator sends you the reports when the seats are done. Then: a route nobody built, a claim
asserted and not measured with the command that settles it, a contradiction between seats and which
side a command supports, and what the synthesis must not adopt without a further run. Write it to
`docs/panel/239-reports/critic-reports.md`.

## Rules

`00-shared.md` § Rules every seat keeps binds you as it binds the seats: every number from a command
you ran, no paid run, no credential printed, nothing written outside the repository's root, a program
that may not end bounded, no git command that writes, no em dashes.
