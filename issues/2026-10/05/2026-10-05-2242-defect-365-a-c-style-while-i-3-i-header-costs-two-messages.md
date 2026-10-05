---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: none
github: none
---

- [ ] **365 — a C-style `while i < 3; i++` header costs two messages** | `while i < 3; i++` over a body: `unexpected_character` and `continuation_outside_brackets`, two messages for one habit, on the base and on lane b12-parse12's compiler alike (measured by the coordinator, 2026-10-05, `<scratchpad>/batch12/filing/` `301-k-while.hero`) | the parser's C-habit readers, `selfhost/parse/loop_habit.hero` · defect 301's sibling · **class: adjacent**

    **Origin:** lane b12-parse12, 2026-10-05, found beside its items (its report's *found beside*); reproduced by the coordinator before 22:42 and filed under the author's instruction of that evening, meant as: *any defect found that is not an improvement goes straight into the batch*.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.
