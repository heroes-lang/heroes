---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 3fc7ed936198ef55882d94574dece68d1faea5d8
github: none
---

- [ ] **512 — `check/reach.hero`'s `case_pattern` searches a variant's cases by name at each arm** | 0.54, 1.50 and 4.82 billion instructions at 400, 800 and 1,600 cases with defect 409's repair, still quadratic (lane b15-check) | `selfhost/check/reach.hero` (`case_pattern`) · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-check's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the square of the cases.

    Repaired at `3fc7ed93`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Each variant's case names are read into a table the first time a case of it is named, kept on the checker by declaration, and the cases a `match` covered are a map, where every arm walked the names and every earlier arm and `exhaustive` walked the covered list again. `check` of a `match` naming every case reads 0.23, 0.36 and 0.65 billion instructions at 400, 800 and 1,600 cases where it read 0.45, 1.31 and 4.47; output byte-identical.
