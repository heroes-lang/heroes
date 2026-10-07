---
kind: defect
area: resolve
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **501 — a program's own `validated` replaces the library's in silence** | panel 162's text says `validated` moves into the table of built-ins; it is still on `resolve/builtin_names.hero`'s exception list, so a program's own `validated` silently replaces the library's in its module; reserving it needs `emit/bytes_text.hero`'s own `validated` renamed (lane b14-resolve) | `selfhost/resolve/builtin_names.hero`, `selfhost/emit/bytes_text.hero` · defect 455 · panel 162 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-resolve's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a rule the compiler enforces missing one name a sitting named.
