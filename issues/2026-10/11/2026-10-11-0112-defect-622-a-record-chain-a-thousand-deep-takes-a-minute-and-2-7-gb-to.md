---
kind: defect
area: emit
milestone: none
filed: 2026-10-11
commit: none
github: none
---

- [ ] **622 — a record chain a thousand deep takes a minute and 2.7 GB to build under `--sanitize` at `-O2`** | defect 539's thousand-deep case, `run --sanitize` (a build at `-O2` under `-fsanitize=address,undefined`): in Ubuntu 24.04's clang 18.1.3 on arm64, the CI's own, in a container of four CPUs on this Mac, the build took 59.5 s and 2.77 GB peak, the 875-deep 51.3 s and 2.20 GB, against 21.5 s at `-O2` alone and 9.1 s under `--sanitize` at `-O0`, `build --sanitize`'s level (its 0.7 s on a second build was the cache's); Apple clang 21 on this Mac 35.7 s and 3.67 GB. `-ftime-report` of the unit's compile with the build's own words: 28.9 s of middle-end passes, JumpThreading 12.1 s, the inliner 8.8 s, EarlyCSE 3.7 s, AddressSanitizer itself 0.4 s; the rest is the backend. On the CI's arm64 runner, 2.24 times slower by the seed's build (21.7 s against 9.7 s), it was past the harness's watchdog of 120 s, which is defect 605, repaired by the watchdog's bound; the cost is this item | the per-type functions of a deep chain that `selfhost/emit/` writes past `typeorder`'s depth (defect 566 gave them `nodebug` and measured `noinline` worse at `-O2` alone, MemCpyOpt 86.5e9), and what ASan's instrumentation lets JumpThreading and the inliner do with them; the shapes beside: the 875 case, an array of the thousand-deep record, `-O1` under ASan · defects 539, 566, 605 · **class: improvement**

    **Origin:** measured by lane b20-tools at 01:03 on 2026-10-11 while repairing defect 605 (`.claude/worktrees/scratch-b15/b20-tools/d605b-c4/`, ignored by git: the build's `/usr/bin/time -v`, the unit's clang words read through a wrapper on `PATH`, and its `-ftime-report`).

    **Class: improvement**, 2026-10-11: nothing is wrong at exit 0; the program builds and prints `x` and `yz` in every configuration, and under defect 605's bound of 600 s by the measurement that set it (the CI's leg after the push is the judge, unrun). A build of a minute for a 4,028-line program is a cost, not a refusal (CLAUDE.md § 13), and no needed program waits on it.
