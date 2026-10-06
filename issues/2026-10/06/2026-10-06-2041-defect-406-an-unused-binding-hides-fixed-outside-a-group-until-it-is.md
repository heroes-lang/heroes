---
kind: defect
area: check
milestone: none
filed: 2026-10-06
commit: self
github: none
---

- [x] **406 — an unused binding hides `fixed_outside_a_group` until it is fixed** | a program with an unused binding and fixed arrays outside a group was told 2 messages, then 8 once the binding was removed (the lane's `shapes.hero`) | `selfhost/check/`, the order the checker stops in · **class: adjacent**

    **Origin:** lane b13-front, 2026-10-06 (its report, *found beside, not filed*), the lane's measurement on its branch from `7001dfb3`, not re-run by the coordinator; filed by the coordinator at 20:41.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a mistake told only once another is fixed, class (b) of defect 212's reading.

    2026-10-06, reproduced by lane b13-fixed405 on its branch from `fd81c7ff`: one unused binding, 2 messages, both `unused_binding`; the binding removed, 9. `heroes check` runs its stages in order, parse, modules, names, types, each only when the one before said nothing (`selfhost/cli/check.hero`, lines 5 to 10 and 54 to 60); `unused_binding` is told by the names stage and `fixed_outside_a_group` by the types stage. Under `--permissive` all eight are told in one run.

## The ruling

Not a defect to repair: the behaviour is the one design.md §4.17 rules, *a later stage's mistakes wait for the earlier stage's by design, since `heroes check` runs a stage only when the one before said nothing* (panel 187, ratified 2026-10-03; design.md line 2200 at the round's `3bc3b2a5`). Moving the fixed-array rule into the names stage would hide `fixed_array_length` and the type errors behind it instead, the same shape in the other direction. What §4.17 counts against this, a mistake of the PARSE stage told only once another is fixed, is read at a recovery round's gate over its frozen corpus; this item is not of that stage. Closed 2026-10-06 by the coordinator, on lane b13-fixed405's measurement.
