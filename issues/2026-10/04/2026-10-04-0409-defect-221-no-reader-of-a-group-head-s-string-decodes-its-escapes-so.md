---
kind: defect
area: emit
milestone: none
filed: 2026-10-03
commit: db9fb52e41446d3f9ca546fd7ce24bfd224356c5
github: none
---

- [x] **221 — no reader of a group head's string decodes its escapes, so `extern "a\\b.h"` asks clang for `a\\b.h` and `build` says a header present is missing** | `extern "a\\b.h"` over `function seven() -> i32`, the file `a\b.h` beside the program: `check` 0, `build` 1, `ffi_missing_header`, *`a\\b.h` is not on this machine's include path, clang looked and did not find it*, while the file is there; the emitted unit holds `#include <a\\b.h>`, two backslashes; the same for `\"`, `\n`, `\t`, `\r`, and for `link "a\\b"` with `liba\b.a` there (the trunk's compiler at `826ddc2f`, 2026-10-03, `<scratchpad>/repro188/esc-bs/`) | `selfhost/emit/externs.hero` (`unquoted`), `selfhost/parse/module_text.hero` (`unquote`) · spec § 2's five string escapes, § 13's `Extern` production · panel 188 R1 · **class: blocking**

    **Origin:** panel 188's completeness critic, 2026-10-03, in its first pass over the briefs (the escape face of F1, read as clang's, was the compiler's); confirmed by `od -c` of the emitted units (the compiler-engineer) and for `link` (the ffi-pragmatist); reproduced by the coordinator the same day.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a false message; filed apart from 216 by the rule (a reader that does not decode, where 216's is a string its tool cannot carry), landed in the same lane, since the refusal reads the decoded value.

    **2026-10-03, batch 8's FFI lane**: repaired at `db9fb52e`, gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `db9fb52e`. A group head's string is decoded before any reader takes it (panel 188's R1): `link "a\\b"` over a real `liba\b.a` links and prints 7.

**Closed 2026-10-04** after batch 8's platform legs ran its cases on the tree that closes (`7ec19cb7`, the code of the batch's closing commit): Linux arm64, Debian clang 22.1.8, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the same image under clang 18, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the Windows box, clang 23.1.1, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed (the folder removed from the box, 20 of 20 green). The batch's gate on this Mac is the closing commit's body and the thirteen records closed with it.
