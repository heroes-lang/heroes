---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: 8e24d7eac9eaf8de61351be85c501918956c46ca
github: none
---

- [ ] **480 — `emit/mangle.hero` says the library declares nothing but the inventory's names** | false: the library declares `validated` and `args_checked` too, harmless today (lane b14-resolve) | `selfhost/emit/mangle.hero` · defect 455 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-resolve's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a false comment.

    Repaired at `8e24d7ea`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The comment names the library's two functions the inventory lacks, `validated` and `args_checked`, each spelled by the module that declares it, which for the library's is the library, and a test parsing the library's text fails on a third such name.
