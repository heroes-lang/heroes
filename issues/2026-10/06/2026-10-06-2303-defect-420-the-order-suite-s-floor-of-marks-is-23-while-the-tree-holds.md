---
kind: defect
area: harness
milestone: none
filed: 2026-10-06
commit: 40c055a33b268d375d33c382a403467fde54cab4
github: none
---

- [ ] **420 — the order suite's floor of marks is 23 while the tree holds 28** | `# ORDER:` marks counted 28 against the `order` suite's floor of 23; one more mark asks the floor raised; lane b13-run400 rewrote its draft to avoid adding one | `tests/harness/suite_order.hero` · **class: improvement**

    **Origin:** lane b13-run400, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:03; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a floor behind its count, no program judged wrong.

    Repaired at `40c055a3`, 2026-10-07 (panel 196's R7, before batch 14's base), measured in lane b14-harness-a and gated by `order` alone; the net is owed at the batch's close. The floor was raised from 23 to 30 there, when R7's two marks took the tree to 30 and `floors.check` asked for it, and at `dad2da47` `order` reads 3 passed, 0 failed over 30 marks (`grep` over `selfhost/` counts 30). The floor's rule was right not to fire at 28: it reports a floor outgrown past a quarter, `found * 100 > floor * 125`, and 2,800 is under 2,875, so 28 against 23 is inside the slack by design and the 29th mark, 2,900, is the one that asks the floor raised, which is what the lane saw.
