---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **432 — a use of a binding whose fixed array was refused is judged against the fixed type** | `x: i64[2][3]` outside a group, refused `fixed_outside_a_group`, then `len(x)`: told `bad_operand`, though `len` works on the `[[i64]]` the refusal offers | defect 423's withdrawal, `fixed_judged.hero` on lane b13-fixed405 (unmerged on 2026-10-07), and the uses of the binding · **class: adjacent**

    **Origin:** filed by the coordinator at 00:11 on 2026-10-07, from lane b13-fixed405's report of the evening before (*found beside*); the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): one mistake told again at each use.
