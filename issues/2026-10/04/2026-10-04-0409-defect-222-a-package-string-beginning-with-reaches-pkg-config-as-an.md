---
kind: defect
area: cli
milestone: none
filed: 2026-10-03
commit: 9c1ee8c3266f3b76488c231a36c40ec81a2d1d91
github: none
---

- [x] **222 — a `package` string beginning with `-` reaches `pkg-config` as an option: one is accepted with no package named, another called a package, a third told falsely as not installed** | `extern "ab.h" package "--atleast-pkgconfig-version=0"` over `function seven() -> i32`, `ab.h` a `static inline`: `check` 0, `build` 0, prints `7`, a package clause naming no package; `package "--version"`: *the package `--version` answered with `3.0.7`*; `package "-x"`: *the package `-x` is not installed on this machine*, where `pkg-config` said *unknown option -- x* (the trunk's compiler at `826ddc2f`, this Mac, 2026-10-03, `<scratchpad>/repro188/pkg-option/`, `pkg-opt--/`, `pkg-opt-x/`; the first also in the Linux arm64 image, the seats) | `selfhost/cli/libraries.hero` (the `pkg-config` argv; its comment at 302 to 304 says no `.hero` file can hand it an argument) · panel 188 R7 (b) · **class: blocking**

    **Origin:** panel 188's ffi-pragmatist and compiler-engineer, 2026-10-03, on Q5; reproduced by the coordinator the same day.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a wrong program accepted and a false message; filed apart from 216 (the tool's parse of a whole string, not a string its tool cannot carry), landed in the same lane.

    **Widened to `link` by panel 188's Windows leg**, 2026-10-03 (`docs/panel/188-reports/compiler-engineer.md` § 22.5; the author kept the extension): lld-link reads a `link` beginning with `-` as an option of its own, `link "-out:pwn188"` building at exit 0 and writing a file `pwn188.lib`; refused with the same `option_like_name`.

    **2026-10-03, batch 8's FFI lane**: repaired at `9c1ee8c3`, widened at `d3597207`, gated by its cases and the compiler's own tests; the net and the Windows leg are owed at the batch's close.

## The repair

Repaired at `9c1ee8c3` (widened to `link` at d3597207, kept by the author's 2a of 2026-10-03). A `package` or `link` string beginning with `-` is refused, `option_like_name`, and `--` stands before every package `pkg-config` is handed. Case: `check/fixedbugs-222-a-library-name-beginning-with-a-dash`.

**Closed 2026-10-04** after batch 8's platform legs ran its cases on the tree that closes (`7ec19cb7`, the code of the batch's closing commit): Linux arm64, Debian clang 22.1.8, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the same image under clang 18, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the Windows box, clang 23.1.1, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed (the folder removed from the box, 20 of 20 green). The batch's gate on this Mac is the closing commit's body and the thirteen records closed with it.
