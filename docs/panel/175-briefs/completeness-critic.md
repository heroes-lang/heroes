# Panel 175 — completeness critic

You are not a sixth judge and you give no verdict. You read the six briefs in
this directory and the five reports in `docs/panel/175-reports/`, and you name
what is MISSING: a route nobody listed, a claim asserted and not measured with
the command that settles it, a contradiction between seats with which one is
checkable, and the question the sitting should have asked and did not.
`.claude/skills/panel/SKILL.md` step 3b is your definition.

Work in `<scratchpad>/175-completeness-critic/`, copied from the trunk at
`64c92654` and built from the seed there; never build or run in the trunk or in
another seat's directory. You may READ the seats' probe directories under
`<scratchpad>/175-<seat>/` to rerun what they ran. Write your report to
`docs/panel/175-reports/completeness-critic.md`.

## What the coordinator can tell you without steering you

**The procedure had a fault, and it is the coordinator's.** For a stretch after
the seats launched at 10:24 — its start and end were not clocked; the one time
on record is the llm-ergonomist's report, written in the worktree at 10:33 —
the coordinator's own session was switched into a git worktree, and
Claude Code's isolation then refused the running seats' writes to the trunk and
some of their shell calls. The llm-ergonomist wrote its report into the
worktree and it was moved; the spec-warden reports a stretch of refused shell
calls. Say whether any seat's measurement was lost or bent by it.

**Three things were found outside the briefs and are known to the coordinator**,
so report them only if you find them wrong or incomplete:

- the spec-warden's F1, a handle given back after C reused its address, which
  the coordinator reproduced (`check` 0, `run` 0, three of three) and will file
  as a defect;
- the spec-warden's F4, `owned` on a `const char **` cell stopping the build
  with `internal error`, reproduced and to be filed;
- panel 170's ratified item 8, *spec § 13 owes the handle-only rule, +26
  real*, which the coordinator confirmed has no landing and no ledger row.

**And defect 076 was repaired in a lane during the sitting**, on panel 173 R1
and without a sitting, as the shared brief said it would be. It touched
`runtime/parts/str.c` only.

## Where the seats' routes meet, which is where you should look hardest

Question 1's routes each key the releaser somewhere: the checker, the runtime
set, the type, and now the seats have added others. Question 2's route E speaks
from a handler. Say, for every route a seat recommends, which of these shapes
it has been RUN against and which it has only been argued against: a handle
through a record, a handle acquired in one module and released in another, a
`borrows` result later consumed, a handle whose address C reuses, a consuming
call that transfers rather than closes, and two releasers that are both valid.
