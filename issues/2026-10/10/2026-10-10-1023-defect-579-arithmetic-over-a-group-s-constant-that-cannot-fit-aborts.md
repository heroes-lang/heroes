---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **579 — arithmetic over a group's constant that cannot fit aborts at run time, its value known only to clang and the platform** | panel 206's ffi-pragmatist: `x: i32 = PATH_MAX * 600000` builds and prints `614400000` on this Mac (`PATH_MAX` 1024) and aborts 134 on Linux arm64 (4096); `RAND_MAX + 1` and `INT64_MAX + 1` abort 134 everywhere; a build probe of one `__int128` `_Static_assert` per step built only from constants refuses at the `.hero` line on exactly the platform where the run aborts (inserted by hand into two units, Windows unrun) | the emitted probes of a group's constants, `selfhost/emit/`; panel 206 R4, for a sitting of its own · **class: improvement**

    **Origin:** filed by the coordinator at 10:23 on 2026-10-10 from panel 206 (`docs/panel/206-a-constant-s-written-body-that-cannot-be-computed-is-a-compile-error-and-arithmetic-in-a-function-still-aborts-where-it-runs.md`, R4), the ffi-pragmatist's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): the program aborts cleanly where the value does not fit; a compile-time refusal is hardening, an emitter change for a sitting.
