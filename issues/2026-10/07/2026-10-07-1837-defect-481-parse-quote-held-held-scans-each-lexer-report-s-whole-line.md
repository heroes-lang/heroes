---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **481 — `parse/quote_held.held` scans each lexer report's whole line** | a string of 1,000, 2,000 and 4,000 `\q` escapes costs `check` 0.33, 0.85 and 2.59 billion instructions, 85% of a sample in `line_end`; it keeps the 10,000-opener line quadratic after defect 410 (lane b14-parse); repair: test the report's code before scanning | `selfhost/parse/quote_held.hero` · defect 410 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-parse's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the line's square.
