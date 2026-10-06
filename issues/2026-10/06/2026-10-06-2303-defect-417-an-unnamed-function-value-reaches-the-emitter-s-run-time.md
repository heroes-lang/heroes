---
kind: defect
area: emit
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **417 — an unnamed function value reaches the emitter's run-time fallback instead of a build-time refusal** | the emitter's fallback for a function value it cannot name is `hero_unreachable`, which aborts at run time; a check in the IR verifier would refuse the program at build time | `selfhost/emit/func_ref.hero`, the IR verifier · **class: improvement**

    **Origin:** lane b13-gen402, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:03; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): hardening, no program measured reaching it.
