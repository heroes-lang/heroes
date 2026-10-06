---
kind: defect
area: compiler
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **409 — a push onto a record's array field copies the whole array** | `r.f @ r.f.push(x)` copies the array at every push (by design, panel 037: only a plain name grows in place): 3.0, 12.1 and 48.1 billion instructions at 10,000, 20,000 and 40,000 pushes, against 21.6, 27.3 and 38.6 million for a local; 368 such lines in `selfhost/`, of which defect 393's three in `print/elements.hero` were quadratic | `grep` of `@ .*\.push(` on a field in `selfhost/` · **class: improvement**

    **Origin:** lane b13-front, 2026-10-06 (its report, *found beside, not filed*), the lane's measurement on its branch from `7001dfb3`, not re-run by the coordinator; filed by the coordinator at 20:41.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cost, no program judged wrong; an audit of the lines inside loops.
