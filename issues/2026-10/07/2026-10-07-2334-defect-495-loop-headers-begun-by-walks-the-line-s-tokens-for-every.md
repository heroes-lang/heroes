---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **495 — `loop_headers.begun_by` walks the line's tokens for every `;`** | `x; ` 1,333 times on one line: 0.21, 0.61, 2.16 and 8.30 billion instructions at 500, 1,000, 2,000 and 4,000 characters; `opened_by` does the same inside a `for (` header, 4.31 billion at 4,000; a cache over the stream must survive the lexer replacing and truncating `l.tokens` (lane b14-text) | `selfhost/loop_headers.hero` · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-text's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the line's square.
