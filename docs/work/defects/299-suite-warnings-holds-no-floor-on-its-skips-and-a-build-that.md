- [ ] **299 — `suite_warnings` holds no floor on its skips, and a build that fails for another reason may read as passing** | lane b9-harness read in `tests/harness/suite_warnings.hero` that its skips have no floor (every other form's skip floor is a third) and that a build failing for a reason other than a missing library may read as passing; **read, not run**: a question until a planted failing build is run through it (2026-10-04) | `tests/harness/suite_warnings.hero` · defect 246's witness, which the suite now asks · **class: improvement**

    **Origin:** lane b9-harness, 2026-10-04 (its final reply's *found beside*), a reading.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): an instrument's hole, unmeasured; if a planted failing build reads green, it is `blocking` by 246's reason.
