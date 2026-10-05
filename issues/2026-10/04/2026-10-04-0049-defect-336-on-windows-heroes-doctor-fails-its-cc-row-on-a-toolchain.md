---
kind: defect
area: cli
milestone: none
filed: 2026-10-04
commit: 20b55d02f4adc2c63f85d76dc743eeb378f44ab9
github: none
---

- [x] **336 — on Windows `heroes doctor` fails its `cc` row on a toolchain that builds and runs programs, and its advice is for Linux** | panel 191's compiler-engineer on the Windows box (clang 23.1.1, no `cc` on the path), 2026-10-04: `heroes doctor` exits 2 with `FAIL cc not found — install a C toolchain (build-essential, base-devel, …)` under the trunk's compiler of `7f4c0cc5` and under the sitting's route alike, on the machine where `heroes build` builds and runs a program (its rows 8, 11 and 12); the row is asked of every system whose `uname -s` does not answer `Darwin`, so Windows is asked Linux's question and handed Linux's advice, and the CI's Windows leg reads it green only because its image carries MinGW-Builds gcc 15.2.0 as `cc`, a compiler Heroes does not use there (run 37196853219's log, *ok cc ... MinGW-Builds ... 15.2.0*, the critic's reading) | `selfhost/cli/doctor.hero:108` to `:119` (the branch on `uname -s`, the `cc` row and its advice) · `docs/panel/191-reports/compiler-engineer.md` (the row table and the sentence under it) · `docs/panel/191-reports/completeness-critic.md` § 6, item 4 · panel 191's R8 · **class: blocking**

    **Origin:** panel 191's compiler-engineer, 2026-10-04, on the Windows box, set outside the sitting's question; named by the critic's second pass as filed nowhere (`git grep -i doctor` over `docs/work/defects/` finding only 243); filed with the sitting's synthesis.

    **Class: blocking**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a false message, *cc not found* with exit 2 on a machine whose toolchain works, and advice for another platform; the CI's green on it is its image's, not the tool's.

    **2026-10-04:** Repaired at `20b55d02`, gated by its cases and the compiler's own tests on this Mac (1,214, all passed); the net is owed at the batch's close, and the Windows box owes the row red at the base and green with the repair.

## The repair

Repaired at `20b55d02`. On Windows `heroes doctor` asked for a `cc` it never uses and gave Linux's advice; it now asks the runtime whether this is Windows (`exe_suffix()`, as `link_flags()` does), and Windows has a row of its own, clang compiling, linking and running a small C program, a refusal quoting clang's first line with the Build Tools advice; macOS and Linux keep their rows. Its cases are the compiler's own tests.

**Closed 2026-10-05**, after batch 11's platform legs, each on `2dd5611c`, its tree as pushed at `61e085ae` but for records: Linux arm64 in its container under Debian clang 22.1.8 and again under 18.1.8, the compiler's own tests 1,233, all passed, and 22 suites, 5,244 passed and 0 failed, each time; the Windows box under clang 23.1.1, lane b11-windows' three trees, each with its compiler built from the seed and then from `selfhost/`: in `land`, the round's own runtime, the net's own tests 280, all passed, the compiler's own tests 1,233 with one failed, defect 337's, and 21 of 22 suites 0 failed, `unsupported` 130 passed and 1 failed, 337's other case; in `base`, the runtime as it stood at `7c615049`, the compiler's own tests failing 337's, 345's, 346's and 239's, and the net's own failing 12, 238's eleven and 239's walk; `heroes doctor` exit 0 in `land` with its MSVC row, where batch 10's compiler on the same box says *FAIL cc not found* with Linux's advice at exit 2. The CI after the push, at `61e085ae`: Darwin arm64, Linux arm64 and Linux x86-64 green; Windows x86-64 one of the net's own tests failed, defect 238's W16 and W21, its fixture's `clang -shared` exiting 1120 under clang 20.1.8, a row of 238, which stays open.
