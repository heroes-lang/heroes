---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: none
github: none
---

- [ ] **344 — an unclosed list or map re-walks to the end of its declaration for each opener, so N unclosed lines cost the square of N** | `list_line.ended_at_a_binding` and `unclosed.never_closed` walk to the declaration's end once per `[` or `{`: unclosed `[` lines took 46.8 billion instructions at 2,000 and 738.7 billion at 8,000, unclosed `{` lines 24.0 and 375.0 billion (lane b11-parse, 2026-10-05, the lane's report) | `selfhost/parse/list_line.hero` and `selfhost/parse/unclosed.hero`, batch 12's files · defects 266 and 342 · **class: adjacent**

    **Origin:** lane b11-parse, 2026-10-05, beside defect 266's repair (its final report, *Found beside*).

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a broken program whose report costs the square of its openers; the messages are right.
