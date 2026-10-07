---
kind: defect
area: harness
milestone: none
filed: 2026-10-07
commit: 525ad9d785b78d298b6fdc7ea30c192713f0e7c8
github: none
---

- [ ] **485 — a narrowed `annotations` or `fixes` cannot select a surface-fixtures group** | it exits 2, so a fixture's marks are judged only by the whole suite (lane b14-harness-b) | `tests/harness/suite_annotations.hero`, `suite_fixes.hero` · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-harness-b's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach.

    Repaired at `525ad9d7`, 2026-10-08 (lane b15-harness), gated by its cases and the net's own tests; the net is owed at the batch's close. Reproduced on the base for five words, each exiting 2 (`annotations qualifytypo`, `fixes qualifytypo`, `annotations ffi-call-arity`, `annotations shape074`, `fixes applyx`); a narrowed `annotations` now asks each run-root group whose directory's name or one of whose files' names holds the word, and a narrowed `fixes` each such fixture case, whole, as the whole suite does: the five read 1, 2, 1, 1 and 2 passed, 0 failed, and the whole suites 882 and 883 passed, 0 failed.
