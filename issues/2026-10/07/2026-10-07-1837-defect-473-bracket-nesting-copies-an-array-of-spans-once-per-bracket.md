---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: 62f312ec52177e062092e5a0e42bb812c49fd1e6
github: none
---

- [x] **473 — bracket nesting copies an array of spans once per bracket** | `check` of `x = [[...1...]]`: `token.Span` copies 1,004,007 at depth 1,000 and 4,007,055 at 2,000; a type written deep in a signature does the same; not located to a line (lane b14-check) | the lexer or the parser · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-check's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the nesting's square.

    Repaired at `62f312ec`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Located by the emitted C's `token.Span` copier counted: the lexer's pop by `slice` at every closer (`state.emit`) and the pairing walk's (`pairing.table`); the lexer's stack is now its slots and a count (`openers.hero`), the pairing's a count over its array: spans copied at depth 1,000, 2,000, 4,000, 1,004,007, 4,007,055, 16,013,151 to 4,858, 8,906, 17,002, `check` 1.93 to 0.45 billion instructions retired at 4,000, every output byte-identical.

## The repair

Repaired at `62f312ec`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Located by the emitted C's `token.Span` copier counted: the lexer's pop by `slice` at every closer (`state.emit`) and the pairing walk's (`pairing.table`); the lexer's stack is now its slots and a count (`openers.hero`), the pairing's a count over its array: spans copied at depth 1,000, 2,000, 4,000, 1,004,007, 4,007,055, 16,013,151 to 4,858, 8,906, 17,002, `check` 1.93 to 0.45 billion instructions retired at 4,000, every output byte-identical.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
