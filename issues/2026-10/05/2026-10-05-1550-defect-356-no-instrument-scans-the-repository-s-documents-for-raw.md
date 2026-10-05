---
kind: defect
area: records
milestone: none
filed: 2026-10-05
commit: none
github: none
---

- [ ] **356 — no instrument scans the repository's documents for raw invisible characters, and the tool that writes them has put three there** | the file-writing tool turned a written backslash escape into a raw ZERO WIDTH SPACE twice in `docs/panel/192-reports/historian.md` and once in `completeness-critic.md`, each found only by a scan by hand (panel 192's historian and critic, 2026-10-05) | `tests/harness/suite_records.hero` (no check reads a document's characters) · panel 192's R2, the same characters refused in a program's source · **class: improvement**

    **Origin:** panel 192's historian and critic, 2026-10-05; filed by the synthesis's R12.

    **Class: improvement**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): hardening of the records; no program is wrong for it.
