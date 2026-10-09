---
kind: defect
area: compiler
milestone: none
filed: 2026-10-09
commit: 31cbf86a10f37053ad0391c2b98f719ffbf10827
github: none
---

- [ ] **516 — `diag.hero`'s note on `machine_locked_path` says what defect 446 measured false** | it calls the name a thesis rule *because C would take the program on exactly one machine*; defect 446 measured ld64, GNU ld and lld-link reading a rooted `-l` as another path, never the one written (lane b15-box) | `selfhost/diag.hero` · defect 446 · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-box's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a comment that states a premise measured false.

    Repaired at `31cbf86a`, 2026-10-09 (lane b16-compiler), gated by the `annotations` suite and the compiler's own tests; the net is owed at the batch's close. The note now says what holds for the names the code still refuses, that without the rule the tools read the name where one machine's files decide what it names, run that morning for two of them (clang opens a rooted header as written; `pkg-config` reads a `.pc` from the directory it runs in, and from its parent finds none), and that a rooted `link` is `unwritable_name` because no linker reads it as written (defect 446's measurement). A comment only.
