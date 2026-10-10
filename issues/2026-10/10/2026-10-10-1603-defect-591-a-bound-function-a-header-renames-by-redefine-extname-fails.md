---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: 6fe4c37f9d7b3c225be54105e64af78edcf63996
github: none
---

- [ ] **591 — a bound function a header renames by `#pragma redefine_extname` fails to link as the compiler's fault, at exit 2** | a header of the program's own writing `#pragma redefine_extname labs llabs` then `#include <stdlib.h>`, binding `labs`, builds at exit 2 with *internal error: linking failed*, *Undefined symbols: "llabs"*: clang gives the pragma's name as a literal label, which Mach-O spells without the leading underscore, so the link fails as it does for the same header in C; the linker's missing name is not the one the program declared, so the round's narrowing by `declaration()` leaves it the compiler's, where the author's header is what names it; measured on this Mac (Apple clang 21) by lane b19-pack at 16:00 | the link round's reading of an undefined symbol (`.claude/rules/c-boundary.md` § A clang failure that the author's own extern caused), which could map the label back to the bound name · **class: blocking**

    **Origin:** found by lane b19-pack attacking the shapes beside defect 582 (`.claude/worktrees/scratch-b15/b19-pack/shapes/`, ignored by git), filed by the lane at 16:03 on 2026-10-10.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told.

    Repaired at `6fe4c37f9d7b3c225be54105e64af78edcf63996`, 2026-10-11 (lane b20-pragma), with defect 592, gated by its cases and the compiler's own tests; the net is owed at the batch's close. The link's failure path reads clang's dump of the module's header list, followed by the unit's own `heroes_standard.h` and `main` (`cli/label_dump.hero`), for the C name each `AsmLabelAttr` labels and where clang placed it; an undefined symbol that is the label of a function a group binds is told `ffi_missing_link` on the binding, the file and line named (`cli/renamed.hero`), the twelfth member of `.claude/rules/c-boundary.md`'s class. Measured on this Mac (Apple clang 21), each exit 2 before and exit 1 after at `-O0` and `-O2`: the reproducer (`labs llabs`), a label no platform has, an `__asm__` label on the header's own declaration, the pragma in a header the group's header includes, the pragma after the header's declaration, and the pragma through a macro's `_Pragma` (placed as *a header of this group*, the dump saying clang's scratch buffer); `__asm__("_llabs")` links and prints 42, Mach-O's own spelling. In the Linux arm64 image (clang 22.1.8), at `6fe4c37f9d7b3c225be54105e64af78edcf63996`: `unsupported (only fixedbugs-59): 6 passed, 0 failed`, `redefine_extname labs llabs` links and prints 42 (ELF's symbol is the label as written), and `__asm__("_llabs")` is told. Measured and not taken: protecting the program, since C cannot replace a label once a declaration has one (a second `redefine_extname` does not, a label written after it is *conflicting asm label*). Cases `unsupported/fixedbugs-591-*` (3). Cost: no change past the runs' spread, the numbers in the commit's body.
