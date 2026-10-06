---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: 5a0f1db0d660627fa81eece0dfb8b862eff84f61
github: none
---

- [ ] **370 — lone operator lines under a statement cost three messages for two lines** | `k = n` over a deeper `-` and a deeper `-` again: `continuation_outside_brackets`, `unexpected_block` and `expected_expression`, on the base and on lane b12-parse12's compiler alike (measured by the coordinator, 2026-10-05, `<scratchpad>/batch12/filing/` `351-h-statement.hero`) | `selfhost/sign_above.hero` · defect 351's sibling, under a statement rather than an arm · **class: adjacent**

    **Origin:** lane b12-parse12, 2026-10-05, found beside its items (its report's *found beside*); reproduced by the coordinator before 22:42 and filed under the author's instruction of that evening, meant as: *any defect found that is not an improvement goes straight into the batch*.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): more messages than mistakes.

    Repaired at `5a0f1db0`, 2026-10-06 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
