---
kind: defect
area: harness
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **486 — the map command misses `tests/harness`, which `canonical` walks** | `canonical`'s `SOURCE_DIRS` walks `tests/harness`, the map command does not search for it, so the `tests/harness/**` row runs one suite too few (lane b14-hooks; `grep -n '"tests/harness"' tests/harness/suite_canonical.hero`) | `.claude/rules/verification.md` § The map · defect 288 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-hooks's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a map running one suite too few.
