---
kind: defect
area: cli
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **528 — a package whose `-l` the platform's linker cannot resolve makes `build` exit 2** | `libiodbc` (its `-liodbc` absent from the machine) and `libuv-static` (`-l:libuv.a`) exit 2, *internal error: linking failed*, under the frozen compiler of panel 198 and under its route K alike; defect 224 tells a `link` name the linker cannot find, and a package's `-l` is not told | the link step's reading of the linker's *missing library* line, `selfhost/cli/` · defect 224 · panel 198 · **class: blocking**

    **Origin:** filed by the coordinator at 11:10 on 2026-10-09 from panel 198's completeness critic, second pass (its report goes into the sitting's record with the synthesis, its probes are under `.claude/worktrees/scratch-b15/198-critic/probe/`, ignored by git); the seat's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told.

    Its `libuv-static` half is told since `4b956b61`, 2026-10-09 (lane b16-land198, panel 198's R2, ratified by the author that day; defect 527's repair): `-l:libuv.a` is refused at exit 1, *`-l:<file>` names a file, where `-l` names a library*, where it was exit 2 (measured on this Mac with a stand-in `pkg-config`, and asked of the filter by a compiler test). Its `libiodbc` half, a library the machine lacks, is not that route and keeps this item open.
