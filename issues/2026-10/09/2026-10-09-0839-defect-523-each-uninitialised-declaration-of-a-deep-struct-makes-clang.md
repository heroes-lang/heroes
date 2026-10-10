---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: f0a126758de76004bb02ee7f7d4d40deb64350cc
github: none
---

- [x] **523 — each uninitialised declaration of a deep struct makes clang walk its nesting again** | 2,000 declarations at depth 1,000, 2,000 and 4,000 cost +0.57, +1.10 and +2.16 billion instructions over the same declarations with `= {0}`; the one emitted shape that avoids it reverses panel 182's ruling that a temporary gets no initialiser, so it is a sitting's (lane b15-parse) | the prologue in `selfhost/emit/` · panel 182 · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-parse's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a cost a ruling stands in front of.

    Measured at `f0a12675`, 2026-10-09 (panel 200, ratified at 20:57): a known cost; most of clang's walk is a call result's assignment, the missing initialiser about 2.2e9 of 12.5e9 at depth 1,000, the seed's deepest chain 8, the corpus's 5, four stress cases past 32; panel 182 stands. Closed on its measurement (panel 200's R5); the deep record's crash under `= {0}` filed apart as defect 539.

## The repair

Measured at `f0a12675`, 2026-10-09 (panel 200, ratified at 20:57): a known cost; most of clang's walk is a call result's assignment, the missing initialiser about 2.2e9 of 12.5e9 at depth 1,000, the seed's deepest chain 8, the corpus's 5, four stress cases past 32; panel 182 stands. Closed on its measurement (panel 200's R5); the deep record's crash under `= {0}` filed apart as defect 539.

**Closed 2026-10-09** with batch 17 (lanes b17-fix, b17-emit and b17-check, landing panels 200 and 201 as ratified that evening, merged into one round tree made from the trunk), under the optimistic chain (`.claude/rules/verification.md` § The optimistic chain), its closing gate run on the round at `46b80b82`: the seed regenerated over two generations, the runtime's ABI moved to 30 by defect 470, 47,052,134 bytes, SHA-256 beginning `2b668ee89a3697cd`, its fixpoint by `cmp`, the committed seed of ABI 29 built against the runtime it was emitted for; the compiler's own tests 1,542, all passed; the net's own tests 326, all passed; the full net 7,306 passed over 29 suites, 0 failed, `run` in four shards of the harness's own (defect 537), 426 of 426 cases read, one skipped on this Mac (defect 437's case, `sys/prctl.h`); one floor told outgrown, `full`, raised in the closing commit as defect 536's rule asks. The census and panel 187's R2 run after the push, beside the CI.
