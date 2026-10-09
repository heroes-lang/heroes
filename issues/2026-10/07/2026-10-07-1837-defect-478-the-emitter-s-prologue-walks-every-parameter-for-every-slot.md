---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: 0fa162168893ef5c2bf1d81cbb8599b22dae1425
github: none
---

- [ ] **478 — the emitter's prologue walks every parameter for every slot** | `emit/body.hero`'s prologue, the shape defect 230 removed from `own.hero`: 88 of 995 samples at many-params-3200 (lane b14-ir, read and sampled, not counted) | `selfhost/emit/body.hero` · defect 230 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-ir's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with parameters times slots.

    Repaired at `0fa16216`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The prologue asks one set, made once per function, which slots are parameters, where each slot walked every parameter: a function of 400, 800, 1,600 and 3,200 `@` parameters emits in 6.382, 9.866, 17.231 and 33.400 billion instructions before and 6.331, 9.678, 16.463 and 30.279 after, the C byte-identical.
