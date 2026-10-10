---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **590 — a header's file-scope `#pragma clang fp eval_method` makes the unit's own `<math.h>` refuse, an internal error at exit 2** | a header of the program's own writing `#pragma clang fp eval_method(double)` then `#include <stdlib.h>`, binding `llabs`, builds at exit 2 with *internal error: compiling the generated C failed*: the unit reads `<math.h>` after its groups (`runtime/heroes_standard.h`, panel 205's R3) and clang refuses every `__FLT_EVAL_METHOD__` there, *cannot be expanded inside a scope containing '#pragma clang fp eval_method'*; the header compiles alone, and with a C file that includes no `<math.h>`, measured on this Mac (Apple clang 21) by lane b19-pack at 16:00; `float_control(push)`/`pop` around the groups does not lift it, the preprocessor keeping the pragma's location for the rest of the unit | the order of `heroes_standard.h` after the groups, or the round's reading of a clang failure inside it (`ffi_header_refused`'s family, `.claude/rules/c-boundary.md`) · **class: blocking**

    **Origin:** found by lane b19-pack attacking the shapes beside defect 582 (`.claude/worktrees/scratch-b15/b19-pack/shapes/`, ignored by git), filed by the lane at 16:03 on 2026-10-10.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told, the author's header blamed on the compiler.
