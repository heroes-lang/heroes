---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: 32356d0e549c7e16134495d1e9746f450ae3b723
github: none
---

- [ ] **461 — rewriting a file on Windows drops its owner, its explicit ACL entries and its named streams** | the compiler's publish route on Windows (`fmt --in-place` among its users) replaces a file without carrying its owner, explicit ACL entries or named streams, a downloaded file's mark of the web among them; defect 136's closing recorded it *without a case* (`issues/2026-10/01/2026-10-01-0012-defect-136-*.md:123`); batch 14's `write_file` keeps writing in place over an existing file on Windows for the same reason (lane b14-runtime) | `runtime/parts/replace.c`, `runtime/parts/write.c`, the Windows route · defects 136 and 438 · **class: adjacent**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-runtime's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a security property lost in silence, on one platform; a measurement on the box owed (`ReplaceFileW`, or the copy created with the original's security descriptor).

    Repaired at `32356d0e`, 2026-10-09 (lane b15-runtime, batch 16), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Measured on the box (clang 23.1.1, NTFS): the publish route lost a file's explicit ACE, its `Zone.Identifier` stream and its owner, and `write_file` over a hidden file holding them failed in place. The stage is now created under the file's own owner, group and DACL and put in place by `ReplaceFileW`, which keeps the DACL, the streams and the attributes, and `write_file` takes that route over a file already there, so a write cut short leaves the old text on Windows as defect 438 made it on POSIX; the runtime asks for advapi32 itself, without which every program's link failed. Cases `run/fixedbugs-461-*`, two, red on the base on the box and green at every level the run suite builds. Cost there, a write over a file already there: about 3.6 ms where it was 0.18 at 1 KiB, the flush 2.3 ms of it. Not run: an owner this process may not assign (no second account on the box).
