---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **587 — a macro its header marks with `#pragma clang deprecated` prints clang's raw warning at the compiler's own lines** | a program binding as a `constant` a macro the header marks `#pragma clang deprecated(OLD_MACRO)` builds at exit 0 and prints clang's `-Wdeprecated-pragma` text at the emitted unit's static asserts and accessor (`macro.c:49:29`, `:50:37`, `:73:12`), a group defect 571's guard does not name; this Mac and Linux arm64 alike (the seat's `repro/shapes/macro.hero`) | `selfhost/emit/deprecation.hero` (defect 571's lines, in batch 18's round on 2026-10-10), the probe and accessor lines; panel 208, landing with defect 584 · **class: blocking**

    **Origin:** found by panel 208's ffi-pragmatist beside defect 584 (`docs/panel/208-reports/ffi-pragmatist.md` § 3b, its runs under `.claude/worktrees/scratch-b15/208-ffi-pragmatist/`, ignored by git), filed by the coordinator at 15:58 on 2026-10-10, the seat's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a clang warning on a correct program.
