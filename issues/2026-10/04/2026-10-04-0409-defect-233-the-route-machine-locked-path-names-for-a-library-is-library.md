- [x] **233 — the route `machine_locked_path` names for a library is `LIBRARY_PATH`, which lld-link ignores, so the advice fails on Windows** | `extern "stdio.h" link "/opt/foo/lib/libfoo.a"`: `check` exit 1, `machine_locked_path`, *set `LIBRARY_PATH` to the directory holding it* (`tests/golden/check/ffi-a-group-head-names-not-locates`); on the Windows box (clang 23.1.1, `lld-link`) `extern "lp.h" link "lp188"` over a library only in a side directory, built with `LIBRARY_PATH` naming that directory, stops at *could not open 'lp188.lib'*, exit 2, where `--library <dir>` builds and prints 7 and `LIB` fails too (the compiler-engineer's `libprobe.sh`, 2026-10-03, panel 188's Windows leg, its report's § 22.4) | the leaf's message (`selfhost/head_names.hero` after stage E, `route`) · `ffi_missing_library`'s note (`selfhost/emit/ffi_build.hero`), which already names `--library <dir>` · **class: blocking**

    **Origin:** the compiler-engineer's Windows leg of 2026-10-03 (`docs/panel/188-reports/compiler-engineer.md` § 22.4), panel 188 R12's platform facts; filed by the coordinator's instruction into batch 8's FFI lane, which repairs it.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a false message, a route that fails on Windows where the compiler's own `--library <dir>` works on every platform measured.

    **2026-10-03, batch 8's FFI lane**: repaired at `c6bd5a6a`, gated by its cases and the compiler's own tests; the net and the Windows leg are owed at the batch's close.

## The repair

Repaired at `c6bd5a6a`. `machine_locked_path`'s route for a library is the compiler's own `--library <dir>`, which the Windows box measured working where `LIBRARY_PATH` is ignored; design.md §4.19 carries a dated correction (the author's 2a of 2026-10-04).

**Closed 2026-10-04** after batch 8's platform legs ran its cases on the tree that closes (`7ec19cb7`, the code of the batch's closing commit): Linux arm64, Debian clang 22.1.8, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the same image under clang 18, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the Windows box, clang 23.1.1, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed (the folder removed from the box, 20 of 20 green). The batch's gate on this Mac is the closing commit's body and the thirteen records closed with it.
