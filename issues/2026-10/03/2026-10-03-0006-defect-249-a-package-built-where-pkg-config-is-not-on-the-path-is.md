---
kind: defect
area: cli
milestone: none
filed: 2026-10-03
commit: c0b4de707c1ca691a67bca927f5b9c9430bb89ad
github: none
---

- [x] **249 — a `package` built where `pkg-config` is not on the PATH is answered in a bare line, with no code, no place and no route** | `extern "zlib.h" package "zlib"` over `constant Z_OK: i64`, `build` with `PATH=/usr/bin:/bin`: exit 1 and the one line *pkg-config is not on this machine, and `package "zlib"` asks it where the library is*, with no `error[...]`, no `at` and no way out named (install `pkg-config`, or name the library with `link`) (batch 8's round compiler at `1eb854c3`, this Mac, 2026-10-04, `<scratchpad>/filings-b8/probe/nopc.hero`); the Windows box has no `pkg-config` (batch 8's FFI lane) | `selfhost/cli/libraries.hero:311` (the message) · `tests/harness/shell.hero:582`, which reads the same words as a machine lacking the library · **class: adjacent**

    **Origin:** batch 8's FFI lane, 2026-10-03 (its report's finding 2); reproduced by the coordinator, 2026-10-04.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than design.md §4.17 asks: no code, no place, no route.

    Repaired at `c0b4de70`, 2026-10-04 (lane b10-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The harness half this item names, `shell.machine_lacks_the_library` at `tests/harness/shell.hero:582`, was removed by defect 246's repair (`9de942c2`), and `tests/harness/absence.hero` reads the repaired message as it read the bare line.

## The repair

Repaired at `c0b4de70`. `build`, `run` and `test` of a `package` group where `pkg-config` is not on the PATH answered one bare line at exit 1, no code, no place, no way out. The refusal now opens with the package marker, so it is the group's `ffi_package` at its string, with the way out: install pkg-config, or name the library directly with `link`; after the colon it keeps the old sentence word for word, which `tests/harness/absence.hero` reads as a machine without `pkg-config`. Measured with `PATH=/usr/bin:/bin` on this Mac, as the Windows box stands: `unsupported` whole 133 passed, 0 failed, four cases skipped by that reading. The harness half the item named went with defect 246 (`9de942c2`). Its cases are two compiler tests: the message read into the diagnostic, and the refusal where no `pkg-config` starts.

**Closed 2026-10-05**, after batch 10's platform legs, each on `4c3524fb`, the batch's closing tree: Linux arm64 in its container under Debian clang 22.1.8 and again under 18.1.8, the compiler's own tests 1,213, all passed, and 22 suites, every one 0 failed, each time; the Windows box under clang 23.1.1, the compiler's own tests 1,213, all passed, and 21 of its 22 suites 0 failed in the leg's folder, `unsupported` reading 127 passed and 2 failed there, two `fixedbugs-157-*` cases told `internal error` over three NUL bytes, because the box's crash at 18:46 on 2026-10-04 had left 11 files of that folder's build cache as zeros (defect 357); `unsupported` from the same archive with the same `heroes.exe`, in a fresh folder, 129 passed and 0 failed.
