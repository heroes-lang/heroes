---
kind: defect
area: compiler
milestone: none
filed: 2026-10-03
commit: c898bd944030d65afdd49feacf02bd1c467975ba
github: none
---

- [x] **236 — a used module that is present but unreadable is told that the file is not there** | `main.hero` over `use geom` and `print(geom.two())`, beside a `geom.hero` declaring `two` whose mode is 000: `check` exit 1, `unknown_module`, *there is no module `geom`, `use geom` reads `geom.hero`, from the directory of `main.hero`, the file you compile, and that file is not there*, while the file is there; the main file unreadable is *cannot read* at exit 2 (the trunk's compiler, from the seed at `dcaca1a3`, 2026-10-03, `<scratchpad>/batch8/source/shapes227/m17/`) | `selfhost/modules.hero:125` (every read failure taken as absence) · **class: blocking**

    **Origin:** lane b8-source beside defect 227, 2026-10-03, reported to the coordinator, who filed it the same day; the lane reproduced it on the trunk's compiler and restored the file's mode.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a false message; `read_file` answers `file_not_found` for a file it may not open, and the loader takes every failure to read for absence.

    **2026-10-03, lane b8-source, a module whose file is there and cannot be read stops the read as the root does, *cannot read* and the file at exit 2, and only a path where nothing is or can be stays absent** (`selfhost/module/reading.hero`; beside it a directory, a dangling link, a nested file, an unsearchable directory, a module two uses down and a module not UTF-8, the same; a path below a regular file and an absent module still `unknown_module`; a round of `check --apply` that cannot read writes nothing): repaired at `c898bd94`, gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `c898bd94`. A module whose file is there and cannot be read stops the read as an unreadable root does, *cannot read* and the file, and only a path where nothing is or can be stays absent (`selfhost/module/reading.hero`). Its test is what found defect 273 on Linux, repaired in the same batch.

**Closed 2026-10-04** after batch 8's platform legs ran its cases on the tree that closes (`7ec19cb7`, the code of the batch's closing commit): Linux arm64, Debian clang 22.1.8, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the same image under clang 18, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the Windows box, clang 23.1.1, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed (the folder removed from the box, 20 of 20 green). The batch's gate on this Mac is the closing commit's body and the thirteen records closed with it.
