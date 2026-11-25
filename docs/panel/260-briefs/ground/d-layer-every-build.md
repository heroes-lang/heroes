---
kind: defect
area: cli
milestone: M-rebel-library
filed: 2026-11-25
commit: none
github: none
---

- [ ] **Every build asks all 43 C members of the platform layer what they reach, and a cold build compiles them all: `print(1)` builds ten times slower** | A build of `examples/gallery/00-first.hero` reads 0.25 s warm and 0.63 to 0.90 s cold with the compiler of 17c50eba6, 2.61 to 2.76 s warm and 23.9 to 24.5 s cold with the compiler of 0521cd903, on this Mac in the same minutes; the CI's three platforms run the net 2.5 to 3.3 times slower and Linux arm64 passed GitHub's 360-minute ceiling | `selfhost/cli/layer.hero`, `selfhost/cli/layer_reach.hero`, `selfhost/cli/opening.hero`, `runtime/runtime.c`'s layer block

    **Origin:** 2026-11-25, the coordinator, reading the CI run of the third push (run 45493660465, fc19cb341): job *Linux arm64* cancelled by the 360-minute ceiling inside *The net, in Heroes* (22:01:21Z to 03:43:55Z, every suite still advancing: no hang). Against run 45013609088 (17c50eba6, 2026-11-22), same leg: `run` 63.5 min for 833 cases then, 161 min for 1062 now (4.6 s a case, 9.1); `determinism` 21 min for 875, 81 for 1110; `emission` 10.4 min for 2794, 33.5 for 3317; `generated` 7.6 min then 17.1 for the same 133 cases. Darwin's net parts 1, 2 and 4 went from 63, 62 and 71 minutes to 211, 190 and 203; Windows' part 2 from 97 to 241. The CI's step *One program, compiled and run* (`./heroes run examples/gallery/00-first.hero`, cold) read 2.6 s on 2026-11-22 and 16.2 s on 2026-11-24. Logs: `.claude/worktrees/rebel-scratch/ci/linux-arm64-1122.log`, `linux-arm64-3.log`.

    **Measured on this Mac, 2026-11-25 05:35**, the machine NOT still (five lanes running, load 6.9 to 8.7, so the absolute numbers are loose and the ratio is what stands): seeds of 17c50eba6 and of 0521cd903 built with `clang -O2` in `.claude/worktrees/rebel-scratch/perf/old` and `perf/new`, each beside its own `runtime/`, three rounds interleaved, a fresh folder for each cold build: old cold 0.90, 0.63, 0.65 s, warm 0.25, 0.25, 0.26 s; new cold 24.46, 23.88, 24.14 s, warm 2.61, 2.66, 2.76 s. A warm build of the new compiler, with `clang` wrapped to log its calls (`perf/wrap/clang`, log `perf/clang.log`), calls clang 48 times: 43 of them are `-M` over one C member of `runtime/platform/` each (`build/layer-reached.d.*`), the reach `layer.hero` asks of each member at every build (defect 2289's question, `opening.reached`). `runtime/platform/` held 2 files at 17c50eba6 and holds 72 at 0521cd903; `runtime.c`'s block names 72 of them.

    **Recommendation, a question rather than a premise:** a build should cost what the program reaches, not what the layer holds: a program whose groups name no header of the layer asks no member and compiles none; one that reaches the clock asks and compiles the clock's closure. Whether that is the reach `layer_reach.hero` already computes for the Swift members (panel 253's R3, defect 2625), extended to the C members, or a cached reach the build re-validates by the bytes of the files it recorded, is for the lane to measure; the first changes panel 234's R6 (*the linker, not the compiler, takes the members*) and is a soundness-lane panel's.

    **Class: blocking**, 2026-11-25: a CI leg red (`.claude/rules/verification.md` § The optimistic chain) and every program built ten times slower (CLAUDE.md § Verification, *never slow the compiler down*).
