---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: 505f3dc0187fb2276617a378a22cdf37e1a62a92
github: none
---

- [ ] **584 — a program's own call of a deprecated C function prints clang's raw warning at exit 0** | after defect 571 quieted a header's `deprecated` attribute on the compiler's own lines, a program's own call of a function its header marks deprecated still prints clang's raw `-Wdeprecated-declarations` text and builds at exit 0; design.md says the language has no warning level (`:3744`, *a diagnostic is exit 1 or nothing*), so it is either a refusal or silence, a diagnostic-class question for a sitting | the emitted call site and the compile words, `selfhost/emit/` and `selfhost/cli/`; for a sitting · **class: blocking**

    **Origin:** found by lane b18-guard beside panel 205's landing (its final reply and notes, `.claude/worktrees/scratch-b15/b18-guard/notes.txt`, ignored by git), filed by the coordinator at 13:56 on 2026-10-10.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a clang warning on a correct program.

    **A shape of it, written here by panel 208's synthesis**, 2026-10-10: `main`'s prologue temporary of a record its header marks deprecated (`struct old_pair t2;`), the compiler-engineer's s3 (`docs/panel/208-reports/compiler-engineer.md`, its table of shapes), printed clang's warning at the emitted unit, a line no author wrote, beside the program's own signature: defect 571's prologue toggle did not reach that temporary. The same cause, the warning spoken again over the program's definitions, so it closes with this item.

    Repaired at `505f3dc0`, 2026-10-10 (lane b19-dep), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 208's R1: `-Wdeprecated-declarations`, `-Wdeprecated-pragma` and `-Wattribute-warning` ignored over the whole unit after the groups' close (`emit/macro_guard.hero`, `emit/deprecation.hero`), the program's own lines as the compiler's; defect 571's spoken line over the definitions and its prologue and accessor toggles removed; panel 205's header region and raised checks unchanged. Measured: seven `run` cases, `tests/golden/run/fixedbugs-584-*` (a function called; bound and not called; a record in a signature and as `main`'s temporary, this item's s3; a record as a local; an enumerator read; GCC's `warning` attribute called; `sprintf` under a first-group header defining `_FORTIFY_SOURCE 0`), each exit 0 and silent with its value, five of them printing clang's text with the base compiler; defect 571's four run cases still green.
