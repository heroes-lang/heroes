- [ ] **289 — no golden pins `check`'s full diagnostic form, so the notes and excerpts of a check-stage diagnostic are pinned nowhere** | 0 of the 466 `.expected` files of `tests/golden/check/` hold an `at` line or a gutter (counted by the coordinator at `703af779`, 2026-10-04): every one is `check --brief`, which prints neither the excerpt nor the notes, so a note or an excerpt that changes moves no check golden | `tests/harness/suite_golden.hero` (the `check` form's flag) · defect 244 (the excerpt's control characters) and 271 (a fix's place), both in the parts nothing pins · **class: improvement**

    **Origin:** lane b9-harness, 2026-10-04 (its reply's *found beside*); counted by the coordinator.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): coverage; no program moves.

    Repaired at `27998f47`, 2026-10-04, gated by its cases and the net's own tests; the net is owed at the batch's close.
