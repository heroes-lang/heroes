---
kind: defect
area: resolve
milestone: none
filed: 2026-10-05
commit: none
github: none
---

- [ ] **363 — `constant` bodies refused by `constant_body` cost the square of their number** | N declarations `constant C<i>: i64` over `len(args())`: `check --brief` retires 1.36 billion instructions at 500 and 13.52 billion at 2,000, nine times as many for four times the input, each a refusal (the base's compiler at `00217c39`, measured by the coordinator, 2026-10-05, `<scratchpad>/batch12/filing/` `n1-*`) | `selfhost/resolve/cycles.hero`, `bodies` (it keeps the outermost offender by comparing every offender with every other, and runs a linear `contains` on the refused list, read by lane b12-parse12, not profiled) · **class: adjacent**

    **Origin:** lane b12-parse12, 2026-10-05, found beside its items (its report's *found beside*); reproduced by the coordinator before 22:42 and filed under the author's instruction of that evening, meant as: *any defect found that is not an improvement goes straight into the batch*.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a cost found beside the work, the square of a count, no message wrong.
