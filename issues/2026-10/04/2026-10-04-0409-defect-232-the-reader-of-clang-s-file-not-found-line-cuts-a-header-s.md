---
kind: defect
area: emit
milestone: none
filed: 2026-10-03
commit: 850ca7089e4e734a8cfb0544c48c50535989fba5
github: none
---

- [x] **232 — the reader of clang's *file not found* line cuts a header's name at a quote and cannot read a code point clang prints as `<U+XXXX>`, so a missing header named with either stops `build` at exit 2** | `extern "a'b.h"` over `function seven() -> i32`, no header beside it: `check` 0, `build` 2, *internal error: compiling the generated C failed*, clang's *'a'b.h' file not found* not matched back to the group, where `extern "ab.h"` missing is `ffi_missing_header` at exit 1 (the trunk's compiler at `826ddc2f`, 2026-10-03, `<scratchpad>/repro188/squote-missing/`); a private-use U+E000, an unassigned U+0378 or a noncharacter U+FFFE in a missing header's name the same, clang printing it `<U+E000>`, on both platforms (the compiler-engineer, stage E) | `selfhost/emit/ffi_build.hero` (`missing_header`, which takes the name up to the first quote and matches it whole) · **class: blocking**

    **Origin:** panel 188's completeness critic, 2026-10-03, in its third pass over stage D (F1 ran `a'b.h` only with its header present), and the compiler-engineer building stage E for the escaped code points; the quote reproduced by the coordinator the same day.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told. Panel 188's R4 and R5 refuse `'`, U+2028 and U+2029 at `check`, so once they land a program `check` passes reaches this reader with a private-use, unassigned or noncharacter code point only; R4 is a thesis rule the author may turn, so the reader is repaired on its own, reading clang's line against each declared header as stage C did for the linker's.

    **2026-10-03, batch 8's FFI lane**: repaired at `850ca708`, its leaf reading with `bytes` at `f9b9605f`, gated by its cases and the compiler's own tests; the net and the platform legs are owed at the batch's close.

## The repair

Repaired at `850ca708` (its leaf reading with `bytes` at f9b9605f). The reader of clang's *file not found* line takes a header's name whole, a quote and a code point clang prints as `<U+XXXX>` included: three `unsupported/fixedbugs-232-*` cases went from an internal error at exit 2 to `ffi_missing_header` at exit 1 in the census, and a fourth names `fixedbugs-232-gone'x.h` whole.

**Closed 2026-10-04** after batch 8's platform legs ran its cases on the tree that closes (`7ec19cb7`, the code of the batch's closing commit): Linux arm64, Debian clang 22.1.8, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the same image under clang 18, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the Windows box, clang 23.1.1, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed (the folder removed from the box, 20 of 20 green). The batch's gate on this Mac is the closing commit's body and the thirteen records closed with it.

**Corrected 2026-10-04 at 09:53, read against the legs' own counts: on Linux arm64 the sentence above holds; on the Windows box it is a question.** The `unsupported` form of that day counted a case it skipped as neither passed nor failed, and named none below its one-third floor (defect 246). It read 129 passed on the Windows box against 137 on this Mac at the same code (`703af779`, whose harness, cases, compiler, runtime and seed equal tree `7ec19cb7`'s, read by lane b9-harness, `<scratchpad>/batch9/harness/base-unsupported.txt`), so eight cases there were skipped and never named. On Linux arm64, run again in the same image (Debian clang 22.1.8), in a run that ended at 09:51:52 (`<scratchpad>/probe-b9-unsupported/arm64-c22.log`), the trunk's compiler matches the expectation of each of this item's four cases, so the 133 passed there included them. Whether the box skipped any of them is unrun: it has not answered since about 09:03, and batch 9's Windows leg names every case it steps aside.
