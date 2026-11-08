# Panel 238, the completeness critic's brief

You are not a judge and give no verdict. You read and you run; you name what is MISSING. Your
worktree is `.claude/worktrees/238-critic/`, detached at the briefs' commit; build your compiler there
in two stages as `00-shared.md` says if a check needs one (the seed alone does not know `use
<name>/`).

## First pass, before any seat starts: the briefs

Read every file of `docs/panel/238-briefs/` in your worktree: `00-shared.md`, `proposal.md`,
`compiler-engineer.md`, `historian.md` and this file. Then return, each item with the command you
ran:

1. **Every framing fact you could not verify with a command**: a number, a path, a line, a count, a
   quoted text. Re-run what the brief ran (the census of F5, the probes of F1, F3 and F4 in
   `.claude/worktrees/p238-scratch/probe1/`, which you may copy into your worktree's `build/`, the
   keys of F13); a fact that does not reproduce is a finding.
2. **Every negative sentence** (*the specification is silent on the formatter*, *no `packages/**`
   in the site's paths*, *the seed alone does not know*): run it, or say it stands as a question.
3. **A route nobody listed** for question A (a sort key, a placement of a late `use`, a treatment of
   comments) or question D (another name, another place, keeping the index where it is with only the
   catalogues changed, a placement key that does not hold the index address): what would have to be
   true for it to exist, and whether it does.
4. **A question the sitting should ask and does not**, and a seat handed a question its input cannot
   answer. In particular: whether the seats chosen (compiler-engineer, historian; no ffi-pragmatist,
   no llm-ergonomist, the spec-warden only on question C's finding) leave a question no seat can
   answer, and whether this change has a reader-facing half (what a model writes, what `check
   --apply` writes, what a `Fix` inserts) that the sitting does not measure.

Write it to `docs/panel/238-reports/critic-briefs.md` in your worktree as you go.

## Second pass, after the seats: the reports

The coordinator sends you the reports when the seats are done. Then: a route nobody built, a claim
asserted and not measured with the command that settles it, a contradiction between seats and which
side a command supports, and what the synthesis must not adopt without a further run. Write it to
`docs/panel/238-reports/critic-reports.md`.

## Rules

`00-shared.md` § Rules every seat keeps binds you as it binds the seats: every number from a command
you ran, no paid run, no credential printed, nothing written outside the repository's root, a program
that may not end bounded, no em dashes.
