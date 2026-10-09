---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 3fc7ed936198ef55882d94574dece68d1faea5d8
github: none
---

- [x] **512 — `check/reach.hero`'s `case_pattern` searches a variant's cases by name at each arm** | 0.54, 1.50 and 4.82 billion instructions at 400, 800 and 1,600 cases with defect 409's repair, still quadratic (lane b15-check) | `selfhost/check/reach.hero` (`case_pattern`) · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-check's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the square of the cases.

    Repaired at `3fc7ed93`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Each variant's case names are read into a table the first time a case of it is named, kept on the checker by declaration, and the cases a `match` covered are a map, where every arm walked the names and every earlier arm and `exhaustive` walked the covered list again. `check` of a `match` naming every case reads 0.23, 0.36 and 0.65 billion instructions at 400, 800 and 1,600 cases where it read 0.45, 1.31 and 4.47; output byte-identical.

## The repair

Repaired at `3fc7ed93`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Each variant's case names are read into a table the first time a case of it is named, kept on the checker by declaration, and the cases a `match` covered are a map, where every arm walked the names and every earlier arm and `exhaustive` walked the covered list again. `check` of a `match` naming every case reads 0.23, 0.36 and 0.65 billion instructions at 400, 800 and 1,600 cases where it read 0.45, 1.31 and 4.47; output byte-identical.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
