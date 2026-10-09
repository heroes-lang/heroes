---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: 1ef2ccfe0e7035b514372ac5d3da048730e97e7c
github: none
---

- [x] **490 — `cli/deep_types.hero`'s walk without recursion has no witness** | defect 189's shape in the cli's own walk over the deepest by-value chain: a mutant restoring its recursion is caught by nothing (lane b14-emit's reading) | `selfhost/cli/deep_types.hero` · defect 189 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-emit's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): coverage.

    Repaired at `1ef2ccfe`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The walk takes its graph as data (`held_graph` by a queue, `deepest_of` with its own stack), and a test holds it to a chain of 100,000: the mutant restoring the recursion fails it, `panic: stack exhausted in clideeptypes.measured`, defect 170's test passing; the tree passes.

## The repair

Repaired at `1ef2ccfe`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The walk takes its graph as data (`held_graph` by a queue, `deepest_of` with its own stack), and a test holds it to a chain of 100,000: the mutant restoring the recursion fails it, `panic: stack exhausted in clideeptypes.measured`, defect 170's test passing; the tree passes.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
