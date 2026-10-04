- [ ] **292 — `ffi_package` says a package is not installed when it is installed and only a package it requires is missing** | `u3.pc` installed, its `Requires:` naming a package no machine has: `build` is told *the package `u3` is not installed on this machine*, while `pkg-config`'s own words, now shown in the notes, say a package it requires is not found (lane b9-notext's compiler, 2026-10-04, `<scratchpad>/batch9/notext/sweep/241/pcbad/u3.pc`) | `selfhost/cli/libraries.hero` (the headline written for every failed answer) · **class: blocking**

    **Origin:** lane b9-notext, 2026-10-04, reproduced on its compiler (`<scratchpad>/batch9/notext/report.md`, *Found beside* 3).

    **Class: blocking**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a false message, a package installed told not installed.

    Repaired at `8f5ceb39`, 2026-10-04 (lane b10-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
