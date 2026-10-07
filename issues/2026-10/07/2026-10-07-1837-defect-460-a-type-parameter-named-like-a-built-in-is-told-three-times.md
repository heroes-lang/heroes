---
kind: defect
area: resolve
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **460 — a type parameter named like a built-in is told three times** | `g<len>(x: len)`: `builtin_name_taken` and two more messages, because the refused parameter is never bound (lane b14-resolve) | `selfhost/resolve/decls.hero` · defect 455 · **class: adjacent**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-resolve's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a mistake told three times.
