---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **512 — `check/reach.hero`'s `case_pattern` searches a variant's cases by name at each arm** | 0.54, 1.50 and 4.82 billion instructions at 400, 800 and 1,600 cases with defect 409's repair, still quadratic (lane b15-check) | `selfhost/check/reach.hero` (`case_pattern`) · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-check's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the square of the cases.
