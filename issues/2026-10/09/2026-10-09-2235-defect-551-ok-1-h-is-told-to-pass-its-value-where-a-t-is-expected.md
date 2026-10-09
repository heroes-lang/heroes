---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **551 — `ok(1).h()` is told to pass its value where a `T?` is expected, which it did** | `ok(1).h()` with a non-generic `h(n: i64?)` is refused with *pass it where a `T?` is expected*: a UFCS receiver gets no type from its position, a cause apart from defect 544's (lane b17-check) | the receiver of a UFCS call in `selfhost/check/` · defect 544 · **class: blocking**

    **Origin:** filed by the coordinator at 22:35 on 2026-10-09 from lane b17-check's final report (its notes `.claude/worktrees/scratch-b15/b17-check/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a false message on a reachable shape.
