---
kind: defect
area: emit
milestone: none
filed: 2026-10-06
commit: 6c87c041cf26ed32e4ddc795ae9d73e6c13570f2
github: none
---

- [ ] **418 — two emitter modules keep their own copy of the instance lookup for calls** | `emit/inst.hero` and `emit/members.hero` each keep the rule that finds a generic call's instance, which `func_ref.instance_at` now names; one copy could serve both | `selfhost/emit/inst.hero`, `selfhost/emit/members.hero`, `selfhost/emit/func_ref.hero` · **class: improvement**

    **Origin:** lane b13-gen402, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:03; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cleaner form, no program judged wrong.

    Repaired at `6c87c041`, 2026-10-07 (lane b14-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The type arguments an instruction names and the function the program holds at them are each one function of `selfhost/ir/instances.hero`, `named_at` and `index_of`, where three emitter modules kept three copies of the first and three loops of the second (one keeping the last match, two the first); `emission` read 971 passed and 0 failed, every emission byte-identical, and the compiler's own tests 1,358, all passed.
