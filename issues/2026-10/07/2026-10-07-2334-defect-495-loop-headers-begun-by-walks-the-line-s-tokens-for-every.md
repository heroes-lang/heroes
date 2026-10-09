---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: 241b773a1904d7bb807d6981c0b6eacf41364c2c
github: none
---

- [ ] **495 — `loop_headers.begun_by` walks the line's tokens for every `;`** | `x; ` 1,333 times on one line: 0.21, 0.61, 2.16 and 8.30 billion instructions at 500, 1,000, 2,000 and 4,000 characters; `opened_by` does the same inside a `for (` header, 4.31 billion at 4,000; a cache over the stream must survive the lexer replacing and truncating `l.tokens` (lane b14-text) | `selfhost/loop_headers.hero` · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-text's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the line's square.

    Repaired at `241b773a`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. What a line's `;` ask is read once and read on in the lexer's memory (`lex_asked.hero`): the line's start, its first word and `in`, a header's `in` and count, and a stack with no `(` after the word; the three rewrites of an operator's line forget what was read, the premise a test asks; and the parser's spill asks a long head line once (`parse/swallowed.spilled`): `x; ` at 4,000 bytes 6.56 to 0.25 billion instructions retired, a `for (` header of them 4.08 to 0.17, every output byte-identical.
