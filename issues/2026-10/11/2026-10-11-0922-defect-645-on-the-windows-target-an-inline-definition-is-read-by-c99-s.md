---
kind: defect
area: cli
milestone: none
filed: 2026-10-11
commit: 9694ba6ef88b109d118c65cd72c919b972e40131
github: none
---

- [ ] **645 — on the Windows target an `inline` definition is read by C99's rule, where clang reads Microsoft's, so four of `stdio.h`'s functions are refused `ffi_defined_twice`** | C99 makes an `inline` definition external where one declaration lacks `inline` or says `extern` (6.7.4p7), and `cli/defined_twice.hero` reads that rule on every target; clang for `x86_64-pc-windows-msvc` emits every `inline` definition as a COMDAT the linker keeps once (`linkonce_odr`, `weak_odr` for `extern inline`), so the Windows C library's `_snprintf`, `_vsnprintf`, `sprintf` and `vsprintf`, each an `inline` definition with a later declaration without `inline` (`stdio.h` lines 1959 and 1785), link from two units there and were told as defined twice, after defect 643's repair alone left the twoarity row with those four | `selfhost/cli/defined_twice.hero` (`merged`, `definitions`, `inline_only`); `tests/golden/surface-fixtures/twoarity/`; the Windows box · **class: blocking**

    **Origin:** found by lane b21-win at 09:10 on 2026-10-11 on the Windows box, after defect 643's repair: twoarity read 4 `ffi_defined_twice` where it had read 16; `clang -S -emit-llvm` of `inline long long twice(...)` and `extern inline long long thrice(...)` on the box wrote `thrice` as `weak_odr … comdat any` and nothing for an unused `twice`, which a call emits `linkonce_odr`.

    **Class: blocking**, 2026-10-11: a correct program refused and a red CI, by `.claude/rules/verification.md` § Bounded discovery's list; filed apart from 640 as its own cause.

    Repaired at `9694ba6ef88b109d118c65cd72c919b972e40131`, 2026-10-11 (lane b21-win), with defect 643 in one commit, the two causes being one reader's, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Where the toolchain's triple has the environment `msvc` (`dump_tops.microsoft`), a function any declaration of which is `inline` is neither an external definition nor an inline-only one. Measured on the Windows box: the twoarity row runs under both toolchains, *n = 42*, *s = forty-two*, exit 0 (4 `ffi_defined_twice` before); unit tests of the C99 and the Microsoft readings of an `inline` definition with a later plain declaration and of an `extern inline` one.
