---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **522 — a struct assignment in a function body past 32 deep still costs clang's front end the cube of the depth** | defect 469's other half: a chain whose every level's value is built in one function reads 3.39 and 18.75 billion instructions at 250 and 500 levels after 469's repair, and 1.47, 3.40 and 9.26 billion at 250, 500 and 1,000 with every struct assignment written as a byte copy by hand; the rewrite must keep a type check and add no temporary per site (lane b15-parse) | `selfhost/emit/` · defect 469 · panel 106 · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-parse's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the cube of the depth.
