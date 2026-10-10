---
kind: defect
area: cli
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **572 — a package's `-D` reaches every module's unit, so another module's package decides a module's verdict** | panel 205's ffi-pragmatist and critic on Linux arm64: module `w` binds `wcwidth(c: i32)` from `wchar.h`; with no module naming `package "ncursesw"` it is refused *`wchar.h` declares no `wcwidth`*, false, and when `main` names `ncursesw` (whose `.pc` answers `-D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600`) the declaration is found and `w`'s `i32` refused `ffi_parameter_type` against `unsigned int`; a package's answer reaches every unit of the program (`selfhost/cli/produce.hero:142`) | `selfhost/cli/produce.hero` and the units' compile words; panel 205 R3 and R5, landing with R3 · **class: blocking**

    **Origin:** filed by the coordinator at 03:39 on 2026-10-10 from panel 205 (`docs/panel/205-a-library-s-own-header-code-is-judged-as-clang-judges-a-system-header-the-checks-raised-again-after-it-and-a-switch-goes-in-the-first-group.md`, R5), the seats' measurements, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a false message, and a module's verdict decided by another module's group.
