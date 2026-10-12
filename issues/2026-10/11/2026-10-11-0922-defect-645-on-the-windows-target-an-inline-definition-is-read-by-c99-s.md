---
kind: defect
area: cli
milestone: none
filed: 2026-10-11
commit: 9694ba6ef88b109d118c65cd72c919b972e40131
github: https://github.com/heroes-lang/heroes/issues/1997
---

- [x] **645 — on the Windows target an `inline` definition is read by C99's rule, where clang reads Microsoft's, so four of `stdio.h`'s functions are refused `ffi_defined_twice`** | C99 makes an `inline` definition external where one declaration lacks `inline` or says `extern` (6.7.4p7), and `cli/defined_twice.hero` reads that rule on every target; clang for `x86_64-pc-windows-msvc` emits every `inline` definition as a COMDAT the linker keeps once (`linkonce_odr`, `weak_odr` for `extern inline`), so the Windows C library's `_snprintf`, `_vsnprintf`, `sprintf` and `vsprintf`, each an `inline` definition with a later declaration without `inline` (`stdio.h` lines 1959 and 1785), link from two units there and were told as defined twice, after defect 643's repair alone left the twoarity row with those four | `selfhost/cli/defined_twice.hero` (`merged`, `definitions`, `inline_only`); `tests/golden/surface-fixtures/twoarity/`; the Windows box · **class: blocking**

    **Origin:** found by lane b21-win at 09:10 on 2026-10-11 on the Windows box, after defect 643's repair: twoarity read 4 `ffi_defined_twice` where it had read 16; `clang -S -emit-llvm` of `inline long long twice(...)` and `extern inline long long thrice(...)` on the box wrote `thrice` as `weak_odr … comdat any` and nothing for an unused `twice`, which a call emits `linkonce_odr`.

    **Class: blocking**, 2026-10-11: a correct program refused and a red CI, by `.claude/rules/verification.md` § Bounded discovery's list; filed apart from 640 as its own cause.

    Repaired at `9694ba6ef88b109d118c65cd72c919b972e40131`, 2026-10-11 (lane b21-win), with defect 643 in one commit, the two causes being one reader's, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Where the toolchain's triple has the environment `msvc` (`dump_tops.microsoft`), a function any declaration of which is `inline` is neither an external definition nor an inline-only one. Measured on the Windows box: the twoarity row runs under both toolchains, *n = 42*, *s = forty-two*, exit 0 (4 `ffi_defined_twice` before); unit tests of the C99 and the Microsoft readings of an `inline` definition with a later plain declaration and of an `extern inline` one.

## The repair

Repaired at `9694ba6ef88b109d118c65cd72c919b972e40131`, 2026-10-11 (lane b21-win), with defect 643 in one commit, the two causes being one reader's, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Where the toolchain's triple has the environment `msvc` (`dump_tops.microsoft`), a function any declaration of which is `inline` is neither an external definition nor an inline-only one. Measured on the Windows box: the twoarity row runs under both toolchains, *n = 42*, *s = forty-two*, exit 0 (4 `ffi_defined_twice` before); unit tests of the C99 and the Microsoft readings of an `inline` definition with a later plain declaration and of an `extern inline` one.

**Closed 2026-10-11** with batch 21 (lanes b21-misc, b21-apply, b21-win and b21-resolve, merged into the round `lane-round-b21` made from the trunk at `34765750`, with panels 210 and 211 landed and ratified), its closing gate run on the round at `e626001e`: the seed regenerated over two generations, the runtime's ABI at 30, its fixpoint by `cmp`, SHA-256 beginning `9c1e85ce6fad65eb`, 52,028,057 bytes; the compiler's own tests 1,617, all passed; the net's own tests 341, all passed; the full net 8,280 passed over 29 suites, 0 failed, `run` in four shards and `cache` alone after, the `permissive` floor told outgrown and raised from 14 to 20 in the closing commit. The emissions were run whole on Linux arm64 at the same commit before any bless (1,227 passed, 0 failed, the cases this Mac skips included), so nothing was blessed there. The Windows box ran lane b21-win's cases with the CI's clang 20 and link.exe (`unsupported` 238 and 0, `surface` 398 and 0). Under the optimistic chain the CI's legs judge it after the push, a red leg filing a new `blocking` defect naming it.
