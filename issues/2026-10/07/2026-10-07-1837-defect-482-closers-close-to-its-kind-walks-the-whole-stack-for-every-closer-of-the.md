---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **482 — `closers.close_to_its_kind` walks the whole stack for every closer of the wrong kind** | `(` N times then `]` N times: `lex` costs 0.34, 1.21 and 4.59 billion at 1,000, 2,000 and 4,000; the same line gets N `expected_group_close`, one per `]` (lane b14-parse) | `selfhost/closers.hero` · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-parse's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the line's square, and a message per closer.
