---
kind: defect
area: cli
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **562 — a header-only C99 `inline` makes `build` tell `ffi_missing_link` falsely while `run` exits 0** | the ffi-pragmatist's `c99`: a header defining a plain C99 `inline` function (neither `static` nor `extern`) builds at exit 1, `ffi_missing_link`, *no group says which library has it*, which is false, since at `-O0` C wants the one external definition the header does not give; `run`, at `-O2`, links and prints `6`; 264 such definitions in this Mac's installed headers, 146 of them raymath.h's | the reading of an undefined symbol at link, `selfhost/cli/` and `selfhost/emit/ffi*` · panel 202 · **class: blocking**

    **Origin:** filed by the coordinator at 00:25 on 2026-10-10 from panel 202 (`docs/panel/202-every-verb-cuts-a-program-s-c-by-module-and-what-two-modules-headers-disagree-on-is-told-before-the-link.md`): the ffi-pragmatist's case and census, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a false message, and `build` and `run` disagreeing on one program.
