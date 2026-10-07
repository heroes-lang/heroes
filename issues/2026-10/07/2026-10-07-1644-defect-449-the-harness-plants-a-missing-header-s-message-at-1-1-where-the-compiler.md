---
kind: defect
area: harness
milestone: none
filed: 2026-10-07
commit: 271007e78532f4085c422228ae888dd57939207b
github: none
---

- [ ] **449 — the harness plants a missing header's message at `:1:1`, where the compiler says the group's first declaration** | the harness's planted missing-header messages sit at `:1:1`, the compiler's at the group's first declaration (`:2:5` in the lane's case); left because moving it moves other suites' mark tests (lane b14-harness-a, 2026-10-07) | `tests/harness/absence.hero` · defect 328 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-harness-a's final report (*found beside*).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a planted message less exact than the compiler's; no case reads wrong today.

    Repaired at `271007e7`, 2026-10-08 (lane b15-harness), gated by its cases and the net's own tests; the net is owed at the batch's close. `absence.group_place` reads a source as the compiler places the message, at the first declaration of the group naming the header, and every plant of a header's message, `planted_case` and the plants of five suites' tests, stands there, the mark of defect 246's pinned case moved to the function's line; the defect 328 test asks the compiler that a plant is its own headline and `at` line over five shapes, and that `planted_case`'s words are its own (red on the base, `:1:1` against `:2:5`). `special` reads 10 passed, 0 failed over the spec program its check and its test now read once.
