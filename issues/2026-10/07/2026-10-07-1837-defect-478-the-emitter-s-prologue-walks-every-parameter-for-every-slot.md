---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: 0fa162168893ef5c2bf1d81cbb8599b22dae1425
github: none
---

- [x] **478 — the emitter's prologue walks every parameter for every slot** | `emit/body.hero`'s prologue, the shape defect 230 removed from `own.hero`: 88 of 995 samples at many-params-3200 (lane b14-ir, read and sampled, not counted) | `selfhost/emit/body.hero` · defect 230 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-ir's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with parameters times slots.

    Repaired at `0fa16216`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The prologue asks one set, made once per function, which slots are parameters, where each slot walked every parameter: a function of 400, 800, 1,600 and 3,200 `@` parameters emits in 6.382, 9.866, 17.231 and 33.400 billion instructions before and 6.331, 9.678, 16.463 and 30.279 after, the C byte-identical.

## The repair

Repaired at `0fa16216`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The prologue asks one set, made once per function, which slots are parameters, where each slot walked every parameter: a function of 400, 800, 1,600 and 3,200 `@` parameters emits in 6.382, 9.866, 17.231 and 33.400 billion instructions before and 6.331, 9.678, 16.463 and 30.279 after, the C byte-identical.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
