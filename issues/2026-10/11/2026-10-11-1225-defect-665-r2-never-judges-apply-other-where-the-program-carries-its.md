---
kind: defect
area: mutate
milestone: none
filed: 2026-10-11
commit: none
github: none
---

- [ ] **665 — R2 never judges APPLY-OTHER where the program carries its own message** | `mutate/applied.judged_apply` reads APPLY-OTHER's `clean` on what `check --apply` wrote as told, while defect 663's repair takes the unmutated program's own messages out of the same arm before `fresh` and `stands` are read; so a mutant of a corpus program told a message of its own (the frozen `examples/ledger/db/sqlite.hero` and `examples/ledger/main.hero`, told `never_rebound` at 322 by panel 210's rule) is never clean after `--apply`, and a certain fix that writes another program there is never flagged APPLY-OTHER, the one flag of the four that names defect 172's `blocking` class | `selfhost/mutate/applied.hero` (`judged_apply`'s `clean`, `less_own`), `selfhost/mutate/own.hero` (the subtraction) · **class: adjacent**

    **Origin:** found by lane b22-663 at 12:22 on 2026-10-11, reading `judged_apply` while repairing defect 663: its `clean` was left on the unsubtracted arm, the three flags then reading two measures; filed as 665 on the coordinator's word at 12:24.

    **Class: adjacent**, 2026-10-11: real, found beside the work, an instrument blind where it could see; no program is wrongly accepted and no message is false.
