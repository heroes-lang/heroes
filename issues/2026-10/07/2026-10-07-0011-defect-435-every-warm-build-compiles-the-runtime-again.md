---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **435 — every warm build compiles the runtime again** | a warm `heroes build` runs `clang -c runtime/runtime.c` every time, read in the clang-call log by lane b13-run400; its cost unmeasured | `selfhost/cli/`, the runtime object's cache · **class: improvement**

    **Origin:** filed by the coordinator at 00:11 on 2026-10-07, from lane b13-run400's report of the evening before (*found beside*); the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost, no program judged wrong.
