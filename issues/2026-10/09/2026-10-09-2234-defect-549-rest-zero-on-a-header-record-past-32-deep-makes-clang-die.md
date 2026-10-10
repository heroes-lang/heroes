---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: 524627fd8783a46f20d7370150671978d91b46dc
github: none
---

- [x] **549 — `rest: zero` on a header record past 32 deep makes clang die on its `(T){0}`** | `rest: zero` on a header record 1,000 deep writes `t = (G999){0};` (`selfhost/emit/zeroed.hero`), and clang dies (Illegal instruction: 4) on the base and with defect 539's repair; without that line, or with `(G999){}`, it compiles; panel 194 chose `{0}` there on C11 grounds (lane b17-emit; reproducer `.claude/worktrees/scratch-b15/b17-emit/rz/`, ignored by git) | `selfhost/emit/zeroed.hero` · defect 539 · panel 194 · **class: blocking**

    **Origin:** filed by the coordinator at 22:34 on 2026-10-09 from lane b17-emit's final report (its notes `.claude/worktrees/scratch-b15/b17-emit/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 on a correct program the emitter can avoid, defect 539's cause in another writer.

    Repaired at `524627fd8783a46f20d7370150671978d91b46dc`, 2026-10-09, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Past `typeorder`'s depth of 32, asked of the group's record by a walk capped there and through fixed arrays, the construction's whole write is `(T){}`, which clang does not recurse into and `-std=gnu11` accepts, the memset after it zeroing every byte as panel 194 rules; at or below 32 panel 194's C11 `{0}` stands, so no blessed emission moved. Case `run/fixedbugs-549-rest-zero-on-a-group-record-a-thousand-deep-builds`, `G999(rest: zero)` and `G31(rest: zero)` on defect 140's header: exit 2 on the base (Illegal instruction: 4), prints 0 and 0 now, its emission writing `(G999){}` and `(G31){0}`. run (only fixedbugs-549) 1 and 0, rest-zero 7 and 0, emission 1,084 and 0 (only the new case written), wholes 525 and 0, the compiler's own tests 1,542 passed.

## The repair

Repaired at `524627fd8783a46f20d7370150671978d91b46dc`, 2026-10-09, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Past `typeorder`'s depth of 32, asked of the group's record by a walk capped there and through fixed arrays, the construction's whole write is `(T){}`, which clang does not recurse into and `-std=gnu11` accepts, the memset after it zeroing every byte as panel 194 rules; at or below 32 panel 194's C11 `{0}` stands, so no blessed emission moved. Case `run/fixedbugs-549-rest-zero-on-a-group-record-a-thousand-deep-builds`, `G999(rest: zero)` and `G31(rest: zero)` on defect 140's header: exit 2 on the base (Illegal instruction: 4), prints 0 and 0 now, its emission writing `(G999){}` and `(G31){0}`. run (only fixedbugs-549) 1 and 0, rest-zero 7 and 0, emission 1,084 and 0 (only the new case written), wholes 525 and 0, the compiler's own tests 1,542 passed.

**Closed 2026-10-09** with batch 17 (lanes b17-fix, b17-emit and b17-check, landing panels 200 and 201 as ratified that evening, merged into one round tree made from the trunk), under the optimistic chain (`.claude/rules/verification.md` § The optimistic chain), its closing gate run on the round at `46b80b82`: the seed regenerated over two generations, the runtime's ABI moved to 30 by defect 470, 47,052,134 bytes, SHA-256 beginning `2b668ee89a3697cd`, its fixpoint by `cmp`, the committed seed of ABI 29 built against the runtime it was emitted for; the compiler's own tests 1,542, all passed; the net's own tests 326, all passed; the full net 7,306 passed over 29 suites, 0 failed, `run` in four shards of the harness's own (defect 537), 426 of 426 cases read, one skipped on this Mac (defect 437's case, `sys/prctl.h`); one floor told outgrown, `full`, raised in the closing commit as defect 536's rule asks. The census and panel 187's R2 run after the push, beside the CI.
