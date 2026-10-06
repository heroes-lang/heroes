---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: 12fca9eaca3ffc6f625e46b5944aa4d6c8c709cb
github: none
---

- [ ] **366 — a call left open inside a C `for (` header takes the body** | `for (i = 0; i < f(3; i++` over a body: four messages on lane b12-parse12's compiler (`unexpected_character`, `unclosed_bracket`, `for_missing_in`, `missing_body`), five on the base (measured by the coordinator, 2026-10-05, `<scratchpad>/batch12/filing/` `302-f-call-inside.hero`) | `selfhost/parse/loop_habit.hero`, `parse/unclosed.hero` · defect 302's sibling · **class: adjacent**

    **Origin:** lane b12-parse12, 2026-10-05, found beside its items (its report's *found beside*); reproduced by the coordinator before 22:42 and filed under the author's instruction of that evening, meant as: *any defect found that is not an improvement goes straight into the batch*.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake, and a body told missing that is there.

    Repaired at `12fca9ea`, 2026-10-06 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
