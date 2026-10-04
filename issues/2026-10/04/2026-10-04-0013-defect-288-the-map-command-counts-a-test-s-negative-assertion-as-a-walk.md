---
kind: defect
area: golden
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **288 — the map command counts a test's negative assertion as a walk, so the published rows name suites that walk nothing there** | `.claude/rules/verification.md`'s map command greps each suite file for a path's literal, so a suite whose test asserts it does NOT walk a directory is listed as walking it: the rows for `tests/golden/check/**` and `tests/golden/unsupported/**` name `canonical`, which walks neither (lane b9-harness, 2026-10-04, a reading) | `.claude/rules/verification.md` § The map is not written here · **class: improvement**

    **Origin:** lane b9-harness, 2026-10-04 (its reply's *found beside*).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a map over-listing, which runs a suite too many and never one too few; no program moves.
