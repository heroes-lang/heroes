---
kind: defect
area: cli
milestone: none
filed: 2026-10-04
commit: 8f5ceb39252e55758109a4892addeb731c318d32
github: none
---

- [x] **292 — `ffi_package` says a package is not installed when it is installed and only a package it requires is missing** | `u3.pc` installed, its `Requires:` naming a package no machine has: `build` is told *the package `u3` is not installed on this machine*, while `pkg-config`'s own words, now shown in the notes, say a package it requires is not found (lane b9-notext's compiler, 2026-10-04, `<scratchpad>/batch9/notext/sweep/241/pcbad/u3.pc`) | `selfhost/cli/libraries.hero` (the headline written for every failed answer) · **class: blocking**

    **Origin:** lane b9-notext, 2026-10-04, reproduced on its compiler (`<scratchpad>/batch9/notext/report.md`, *Found beside* 3).

    **Class: blocking**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a false message, a package installed told not installed.

    Repaired at `8f5ceb39`, 2026-10-04 (lane b10-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

    Repaired again at `72b6a6f7`, 2026-10-04 (lane b10-cli), on the coordinator's decision that pkg-config 0.29.2, which refuses `--maximum-traverse-depth`, must not fall back to *not installed*: installed is asked by two per-package questions and the search path, measured on pkgconf 3.0.7 and 1.8.1 and pkg-config 0.29.2; gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `8f5ceb39` and again at `72b6a6f7`. Every refusal by `pkg-config` was headed *the package is not installed on this machine*, where `pkg-config`'s own words said a package it requires was not found. Installed is now asked three ways, any one enough: `--maximum-traverse-depth=1 --modversion`, which pkgconf answers per package; a plain `--modversion`, which freedesktop's 0.29.2 answers per package; and a `.pc` of that name on the search path every `pkg-config` reports. Not installed is said only where all three say no. Measured through stand-ins forwarding to 0.29.2 and 1.8.1 in Linux containers and to this Mac's 3.0.7: a requirement missing, two deep, private or too old, installed on each; not there, not installed; a `.pc` with no `Version:`, installed. `;` parting the path on Windows is unmeasured: the box has no `pkg-config`. Its cases are a compiler test end to end against the machine's `pkg-config` and a test of the search path's reading.

**Closed 2026-10-05**, after batch 10's platform legs, each on `4c3524fb`, the batch's closing tree: Linux arm64 in its container under Debian clang 22.1.8 and again under 18.1.8, the compiler's own tests 1,213, all passed, and 22 suites, every one 0 failed, each time; the Windows box under clang 23.1.1, the compiler's own tests 1,213, all passed, and 21 of its 22 suites 0 failed in the leg's folder, `unsupported` reading 127 passed and 2 failed there, two `fixedbugs-157-*` cases told `internal error` over three NUL bytes, because the box's crash at 18:46 on 2026-10-04 had left 11 files of that folder's build cache as zeros (defect 357); `unsupported` from the same archive with the same `heroes.exe`, in a fresh folder, 129 passed and 0 failed.
