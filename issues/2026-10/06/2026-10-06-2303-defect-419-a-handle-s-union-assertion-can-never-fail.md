---
kind: defect
area: emit
milestone: none
filed: 2026-10-06
commit: c20965118b4af4eebc5f4b4a51f8f37819492d0f
github: none
---

- [ ] **419 — a handle's union assertion can never fail** | the emitter writes `_Static_assert(__builtin_classify_type(*(T * *)0) != 13, ...)` for a handle, which classifies a pointer and so always holds; its own comment in `emit/extern_union.hero` says a handle is never a union; removing it moves 7 blessed emissions | `selfhost/emit/extern_union.hero` · **class: improvement**

    **Origin:** lane b13-run400, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:03; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a check that checks nothing, no program judged wrong.

    Repaired at `c2096511`, 2026-10-07 (lane b14-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A compared handle is asked nothing: its C type is a pointer, so the assertion held for every handle, and the one that asks the pointee fires on a handle over a union (clang, *13 != 13*, on a hand-written header) and would refuse a program that compares two addresses correctly, which `run/fixedbugs-419-a-handle-over-a-union-compares-as-an-address` now pins; 11 blessed emissions moved where the card counted 7, `emission` 973 passed and 0 failed, the compiler's own tests 1,359, all passed.
