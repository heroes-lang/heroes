---
kind: defect
area: runtime
milestone: none
filed: 2026-10-05
commit: 8ee8d53c874d9464088e9a5099727ca836fba0f1
github: none
---

- [ ] **345 — `remove_tree` follows a link to a directory into its target and deletes the files there, answering success** | a tree `T` holding `link -> ../A`: removing `T` deleted `A/keep.txt`, outside the tree asked for, and answered success (lane b11-windows, this Mac, 2026-10-05, `<scratchpad>/batch11/windows/shapes/beside/rt1/probe.hero`); no caller's tree holds a link today | `runtime/parts/` (`hero_dir_remove_tree`, its walk) · `selfhost/cli/process.hero` (`remove_tree`, the compiler's callers) · **class: blocking**

    **Origin:** lane b11-windows, 2026-10-05, beside defect 238's R2, which made the three doors `remove_tree` uses wide (its final report, *Found beside*).

    **Class: blocking**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a delete outside the tree a program named, robustness's first case; never deferred.

    **2026-10-05:** Repaired at `8ee8d53c`, gated by its case red first (1,217 tests, 1 failed) and the compiler's own tests on this Mac (1,217, all passed) and on Linux arm64 (1,218, all passed at `805bae19`); the net is owed at the batch's close, and the Windows box its case.
