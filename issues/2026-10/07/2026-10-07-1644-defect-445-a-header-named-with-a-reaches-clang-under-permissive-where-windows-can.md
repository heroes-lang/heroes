---
kind: defect
area: compiler
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **445 — a header named with a `"` reaches clang under `--permissive`, where Windows can hold no such file** | `undefined_header_name` is a thesis rule, so `check --permissive` and a permissive build let a header name holding `"` through to clang (lane b14-box, 2026-10-07) | `selfhost/head_names.hero`, `selfhost/head_windows.hero` · defect 252 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-box's final report (*found beside* 2).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): an edge under `--permissive`, where the thesis rules step aside by design; whether this one should not is a sitting's.
