---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: aeed52ecfa152d6d8424974b968be92883db75f3
github: none
---

- [ ] **571 — a header's `deprecated` attribute fires clang's raw warning at the compiler's own probe line on a correct program** | panel 205's critic, both passes: `<ucontext.h>` bound through a header of the program's own defining `_XOPEN_SOURCE 600` (`hole/uc.hero`) builds at exit 0 and prints clang's `-Wdeprecated-declarations` at the emitted `_Static_assert(HERO_RET_I32(getcontext((void *)0)), ...)`, a line of the compiler's own, under today's compiler and every arm the sitting built | the emitted probe lines, `selfhost/emit/extern_probe.hero`; panel 205 R5 · **class: blocking**

    **Origin:** filed by the coordinator at 03:39 on 2026-10-10 from panel 205 (`docs/panel/205-a-library-s-own-header-code-is-judged-as-clang-judges-a-system-header-the-checks-raised-again-after-it-and-a-switch-goes-in-the-first-group.md`, R5), the seats' measurements, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a clang warning on a correct program.

    Repaired at `aeed52ecfa152d6d8424974b968be92883db75f3`, 2026-10-10, gated by its cases and the compiler's own tests; the net is owed at the batch's close. `-Wdeprecated-declarations` is ignored from the groups' close, so the result assertions, field assertions, typedefs, probes and prototypes say nothing; spoken again over the program's own definitions in both emitters; quiet over each group constant's accessor (`emit/constant_accessor.hero`, split from `emit/body.hero` past §11's 300 lines) and over a prologue declaring a local of a group's record; quiet again over the per-type functions (`emit/deprecation.hero`). A unit binding no group writes none of these lines. Measured beside it: a function, an enumerator and a record marked `deprecated` in a header of the program's own printed 2, 3 and 8 raw warnings on the base and print none; `uc` 2 and none; the program's own call of a deprecated function, and its own signature naming a deprecated record, are still told by clang at the program's line, the verdict unchanged either way (a question for the coordinator, design.md Part 8 saying this language has no warning level). Cases `run/fixedbugs-571-*` (4); run 4 and 0, warnings 502 and 0, emission 1,110 and 0 after its re-bless, emit 11 and 0, unsupported 224 and 0, layout 6 and 0, wholes 538 and 0, the compiler's own tests 1,554 passed; a cold build's instructions moved under 0.1%.
