---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: 0e3bdd7758748f4622ad4739f8e52705fe4cead6
github: none
---

- [ ] **483 — nothing refuses a write to a cursor's tokens that bypasses `stream_tables.relaid`** | defect 258's kept walk rests on every token rewrite going through `relaid`, which asserts the new kind is invisible; a `layout` rule refusing other writes to `c.tokens[...]` would hold the premise (lane b14-parse) | `tests/harness/suite_layout.hero` · defect 258 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-parse's final report (*decisions* 1); the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a premise held by one assertion, no rule.

    Repaired at `0e3bdd77`, 2026-10-08 (lane b15-harness), gated by its cases and the net's own tests; the net is owed at the batch's close. `layout/streams` (`tests/harness/stream_writes.hero`) refuses any store into, or lend of, a place running through a `tokens` field other than `stream_tables.relaid`'s, whatever the holder is named, the lexer's own state exempt by its declared type: a write planted in place of `parse/orphans.hero:54`'s `relaid` reads `layout` 5 passed, 0 failed on the base's suite and red here naming the line; the tree holds no such write.
