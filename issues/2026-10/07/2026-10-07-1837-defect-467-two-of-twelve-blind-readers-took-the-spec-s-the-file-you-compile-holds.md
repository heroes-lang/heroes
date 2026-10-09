---
kind: defect
area: spec
milestone: none
filed: 2026-10-07
commit: f0a126758de76004bb02ee7f7d4d40deb64350cc
github: none
---

- [ ] **467 — two of twelve blind readers took the spec's *the file you compile holds `function main()`* to bind a module** | spec line 22; two of the 12 sessions of measurement 040 on module files added a `main`, which `check` accepts without (lane b14-m212) | `spec/heroes-spec.md:22` · docs/measurements/040 (batch 14's round, unmerged on 2026-10-07) · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-m212's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a spec sentence's reading, a sitting's question.

    Measured at `f0a12675`, 2026-10-09 (panel 201, ratified at 20:57): the sentence stays, 0 tokens; with the module beside the program that uses it, the blind seat's four readers added no `main` under either wording or spec, a used module's `main` is accepted and never runs, and the historian found no language that documents it as a mistake. Closed on its measurement (panel 201's R2); the `no_entry_point` note filed apart as defect 542, a used module's `main` told two ways as defect 543.
