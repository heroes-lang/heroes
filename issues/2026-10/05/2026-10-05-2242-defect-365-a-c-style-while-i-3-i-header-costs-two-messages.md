---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: f8c75ff50ad3685110d621265ec144a3adc4ec53
github: none
---

- [ ] **365 — a C-style `while i < 3; i++` header costs two messages** | `while i < 3; i++` over a body: `unexpected_character` and `continuation_outside_brackets`, two messages for one habit, on the base and on lane b12-parse12's compiler alike (measured by the coordinator, 2026-10-05, `<scratchpad>/batch12/filing/` `301-k-while.hero`) | the parser's C-habit readers, `selfhost/parse/loop_habit.hero` · defect 301's sibling · **class: adjacent**

    **Origin:** lane b12-parse12, 2026-10-05, found beside its items (its report's *found beside*); reproduced by the coordinator before 22:42 and filed under the author's instruction of that evening, meant as: *any defect found that is not an improvement goes straight into the batch*.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.

    Repaired at `f8c75ff5`, 2026-10-06 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
