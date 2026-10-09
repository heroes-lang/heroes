---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: 62f312ec52177e062092e5a0e42bb812c49fd1e6
github: none
---

- [ ] **473 — bracket nesting copies an array of spans once per bracket** | `check` of `x = [[...1...]]`: `token.Span` copies 1,004,007 at depth 1,000 and 4,007,055 at 2,000; a type written deep in a signature does the same; not located to a line (lane b14-check) | the lexer or the parser · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-check's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the nesting's square.

    Repaired at `62f312ec`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Located by the emitted C's `token.Span` copier counted: the lexer's pop by `slice` at every closer (`state.emit`) and the pairing walk's (`pairing.table`); the lexer's stack is now its slots and a count (`openers.hero`), the pairing's a count over its array: spans copied at depth 1,000, 2,000, 4,000, 1,004,007, 4,007,055, 16,013,151 to 4,858, 8,906, 17,002, `check` 1.93 to 0.45 billion instructions retired at 4,000, every output byte-identical.
