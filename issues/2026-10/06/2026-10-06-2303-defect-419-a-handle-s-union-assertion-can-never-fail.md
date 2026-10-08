---
kind: defect
area: emit
milestone: none
filed: 2026-10-06
commit: c20965118b4af4eebc5f4b4a51f8f37819492d0f
github: none
---

- [x] **419 — a handle's union assertion can never fail** | the emitter writes `_Static_assert(__builtin_classify_type(*(T * *)0) != 13, ...)` for a handle, which classifies a pointer and so always holds; its own comment in `emit/extern_union.hero` says a handle is never a union; removing it moves 7 blessed emissions | `selfhost/emit/extern_union.hero` · **class: improvement**

    **Origin:** lane b13-run400, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:03; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a check that checks nothing, no program judged wrong.

    Repaired at `c2096511`, 2026-10-07 (lane b14-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A compared handle is asked nothing: its C type is a pointer, so the assertion held for every handle, and the one that asks the pointee fires on a handle over a union (clang, *13 != 13*, on a hand-written header) and would refuse a program that compares two addresses correctly, which `run/fixedbugs-419-a-handle-over-a-union-compares-as-an-address` now pins; 11 blessed emissions moved where the card counted 7, `emission` 973 passed and 0 failed, the compiler's own tests 1,359, all passed.

## The repair

Repaired at `c2096511`, 2026-10-07 (lane b14-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A compared handle is asked nothing: its C type is a pointer, so the assertion held for every handle, and the one that asks the pointee fires on a handle over a union (clang, *13 != 13*, on a hand-written header) and would refuse a program that compares two addresses correctly, which `run/fixedbugs-419-a-handle-over-a-union-compares-as-an-address` now pins; 11 blessed emissions moved where the card counted 7, `emission` 973 passed and 0 failed, the compiler's own tests 1,359, all passed.

**Closed 2026-10-08**, after the push's platform legs, this defect being at the C boundary (`.claude/rules/verification.md` § The batch): batch 14 closed on this Mac alone and the CI's legs ran its cases afterwards. The CI's four legs on `ee95a6f0` (run 37735987684, created at 08:08 and its Windows leg finished at 10:22 on 2026-10-08) are all green: Darwin arm64 with the net at 6,809 passed and 0 failed, Linux arm64 and Linux x86-64 at 6,790 each, Windows x86-64 at 6,648, and on every leg the compiler's own tests 1,430, the module's 260 and the net's own tests 308, all passed. A case bound to one platform ran where it is bound, read from the legs' logs: the SDL3 event case of defect 213 and the `sys/prctl.h` case of defect 437 are not among the SKIP lines of either Linux leg (they are, as they must be, on Darwin and on Windows), and the Linux legs built SDL3 from source and checked that `pkg-config` answers 3.2.10 before the net started. The leg that had read red on `02256c1e`, Windows, did so on defect 505's test of the order of legs, repaired at `ee95a6f0`.
