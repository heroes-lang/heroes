---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: f65efa513a3a6ee025d8b158bbb327e800a0fbf9
github: none
---

- [ ] **481 — `parse/quote_held.held` scans each lexer report's whole line** | a string of 1,000, 2,000 and 4,000 `\q` escapes costs `check` 0.33, 0.85 and 2.59 billion instructions, 85% of a sample in `line_end`; it keeps the 10,000-opener line quadratic after defect 410 (lane b14-parse); repair: test the report's code before scanning | `selfhost/parse/quote_held.hero` · defect 410 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-parse's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the line's square.

    Repaired at `f65efa51`, 2026-10-08 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A report's code is asked before its line is read, and the lost lines are sorted once and searched (`lost_quote.lost_lines`, `holds`), the shape beside, many lost-quote lines, with it: `check` of 4,000 unknown escapes 2.66 to 0.77 billion instructions retired, 8,000 lost-quote lines 10.47 to 3.73, defect 410's 10,000 openers 7.30 to 1.37, every output byte-identical.
