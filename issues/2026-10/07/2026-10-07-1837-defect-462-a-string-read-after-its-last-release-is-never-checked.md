---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **462 — a string read after its last release is never checked** | only increments and releases look at a string's mark; a read (print, concat) does not, so the doubled control of defect 314 printed garbage bytes before panicking (lane b14-runtime); a check per read costs a load per read, unmeasured | `runtime/parts/str.c` · defect 314 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-runtime's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach over a compiler's fault; no correct program is wrong for it.
