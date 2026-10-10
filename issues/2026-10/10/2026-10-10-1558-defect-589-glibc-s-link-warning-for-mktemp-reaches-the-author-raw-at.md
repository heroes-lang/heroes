---
kind: defect
area: cli
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **589 — glibc's link warning for `mktemp` reaches the author raw at the emitted C's path** | on Linux arm64 a program binding and calling `mktemp` builds at exit 0 and prints `ld`'s *`.../mktemp.c:89:(.text+0x1c): warning: the use of 'mktemp' is dangerous, better use 'mkstemp' or 'mkdtemp'`*, naming the emitted C, not the `.hero` line; glibc's header marks nothing, so no clang group reaches it (the seat's `c/linux_heroes.txt`) | the link step's output, `selfhost/cli/link.hero`; panel 208 decides whether the deprecation's verdict reaches it, landing with defect 584 · **class: blocking**

    **Origin:** found by panel 208's ffi-pragmatist beside defect 584 (`docs/panel/208-reports/ffi-pragmatist.md` § 3b, its runs under `.claude/worktrees/scratch-b15/208-ffi-pragmatist/`, ignored by git), filed by the coordinator at 15:58 on 2026-10-10, the seat's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a tool's raw warning on a correct program, at a path the author did not write.
