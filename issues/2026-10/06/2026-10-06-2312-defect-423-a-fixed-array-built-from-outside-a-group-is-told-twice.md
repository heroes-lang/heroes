---
kind: defect
area: check
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **423 — a fixed array built from `[]` outside a group is told twice** | `xs: i64[2] = []` in a function, then `print(xs[0])`: `check --brief` tells `fixed_outside_a_group` and `type_mismatch` for the one binding (the coordinator's re-run, 23:12) | `selfhost/check/ffi_sweep.hero`, the binding's initialiser checked against a type already refused · **class: adjacent**

    **Origin:** lane b13-fixed405, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:12; reproduced by the coordinator.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): one mistake told as two, the type already refused judged again.
