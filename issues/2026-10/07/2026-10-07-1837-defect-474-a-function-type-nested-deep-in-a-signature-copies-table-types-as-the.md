---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **474 — a function type nested deep in a signature copies table types as the square of its depth** | `table.Ty` copies 41,050 at 250 and 143,925 at 500, no walk function growing; likely a whole-table copy, not located (lane b14-check) | `selfhost/check/` · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-check's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the nesting's square.
