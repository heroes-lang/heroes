---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **436 — a program stopped by Ctrl-Z leaves `heroes` waiting for ever** | a child of `heroes run` stopped by Ctrl-Z (SIGTSTP) leaves `heroes` waiting, since `hero_run_go` waits for exits only; already so before defect 425's repair (the lane's reading) | `runtime/parts/run.c`, `hero_run_go`'s wait · **class: adjacent**

    **Origin:** filed by the coordinator at 04:40 on 2026-10-07, from lane b13-tmpl407's report (*found beside*); the lane's reading, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): the command stops answering where the terminal's job control should hand it back.
