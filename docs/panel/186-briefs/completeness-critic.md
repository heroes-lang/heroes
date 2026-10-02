# Panel 186, the completeness critic's brief

You give no verdict. You make two passes, and the procedure is
`.claude/skills/panel/SKILL.md` § 3b and § 3c.

**First pass, now, before any seat is launched**: read every brief in
`docs/panel/186-briefs/` (`00-shared.md`, the two seats' (`compiler-engineer.md`, `ffi-pragmatist.md`) and `probes/`), and
for every number, path, count and negative sentence in them, run the command
that settles it on the frozen tree (a copy of `ae08ed93` in
`<scratchpad>/186-critic/`, your compiler built inside it from the seed;
never the repository's `./heroes`, never another seat's directory). Write
`docs/panel/186-reports/completeness-critic-briefs.md`: each framing fact you
could not verify, each one you found false (with the command and its
output), each route nobody listed that the question admits, and each place a
seat is handed a premise rather than a question. Then STOP: the coordinator
repairs the briefs and only then launches the seats.

**Second pass, when the coordinator resumes you**: read the seats' reports in
`docs/panel/186-reports/` and the briefs, and write
`docs/panel/186-reports/completeness-critic.md`: what is missing, a claim
asserted and not measured by the command that settles it, a contradiction
between seats and which side is checkable, the question the sitting should
have asked and did not.

**No paid run**: no `claude -p` session, API call or cloud run. One you find
worth running goes in your report with its size. Long commands in the
background, polled with short calls; never more than three processes at
once (three repair lanes work on this machine).
