---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: 758f4ab288bd08de7df907c8f46db9f7bfe9a585
github: none
---

- [x] **469 — clang's front end costs the options chain super-quadratically** | the options shape grows ×7.59 per doubling (exponent 2.92) while its C grows linearly; o5000 costs 27 times v5000; debug information is 1.0% of it (panel 197's critic, second pass, `o5000-E-unit.c`) | the emitted C of a chain through options · panel 197's R7 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from panel 197's R7.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost past the floor, no program wrong.

    Repaired at `758f4ab2`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. What grew is clang's `RecordType::hasConstFields`, 98.5% of a sample of the front end at 2,000 levels, which clang asks of every assignment of a struct at the square of the structs it holds by value, and every level of the chain had a descriptor whose copy was such an assignment; the copy is now `__builtin_memmove`, and the front end at 500, 1,000 and 2,000 levels reads 3.65, 19.24 and 134.92 billion instructions to 1.57, 2.94 and 5.70 (10,000 levels 27.85, `-fintegrated-cc1`, this Mac). The same cause in a function's own assignments, a chain whose every level's value is built in one function, is not reached: 3.39 and 18.75 billion at 250 and 500 levels after this repair, 1.47 and 3.40 with every assignment written as a byte copy by hand, a decision put to the coordinator.

## The repair

Repaired at `758f4ab2`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. What grew is clang's `RecordType::hasConstFields`, 98.5% of a sample of the front end at 2,000 levels, which clang asks of every assignment of a struct at the square of the structs it holds by value, and every level of the chain had a descriptor whose copy was such an assignment; the copy is now `__builtin_memmove`, and the front end at 500, 1,000 and 2,000 levels reads 3.65, 19.24 and 134.92 billion instructions to 1.57, 2.94 and 5.70 (10,000 levels 27.85, `-fintegrated-cc1`, this Mac). The same cause in a function's own assignments, a chain whose every level's value is built in one function, is not reached: 3.39 and 18.75 billion at 250 and 500 levels after this repair, 1.47 and 3.40 with every assignment written as a byte copy by hand, a decision put to the coordinator.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
