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

    **Measured 2026-10-10 by lane b19-dep for panel 208's R3: no linker word drops this text and nothing else, so R3 does not land and the item stays open, the author's.** In `heroes-linux-arm64:latest` (Debian clang 22.1.8, GNU ld 2.44, LLD 22.1.8), a C program of one line calling `mktemp`, linked with `-fuse-ld=bfd`, prints *warning: the use of `mktemp' is dangerous, better use `mkstemp' or `mkdtemp'* at exit 0. Of eight words tried, `-Wl,-w` and `-Wl,--no-warnings` alone drop it, and in the same run they drop `--warn-common`'s *multiple common of `c'* too, on both linkers; GNU ld's own help calls `-w` *Do not display any warning or error messages*. `-Wl,--no-warn-mismatch`, `-Wl,--no-fatal-warnings` and `-Wl,-z,nognuwarning` leave GNU ld printing it; `--no-gnu-warning`, `--no-warn-gnu-warning` and `--no-warn-symbol` are refused by both linkers, exit 1. LLD 22.1.8 prints nothing for a `.gnu.warning` section, with no word. The CI's two Linux legs install `clang` from apt on `ubuntu-latest` and `ubuntu-24.04-arm` (`.github/workflows/ci.yml:316`) and nothing in `selfhost/` passes `-fuse-ld` on Linux, so they link with GNU ld; their binutils 2.42 was not run here, a question rather than a premise. Routes no linker word reaches, for the author: the compiler reading the link's own output, or `-fuse-ld=lld` on Linux.
