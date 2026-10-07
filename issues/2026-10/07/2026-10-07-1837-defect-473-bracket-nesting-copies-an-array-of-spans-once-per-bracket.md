---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **473 — bracket nesting copies an array of spans once per bracket** | `check` of `x = [[...1...]]`: `token.Span` copies 1,004,007 at depth 1,000 and 4,007,055 at 2,000; a type written deep in a signature does the same; not located to a line (lane b14-check) | the lexer or the parser · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-check's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the nesting's square.
