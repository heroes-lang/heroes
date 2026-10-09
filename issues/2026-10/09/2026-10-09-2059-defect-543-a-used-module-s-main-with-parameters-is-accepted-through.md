---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **543 — a used module's `main` with parameters is accepted through `use` and refused checked alone** | `main(n: i64) -> i64` in a module another file uses passes `check` of the program, and `check` of that file alone refuses it `main_parameters` and `main_returns`: one file, two verdicts (`selfhost/check/decls.hero:89` applies the rules to the file compiled only; panel 201's compiler-engineer) | `selfhost/check/decls.hero` · panel 201 R2 · **class: adjacent**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 201 (`docs/panel/201-the-spec-says-a-generic-function-s-parameter-types-no-value-and-a-cycle-that-cannot-end-is-refused.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): one file told two different things.
