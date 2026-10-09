---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **511 — `check/flow.hero`'s branch joins visit every local's group at each meet** | n handles ended, then n `if`s: 3.24, 11.3 and 41.2 billion instructions at 500, 1,000 and 2,000 after defect 499's repair, quadratic (cubic on the base, 93.9 and 742 billion at 500 and 1,000); shared or copy-on-write groups would answer it (lane b15-check) | `selfhost/check/flow.hero` · defect 499 · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-check's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the square of the handles.
