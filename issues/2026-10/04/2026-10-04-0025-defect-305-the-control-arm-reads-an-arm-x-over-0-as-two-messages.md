---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **305 — the control arm reads an arm `x =>` over `_ => 0` as two messages** | under `check --permissive`, an arm `x =>` with no value over `_ => 0`: two messages for the one missing value, where the thesis arm tells one since defect 193's repair (lane b9-recovery's compiler, 2026-10-04) | `selfhost/grammar_expr.hero` (the arm reader) · `tests/golden/permissive/` (defect 211's form, where it can be pinned) · defect 193 · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`), beside 193, the control arm compared by hand.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake, in the control arm the net now runs.

    Measured 2026-10-05 (lane b12-parse12), not reproduced as filed: on the base (`ca5fa51e`) and on the lane's compiler alike, the arm `x =>` over `_ => 0` holds two mistakes, a name where a pattern goes and no value, and each arm tells one message for each: the thesis arm `expected_pattern` at the `x` and `continuation_outside_brackets` at its `=>`, the control arm `expected_pattern` and `expected_end_of_line` at the next arm's `=>`, where the line it joins goes on past the `_`, as a language without the thesis's line rule reads it. With a pattern, `1 =>` over `_ => 0`, each arm tells one; so do the last arm, a padded one, two patterns, a variant's, a nested `match`, a statement's and a body below (nine shapes, `<scratchpad>/batch12/parse12/shapes/305-*`). Left open for the coordinator's ruling, nothing repaired.
