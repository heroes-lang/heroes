---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 228c36769b1da9b3824883a8961853cc6a3569c7
github: none
---

- [ ] **554 — a generic function of two labelled parameters given a function type is told twice** | `g: (function(B, B) -> B) = pick`, `pick` generic with two labelled parameters, gets both `needs_parameter_names` and a label `type_mismatch` for one mistake, on the base as well (lane b17-check) | `selfhost/check/function_value.hero` · **class: adjacent**

    **Origin:** filed by the coordinator at 22:35 on 2026-10-09 from lane b17-check's final report (its notes `.claude/worktrees/scratch-b15/b17-check/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.

    Repaired at `228c3676`, 2026-10-09 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Not a generic's alone: a plain `add` was told twice too, and so was every place a value of the nameless type met a named one, eleven sites. `type_fit.unnamed_shadow` keeps quiet a mismatch between a nameless written type with two positions of one type, refused already, and a type differing from it by its names alone, asked at every site that tells a mismatch by identity; the type's note now says a function given to it fits where its own names agree. A difference of type stays told, and so does a nameless type a generic's letters made where the program writes that type nowhere (where it does, the second mistake waits for the first repair). Three cases that pinned the second message moved, each corrected beneath. Cases `check/fixedbugs-554-…` (15 shadows on the base) and its witness; `check` 650, `full` 32, `permissive` 16, the compiler's 1,544 tests, 0 failed.
