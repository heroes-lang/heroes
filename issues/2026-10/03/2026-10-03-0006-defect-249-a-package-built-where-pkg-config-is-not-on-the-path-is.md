---
kind: defect
area: cli
milestone: none
filed: 2026-10-03
commit: c0b4de707c1ca691a67bca927f5b9c9430bb89ad
github: none
---

- [ ] **249 — a `package` built where `pkg-config` is not on the PATH is answered in a bare line, with no code, no place and no route** | `extern "zlib.h" package "zlib"` over `constant Z_OK: i64`, `build` with `PATH=/usr/bin:/bin`: exit 1 and the one line *pkg-config is not on this machine, and `package "zlib"` asks it where the library is*, with no `error[...]`, no `at` and no way out named (install `pkg-config`, or name the library with `link`) (batch 8's round compiler at `1eb854c3`, this Mac, 2026-10-04, `<scratchpad>/filings-b8/probe/nopc.hero`); the Windows box has no `pkg-config` (batch 8's FFI lane) | `selfhost/cli/libraries.hero:311` (the message) · `tests/harness/shell.hero:582`, which reads the same words as a machine lacking the library · **class: adjacent**

    **Origin:** batch 8's FFI lane, 2026-10-03 (its report's finding 2); reproduced by the coordinator, 2026-10-04.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than design.md §4.17 asks: no code, no place, no route.

    Repaired at `c0b4de70`, 2026-10-04 (lane b10-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The harness half this item names, `shell.machine_lacks_the_library` at `tests/harness/shell.hero:582`, was removed by defect 246's repair (`9de942c2`), and `tests/harness/absence.hero` reads the repaired message as it read the bare line.
