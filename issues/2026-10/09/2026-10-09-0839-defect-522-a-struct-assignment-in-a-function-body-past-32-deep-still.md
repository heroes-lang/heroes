---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: 29d0f3d8f8e7e56af14c1a5ce54eee5fcdedf434
github: none
---

- [x] **522 — a struct assignment in a function body past 32 deep still costs clang's front end the cube of the depth** | defect 469's other half: a chain whose every level's value is built in one function reads 3.39 and 18.75 billion instructions at 250 and 500 levels after 469's repair, and 1.47, 3.40 and 9.26 billion at 250, 500 and 1,000 with every struct assignment written as a byte copy by hand; the rewrite must keep a type check and add no temporary per site (lane b15-parse) | `selfhost/emit/` · defect 469 · panel 106 · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-parse's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the cube of the depth.

    Repaired at `29d0f3d8`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net and `run` whole are owed at the batch's close. Past `typeorder`'s depth a unit defines `HERO_COPY`, which asserts its two sides one type (`__builtin_types_compatible_p`) and moves the bytes, and a load, a store, a construction, a payload and a field read of one of the unit's own structs go through it with no temporary added; a call's result and an array element stay assignments, and below the depth no unit's C moves. clang's front end on the shape at 250, 500 and 1,000 levels reads 1.61, 3.73 and 9.85 billion instructions where it read 3.39, 18.75 and 130.96 (by hand, every assignment a byte copy: 1.46, 3.39 and 9.21), `-O0 -g -c` 6.39, 13.97 and 33.80 where it read 8.37, 30.39 and 160.46. One new `run/` case and its emission, read; two deep emissions moved by the definition alone.

## The repair

Repaired at `29d0f3d8`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net and `run` whole are owed at the batch's close. Past `typeorder`'s depth a unit defines `HERO_COPY`, which asserts its two sides one type (`__builtin_types_compatible_p`) and moves the bytes, and a load, a store, a construction, a payload and a field read of one of the unit's own structs go through it with no temporary added; a call's result and an array element stay assignments, and below the depth no unit's C moves. clang's front end on the shape at 250, 500 and 1,000 levels reads 1.61, 3.73 and 9.85 billion instructions where it read 3.39, 18.75 and 130.96 (by hand, every assignment a byte copy: 1.46, 3.39 and 9.21), `-O0 -g -c` 6.39, 13.97 and 33.80 where it read 8.37, 30.39 and 160.46. One new `run/` case and its emission, read; two deep emissions moved by the definition alone.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
