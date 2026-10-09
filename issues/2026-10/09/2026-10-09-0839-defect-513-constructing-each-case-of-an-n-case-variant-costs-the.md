---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **513 — constructing each case of an N-case variant costs the square of N** | 0.62, 1.59 and 4.90 billion instructions at 400, 800 and 1,600 cases on defect 409's tree; read as `case_construct`'s name scan, unsampled (lane b15-check) | `selfhost/check/` (`case_construct`) · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-check's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the square of the cases.
