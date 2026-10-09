---
kind: defect
area: process
milestone: none
filed: 2026-10-09
commit: 5d6820b2d5af666e86da953737ebb527950631f9
github: none
---

- [ ] **515 — the commit guard judges the index alone, so a commit of unstaged paths passes unjudged** | `.claude/hooks/staged.py` reads `git diff --cached`, so `git commit -F <msg> -- <paths>` of files never staged commits content no hook judged; at about 04:15 on 2026-10-09 a module over its ceiling and unstaged gave `staged.offences()` nothing while `ceiling.verdict()` refused it (lane b15-emit) | `.claude/hooks/staged.py` · defects 287 and 403 · **class: adjacent**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-emit's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach: the commit the hard stops prescribe is the one it does not judge.

    Repaired at `5d6820b2`, 2026-10-09 (lane b16-tools, batch 16), gated by its cases and the hooks' own tests; the net is owed at the batch's close. The check judges what the commit carries: each file a pathspec names that differs from HEAD as the working tree holds it, staged or not, and every other staged file as the index holds it, the mirror of the same cause (a merge concluded with a file staged broken and repaired in the working tree alone was committed broken, measured). 8 cases, 5 red on the base; the hooks' own tests 110 run and 2 failed, the two `Place` tests the base fails the same way with its temporary folder inside the repository.
