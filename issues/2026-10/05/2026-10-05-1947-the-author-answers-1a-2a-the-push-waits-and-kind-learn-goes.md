---
kind: decision
area: records
milestone: M-issue-files
filed: 2026-10-05
commit: self
github: none
---

# The author answers 1a 2a: the push waits for the next batch's legs, and `kind:learn` goes with `learn.md`

2026-10-05, written at 19:47 by the clock (`date`). The coordinator put two
questions with a recommendation each once the reorganisation of `docs/` and
the questions' move had landed on the trunk at `a1383304`, nine commits that
`origin` did not hold; the author answered *"1a 2a"*, the two
recommendations. Recorded as a reading.

1. **No push now** (*1a*): the nine commits travel with the next push of the
   session closing the defects, after that batch's Linux arm64 and Windows
   legs over lanes that merged this trunk, which is CLAUDE.md § Verification's
   rule and no exception to it. One more reason was measured before the
   answer: the CI cancels a run in progress on the same ref
   (`cancel-in-progress: true`, `.github/workflows/ci.yml`), so a push at
   19:40 would have cancelled run 37347062496 on `ca5fa51e`, the run pushed to
   read clang's words on Windows for defect 238.
2. **`kind:learn` leaves GitHub with `learn.md`** (*2a*): the label is deleted
   in the same moment as the push that carries `de053027`, the commit that
   took the form `learn.md` out of `.github/ISSUE_TEMPLATE/`. No issue carried
   the label that day (`gh issue list --label kind:learn --state all` read 0),
   and a label no issue can carry is a false statement on a public tracker.
   The act is the open task
   `issues/2026-10/05/2026-10-05-1947-the-kind-learn-label-leaves-github-when-learn-md-does.md`,
   ticked by whoever makes that push.
