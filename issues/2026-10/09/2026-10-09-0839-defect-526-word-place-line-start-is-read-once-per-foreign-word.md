---
kind: defect
area: compiler
milestone: none
filed: 2026-10-09
commit: 1fb7a3c06eaf53793c4b016383b6533d54ce7736
github: none
---

- [x] **526 — `word_place.line_start` is read once per foreign word** | the cost left on the `fn ` shape after defect 494's repair (lane b15-parse) | `selfhost/word_place.hero` · defect 494 · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-parse's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a cost per word.

    Repaired at `1fb7a3c0`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The outline keeps the line the last word asked about (`text_outline.line_start`, `start_of`), so the words of one line walk to its start once, and a word in the middle of its line still reads nothing else (defect 146). `check` of `fn ` over 4,000, 8,000, 16,000 and 32,000 characters reads 0.25, 0.44, 0.83 and 1.60 billion instructions where it read 1.04, 3.63, 13.62 and 52.79; output byte-identical.

## The repair

Repaired at `1fb7a3c0`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The outline keeps the line the last word asked about (`text_outline.line_start`, `start_of`), so the words of one line walk to its start once, and a word in the middle of its line still reads nothing else (defect 146). `check` of `fn ` over 4,000, 8,000, 16,000 and 32,000 characters reads 0.25, 0.44, 0.83 and 1.60 billion instructions where it read 1.04, 3.63, 13.62 and 52.79; output byte-identical.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
