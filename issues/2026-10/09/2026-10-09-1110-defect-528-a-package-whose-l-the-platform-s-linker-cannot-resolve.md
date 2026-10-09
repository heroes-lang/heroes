---
kind: defect
area: cli
milestone: none
filed: 2026-10-09
commit: 19c8c453ee14447e8244d8fab360546048deac40
github: none
---

- [ ] **528 — a package whose `-l` the platform's linker cannot resolve makes `build` exit 2** | `libiodbc` (its `-liodbc` absent from the machine) and `libuv-static` (`-l:libuv.a`) exit 2, *internal error: linking failed*, under the frozen compiler of panel 198 and under its route K alike; defect 224 tells a `link` name the linker cannot find, and a package's `-l` is not told | the link step's reading of the linker's *missing library* line, `selfhost/cli/` · defect 224 · panel 198 · **class: blocking**

    **Origin:** filed by the coordinator at 11:10 on 2026-10-09 from panel 198's completeness critic, second pass (its report goes into the sitting's record with the synthesis, its probes are under `.claude/worktrees/scratch-b15/198-critic/probe/`, ignored by git); the seat's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told.

    Its `libuv-static` half is told since `4b956b61`, 2026-10-09 (lane b16-land198, panel 198's R2, ratified by the author that day; defect 527's repair): `-l:libuv.a` is refused at exit 1, *`-l:<file>` names a file, where `-l` names a library*, where it was exit 2 (measured on this Mac with a stand-in `pkg-config`, and asked of the filter by a compiler test). Its `libiodbc` half, a library the machine lacks, is not that route and keeps this item open.

    Repaired at `19c8c453`, 2026-10-09 (lane b16-land198), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Both halves are repaired now: a package's `-l` or `-framework` the linker reports missing is `ffi_missing_library` at exit 1, at the package's string, naming the package and the words its answer carried, asked after every `link` group (`selfhost/cli/package_missing.hero`, from `cli/whose.hero`), where `package "libiodbc"` on this Mac was exit 2, *internal error: linking failed*; with the `libuv-static` half of `4b956b61`, the Mac census of 501 packages reads no exit 2. Cases: a compiler test on the reader, over ld64's, GNU ld's and lld-link's wordings and ld64's framework one, and an `absence.hero` case over a `--libs` alone, one beside a `-L` and the split `-l <name>`, red with the base compiler; the compiler's own tests 1521 passed, the net's own tests 322.
