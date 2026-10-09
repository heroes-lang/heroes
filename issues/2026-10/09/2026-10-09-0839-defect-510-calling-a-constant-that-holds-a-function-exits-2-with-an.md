---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: 4d12ddacac9be125e979e0198047c00a35212a41
github: none
---

- [x] **510 — calling a constant that holds a function exits 2 with an internal error** | `DOUBLE(4)`, where a constant `DOUBLE` holds a function: the generated C calls the constant's zero-argument accessor with an argument and the build exits 2, *internal error*; binding it first, `h = DOUBLE` then `h(4)`, works (lane b16-land199; its reproducer `.claude/worktrees/scratch-b15/land199/beside/constcall.hero`, ignored by git) | the call of a constant of function type, `selfhost/ir/` and `selfhost/emit/` · **class: blocking**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b16-land199's final report; the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 on a correct program.

    Repaired at `4d12ddac`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The lowering called the constant's zero-argument accessor with the arguments where the checker had sent the call through its value; it now reads the constant and calls what it holds. The qualified spelling `helper.DOUBLE(4)` was a second door on the same path, the checker answering the error type with no diagnostic, so `check` passed `helper.DOUBLE(4, 5)` at exit 0 and every build stopped at exit 2; it is checked through the constant's type now. Four `run/` cases and two surface rows over `constcall510/`, each red on the base; emission 1,074 with no blessed emission moved, the compiler's own tests 1,504.

## The repair

Repaired at `4d12ddac`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The lowering called the constant's zero-argument accessor with the arguments where the checker had sent the call through its value; it now reads the constant and calls what it holds. The qualified spelling `helper.DOUBLE(4)` was a second door on the same path, the checker answering the error type with no diagnostic, so `check` passed `helper.DOUBLE(4, 5)` at exit 0 and every build stopped at exit 2; it is checked through the constant's type now. Four `run/` cases and two surface rows over `constcall510/`, each red on the base; emission 1,074 with no blessed emission moved, the compiler's own tests 1,504.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
