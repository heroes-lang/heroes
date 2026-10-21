# Panel 227, completeness critic, first pass: the briefs, before any seat starts

Read every file under `docs/panel/227-briefs/` (the shared brief, the compiler-engineer's, the
spec-warden's, the historian's, and `blind/`). **For every framing fact a brief hands a seat**, a
number, a path, a line, a message, a claim that something does or does not exist, run the command
that settles it in the lane `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/thesis`
(read-only: `grep`, `sed -n`, `./heroes-next check` on the probes under
`.claude/worktrees/thesis-drafts/227/probe/`, `./heroes-next measure` on the copies under
`.claude/worktrees/thesis-drafts/227/spec/`) and say whether it holds. Then name what is missing: a
route nobody listed, a seat handed a fact it cannot check, a question the sitting should ask and
does not, a negative sentence written without the search that would support it.

Write `docs/panel/227-reports/critic-briefs.md`: each fact, the command, holds or not; then the
gaps. Write nothing else; never write outside the repository's root; print no credential; start no
paid run.
