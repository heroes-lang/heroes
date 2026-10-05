---
kind: defect
area: cli
milestone: none
filed: 2026-10-04
commit: 50aaac0bf6ea2ec14178ca58e66baad96ec42a97
github: none
---

- [x] **248 — a clang warning on a bound header is printed twice, once from the program's unit and once from the pointee probe's file the author never wrote** | `extern "lib.h"` over `constant ANSWER: i64`, `lib.h` holding `return "Jos<e9>";`: `build` exit 0, clang's `warning: illegal character encoding in string literal [-Winvalid-source-encoding]` printed twice, the second naming `build/pointee-<key>/check-<key>.c` (the trunk's compiler at `7d9f2e8f` on this Mac, Linux arm64 and the Windows box, 2026-10-04, panel 189's ffi-pragmatist); the object a second build rewrites with nothing edited is 227's header digest, not this item | `selfhost/cli/pointee.hero` (the pointee probe) and how its clang output reaches the author · **class: adjacent**

    **Origin:** panel 189's ffi-pragmatist, 2026-10-04 00:25 to 00:31 (`docs/panel/189-reports/ffi-pragmatist.md`, *A C header's digest*); filed apart by the coordinator.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): one warning, two copies, one naming an internal file.

    Repaired at `50aaac0b`, 2026-10-04 (lane b10-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `cli/pointee_wants.hero` was not needed.

## The repair

Repaired at `50aaac0b`. A probe's check unit includes the program's headers, and `toolchain.run_clang` printed what clang said on success, so a header's warning reached the terminal from the pointee check's unit, a file the author never wrote, and from the layout check's, as well as from the program's own. Both checks now ask through `toolchain.asked_of_clang`, which hands clang's words back rather than printing them; the program's units tell the warning, and a refusal still carries clang's whole text. Measured by hand, the copies before and after: the defect's Latin-1 string 2 and 1, a `#warning` 2 and 1, two headers 4 and 2, a union arm left out of a construction 4 and 2. The copy `library.c` still prints is another cause, defect 327. Its case is a compiler test, a probe's question handing back a header's warning.

**Closed 2026-10-05**, after batch 10's platform legs, each on `4c3524fb`, the batch's closing tree: Linux arm64 in its container under Debian clang 22.1.8 and again under 18.1.8, the compiler's own tests 1,213, all passed, and 22 suites, every one 0 failed, each time; the Windows box under clang 23.1.1, the compiler's own tests 1,213, all passed, and 21 of its 22 suites 0 failed in the leg's folder, `unsupported` reading 127 passed and 2 failed there, two `fixedbugs-157-*` cases told `internal error` over three NUL bytes, because the box's crash at 18:46 on 2026-10-04 had left 11 files of that folder's build cache as zeros (defect 357); `unsupported` from the same archive with the same `heroes.exe`, in a fresh folder, 129 passed and 0 failed.
