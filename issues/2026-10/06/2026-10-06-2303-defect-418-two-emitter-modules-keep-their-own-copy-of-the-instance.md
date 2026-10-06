---
kind: defect
area: emit
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **418 — two emitter modules keep their own copy of the instance lookup for calls** | `emit/inst.hero` and `emit/members.hero` each keep the rule that finds a generic call's instance, which `func_ref.instance_at` now names; one copy could serve both | `selfhost/emit/inst.hero`, `selfhost/emit/members.hero`, `selfhost/emit/func_ref.hero` · **class: improvement**

    **Origin:** lane b13-gen402, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:03; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cleaner form, no program judged wrong.
