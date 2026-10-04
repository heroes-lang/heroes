---
kind: defect
area: harness
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **330 — the net's own test that the captures are removed can never fail: it looks for whole paths with the prefix `stdout-`** | `shell.hero`'s test *the captures are removed ...* compares each listed path, whole, with the prefix `stdout-`; with two capture files planted it counts 0 and passes; the `captures_left` helper lane b10-harness added reads them right | `tests/harness/shell.hero` (the test) · defect 320's capture names · **class: improvement**

    **Origin:** lane b10-harness, 2026-10-04, measured on its worktree at `bd1f168c` (its final reply's *Found beside*).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): an instrument that cannot fire, hardening of the net.
