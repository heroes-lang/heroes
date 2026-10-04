---
kind: defect
area: compiler
milestone: none
filed: 2026-10-03
commit: e839729705724404c8e48e2906690c8afb98b608
github: none
---

- [x] **234 — on Windows a header name NTFS stores as another file is accepted, and the include opens that file at exit 0 where the Mac and Linux say it is missing** | under clang 23.1.1 on the Windows box, each tree holding only what the row needs: `#include <ab.h.>`, `<ab.h >`, `<ab.h..>` and `<ab.h::$DATA>` open `ab.h`; `<ab:c.h>` opens the alternate data stream `c.h` of a file `ab`; `<d./ab.h>` opens `d/ab.h`; `<zz/../ab.h>` opens `ab.h` with no `zz`; `<ABCDEF~1.H>` opens `abcdefghij.h` by its 8.3 name; `<NUL>` opens the null device; each *file not found* on this Mac and in the Linux arm64 image (the ffi-pragmatist, `docs/panel/188-reports/ffi-pragmatist.md` § Windows, measured, 2026-10-03); every one passes the ratified rules of panel 188 | the leaf's judgement of a group head's string (`selfhost/head_names.hero` after panel 188's landing) · panel 188 R2 and R6 · panel 055 · **class: blocking**

    **Origin:** panel 188's ffi-pragmatist, 2026-10-03, measuring R12's Windows facts after the ratification; read by the coordinator from the seat's tables, not yet run by the coordinator.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a wrong program accepted on one platform, the header bound being another file than the one the string names. Its repair widens what `check` refuses, which is the author's to rule on (`docs/work/DECIDE.md`, `panel 188`): the rule measured to close it refuses 0 of the real header names surveyed on the three platforms and none of the tree's 700 group-head strings.

    **2026-10-03, batch 8's FFI lane**: repaired at `e8397297`, a package's rules moved out of `head_names.hero` into `head_package.hero` first at `1a8eb095`, gated by its cases and the compiler's own tests; the net is owed at the batch's close (the line added at the round's merge, the item having been filed on the trunk after the lane's base).

## The repair

Repaired at `e8397297` (a package's rules moved into `head_package.hero` first at 1a8eb095). On Windows a name NTFS stores as another file's is refused, `unwritable_name`, judged on all three strings part by part. Case: `check/fixedbugs-234-a-name-windows-reads-as-another-file`.

**Closed 2026-10-04** after batch 8's platform legs ran its cases on the tree that closes (`7ec19cb7`, the code of the batch's closing commit): Linux arm64, Debian clang 22.1.8, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the same image under clang 18, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the Windows box, clang 23.1.1, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed (the folder removed from the box, 20 of 20 green). The batch's gate on this Mac is the closing commit's body and the thirteen records closed with it.
