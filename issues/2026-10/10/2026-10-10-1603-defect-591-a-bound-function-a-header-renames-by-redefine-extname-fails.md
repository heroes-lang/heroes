---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **591 — a bound function a header renames by `#pragma redefine_extname` fails to link as the compiler's fault, at exit 2** | a header of the program's own writing `#pragma redefine_extname labs llabs` then `#include <stdlib.h>`, binding `labs`, builds at exit 2 with *internal error: linking failed*, *Undefined symbols: "llabs"*: clang gives the pragma's name as a literal label, which Mach-O spells without the leading underscore, so the link fails as it does for the same header in C; the linker's missing name is not the one the program declared, so the round's narrowing by `declaration()` leaves it the compiler's, where the author's header is what names it; measured on this Mac (Apple clang 21) by lane b19-pack at 16:00 | the link round's reading of an undefined symbol (`.claude/rules/c-boundary.md` § A clang failure that the author's own extern caused), which could map the label back to the bound name · **class: blocking**

    **Origin:** found by lane b19-pack attacking the shapes beside defect 582 (`.claude/worktrees/scratch-b15/b19-pack/shapes/`, ignored by git), filed by the lane at 16:03 on 2026-10-10.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told.
