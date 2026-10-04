- [ ] **223 — a `package` string reaches `pkg-config`'s list and version grammar, which § 13 does not name, and a refused version is told as a package not installed** | `extern "ab.h" package "zlib >= 99"`: `build` 1, *the package `zlib >= 99` is not installed on this machine*, while zlib 1.2.12 is installed (this Mac; 1.3.1 in the Linux arm64 image, the ffi-pragmatist); `package "zlib sqlite3"`: `build` 0 (the trunk's compiler at `826ddc2f`, 2026-10-03, `<scratchpad>/repro188/pkg-version/`, `pkg-list/`) | `selfhost/cli/libraries.hero` · spec § 13 (*"A group may name a **package**"*) · panel 188 R7 (c) · **class: blocking**

    **Origin:** panel 188's ffi-pragmatist and compiler-engineer, 2026-10-03, on Q5; the ruling that a package names one package is the sitting's (R7 (c)); reproduced by the coordinator the same day.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a false message; filed apart from 216 (`pkg-config`'s grammar, not a string its tool cannot carry), landed in the same lane.

    **2026-10-03, batch 8's FFI lane**: repaired at `6779162c`, gated by its cases and the compiler's own tests; the net is owed at the batch's close.
