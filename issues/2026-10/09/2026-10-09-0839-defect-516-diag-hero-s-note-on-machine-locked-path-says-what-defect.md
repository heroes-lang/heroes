---
kind: defect
area: compiler
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **516 — `diag.hero`'s note on `machine_locked_path` says what defect 446 measured false** | it calls the name a thesis rule *because C would take the program on exactly one machine*; defect 446 measured ld64, GNU ld and lld-link reading a rooted `-l` as another path, never the one written (lane b15-box) | `selfhost/diag.hero` · defect 446 · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-box's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a comment that states a premise measured false.
