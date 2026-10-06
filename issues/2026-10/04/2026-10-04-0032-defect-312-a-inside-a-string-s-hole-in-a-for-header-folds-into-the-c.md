---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: 7101a5e65d55bf37c81448c1979637f6c978d89b
github: none
---

- [ ] **312 — a `;` inside a string's hole in a `for (` header folds into the C-header message, which then names a header the line does not hold** | `for (f"{a; b}")`: the `;` inside the `f"..."` hole is folded into defect 194's one C-header message, which names a three-clause header the line does not hold (lane b9-recovery's compiler, 2026-10-04) | `selfhost/scan.hero` (the `;` inside a hole), `selfhost/parse/loop_habit.hero` · defect 194's repair · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`), beside 194; the lane read it `improvement`.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a message naming a construct the line does not hold; the coordinator reads it less exact than true, so `adjacent`, against the lane's `improvement`.

    Repaired at `7101a5e6`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
