---
kind: defect
area: check
milestone: none
filed: 2026-10-06
commit: 3e92cbfb382779b9b557e609e38d111634d7c62f
github: none
---

- [ ] **423 — a fixed array built from `[]` outside a group is told twice** | `xs: i64[2] = []` in a function, then `print(xs[0])`: `check --brief` tells `fixed_outside_a_group` and `type_mismatch` for the one binding (the coordinator's re-run, 23:12) | `selfhost/check/ffi_sweep.hero`, the binding's initialiser checked against a type already refused · **class: adjacent**

    **Origin:** lane b13-fixed405, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:12; reproduced by the coordinator.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): one mistake told as two, the type already refused judged again.

    Repaired at `3e92cbfb`, 2026-10-07 (lane b13-fixed405): a value judged against a fixed array the sweep refused keeps no second message where it fits the `[T]` the refusal offers (`check/fixed_judged.hero`); case `check/fixedbugs-423-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; merged into round b13 by the coordinator, the card filled by the coordinator since the lane was told not to edit it.
