---
kind: defect
area: print
milestone: none
filed: 2026-10-04
commit: f38c25b25650ad0af550cf725115c4443b6a5864
github: none
---

- [ ] **333 — `heroes fmt` breaks an empty call as `.must(` over `)` on the next line, and the first line stays past the width** | a line past the width ending in an empty call, `x.must()`: `fmt` writes `.must(` and `)` on the line below, which makes nothing shorter; the result is stable under a second `fmt` and legal | `selfhost/print/fmt.hero` (the break inside a call's parentheses) · **class: improvement**

    **Origin:** lane b10-harness, 2026-10-04, measured on its worktree at `bd1f168c` (its final reply's *Found beside*); seen in its own files while formatting.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a canonical form less tidy than it could be; no meaning moves.

    Repaired at `f38c25b2`, 2026-10-07 (lane b14-text), gated by its cases and the compiler's own tests; the net is owed at the batch's close. An empty bracket with no comment in or before it is printed shut, its callee or receiver asked open as the value was, so the line breaks inside the nearest bracket before it that holds something, `).must()` under its arguments, or stays long: `fmt` over the tree's tracked files moves 32 files at 72 places, every one this shape, 29 of them files `canonical` holds, which the round reformats.
