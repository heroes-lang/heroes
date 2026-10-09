---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 47bc65f3dc901c53795c8d3f82d8026a05e640c4
github: none
---

- [x] **513 — constructing each case of an N-case variant costs the square of N** | 0.62, 1.59 and 4.90 billion instructions at 400, 800 and 1,600 cases on defect 409's tree; read as `case_construct`'s name scan, unsampled (lane b15-check) | `selfhost/check/` (`case_construct`) · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-check's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the square of the cases.

    Repaired at `47bc65f3`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The checker's `case_construct` asks the table defect 512 keeps, and the lowering keeps its own (`ir/layout.case_index`), where each walked the variant's names at every construction, the lowering too. At 400, 800 and 1,600 cases `check` reads 0.34, 0.61 and 1.14 billion instructions where it read 0.56, 1.52 and 4.78, and `build --dump-ir` 0.69, 1.21 and 2.22 where it read 1.24, 3.39 and 10.92; the IR and the C byte-identical.

## The repair

Repaired at `47bc65f3`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The checker's `case_construct` asks the table defect 512 keeps, and the lowering keeps its own (`ir/layout.case_index`), where each walked the variant's names at every construction, the lowering too. At 400, 800 and 1,600 cases `check` reads 0.34, 0.61 and 1.14 billion instructions where it read 0.56, 1.52 and 4.78, and `build --dump-ir` 0.69, 1.21 and 2.22 where it read 1.24, 3.39 and 10.92; the IR and the C byte-identical.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
