---
kind: defect
area: compiler
milestone: none
filed: 2026-10-05
commit: none
github: none
---

- [ ] **339 — `1_u`, `0644_u` and `00_x7` are told twice, `misplaced_separator` and then `expected_end_of_line`** | `x = 1_u`, `x = 0644_u`, `x = 00_x7`: each `check`s to two messages for the one mistake, the separator's and then the line's end at the letter (lane b11-misc's compiler, 2026-10-05, the lane's report) | `selfhost/number.hero` (the separator scan) · defect 329, which made a leading zero's joined letters one message · **class: adjacent**

    **Origin:** lane b11-misc, 2026-10-05, beside defect 329's repair (its final report, *Found beside*): the cause is in the separator scan, not in 329's.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake, not one of `blocking`'s list.
