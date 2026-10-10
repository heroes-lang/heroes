---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **588 — a binding of a function its header marks `unavailable` is an internal error at exit 2** | a program binding a function the header marks `__attribute__((unavailable))`, never calling it, exits 2 with *internal error: compiling the generated C failed* and clang's text at the unit's lines, on this Mac and Linux arm64 (the seat's `repro/shapes/unav_bound.hero`); the program's own `extern` is the cause, so it belongs at exit 1 on the binding's line (`.claude/rules/c-boundary.md`, the six members; defects 152, 156, 158 and 164 were this shape and are closed) | the round's recovery of a clang failure into an `ffi_*` class, `selfhost/cli/` and `selfhost/emit/`; panel 208, landing with defect 584 · **class: blocking**

    **Origin:** found by panel 208's ffi-pragmatist beside defect 584 (`docs/panel/208-reports/ffi-pragmatist.md` § 3b, its runs under `.claude/worktrees/scratch-b15/208-ffi-pragmatist/`, ignored by git), filed by the coordinator at 15:58 on 2026-10-10, the seat's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told.
