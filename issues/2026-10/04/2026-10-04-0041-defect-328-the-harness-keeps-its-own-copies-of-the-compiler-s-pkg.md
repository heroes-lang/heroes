---
kind: defect
area: harness
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **328 — the harness keeps its own copies of the compiler's pkg-config spellings, and three of them now lag the compiler** | `absence.notes_of` joins pkg-config's notes in the compiler's old order, trim then split, which defect 285 changed to split then trim; `suite_golden.hero:702` and `absence.hero:849` plant defect 249's old bare sentence; and defect 292's new headline, *is installed, and pkg-config cannot give its flags*, is no spelling the harness reads, so a case whose package is installed and broken would read red rather than be stepped aside by name (no leg is known to hold one) | `tests/harness/absence.hero` (`notes_of`, `:849`) · `tests/harness/suite_golden.hero:702` · defects 249, 285 and 292 · **class: improvement**

    **Origin:** lane b10-cli, 2026-10-04, reproduced on its compiler at `86b29733` (its final reply's *Found beside*); its proposed `surface` rows for 290, 295 and 248 are part of this item's remedy.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): hardening of the net: no case reads wrong today.
