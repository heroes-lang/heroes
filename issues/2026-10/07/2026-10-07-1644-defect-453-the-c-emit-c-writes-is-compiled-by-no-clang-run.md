---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **453 — the C `--emit-c` writes is compiled by no clang run** | `write_c` runs only for `--emit-c`, whose program build compiles module by module, and only `heroes test` compiles a fused unit, so no clang reads the artifact; clang found nothing in the artifacts of all 372 run goldens, and an `-fsyntax-only` pass would cost about 0.21 billion instructions on a small program and 20.9 billion on the seed (lane b14-cli's measurements, not re-run by the coordinator) | `selfhost/cli/artifact.hero` · defect 434 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-cli's final report (*decisions* 1, the lane's recommendation: ask clang `-fsyntax-only` on the artifact before writing it).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): an artifact nobody has checked, every one clean today; it changes what the tool does, so a sitting's question.
