---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: 241b773a1904d7bb807d6981c0b6eacf41364c2c
github: none
---

- [x] **495 — `loop_headers.begun_by` walks the line's tokens for every `;`** | `x; ` 1,333 times on one line: 0.21, 0.61, 2.16 and 8.30 billion instructions at 500, 1,000, 2,000 and 4,000 characters; `opened_by` does the same inside a `for (` header, 4.31 billion at 4,000; a cache over the stream must survive the lexer replacing and truncating `l.tokens` (lane b14-text) | `selfhost/loop_headers.hero` · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-text's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the line's square.

    Repaired at `241b773a`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. What a line's `;` ask is read once and read on in the lexer's memory (`lex_asked.hero`): the line's start, its first word and `in`, a header's `in` and count, and a stack with no `(` after the word; the three rewrites of an operator's line forget what was read, the premise a test asks; and the parser's spill asks a long head line once (`parse/swallowed.spilled`): `x; ` at 4,000 bytes 6.56 to 0.25 billion instructions retired, a `for (` header of them 4.08 to 0.17, every output byte-identical.

## The repair

Repaired at `241b773a`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. What a line's `;` ask is read once and read on in the lexer's memory (`lex_asked.hero`): the line's start, its first word and `in`, a header's `in` and count, and a stack with no `(` after the word; the three rewrites of an operator's line forget what was read, the premise a test asks; and the parser's spill asks a long head line once (`parse/swallowed.spilled`): `x; ` at 4,000 bytes 6.56 to 0.25 billion instructions retired, a `for (` header of them 4.08 to 0.17, every output byte-identical.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
