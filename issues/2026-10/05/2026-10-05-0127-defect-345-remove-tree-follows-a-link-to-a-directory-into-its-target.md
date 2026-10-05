---
kind: defect
area: runtime
milestone: none
filed: 2026-10-05
commit: 8ee8d53c874d9464088e9a5099727ca836fba0f1
github: none
---

- [x] **345 — `remove_tree` follows a link to a directory into its target and deletes the files there, answering success** | a tree `T` holding `link -> ../A`: removing `T` deleted `A/keep.txt`, outside the tree asked for, and answered success (lane b11-windows, this Mac, 2026-10-05, `<scratchpad>/batch11/windows/shapes/beside/rt1/probe.hero`); no caller's tree holds a link today | `runtime/parts/` (`hero_dir_remove_tree`, its walk) · `selfhost/cli/process.hero` (`remove_tree`, the compiler's callers) · **class: blocking**

    **Origin:** lane b11-windows, 2026-10-05, beside defect 238's R2, which made the three doors `remove_tree` uses wide (its final report, *Found beside*).

    **Class: blocking**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a delete outside the tree a program named, robustness's first case; never deferred.

    **2026-10-05:** Repaired at `8ee8d53c`, gated by its case red first (1,217 tests, 1 failed) and the compiler's own tests on this Mac (1,217, all passed) and on Linux arm64 (1,218, all passed at `805bae19`); the net is owed at the batch's close, and the Windows box its case.

## The repair

Repaired at `8ee8d53c`. `remove_tree` followed a link to a directory into its target and deleted the files there, answering success; removal no longer follows links, a link inside the tree removed as the link and a path that is itself a link removed alone, as `rm -rf` does; on Windows a link is detected by the check `replace.c` used, moved into `fs.c`. Its case is a compiler test making a link to a directory and one to a file inside a tree; the reproducer now keeps `A/keep.txt`.

**Closed 2026-10-05**, after batch 11's platform legs, each on `2dd5611c`, its tree as pushed at `61e085ae` but for records: Linux arm64 in its container under Debian clang 22.1.8 and again under 18.1.8, the compiler's own tests 1,233, all passed, and 22 suites, 5,244 passed and 0 failed, each time; the Windows box under clang 23.1.1, lane b11-windows' three trees, each with its compiler built from the seed and then from `selfhost/`: in `land`, the round's own runtime, the net's own tests 280, all passed, the compiler's own tests 1,233 with one failed, defect 337's, and 21 of 22 suites 0 failed, `unsupported` 130 passed and 1 failed, 337's other case; in `base`, the runtime as it stood at `7c615049`, the compiler's own tests failing 337's, 345's, 346's and 239's, and the net's own failing 12, 238's eleven and 239's walk; `heroes doctor` exit 0 in `land` with its MSVC row, where batch 10's compiler on the same box says *FAIL cc not found* with Linux's advice at exit 2. The CI after the push, at `61e085ae`: Darwin arm64, Linux arm64 and Linux x86-64 green; Windows x86-64 one of the net's own tests failed, defect 238's W16 and W21, its fixture's `clang -shared` exiting 1120 under clang 20.1.8, a row of 238, which stays open.
