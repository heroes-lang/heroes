# 049 — M-rotated-records: the records become directories

## Goal

Make it possible to work on more than one step at a time.

The author asked for that directly — worktrees, git flow, something — and then
moved the question to its cause: *"could it make sense to change the structure of
those files, maybe keep them leaner with sub-files, so as to avoid conflicts"*,
then *"rebuild DONE and DESIGN-LOG as folders, for the ones already done too"*,
then *"can't we keep only the map table in the ROADMAP and move all the content
into folders"*.

The measurement said the diagnosis was right and the first instinct was wrong.
Of the 100 most recent commits, `DESIGN-LOG.md` was touched by **40**,
`docs/ROADMAP.md` by **31**, `docs/work/DONE.md` by **26**, `docs/work/DECIDE.md`
by **24**. The compiler is almost never the collision: two sessions meet in the
registers. And a git workflow does not help, because the conflict is not about
branches — it is that one line is the only place two sessions can write.

So the milestone is a change of shape: **one entry, one file. One milestone, one
file. One lane.**

## What surprised

**The past cannot move, and that is not caution — it is the only honest option.**
This tree holds **161 citations by line number** into its two largest records,
and `.claude/rules/spec-shape.md` already says (CL-037) that a line number is the
one citation shape no instrument can see. **98 of the 133 into `DESIGN-LOG.md`
sit inside panels, journals and the record itself**, where §14 forbids anybody to
repoint them. Moving an entry out would have left every one of them aimed at
whatever slid into its place, with the whole net green. It is CL-069's shape one
level down: there, `§1`–`§15` were kept because 3280 citations named them.

**The map inverts the problem.** The entry moves and its LINE does not: the file
stays at its path with the line count it had, and each line names the file that
now holds what that line held. Then the thing CL-037 calls unreadable becomes
readable, because a line that is a path can be followed — and `records/positions`
follows all 161. **A shape adopted to avoid damage turned out to close a known
blind spot.**

**The rule this project cares most about had no executor.** CLAUDE.md §14 —
*a record is never rewritten* — is stated in four documents, and until this
milestone nothing performed it. Commit `76799a18` had already removed a `- [x]`
line from the record, and every suite stayed green. The oldest story in
`suite_records.hero` is *a rule in a preamble is a wish*, and it was true about
the rule that protects the record.

**Two citations were already broken and nobody could tell.** `DONE.md:924`, cited
from a measurement and from a decision entry, had pointed at a **blank line**
since the day it was written. It reached nothing. Making the blank lines inside
an entry carry their entry's pointer fixed a citation that had never worked.

**The geometry is the promise, and prose does not land on a line count by
itself.** The preamble of a map must occupy exactly the lines it occupied before.
It was got wrong on the first attempt for `beats.md` (one line too many, and
every beat moved down by one) and took three attempts for `DONE.md`'s 33.

## What broke and why

**Every check that fired was right, and two of them were right about the wrong
thing**, which is the more useful half.

- `records/rotated` reported `DESIGN-LOG.md` as naming a file git does not
  track. The name was `docs/records/log/`, still `date | decision | reason | design.md`
  — the reader ran from the tree's name to the first `.md` and sewed the map's
  own preamble into a path. **A path has no space in it**; the name now ends at
  a space, a backtick or a tab, and the sewn line is a test case.
- `records/citations` refused a new test's literals, because
  `docs/doneness/not-under-it.md` reads as a path claim. A test about tree
  walking cannot use names that look like this project's.
- `records/lists` found a **third banner** in `M-publication-gate.md`: the last
  item of `SCHEDULED.md` ran to the end of the file and carried the list's own
  closing fence into the milestone it moved to.
- `records/numbered` found `M5c` in `M-online-compiler.md`, legal while it sat
  inside the ROADMAP (exempt by suffix) and not legal in a live file. The repair
  was the text, not a new exemption — and `records/exemptions` refused the
  exemption reached for first, correctly, because it selected nothing.
- `records/numbering` read **zero** defect numbers the moment `DONE.md` became a
  map, and **said so instead of passing**. That is its floor, written the day
  before it was needed.

**The three new checks were each watched failing in a detached worktree before
they were trusted** (CLAUDE.md § 9: a check nobody has seen fail is a
decoration): a line cut from the record, named back by its own text; a tracked
file carrying a conflict marker, named by path; a second `027` filed the way a
parallel session would file it. The third proof showed something nobody asked
for — `records/appended` stayed quiet on the append itself, which is the
distinction it exists to make.

**And the milestone was written while the thing it describes was happening.** A
parallel session committed four times into this checkout during the work,
including once **between** step 1 and step 2, and pushed. `DESIGN-LOG.md` grew
from 1036 to 1119 lines while it was being measured. Every number here was
re-measured at the moment it was written, which is the rule that made the account
survive.

## What landed, and what carried forward

**Landed**: `DESIGN-LOG.md` → `docs/records/log/` (620 entries), `docs/work/DONE.md` →
`docs/done/` (491), `docs/records/book/beats.md` → `docs/records/book/beats/` (114), and
§ The milestones, one by one → `docs/milestones/` (43 files), which absorbed the
53 items `docs/work/SCHEDULED.md` held until 2026-09-12. Each rotated file keeps
its path and its exact line count. Five checks with them — `records/appended`,
`records/numbering`, `records/unmerged`, `records/rotated`, `records/positions` —
taking the suite from 15 checks to 20 and the net's own tests from 130 to 137.
The ROADMAP falls from 2768 lines to 761 and from 58,318 tokens to 23,631; the
three maps fall from 607,542 tokens to 162,206 between them.

**The chain entry**: row 74, closed 2026-09-12, `m-rotated-records`. It stood
untagged for one commit, which is the part worth keeping: the milestone filed the
decision that blocked its own tag rather than taking it, because
`site/src/lib/chain.ts` is outward-facing. The author answered the same day and
took the recommendation, and the tag went on the commit that settled it, which is
where CLAUDE.md § Verification puts it.

**And settling it found the same assumption one level up.** Removing the throw
was not the whole repair: `chainSection` wrote *"the open one is X"* from
`rows.find`, so with two lanes the page would have named the first milestone and
been silently wrong about the second. The page lists them all now, in both
editions. A guard and the prose beside it had both been written when one open row
was the only shape there was.

**Carried forward**: the lane protocol is written (`.claude/rules/records.md`
§ Working in lanes) and is one decision short of usable. A second lane can hold
its own milestone file, its own log entries, its own record entries and its own
beat without touching another session's line — but it cannot mark its row
`**OPEN**`, because `site/src/lib/chain.ts` throws on the second one. Until then
a second lane runs with its row `scheduled` and says so in its commits.
