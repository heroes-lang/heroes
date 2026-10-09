---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: cb8c5704b35485dbcdd8b3b8e21e5ab91c9ec21d
github: none
---

- [ ] **482 — `closers.close_to_its_kind` walks the whole stack for every closer of the wrong kind** | `(` N times then `]` N times: `lex` costs 0.34, 1.21 and 4.59 billion at 1,000, 2,000 and 4,000; the same line gets N `expected_group_close`, one per `]` (lane b14-parse) | `selfhost/closers.hero` · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-parse's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the line's square, and a message per closer.

    Repaired at `cb8c5704`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A closer no opener of its kind answers is remembered until an opener is pushed (`lex_asked.hero`), the parser's drop of a failed line counts its stack, and a run of closers told alike is one report noting the rest (`closer_runs.hero`): `(` then `]` 4,000 each, `check` 14.36 to 1.85 billion instructions retired and 4,000 messages to 1, `lex` 4.22 to 1.15 (the growth left is defect 473's pop by a copy).
