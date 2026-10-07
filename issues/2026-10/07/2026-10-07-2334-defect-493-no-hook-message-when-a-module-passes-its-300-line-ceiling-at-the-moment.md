---
kind: defect
area: process
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **493 — no hook message when a module passes its 300-line ceiling at the moment of writing** | `verify.hero`, `typeorder.hero`, `container.hero` and `ops.hero` passed 300 in lane b14-emit's work with no word from the write hook; `layout` whole found them later (lane b14-emit) | `.claude/hooks/fmt_check.py`, `.claude/hooks/ceiling.py` · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-emit's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): what a hook can see on the touched file waits for a suite (`.claude/rules/verification.md` § A suite is the last judge).
