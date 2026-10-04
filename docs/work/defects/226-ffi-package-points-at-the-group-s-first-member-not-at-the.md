- [ ] **226 — `ffi_package` points at the group's first member, not at the `package` string it is about** | `extern "ab.h" package "zz9nothere"` over `function seven() -> i32`: `build` 1, *the package `zz9nothere` is not installed on this machine*, `at p.hero:2:5`, the member's line, where the string is on line 1 (the trunk's compiler at `826ddc2f`, 2026-10-03, `<scratchpad>/repro188/pkg-span/`) | `selfhost/emit/ffi_build.hero` (where `ffi_package` takes its span) · **class: adjacent**

    **Origin:** panel 188's spec-warden, 2026-10-03; reproduced by the coordinator the same day.

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    **2026-10-03, batch 8's FFI lane**: repaired at `b9c4bda0`, gated by its cases and the compiler's own tests; the net is owed at the batch's close.
