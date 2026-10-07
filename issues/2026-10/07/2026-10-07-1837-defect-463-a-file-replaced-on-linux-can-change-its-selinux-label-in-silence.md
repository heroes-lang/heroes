---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **463 — a file replaced on Linux can change its SELinux label in silence** | the stage skips any `security.*` attribute it cannot set, so a replaced file could silently change label; no SELinux in the container to test (lane b14-runtime, unrun) | `runtime/parts/replace.c`, `runtime/parts/write.c` (batch 14's round, unmerged on 2026-10-07) · defect 438 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-runtime's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a measurement owed on a Linux with SELinux.
