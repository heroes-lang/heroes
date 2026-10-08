---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: 7259a7b6cbe2c4c8022a9e87e618e5b2a91caf0b
github: none
---

- [x] **462 — a string read after its last release is never checked** | only increments and releases look at a string's mark; a read (print, concat) does not, so the doubled control of defect 314 printed garbage bytes before panicking (lane b14-runtime); a check per read costs a load per read, unmeasured | `runtime/parts/str.c` · defect 314 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-runtime's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach over a compiler's fault; no correct program is wrong for it.

    Repaired at `7259a7b6`, 2026-10-08 (lane b15-runtime), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Every reader of a str asks the block's mark, and every reader of an array or a map the descriptor the runtime sets to none before the block goes (the same cause in the two containers beside it), inlined: the compiler's own `check` +1.2% in instructions retired at -O0 (+3.0 to +3.4% as calls), a loop of string, array and map traffic +0.8 to +1.0% at -O0 and +1.8 to +2.0% at -O2; panel 190's control, rebuilt from this tree's C with one release doubled, printed five NUL bytes on the base and stops at its first read now; cases `run/fixedbugs-462-*`, three, each red on the base.

## The repair

Repaired at `7259a7b6`, 2026-10-08 (lane b15-runtime), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Every reader of a str asks the block's mark, and every reader of an array or a map the descriptor the runtime sets to none before the block goes (the same cause in the two containers beside it), inlined: the compiler's own `check` +1.2% in instructions retired at -O0 (+3.0 to +3.4% as calls), a loop of string, array and map traffic +0.8 to +1.0% at -O0 and +1.8 to +2.0% at -O2; panel 190's control, rebuilt from this tree's C with one release doubled, printed five NUL bytes on the base and stops at its first read now; cases `run/fixedbugs-462-*`, three, each red on the base.

**Closed 2026-10-09**, after the push's platform legs, this defect being at the C boundary or changing what clang gets on every platform (`.claude/rules/verification.md` § The batch): batch 15 closed on this Mac alone (`811f8398`) and the CI's legs ran its cases afterwards, run 37844345400, created at 23:06 on 2026-10-08 and its Windows leg finished at 01:31 on 2026-10-09. Darwin arm64 read the net 6,960 passed and 0 failed, Linux arm64 and Linux x86-64 6,941 each, and on those three legs the compiler's own tests 1,478, the module's 269 and the net's own tests 318, all passed; the Windows leg read the compiler's own tests 1,478 and the module's 269, all passed, and the net 6,811 passed and 1 failed, defect 322's sanitiser case alone (defect 509), its net's own tests not reached. This defect's cases are not among any leg's SKIP lines, read from the four logs, and passed on every leg; defect 447's vcpkg step ran on the Windows leg and its SDL3 case ran there, where batch 14's leg had skipped it. Defect 463's case is the hand-written one its lane ran on a Linux with SELinux enforcing, which no leg is.
