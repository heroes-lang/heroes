---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **483 — nothing refuses a write to a cursor's tokens that bypasses `stream_tables.relaid`** | defect 258's kept walk rests on every token rewrite going through `relaid`, which asserts the new kind is invisible; a `layout` rule refusing other writes to `c.tokens[...]` would hold the premise (lane b14-parse) | `tests/harness/suite_layout.hero` · defect 258 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-parse's final report (*decisions* 1); the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a premise held by one assertion, no rule.
