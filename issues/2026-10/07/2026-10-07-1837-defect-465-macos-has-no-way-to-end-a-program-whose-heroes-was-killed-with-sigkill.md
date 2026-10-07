---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **465 — macOS has no way to end a program whose `heroes` was killed with SIGKILL** | defect 437 closed it on Linux with `PR_SET_PDEATHSIG`; on the Mac lane b14-runtime built and measured a sentinel process per runner holding a pipe, which killed the child 3 of 3 for about 0.85 million instructions a fork (`<scratchpad>/batch14/runtime/sentinel/sentinel.c`); it adds a long-lived process to every `heroes` that launches anything | `runtime/parts/run.c` · defect 437 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-runtime's final report (*decisions* 2); the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a design question, a sitting's.
