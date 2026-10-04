---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: none
github: none
---

- [ ] **342 — an arm of N `|` patterns costs the square of N in `operator_arm.hero`, though the program is correct** | a `match` arm of 1,000 patterns joined by `|` checks in 0.78 billion instructions retired and one of 4,000 in 9.07 billion, four times the input for 11.6 times the cost (lane b11-parse, 2026-10-05, `/usr/bin/time -l` on this Mac, the lane's report) | `selfhost/parse/operator_arm.hero:70` · defects 266 and 267, the same shape of cost · **class: adjacent**

    **Origin:** lane b11-parse, 2026-10-05, beside defects 266 and 267 (its final report, *Found beside*).

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a correct program whose check costs the square of its size; no message or value is wrong.
