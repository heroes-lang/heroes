# M-rechecked-issues — every open issue re-checked against the tree

**Scheduled 2026-10-14, at row 70, by the author's instruction**, meant as:
*a step that analyses all the issues currently open, to see which are still
valid, which are already solved and which are no longer pertinent, resolves
some, and leaves the fewest open*. It runs in a lane beside M-thesis-harness,
whose files it does not touch. No warrant is claimed.

**What was counted before it was scheduled.** 65 open issues on the morning
of 2026-10-14 (16 defects, 23 features, 26 tasks), 52 that evening once
round 17 had closed its repairs; of the 49 features and tasks, 44 were filed
in September against a tree that has since closed rows 33 to 65; 13 issues
named no milestone; the oldest open one is issue 1, of 2026-08-03. Reading
their titles found premises already false (issue 860 asks for three repairs
that landed as defects 381, 010 and 143; issue 1234 says nothing compiles what
`--emit-c` wrote, against defect 298's account). A reading is not a
measurement, which is why this row exists.

**What it delivers.** Every issue open at its start carries a dated comment
saying against which commit it was run again and what came out, and leaves
in one of four ways: closed with the commit that settled it; closed with the
reason it no longer applies, the reason a comment and never an edit; repaired
here when it is small (issues 1075 and 1077, the hole report inside
`heroes check --json`, and `non_exhaustive`, which design.md Part 11 counts
in the control arm and `selfhost/diag.hero`'s `is_thesis_rule` does not); or
re-homed on a row still to come (issue 1001 to M-panic-location, issue 1234
to M-generated-programs). Beside the issues: design.md Part 7's return
conditions and Part 8's warts 8, 15 and 16 run again with their own commands;
the formatter's probe M-inferred-cell's changes still owe; the false sentences
the re-ordering found, corrected underneath (`editors/README.md`, the
`heroes_lldb.py` module doc, `selfhost/cli/help.hero`'s footer, the site's
log, `docs/roadmap/production-ready.md`, and `selfhost/emit/header_region.hero`'s
citation of `2067ec875`, a commit GitHub does not hold); `tools/spike/` out of
the tree; and one line in `/step`'s close checklist so the pile does not grow
back: a feature or a task that two milestone tags have passed gets a dated
re-check.

**Acceptance.** `records/homes` and `records/tagged` green with GitHub asked;
every issue open at the start carries its dated comment; the counts after
measured beside the counts before, in the journal and never in the ROADMAP.

**Named for the deliverable**, the issues re-checked. Not
`M-triaged-issues` or `M-issue-triage`, which name the act and the area, so a
second triage could never take the name; not `M-settled-backlog` or
`M-settled-issues`, which claim a state the close cannot deliver, `settled`
being the word `close` already writes for a decision.
