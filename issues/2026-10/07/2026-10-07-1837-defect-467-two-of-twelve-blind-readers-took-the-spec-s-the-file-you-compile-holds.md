---
kind: defect
area: spec
milestone: none
filed: 2026-10-07
commit: f0a126758de76004bb02ee7f7d4d40deb64350cc
github: none
---

- [x] **467 — two of twelve blind readers took the spec's *the file you compile holds `function main()`* to bind a module** | spec line 22; two of the 12 sessions of measurement 040 on module files added a `main`, which `check` accepts without (lane b14-m212) | `spec/heroes-spec.md:22` · docs/measurements/040 (batch 14's round, unmerged on 2026-10-07) · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-m212's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a spec sentence's reading, a sitting's question.

    Measured at `f0a12675`, 2026-10-09 (panel 201, ratified at 20:57): the sentence stays, 0 tokens; with the module beside the program that uses it, the blind seat's four readers added no `main` under either wording or spec, a used module's `main` is accepted and never runs, and the historian found no language that documents it as a mistake. Closed on its measurement (panel 201's R2); the `no_entry_point` note filed apart as defect 542, a used module's `main` told two ways as defect 543.

## The repair

Measured at `f0a12675`, 2026-10-09 (panel 201, ratified at 20:57): the sentence stays, 0 tokens; with the module beside the program that uses it, the blind seat's four readers added no `main` under either wording or spec, a used module's `main` is accepted and never runs, and the historian found no language that documents it as a mistake. Closed on its measurement (panel 201's R2); the `no_entry_point` note filed apart as defect 542, a used module's `main` told two ways as defect 543.

**Closed 2026-10-09** with batch 17 (lanes b17-fix, b17-emit and b17-check, landing panels 200 and 201 as ratified that evening, merged into one round tree made from the trunk), under the optimistic chain (`.claude/rules/verification.md` § The optimistic chain), its closing gate run on the round at `46b80b82`: the seed regenerated over two generations, the runtime's ABI moved to 30 by defect 470, 47,052,134 bytes, SHA-256 beginning `2b668ee89a3697cd`, its fixpoint by `cmp`, the committed seed of ABI 29 built against the runtime it was emitted for; the compiler's own tests 1,542, all passed; the net's own tests 326, all passed; the full net 7,306 passed over 29 suites, 0 failed, `run` in four shards of the harness's own (defect 537), 426 of 426 cases read, one skipped on this Mac (defect 437's case, `sys/prctl.h`); one floor told outgrown, `full`, raised in the closing commit as defect 536's rule asks. The census and panel 187's R2 run after the push, beside the CI.
