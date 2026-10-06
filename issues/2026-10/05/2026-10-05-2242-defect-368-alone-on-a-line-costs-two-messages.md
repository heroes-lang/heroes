---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: a96ef730b36b0ce7955aba908dc492e944f51e2d
github: none
---

- [ ] **368 — `))` alone on a line costs two messages** | a line holding `))` alone inside a function: `expected_expression` and `expected_end_of_line`, on the base and on lane b12-parse12's compiler alike (measured by the coordinator, 2026-10-05, `<scratchpad>/batch12/filing/` `306-k-alone.hero`) | the stray-closer reader, `selfhost/closers.hero` · defect 306's sibling · **class: adjacent**

    **Origin:** lane b12-parse12, 2026-10-05, found beside its items (its report's *found beside*); reproduced by the coordinator before 22:42 and filed under the author's instruction of that evening, meant as: *any defect found that is not an improvement goes straight into the batch*.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.

    Repaired at `a96ef730`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
