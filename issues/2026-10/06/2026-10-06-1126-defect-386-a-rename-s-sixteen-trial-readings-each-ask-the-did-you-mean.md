---
kind: defect
area: resolve
milestone: none
filed: 2026-10-06
commit: b7220ff862a5dacd515ca477c839e7023864177c
github: none
---

- [ ] **386 — a rename's sixteen trial readings each ask the did-you-mean again for every unknown name** | after defect 372's repairs the critic's file, one line of 2,000 `totl` with `total` in scope, still costs 68.5 billion instructions against 3.9 with no rename to try: each of `rename_fit.judged`'s sixteen trial readings resolves the whole program, and its did-you-mean queries repeat the first reading's; a memo of the last query in `resolve/state.near_names`, exact because `nearest` depends on the name and the candidate list alone, read 24.5 billion, the verdicts byte-identical (lane fit12's measurement, scratch only, not re-run by the coordinator) | `selfhost/rename_fit.hero`, `selfhost/resolve/state.hero` · panel 187, defect 220 · defect 372 · **class: improvement**

    **Origin:** lane b12-fit12, 2026-10-06 (its final reply, decision 1); filed by the coordinator at 11:25 (the file's name says 1126, written before the clock was read).

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): the sixteen readings are panel 187's cost, linear in the program since 372's repairs; a cost no rule promises against, outside the batch under the author's instruction of 2026-10-05. Fewer readings per rename would change which renames are made certain, which is a sitting's question, never a lane's.

    Repaired at `b7220ff8`, 2026-10-07 (lane b14-resolve), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The entry's numbers predate defect 385's repair and do not reproduce on the base (`dad2da47`): the critic's file read 24.1 billion instructions there against 1.45 with no rename to try; its cause does, the did-you-mean being two thirds of the samples at 4,000 names and nine tenths over 400 names each a slip of its own local, nearly all inside the trials. `resolve/asked.hero` keeps each answer under its question, the name and its lists each numbered by its value, so no answer reaches another question and each list is held once, and `rename_fit.judged` hands one through its trials (`resolve.resolve_asking`). Against the compiler before it, every `check --json` byte-identical: the critic's line 24.22 to 8.97 billion at 2,000 names and 47.58 to 17.06 at 4,000; 400 names each a slip of its own local 65.79 to 13.42; names nothing is near pay the lookup, 1.4% at 1,600; `check selfhost/main.hero` 66.022 against 66.028. A memo of the last query alone, the entry's form, cannot reach the second shape: lane fit12 measured it at 47.78 against 47.80.
