---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **566 — records a thousand deep holding a `str` take past the watchdog to build at `-O2` on Windows, and the CI's Windows leg is red** | batch 17's push (`65b78f2e`), run 37993565819: the Windows x86-64 leg reads 7,143 passed and 4 failed, `run` and `warnings` on `fixedbugs-539-records-875-deep-holding-a-str-build` and `fixedbugs-539-records-a-thousand-deep-holding-a-str-build` at `-O2`, each `build` ending at exit 124, the harness's watchdog of 120 s (`tests/harness/shell.hero:283`); on this Mac at 00:37, load 3.35, the thousand-deep case builds in 21.79 s at `-O2` against 2.74 s at `-O0`, `real` near `user`, and its C is 4,599,048 bytes against 3,742,441 at 875 deep, 1.23 times the bytes for 1.14 times the depth | the C defect 539's repair emits for a deep record's slots and their release, `selfhost/emit/`, and what clang's optimiser does with it; the shapes beside: the 875 case, an `i64` chain at 7,000 deep, `-O1`, a record held in an array · defect 539 · **class: blocking**

    **Origin:** filed by the coordinator at 00:39 on 2026-10-10 from the CI's Windows x86-64 leg of batch 17's push, its job log read through the API (kept under `.claude/worktrees/scratch-b15/gate17/post/ci-win.log`, ignored by git), as `.claude/rules/verification.md` § The optimistic chain item 5 asks; the Mac's two builds and the emitted C's sizes measured by the coordinator with the trunk's compiler built from the seed at `86189b2b`.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a red CI, and a program that builds on one platform and not, within the bound, on another.
