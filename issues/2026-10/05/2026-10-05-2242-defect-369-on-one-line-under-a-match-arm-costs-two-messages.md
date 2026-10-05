---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: f527a1efd4b011636a918f974bdd61669d178c6c
github: none
---

- [ ] **369 — `- -` on one line under a match arm costs two messages** | `- -` on its own line under an arm of a `match` over an integer: `continuation_outside_brackets` and `unexpected_block`, on the base and on lane b12-parse12's compiler alike (measured by the coordinator, 2026-10-05, `<scratchpad>/batch12/filing/` `351-g-one-line.hero`) | `selfhost/sign_above.hero` · defect 351's sibling · **class: adjacent**

    **Origin:** lane b12-parse12, 2026-10-05, found beside its items (its report's *found beside*); reproduced by the coordinator before 22:42 and filed under the author's instruction of that evening, meant as: *any defect found that is not an improvement goes straight into the batch*.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.

    Repaired at `f527a1ef`, 2026-10-06 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
