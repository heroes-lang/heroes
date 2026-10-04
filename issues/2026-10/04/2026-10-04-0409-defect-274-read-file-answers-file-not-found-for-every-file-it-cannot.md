---
kind: defect
area: module
milestone: none
filed: 2026-10-04
commit: 34836e7343704a9544f128a245492c8b445db1c7
github: none
---

- [x] **274 — `read_file` answers `file_not_found` for every file it cannot open, so a file that is there and may not be read is told *no file at* its path** | `read_file("locked.txt")` over a file of mode 000, run as an ordinary user: `.err` `file_not_found`, *no file at locked.txt*, at exit 0 (batch 8's round compiler, this Mac, 2026-10-04, `<scratchpad>/filings-b8/probe/rd/locked.hero`); defect 236's lane measured the same on 2026-10-03 for a file in a directory it may not search (`selfhost/module/reading.hero`'s doc); on Windows `fopen` of a directory fails `EACCES`, so defect 273's case would read `file_not_found` there | `runtime/parts/os.c` (`hero_file_read`: every `fopen` failure is `HERO_OS_NOT_FOUND`, `errno` unread) · the library's `read_file` (`selfhost/library_source.hero`), whose doc names three program states, *there is no such file*, *the disk failed* and *it read, and it is not text* · **class: blocking**

    **Origin:** the coordinator, 2026-10-04, repairing defect 273 in the same function; defect 236's lane had measured the shape on 2026-10-03 and answered it in the compiler alone (`selfhost/module/reading.hero` asks the filesystem when a read fails).

    **Class: blocking**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a false message, *no file at* a path where a file is, in every program that calls `read_file`.

## The repair

Repaired at `34836e73`. `read_file` says `file_not_found` only where nothing is at the path, `ENOENT` or `ENOTDIR`, and `read_failed` for every other failure. Case: the compiler test *a file that is there and may not be read is read_failed, never file_not_found*; defect 273's case on Windows.

**Closed 2026-10-04** after batch 8's platform legs ran its cases on the tree that closes (`7ec19cb7`, the code of the batch's closing commit): Linux arm64, Debian clang 22.1.8, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the same image under clang 18, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the Windows box, clang 23.1.1, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed (the folder removed from the box, 20 of 20 green). The batch's gate on this Mac is the closing commit's body and the thirteen records closed with it.
