---
kind: defect
area: process
milestone: none
filed: 2026-10-09
commit: 5d6820b2d5af666e86da953737ebb527950631f9
github: none
---

- [x] **515 — the commit guard judges the index alone, so a commit of unstaged paths passes unjudged** | `.claude/hooks/staged.py` reads `git diff --cached`, so `git commit -F <msg> -- <paths>` of files never staged commits content no hook judged; at about 04:15 on 2026-10-09 a module over its ceiling and unstaged gave `staged.offences()` nothing while `ceiling.verdict()` refused it (lane b15-emit) | `.claude/hooks/staged.py` · defects 287 and 403 · **class: adjacent**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-emit's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach: the commit the hard stops prescribe is the one it does not judge.

    Repaired at `5d6820b2`, 2026-10-09 (lane b16-tools, batch 16), gated by its cases and the hooks' own tests; the net is owed at the batch's close. The check judges what the commit carries: each file a pathspec names that differs from HEAD as the working tree holds it, staged or not, and every other staged file as the index holds it, the mirror of the same cause (a merge concluded with a file staged broken and repaired in the working tree alone was committed broken, measured). 8 cases, 5 red on the base; the hooks' own tests 110 run and 2 failed, the two `Place` tests the base fails the same way with its temporary folder inside the repository.

## The repair

Repaired at `5d6820b2`, 2026-10-09 (lane b16-tools, batch 16), gated by its cases and the hooks' own tests; the net is owed at the batch's close. The check judges what the commit carries: each file a pathspec names that differs from HEAD as the working tree holds it, staged or not, and every other staged file as the index holds it, the mirror of the same cause (a merge concluded with a file staged broken and repaired in the working tree alone was committed broken, measured). 8 cases, 5 red on the base; the hooks' own tests 110 run and 2 failed, the two `Place` tests the base fails the same way with its temporary folder inside the repository.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
