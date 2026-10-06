---
kind: defect
area: compiler
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **414 — a backtick never closed before a hole is told twice** | `s = `never closed ${x}` with no closing backtick is told twice, at the backtick and at the `$`, on the base and after defect 407's repair alike; the cause is not 407's, there being no close at all | `selfhost/backtick_strings.hero`, `selfhost/refused_stretch.hero` · **class: adjacent**

    **Origin:** lane b13-tmpl407, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:03; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): one mistake told as two, beside the shape 391 and 407 repaired.
