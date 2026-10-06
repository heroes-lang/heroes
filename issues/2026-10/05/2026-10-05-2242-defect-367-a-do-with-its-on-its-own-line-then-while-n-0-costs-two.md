---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: f1344c53563cd5666d35bb6d02ebf7dbba46ca99
github: none
---

- [ ] **367 — a `do` with its `{` on its own line, then `while (n > 0)`, costs two messages that name neither** | `do` over `{`, `print(n)`, `}`, `while (n > 0)`: `expected_expression` and `missing_body`, on the base and on lane b12-parse12's compiler alike (measured by the coordinator, 2026-10-05, `<scratchpad>/batch12/filing/` `303-h-allman.hero`) | `selfhost/parse/braced_lines.hero`, `parse/loop_habit.hero` · defect 303's sibling · **class: adjacent**

    **Origin:** lane b12-parse12, 2026-10-05, found beside its items (its report's *found beside*); reproduced by the coordinator before 22:42 and filed under the author's instruction of that evening, meant as: *any defect found that is not an improvement goes straight into the batch*.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake, neither naming the C habit.

    Repaired at `f1344c53`, 2026-10-06 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
