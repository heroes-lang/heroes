---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **571 — a header's `deprecated` attribute fires clang's raw warning at the compiler's own probe line on a correct program** | panel 205's critic, both passes: `<ucontext.h>` bound through a header of the program's own defining `_XOPEN_SOURCE 600` (`hole/uc.hero`) builds at exit 0 and prints clang's `-Wdeprecated-declarations` at the emitted `_Static_assert(HERO_RET_I32(getcontext((void *)0)), ...)`, a line of the compiler's own, under today's compiler and every arm the sitting built | the emitted probe lines, `selfhost/emit/extern_probe.hero`; panel 205 R5 · **class: blocking**

    **Origin:** filed by the coordinator at 03:39 on 2026-10-10 from panel 205 (`docs/panel/205-a-library-s-own-header-code-is-judged-as-clang-judges-a-system-header-the-checks-raised-again-after-it-and-a-switch-goes-in-the-first-group.md`, R5), the seats' measurements, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a clang warning on a correct program.
