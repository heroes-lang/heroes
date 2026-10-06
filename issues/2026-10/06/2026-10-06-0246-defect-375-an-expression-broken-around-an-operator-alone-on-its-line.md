---
kind: defect
area: parse
milestone: none
filed: 2026-10-06
commit: 2cd107a7499f59866e9ea956b219960fdeda20eb
github: none
---

- [ ] **375 — an expression broken around an operator alone on its line costs two messages** | `k = n` over a deeper `-` over a deeper `1`: `continuation_outside_brackets` and `unexpected_block`, on the base and on lane b12-parse12's compiler alike (measured by the coordinator at 02:46 on 2026-10-06, `<scratchpad>/batch12/filing/` `n4.hero`); defect 370 leaves the operand-below shape alone on purpose | `selfhost/open_line.hero`, `selfhost/sign_above.hero` · defect 370's sibling · **class: adjacent**

    **Origin:** lane b12-parse12, 2026-10-06, found beside defects 363 and 370 (its report's *found beside*); reproduced by the coordinator at 02:46 and filed into batch 12 under the author's instruction of 2026-10-05, any defect found that is not an improvement goes into the batch.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.

    Repaired at `2cd107a7`, 2026-10-06 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
