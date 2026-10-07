---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **492 — the emitter's instance lookup scans every function per call** | after defect 418 made it one lookup, it still walks every function for each call (lane b14-emit, unmeasured) | `selfhost/ir/instances.hero` · defect 418 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-emit's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with functions times calls, unmeasured.
