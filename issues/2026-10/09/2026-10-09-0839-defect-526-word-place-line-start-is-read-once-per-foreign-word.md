---
kind: defect
area: compiler
milestone: none
filed: 2026-10-09
commit: 1fb7a3c06eaf53793c4b016383b6533d54ce7736
github: none
---

- [ ] **526 — `word_place.line_start` is read once per foreign word** | the cost left on the `fn ` shape after defect 494's repair (lane b15-parse) | `selfhost/word_place.hero` · defect 494 · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-parse's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a cost per word.

    Repaired at `1fb7a3c0`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The outline keeps the line the last word asked about (`text_outline.line_start`, `start_of`), so the words of one line walk to its start once, and a word in the middle of its line still reads nothing else (defect 146). `check` of `fn ` over 4,000, 8,000, 16,000 and 32,000 characters reads 0.25, 0.44, 0.83 and 1.60 billion instructions where it read 1.04, 3.63, 13.62 and 52.79; output byte-identical.
