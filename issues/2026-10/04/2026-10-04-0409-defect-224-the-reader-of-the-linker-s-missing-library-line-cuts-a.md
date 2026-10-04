- [x] **224 — the reader of the linker's *missing library* line cuts a library's name at a quote under ld64 and at whitespace under GNU ld, and `build` exits 2** | `extern "ab.h" link "a'b"`: `build` 2, *internal error: linking failed*, ld64's *library 'a'b' not found*, *clang refused the generated C* (this Mac); `link "a b"`: the same under GNU ld's *cannot find -la b* (Linux arm64, the seats) (the trunk's compiler at `826ddc2f`, 2026-10-03, `<scratchpad>/repro188/link-squote/`) | `selfhost/emit/ffi_build.hero` (`missing_library`, `link_head`, `strip_colons`) · panel 188 R7 (a) · **class: blocking**

    **Origin:** panel 188's compiler-engineer and ffi-pragmatist, 2026-10-03, on Q5; reproduced by the coordinator on this Mac the same day.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told; filed apart from 216 (the compiler's reader of the linker's text), landed with 221's repair, which widens it (a TAB in a `link` name once decoded).

    **Widened by panel 188's Windows leg**, 2026-10-03 (`docs/panel/188-reports/compiler-engineer.md` § 22.2): lld-link writes *could not open 'X.lib': no such file or directory*, which the reader did not catch, so a group naming a missing library was exit 2 on Windows, 11 of the leg's 155 cases; the same cause in another linker's wording.

    **2026-10-03, batch 8's FFI lane**: repaired at `0242a730`, widened at `25bc5e04`, gated by its cases and the compiler's own tests; the net and the Windows leg are owed at the batch's close.

## The repair

Repaired at `0242a730` (lld-link's wording at 25bc5e04). The reader of the linker's *missing library* line takes the name whole under ld64, GNU ld and lld-link, quotes and spaces included; its unit tests carry the three wordings.

**Closed 2026-10-04** after batch 8's platform legs ran its cases on the tree that closes (`7ec19cb7`, the code of the batch's closing commit): Linux arm64, Debian clang 22.1.8, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the same image under clang 18, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the Windows box, clang 23.1.1, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed (the folder removed from the box, 20 of 20 green). The batch's gate on this Mac is the closing commit's body and the thirteen records closed with it.
