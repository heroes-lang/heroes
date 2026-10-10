---
kind: defect
area: cli
milestone: none
filed: 2026-10-10
commit: 7799b74678bf7c45b9a53b913221aa783867cd24
github: none
---

- [ ] **562 — a header-only C99 `inline` makes `build` tell `ffi_missing_link` falsely while `run` exits 0** | the ffi-pragmatist's `c99`: a header defining a plain C99 `inline` function (neither `static` nor `extern`) builds at exit 1, `ffi_missing_link`, *no group says which library has it*, which is false, since at `-O0` C wants the one external definition the header does not give; `run`, at `-O2`, links and prints `6`; 264 such definitions in this Mac's installed headers, 146 of them raymath.h's | the reading of an undefined symbol at link, `selfhost/cli/` and `selfhost/emit/ffi*` · panel 202 · **class: blocking**

    **Origin:** filed by the coordinator at 00:25 on 2026-10-10 from panel 202 (`docs/panel/202-every-verb-cuts-a-program-s-c-by-module-and-what-two-modules-headers-disagree-on-is-told-before-the-link.md`): the ffi-pragmatist's case and census, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a false message, and `build` and `run` disagreeing on one program.

    Repaired at `7799b74678bf7c45b9a53b913221aa783867cd24`, 2026-10-10, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Each module's unit holds the address of every function it calls through a group of a header not the compiler's own, inside a kept function (`emit/extern_refs.hero`), so the link wants its external definition at `-O0` and `-O2` alike, and an undefined symbol whose header list gives it only a C99 inline definition is told on the binding (`cli/inline_only.hero`): *`twice` is defined only `inline` in `a.h` line 1, and nothing else defines it*, the note offering `static inline` or a library; `build`, `run` and `test` of the ffi-pragmatist's `c99` each exit 1 so. Case `unsupported/fixedbugs-562-*` and surface's row for `run`; unsupported 204, surface 402, warnings 494, emission 1,094, each 0 failed, the compiler's own tests 1,548 passed.
