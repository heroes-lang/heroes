- [ ] **320 — two runs of the net's own tests in one tree collide on the fixed scratch `build/harness-selftest`** | two `heroes test tests/harness/main.hero` at once in one tree: 10 and 8 failures, every one a collision on the same scratch folder, where either alone reads 240 passed and 0 failed (the lane's run, discarded) | `tests/harness/absence.hero` (`scratch = "build/harness-selftest"`, six tests), `tests/harness/probe.hero:287`, and the net's other tests that name the folder · **class: improvement**

    **Origin:** lane b9-annot, 2026-10-04, each reproduced on its worktree's harness (its final reply's *Found beside*; scratch `<scratchpad>/batch9/annot/`).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a run of the net's own tests beside another can fail without a defect in what it tests; hardening.

    Repaired at `c0f4f521`, 2026-10-04, gated by its cases and the net's own tests; the net is owed at the batch's close.
