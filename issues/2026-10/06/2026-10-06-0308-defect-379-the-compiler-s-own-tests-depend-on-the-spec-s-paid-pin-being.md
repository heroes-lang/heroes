---
kind: defect
area: harness
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **379 — the compiler's own tests depend on the spec's paid pin being fresh** | after lane b12-str192 amended the spec (panel 192's R8), the compiler test *no operand measures the spec* and the surface row *measure runs with no operand* read red until a paid `heroes measure --refresh` re-pins the spec's digest, so a lane that amends the spec cannot keep its own tests green (its report, 2026-10-06) | the spec's pinned digest and the tests that read it (`selfhost/measure/`, `tests/harness/suite_surface.hero`) · **class: improvement**

    **Origin:** lane b12-str192, 2026-10-06, found beside panel 192's landing (its report's *found beside*); filed by the coordinator at 03:08.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cleaner form: no program moves, and the refresh is owed at the round anyway.
