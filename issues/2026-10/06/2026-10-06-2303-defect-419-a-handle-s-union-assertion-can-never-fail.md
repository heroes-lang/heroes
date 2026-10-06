---
kind: defect
area: emit
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **419 — a handle's union assertion can never fail** | the emitter writes `_Static_assert(__builtin_classify_type(*(T * *)0) != 13, ...)` for a handle, which classifies a pointer and so always holds; its own comment in `emit/extern_union.hero` says a handle is never a union; removing it moves 7 blessed emissions | `selfhost/emit/extern_union.hero` · **class: improvement**

    **Origin:** lane b13-run400, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:03; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a check that checks nothing, no program judged wrong.
