---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **489 — `check` costs a deep record chain more than linearly** | 16.5 billion instructions at 5,000 records, 60.7 billion at 10,000, about the square (lane b14-emit, while witnessing defect 189) | `selfhost/check/`, `selfhost/resolve/` · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-emit's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the chain's square.
