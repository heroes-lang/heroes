---
kind: defect
area: process
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **515 — the commit guard judges the index alone, so a commit of unstaged paths passes unjudged** | `.claude/hooks/staged.py` reads `git diff --cached`, so `git commit -F <msg> -- <paths>` of files never staged commits content no hook judged; at about 04:15 on 2026-10-09 a module over its ceiling and unstaged gave `staged.offences()` nothing while `ceiling.verdict()` refused it (lane b15-emit) | `.claude/hooks/staged.py` · defects 287 and 403 · **class: adjacent**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-emit's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach: the commit the hard stops prescribe is the one it does not judge.
