---
kind: defect
area: compiler
milestone: none
filed: 2026-10-03
commit: none
github: none
---

- [ ] **252 — what lld-link and a Windows `pkg-config` do with a `>`, a quote or a backslash in a `link` or `package` name, and with a `/` in a `link`, is unmeasured, and the rules admit them** | batch 8 refuses `*`, `<`, `?` and `|` in a group head's name for Windows, and the names NTFS stores as another file's (234, 235), judging all three strings, and admits `>`, `"` and `\` in a `link` or a `package` and `/` in a `link` (batch 8's FFI lane, 2026-10-03, its report's finding 4); `selfhost/head_windows.hero:21` says *What lld-link and a Windows `pkg-config` do with such names is unrun* | `selfhost/head_windows.hero` · `selfhost/head_names.hero` · the Windows box · **class: improvement**

    **Origin:** batch 8's FFI lane, 2026-10-03 (its report's finding 4).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a measurement owed on the box; no program measured wrong.
