---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: 9e9af1ad54ec9a094fc3008fc086853a553e42d1
github: none
---

- [ ] **458 — `end_lease(@c, 1)` on a lease cell is told *this argument is not such a cell*, which is false of `@c`** | `end_lease(@c, 1)` where `c` is a lease cell gets `builtin_shape` and `end_lease_of_no_lease`, the second saying the argument is not such a cell, false of `@c`; the mistake is the extra argument (lane b14-resolve, read on the base) | `selfhost/check/leasing.hero` · **class: blocking**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-resolve's final report; the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a false message beside the true one.

    Repaired at `9e9af1ad`, 2026-10-07 (lane b14-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Reproduced on the base: `end_lease(@c, 1)`, `end_lease(1, @c)`, `end_lease(@c, @d)` and `end_lease()` were each told `builtin_shape` and the false `end_lease_of_no_lease`, and `end_lease(c)` and `c.end_lease()` on a lease cell the false one alone. `end_lease_at` now leaves the count to `builtin_shape`, keeps `end_lease_of_no_lease` for an argument that is no lease cell, tells a lease cell without `@` `marker_mismatch` with its certain fix and the dotted form `ufcs_on_mutable`: six `fixedbugs-458-*` check cases, four red on the base and two pins, and `end-lease-takes-only-a-lease` moved by design.
