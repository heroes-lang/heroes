---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **570 — a header's own `#pragma clang diagnostic ignored` turns a binding's sign check off for the rest of its unit** | a header of the program's own writing `#pragma clang diagnostic ignored "-Wsign-conversion"` (and `-Wshorten-64-to-32`, `-Wincompatible-pointer-types`) with no push or pop, then `#include <stdlib.h>`, named by a group binding `abs(x: u32) -> i32` against C's `int abs(int)`: `build` exits 0 and the program prints `5`, clang's raw `-Wabsolute-value` warning on stderr at the emitted `_Static_assert`; the same binding through a header that only includes `<stdlib.h>` is refused `ffi_parameter_type`, *declared a different sign from the header's `int`*; measured by the coordinator at 02:49 with the trunk's compiler (`.claude/worktrees/scratch-b15/hole570/`); the width check survives (`abs(x: i64)` refused), the probe region raising `-Wimplicit-int-conversion` again after the headers, so the sign check rests on the command line's `-Werror=sign-conversion` alone, which any header, a package's among them, can turn off | the probe region of a module's unit, `selfhost/emit/extern_probe.hero:136` (the four warnings it raises again), and every warning a check of the FFI rests on (panel 205's critic, first pass, route C) · panel 205 · **class: blocking**

    **Origin:** found by panel 205's completeness critic, first pass, at about 02:45 on 2026-10-10 (`docs/panel/205-reports/completeness-critic-pass1.md` in the sitting's tree, its case `.claude/worktrees/scratch-b15/critic-205/hole/leak.hero`, ignored by git), reproduced and filed by the coordinator at 02:50.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a wrong binding accepted, the compiler's promise that clang checks a declaration against its header broken.

    **Widened and ruled 2026-10-10** by panel 205 (`docs/panel/205-a-library-s-own-header-code-is-judged-as-clang-judges-a-system-header-the-checks-raised-again-after-it-and-a-switch-goes-in-the-first-group.md`, ratified at 03:38): fourteen rows of this cause build today on this Mac and Linux arm64, measured by the spec-warden, the compiler-engineer and the critic: a sign, a handle of another tag (defect 029's class), an integer for a pointer (`read_p(p: i64)` against `int *`, aborting 134 at run time), a callback, an undeclared function (`getpid(x: f64)`, no warning), under `ignored`, `GCC diagnostic ignored`, a downgrade to `warning`, `-Wconversion`, `-Weverything` and a push with no pop; `mac`, a `_Pragma` inside a constant's macro; and `callee`, a macro naming the bound function that expands to a `_Pragma`. R1: every warning an FFI check rests on is raised again after the groups' close and in the probe region, and `callee` is asked of clang and refused with a true message.
