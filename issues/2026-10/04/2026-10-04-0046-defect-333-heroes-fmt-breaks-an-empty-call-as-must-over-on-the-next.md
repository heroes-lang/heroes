---
kind: defect
area: print
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **333 — `heroes fmt` breaks an empty call as `.must(` over `)` on the next line, and the first line stays past the width** | a line past the width ending in an empty call, `x.must()`: `fmt` writes `.must(` and `)` on the line below, which makes nothing shorter; the result is stable under a second `fmt` and legal | `selfhost/print/fmt.hero` (the break inside a call's parentheses) · **class: improvement**

    **Origin:** lane b10-harness, 2026-10-04, measured on its worktree at `bd1f168c` (its final reply's *Found beside*); seen in its own files while formatting.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a canonical form less tidy than it could be; no meaning moves.
