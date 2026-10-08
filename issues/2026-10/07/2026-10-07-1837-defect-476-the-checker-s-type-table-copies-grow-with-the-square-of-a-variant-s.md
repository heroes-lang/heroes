---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: 09fc600b525df5df94aa78f217c02c970c25009f
github: none
---

- [ ] **476 — the checker's type table copies grow with the square of a variant's case count** | match-arms 400 to 800: 96,589 to 352,389 copies (lane b14-ir) | `selfhost/check/table.hero` · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-ir's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the cases' square.

    Repaired at `09fc600b`, 2026-10-08 (lane b15-check), gated by its cases; the net is owed at the batch's close. Located, and repaired by another lane: each case an arm names is a type of its own, and `check/table.intern` pushes through the table's own field, defect 409's shape, repaired in lane b14-p409 at `2a26fde2`, carried into this round and not in this lane's base, so this item closes only with that commit merged. `check` of 400 and 800 cases matched by as many arms copies 96,589 and 352,389 table types on the base, the numbers above, and 1,568 and 1,871 on the base with that commit's tree. The instructions still grow with the cases' square, 5.04 billion at 1,600 and 18.1 at 3,200 on the base, in `check/reach.hero`'s search of a variant's cases by name at each arm, a cause of its own reported to the coordinator. `09fc600b` pins the shape at 400.
