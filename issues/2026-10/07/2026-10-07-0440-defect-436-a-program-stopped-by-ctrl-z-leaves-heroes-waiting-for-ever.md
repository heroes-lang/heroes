---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: e38dd76590525149f504cd1a0631ba661fe5a345
github: none
---

- [ ] **436 — a program stopped by Ctrl-Z leaves `heroes` waiting for ever** | a child of `heroes run` stopped by Ctrl-Z (SIGTSTP) leaves `heroes` waiting, since `hero_run_go` waits for exits only; already so before defect 425's repair (the lane's reading) | `runtime/parts/run.c`, `hero_run_go`'s wait · **class: adjacent**

    **Origin:** filed by the coordinator at 04:40 on 2026-10-07, from lane b13-tmpl407's report (*found beside*); the lane's reading, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): the command stops answering where the terminal's job control should hand it back.

    Repaired at `e38dd765`, 2026-10-07 (lane b13-tmpl407): a stopped child is never read as an exit: stop reports are consumed and the wait goes on; with a terminal, Ctrl-Z takes the terminal back and stops `heroes` itself with `raise(SIGTSTP)`, `fg` handing it back and continuing the child; SIGCONT sent after 425's SIGTERM; on Darwin the base killed the stopped program (exit 137), worse than filed (`runtime/parts/run.c`, the POSIX side); case `run/fixedbugs-436-a-child-that-stops-is-waited-on-and-not-killed`, pty probes on this Mac and Linux arm64 (the lane's); a C-boundary defect, so it closes after the batch's platform legs; the card filled by the coordinator.
