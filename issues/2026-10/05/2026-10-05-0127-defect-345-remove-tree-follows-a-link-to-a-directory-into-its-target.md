---
kind: defect
area: runtime
milestone: none
filed: 2026-10-05
commit: none
github: none
---

- [ ] **345 — `remove_tree` follows a link to a directory into its target and deletes the files there, answering success** | a tree `T` holding `link -> ../A`: removing `T` deleted `A/keep.txt`, outside the tree asked for, and answered success (lane b11-windows, this Mac, 2026-10-05, `<scratchpad>/batch11/windows/shapes/beside/rt1/probe.hero`); no caller's tree holds a link today | `runtime/parts/` (`hero_dir_remove_tree`, its walk) · `selfhost/cli/process.hero` (`remove_tree`, the compiler's callers) · **class: blocking**

    **Origin:** lane b11-windows, 2026-10-05, beside defect 238's R2, which made the three doors `remove_tree` uses wide (its final report, *Found beside*).

    **Class: blocking**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a delete outside the tree a program named, robustness's first case; never deferred.
