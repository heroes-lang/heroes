---
kind: defect
area: harness
milestone: none
filed: 2026-10-06
commit: 9d884765c8e13bfa0eb3b65d9e628e7455f6c6d3
github: none
---

- [ ] **421 — a qualified generic function value has no surface row** | `helper.ident`, a generic function of another module used as a value, checks and runs after defect 402's repair (prints `q1` and `31`, the lane's run by hand) and no golden or surface row pins it, the shape needing two modules | `tests/harness/suite_surface.hero` · **class: improvement**

    **Origin:** lane b13-gen402, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:03; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): coverage, the repaired shape pinned by nothing.

    Repaired at `9d884765`, 2026-10-07 (lane b14-harness-b), gated by its cases and the net's own tests; the net is owed at the batch's close. `tests/golden/surface-fixtures/value421/` and four rows of `suite_surface`'s verb table pin `helper.ident` and `helper.first` named as values at ten shapes, at the default level and under `--sanitize`, through `use helper as h`, and refused `cannot_infer` where nothing asks a type: the four rows red on the compiler before defect 402's repair, each on `type_mismatch`, and surface 382 passed, 0 failed, on this one.
