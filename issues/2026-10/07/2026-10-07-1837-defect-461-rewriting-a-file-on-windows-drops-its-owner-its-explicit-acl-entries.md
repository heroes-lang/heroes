---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **461 — rewriting a file on Windows drops its owner, its explicit ACL entries and its named streams** | the compiler's publish route on Windows (`fmt --in-place` among its users) replaces a file without carrying its owner, explicit ACL entries or named streams, a downloaded file's mark of the web among them; defect 136's closing recorded it *without a case* (`issues/2026-10/01/2026-10-01-0012-defect-136-*.md:123`); batch 14's `write_file` keeps writing in place over an existing file on Windows for the same reason (lane b14-runtime) | `runtime/parts/replace.c`, `runtime/parts/write.c`, the Windows route · defects 136 and 438 · **class: adjacent**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-runtime's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a security property lost in silence, on one platform; a measurement on the box owed (`ReplaceFileW`, or the copy created with the original's security descriptor).
