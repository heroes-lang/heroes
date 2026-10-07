---
kind: defect
area: check
milestone: none
filed: 2026-10-06
commit: c057f4584241d2e0574619bea702dff41aa47735
github: none
---

- [ ] **424 — the fixed-array note at a function's result still says no binding, constant or parameter** | at a function-result position the `fixed_outside_a_group` note keeps 399's wording, *no binding, constant or parameter*, and does not name the result | `selfhost/ffi_errors.hero` · **class: improvement**

    **Origin:** lane b13-fixed405, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:12; the lane's reading, not re-run by the coordinator.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a message less exact than it could be at one position.

    Repaired at `c057f458`, 2026-10-07 (lane b13-fixed405): the note names a function's result among the places that hold no fixed array; case `full/fixedbugs-424-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; merged into round b13 by the coordinator, the card filled by the coordinator since the lane was told not to edit it.
