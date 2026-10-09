---
kind: defect
area: parse
milestone: none
filed: 2026-10-09
commit: c7b3473f48d263691ff7c57d22c199e7df109555
github: none
---

- [x] **525 — lists closed by `)` keep one message each under defect 482's run** | defect 482's repair tells a run of wrong closers once, and lists closed by `)` keep one message each, each naming its own `[` column (lane b15-parse) | `selfhost/parse/` · defect 482 · **class: adjacent**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-parse's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.

    Repaired at `c7b3473f`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Reports at adjacent closers alike but for the numbers in them are one run, told at its first with a note that the closers after it are each for the next opener out (`closer_runs.told_once`), so `[[[[1))))` is told once as `((((1]]]]` is; a mixed run stays apart. One new `full` case pinning the note, red on the base; `fixedbugs-482-a-run-of-closers-of-the-wrong-kind-is-told-once` moved from three reports to one.

## The repair

Repaired at `c7b3473f`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Reports at adjacent closers alike but for the numbers in them are one run, told at its first with a note that the closers after it are each for the next opener out (`closer_runs.told_once`), so `[[[[1))))` is told once as `((((1]]]]` is; a mixed run stays apart. One new `full` case pinning the note, red on the base; `fixedbugs-482-a-run-of-closers-of-the-wrong-kind-is-told-once` moved from three reports to one.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
