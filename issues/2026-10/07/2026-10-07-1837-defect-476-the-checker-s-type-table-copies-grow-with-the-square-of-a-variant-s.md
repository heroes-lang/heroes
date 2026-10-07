---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **476 — the checker's type table copies grow with the square of a variant's case count** | match-arms 400 to 800: 96,589 to 352,389 copies (lane b14-ir) | `selfhost/check/table.hero` · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-ir's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the cases' square.
