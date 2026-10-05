---
kind: defect
area: cli
milestone: none
filed: 2026-10-04
commit: 20b55d02f4adc2c63f85d76dc743eeb378f44ab9
github: none
---

- [ ] **336 — on Windows `heroes doctor` fails its `cc` row on a toolchain that builds and runs programs, and its advice is for Linux** | panel 191's compiler-engineer on the Windows box (clang 23.1.1, no `cc` on the path), 2026-10-04: `heroes doctor` exits 2 with `FAIL cc not found — install a C toolchain (build-essential, base-devel, …)` under the trunk's compiler of `7f4c0cc5` and under the sitting's route alike, on the machine where `heroes build` builds and runs a program (its rows 8, 11 and 12); the row is asked of every system whose `uname -s` does not answer `Darwin`, so Windows is asked Linux's question and handed Linux's advice, and the CI's Windows leg reads it green only because its image carries MinGW-Builds gcc 15.2.0 as `cc`, a compiler Heroes does not use there (run 37196853219's log, *ok cc ... MinGW-Builds ... 15.2.0*, the critic's reading) | `selfhost/cli/doctor.hero:108` to `:119` (the branch on `uname -s`, the `cc` row and its advice) · `docs/panel/191-reports/compiler-engineer.md` (the row table and the sentence under it) · `docs/panel/191-reports/completeness-critic.md` § 6, item 4 · panel 191's R8 · **class: blocking**

    **Origin:** panel 191's compiler-engineer, 2026-10-04, on the Windows box, set outside the sitting's question; named by the critic's second pass as filed nowhere (`git grep -i doctor` over `docs/work/defects/` finding only 243); filed with the sitting's synthesis.

    **Class: blocking**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a false message, *cc not found* with exit 2 on a machine whose toolchain works, and advice for another platform; the CI's green on it is its image's, not the tool's.

    **2026-10-04:** Repaired at `20b55d02`, gated by its cases and the compiler's own tests on this Mac (1,214, all passed); the net is owed at the batch's close, and the Windows box owes the row red at the base and green with the repair.
