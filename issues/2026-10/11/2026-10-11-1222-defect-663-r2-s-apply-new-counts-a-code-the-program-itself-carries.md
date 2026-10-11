---
kind: defect
area: mutate
milestone: none
filed: 2026-10-11
commit: none
github: none
---

- [ ] **663 — R2's `APPLY-NEW` counts a code the unmutated program itself carries, so a corpus program's verdict moving reads as an APPLY move of every mutant of it** | `mutate/applied.judged_apply` flags `APPLY-NEW` where a code stands in what `check --apply` wrote that the mutant's normal arm was never told; the mutant's codes are the only ones subtracted, and the program's own (`corpus.jsonl`, which `mutate/own.hero` subtracts from a mutant's messages since defect 210) are not. A mutant stopped at the parser is told its own mistake alone; `--apply` repairs it, the applied program reaches the resolver, and there it is told what the unmutated program is told, a code the mutant never showed and the program always carried. Measured on panel 187's R2 over the plan cut for defect 661, the compiler of `341bebcd` against `397381f1`'s: 26 APPLY moves, 24 of them mutants of `examples/ledger/db/sqlite.hero` (16) and `examples/ledger/main.hero` (8), whose frozen copies are told `never_rebound` at line 322 by panel 210's rule (the corpus lines `0 -> 1 messages` unmutated); key `0f89625d05eed035`, `union` at line 140: normal `reserved_word` at 140 in both runs, `apply` exit 0 and changed in both, `apply_flags` `[]` against `[APPLY-NEW]`, the new code the program's own at 322. heroes-lang-98 read the 26 and named the shape; the repair is the subtraction `own.hero` already makes, the program's codes taken out of `fresh` (and of `stands`, where `APPLY-SAME` reads a code the program carries) as its messages are taken out of the mutant's | `selfhost/mutate/applied.hero` (`judged_apply`'s `fresh` and `stands`), `selfhost/mutate/own.hero` (the subtraction to reuse), `selfhost/mutate/planted.hero` (where the program's record is at hand when a single is judged), `selfhost/mutate/differential.hero` (`flag_moves`, which names each move a finding) · **class: adjacent**

    **Origin:** asked by heroes-lang-98 at 12:15 on 2026-10-11, reading the 26 APPLY moves of the R2 the landing session of panel 209 ran at 11:27 (`.claude/worktrees/scratch-b15/r2-plan-397381f1-341bebcd/differential.txt`, untracked); measured by that session at 12:20 on the key named.

    **Class: adjacent**, 2026-10-11: real, found beside the work, an instrument's finding less exact than it could be; no program is wrongly accepted and no message is false.
