---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **497 — `emit/callback_guard.hero`'s `guarded` is two fifths of `--emit-c`'s emitting thread** | 36,330 of 90,720 samples on the emitting thread during `--emit-c` of the compiler's own source, mostly `memmove` and `Inst` retain and release, reached from `callback_thunk.lookup_table`; a value-copy cost, not a push (lane b14-p409) | `selfhost/emit/callback_guard.hero`, `selfhost/emit/callback_thunk.hero` · defect 409 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-p409's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): the largest cost left in the emitter.
