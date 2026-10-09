---
kind: defect
area: parse
milestone: none
filed: 2026-10-09
commit: 608715260c645afd8dacc15479247bb3396170f1
github: none
---

- [x] **524 — a function head that fails over several lines holds back its missing-body message** | a failing head on one line is told 2 messages, the same head broken over several lines 1 (lane b15-parse) | `selfhost/parse/`, the head's recovery · **class: adjacent**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-parse's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a mistake told only after the first is fixed.

    Repaired at `60871526`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The report that a failed head's body is missing is held back only where the head went on, inside its brackets, to a line at its body's margin, one level below its own, where its recovery may have read the body (`body_margin.crossed`); a head whose lines past its first all stand at another margin is told its missing body as on one line. A head broken at the body's own margin stays held back, reading as a swallowed body does. One new `check` case, its two heads red on the base; `fixedbugs-456-a-broken-type-in-a-head-last-in-its-file` moved as its own note foretold.

## The repair

Repaired at `60871526`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The report that a failed head's body is missing is held back only where the head went on, inside its brackets, to a line at its body's margin, one level below its own, where its recovery may have read the body (`body_margin.crossed`); a head whose lines past its first all stand at another margin is told its missing body as on one line. A head broken at the body's own margin stays held back, reading as a swallowed body does. One new `check` case, its two heads red on the base; `fixedbugs-456-a-broken-type-in-a-head-last-in-its-file` moved as its own note foretold.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
