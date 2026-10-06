---
kind: defect
area: resolve
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **374 — a correct chain of constants costs more than its length** | `constant C<i>: i64` over `C<i+1>` down to a last `1`, a correct program: `check --brief` retires 1.01 billion instructions at 1,000 and 5.47 billion at 4,000, five point four times as much for four times the input (the base's compiler at `00217c39`, measured by the coordinator at 02:46 on 2026-10-06, `<scratchpad>/batch12/filing/` `n3-*.hero`); lane b12-parse12 measured 0.95 to 11.84 billion from 1,000 to 8,000 after defect 363's repair | `selfhost/resolve/cycles.hero`, `walk`, which pops its path with a slice, a copy a step (a sample at 16,000, lane b12-parse12) · the checker's `constant_body` · **class: adjacent**

    **Origin:** lane b12-parse12, 2026-10-06, found beside defects 363 and 370 (its report's *found beside*); reproduced by the coordinator at 02:46 and filed into batch 12 under the author's instruction of 2026-10-05, any defect found that is not an improvement goes into the batch.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cost found beside the work on a correct program, no message wrong.
