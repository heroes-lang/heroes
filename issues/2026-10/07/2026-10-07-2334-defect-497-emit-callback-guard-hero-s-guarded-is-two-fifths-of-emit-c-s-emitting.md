---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: fe2b01ee050a65d06bd3377634f3f3975f031e0a
github: none
---

- [x] **497 — `emit/callback_guard.hero`'s `guarded` is two fifths of `--emit-c`'s emitting thread** | 36,330 of 90,720 samples on the emitting thread during `--emit-c` of the compiler's own source, mostly `memmove` and `Inst` retain and release, reached from `callback_thunk.lookup_table`; a value-copy cost, not a push (lane b14-p409) | `selfhost/emit/callback_guard.hero`, `selfhost/emit/callback_thunk.hero` · defect 409 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-p409's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): the largest cost left in the emitter.

    Repaired at `fe2b01ee`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Which functions C may call back is one record, `callback_guard.Callbacks`, asked once per program in `members.Whole` and in the fused unit, where every translation unit asked the whole program twice: the compiler's own `--emit-c` over the base's source read 1,314.578 billion instructions and reads 818.095, its C byte-identical, and 100 modules of ten functions binding `atexit` read 32.191 billion against 28.438.

## The repair

Repaired at `fe2b01ee`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Which functions C may call back is one record, `callback_guard.Callbacks`, asked once per program in `members.Whole` and in the fused unit, where every translation unit asked the whole program twice: the compiler's own `--emit-c` over the base's source read 1,314.578 billion instructions and reads 818.095, its C byte-identical, and 100 modules of ten functions binding `atexit` read 32.191 billion against 28.438.

**Closed 2026-10-08** with batch 15 (lanes b15-parse, b15-check, b15-emit, b15-runtime, b15-harness, b15-deepc, b15-hooks, b15-mutate, b15-box and b15-print, and batch 14's lanes b14-p409 and b14-land197 carried, merged into one round made from the trunk at `56def9b4`; the batch closed on the round as it stood when a reboot at about 11:18 stopped every lane and the author asked at about 16:20 to resume in order, fewer lanes at a time, the round's finished repairs gated and pushed first), its closing gate run on the round at `c02f0840` and the seven suites read red there re-run after their repairs: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 44,867,177 bytes, SHA-256 beginning `dc4079f9bb2026d2`, its fixpoint by `cmp`; the compiler's own tests 1,478, all passed; the net's own tests 318, all passed; the full net 6,971 passed over 29 suites, 0 failed, after three floors were raised to the merged tree's counts (`layout` 527, `permissive` 14, `lines` 398), nine new emissions blessed (defects 451's and 462's cases), defect 462's read check moved `run/fixedbugs-c-writes-over-a-strings-header`'s output (its panic now comes at the read, before the print), a citation in defect 477's case corrected to spec § 9, and `docs/platforms/` made a home of C for defect 463's probe; the census of `check` over 3,038 tracked files moving 15 verdicts and of `--emit-c` moving the C of 2 and 16 refusals, every move attributed (in `check`, 12 to defects 445's and 446's cases and the cases they moved, 3 to defect 504's; the C, defects 219's and 468's per-type touches on the two chains past 32 deep; the refusals, the same 15 and `selfhost/main.hero`, whose emission by the trunk's compiler passed the census's 120 s bound under six parallel jobs, exit 124, not a refusal). The site's build read 188 pages and its claims held. As batch 14's, the formatter's probe by hand and panel 187's R2 replay run after the push, and Linux arm64 and Windows are the CI's legs.
