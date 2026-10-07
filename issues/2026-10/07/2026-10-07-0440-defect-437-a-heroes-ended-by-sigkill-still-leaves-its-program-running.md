---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **437 — a `heroes` ended by SIGKILL still leaves its program running** | no process can catch SIGKILL, so a `heroes` killed that way still orphans its child after defect 425's repair; Linux offers `PR_SET_PDEATHSIG`, which the lane did not add, and macOS has no equal | `runtime/parts/run.c`, the child before exec · **class: improvement**

    **Origin:** filed by the coordinator at 04:40 on 2026-10-07, from lane b13-tmpl407's report (*found beside*); the lane's reading, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): hardening of the shape 425 closed for every catchable signal.
