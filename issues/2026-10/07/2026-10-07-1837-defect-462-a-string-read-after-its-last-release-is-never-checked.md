---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: 7259a7b6cbe2c4c8022a9e87e618e5b2a91caf0b
github: none
---

- [ ] **462 — a string read after its last release is never checked** | only increments and releases look at a string's mark; a read (print, concat) does not, so the doubled control of defect 314 printed garbage bytes before panicking (lane b14-runtime); a check per read costs a load per read, unmeasured | `runtime/parts/str.c` · defect 314 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-runtime's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach over a compiler's fault; no correct program is wrong for it.

    Repaired at `7259a7b6`, 2026-10-08 (lane b15-runtime), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Every reader of a str asks the block's mark, and every reader of an array or a map the descriptor the runtime sets to none before the block goes (the same cause in the two containers beside it), inlined: the compiler's own `check` +1.2% in instructions retired at -O0 (+3.0 to +3.4% as calls), a loop of string, array and map traffic +0.8 to +1.0% at -O0 and +1.8 to +2.0% at -O2; panel 190's control, rebuilt from this tree's C with one release doubled, printed five NUL bytes on the base and stops at its first read now; cases `run/fixedbugs-462-*`, three, each red on the base.
