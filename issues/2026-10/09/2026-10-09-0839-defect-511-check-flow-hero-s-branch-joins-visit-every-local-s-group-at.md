---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 87e49bae9c4256fa05eecec94422490be9f6ddd7
github: none
---

- [x] **511 — `check/flow.hero`'s branch joins visit every local's group at each meet** | n handles ended, then n `if`s: 3.24, 11.3 and 41.2 billion instructions at 500, 1,000 and 2,000 after defect 499's repair, quadratic (cubic on the base, 93.9 and 742 billion at 500 and 1,000); shared or copy-on-write groups would answer it (lane b15-check) | `selfhost/check/flow.hero` · defect 499 · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-check's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the square of the handles.

    Repaired at `87e49bae`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The walk splits the flow where control splits, each event logs its local once per open split (`check/flow_log.hero`), and a meet starts from the split and meets only the locals the paths logged, a local no path changed being the base's on both. `check` of n handles ended then n `if`s reads 0.90, 1.72 and 3.36 billion instructions at 500, 1,000 and 2,000 where it read 3.26, 11.39 and 41.35; every output byte-identical, and the 224 tracked programs that consume, transfer or retain check unmoved. Ends inside both branches of n `if`s go from 2.56, 8.16 and 27.99 to 1.49, 3.73 and 10.73, what is left being a path's first change copying the flat group array, a cause apart.

## The repair

Repaired at `87e49bae`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The walk splits the flow where control splits, each event logs its local once per open split (`check/flow_log.hero`), and a meet starts from the split and meets only the locals the paths logged, a local no path changed being the base's on both. `check` of n handles ended then n `if`s reads 0.90, 1.72 and 3.36 billion instructions at 500, 1,000 and 2,000 where it read 3.26, 11.39 and 41.35; every output byte-identical, and the 224 tracked programs that consume, transfer or retain check unmoved. Ends inside both branches of n `if`s go from 2.56, 8.16 and 27.99 to 1.49, 3.73 and 10.73, what is left being a path's first change copying the flat group array, a cause apart.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
