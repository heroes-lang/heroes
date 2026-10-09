---
kind: defect
area: compiler
milestone: none
filed: 2026-10-09
commit: 7e761a991d0589f257ca1d84692dfd0e90adf87d
github: none
---

- [x] **521 — `stack exhausted in mixed.helper` names the frame that ran out, not the recursion** | `climbs(helper(n))`: the panic names `helper`, the last frame, where the recursion is `climbs` (panel 199's compiler-engineer, route (M)) | the stack guard's message, `runtime/parts/stack.c` · panel 199 · **class: adjacent**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from panel 199's R7 and its compiler-engineer's report.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `7e761a99`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net, `run` whole and, as a runtime item, the push's platform legs are owed. At the abort the guard walks the frames again, a function met twice being a recursion, and says it beside the frame that ran out: *panic: stack exhausted in behind.helper, inside the recursion of behind.climbs*, a mutual one in call order from the first by name; where nothing repeats or the cycle is the frame named, the first sentence stands alone, and Windows, which walks no frames, is unchanged. Run on this Mac and Linux arm64 at both levels, every line true; three surface checks over `recursion521/`, two red on the base runtime.

## The repair

Repaired at `7e761a99`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net, `run` whole and, as a runtime item, the push's platform legs are owed. At the abort the guard walks the frames again, a function met twice being a recursion, and says it beside the frame that ran out: *panic: stack exhausted in behind.helper, inside the recursion of behind.climbs*, a mutual one in call order from the first by name; where nothing repeats or the cycle is the frame named, the first sentence stands alone, and Windows, which walks no frames, is unchanged. Run on this Mac and Linux arm64 at both levels, every line true; three surface checks over `recursion521/`, two red on the base runtime.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
