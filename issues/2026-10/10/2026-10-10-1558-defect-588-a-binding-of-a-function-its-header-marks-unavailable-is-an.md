---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: 16cad3ffd9885fb1f67e7a21bafd4c69e17650af
github: none
---

- [ ] **588 — a binding of a function its header marks `unavailable` is an internal error at exit 2** | a program binding a function the header marks `__attribute__((unavailable))`, never calling it, exits 2 with *internal error: compiling the generated C failed* and clang's text at the unit's lines, on this Mac and Linux arm64 (the seat's `repro/shapes/unav_bound.hero`); the program's own `extern` is the cause, so it belongs at exit 1 on the binding's line (`.claude/rules/c-boundary.md`, the six members; defects 152, 156, 158 and 164 were this shape and are closed) | the round's recovery of a clang failure into an `ffi_*` class, `selfhost/cli/` and `selfhost/emit/`; panel 208, landing with defect 584 · **class: blocking**

    **Origin:** found by panel 208's ffi-pragmatist beside defect 584 (`docs/panel/208-reports/ffi-pragmatist.md` § 3b, its runs under `.claude/worktrees/scratch-b15/208-ffi-pragmatist/`, ignored by git), filed by the coordinator at 15:58 on 2026-10-10, the seat's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told.

    Repaired at `16cad3ff`, 2026-10-10 (lane b19-dep), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 208's R2: `emit/ffi_unavailable.hero` reads clang's `'<name>' is unavailable` wherever in the unit it lands, narrowed by `declaration()`, into `ffi_unavailable` on the binding, once, at exit 1, the header's own words in a note; the class's tenth member in `.claude/rules/c-boundary.md`. Measured: the seat's two shapes (`fixedbugs-588-*` under `tests/golden/unsupported/`, bound and not called, and called) exit 1, told once each; by hand beside them a bare `unavailable` with no words, an enumerator and a record by its tag, each exit 1 on its binding; unsupported whole 231 and 0, the compiler's own tests 1561 passed.
