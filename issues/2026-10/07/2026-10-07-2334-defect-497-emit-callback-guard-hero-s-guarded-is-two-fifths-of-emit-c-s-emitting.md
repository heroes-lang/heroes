---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: fe2b01ee050a65d06bd3377634f3f3975f031e0a
github: none
---

- [ ] **497 — `emit/callback_guard.hero`'s `guarded` is two fifths of `--emit-c`'s emitting thread** | 36,330 of 90,720 samples on the emitting thread during `--emit-c` of the compiler's own source, mostly `memmove` and `Inst` retain and release, reached from `callback_thunk.lookup_table`; a value-copy cost, not a push (lane b14-p409) | `selfhost/emit/callback_guard.hero`, `selfhost/emit/callback_thunk.hero` · defect 409 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-p409's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): the largest cost left in the emitter.

    Repaired at `fe2b01ee`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Which functions C may call back is one record, `callback_guard.Callbacks`, asked once per program in `members.Whole` and in the fused unit, where every translation unit asked the whole program twice: the compiler's own `--emit-c` over the base's source read 1,314.578 billion instructions and reads 818.095, its C byte-identical, and 100 modules of ten functions binding `atexit` read 32.191 billion against 28.438.
