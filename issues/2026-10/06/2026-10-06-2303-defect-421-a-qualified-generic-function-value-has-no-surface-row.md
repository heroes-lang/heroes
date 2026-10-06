---
kind: defect
area: harness
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **421 — a qualified generic function value has no surface row** | `helper.ident`, a generic function of another module used as a value, checks and runs after defect 402's repair (prints `q1` and `31`, the lane's run by hand) and no golden or surface row pins it, the shape needing two modules | `tests/harness/suite_surface.hero` · **class: improvement**

    **Origin:** lane b13-gen402, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:03; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): coverage, the repaired shape pinned by nothing.
