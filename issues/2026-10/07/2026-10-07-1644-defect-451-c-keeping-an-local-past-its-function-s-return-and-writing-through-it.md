---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: 8ad30b70652f40e546ae940e29d34d0fafe5327c
github: none
---

- [x] **451 — C keeping an `@` local past its function's return and writing through it later is seen by no leg of the net** | a lent local lives in a reused mapped region (panel 196's R7), so ASan, its free fill and Guard Malloc are all silent when C writes through a pointer it kept past the call (lane b14-cli's case `<scratchpad>/batch14/cli/p321b/cell.hero`, not re-run by the coordinator) | `runtime/parts/lend.c` · defects 321 and 390 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-cli's final report (*found beside* 2).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach over a C library that breaks its own `lent` promise; no correct program is wrong for it.

    Repaired at `8ad30b70`, 2026-10-08 (lane b15-runtime), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A region given back is watched until it is taken again: up to a mebibyte its lent bytes are summed and the sum asked again at the next lend at its depth, its thread's end and the program's end; above a mebibyte its pages are released and its address sealed, so the reach faults and is named where it was exit 139 with no word; under `--sanitize` every region given back is sealed and quarantined, never taken again, so a read and a write while a later lend holds the region fault too. Cost on a loop lending an `@` local and a 32-byte buffer, instructions retired: +10 to +14% summed, x7.8 to x18.4 had every room been sealed in every build (measured, not landed, and the coordinator's to choose). A plain build sees no read and not the region taken again, two cases pinning both; cases `run/fixedbugs-451-*`, six, each red on the base. The Windows arm (decommit, commit again) is unrun there.

## The repair

Repaired at `8ad30b70`, 2026-10-08 (lane b15-runtime), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A region given back is watched until it is taken again: up to a mebibyte its lent bytes are summed and the sum asked again at the next lend at its depth, its thread's end and the program's end; above a mebibyte its pages are released and its address sealed, so the reach faults and is named where it was exit 139 with no word; under `--sanitize` every region given back is sealed and quarantined, never taken again, so a read and a write while a later lend holds the region fault too. Cost on a loop lending an `@` local and a 32-byte buffer, instructions retired: +10 to +14% summed, x7.8 to x18.4 had every room been sealed in every build (measured, not landed, and the coordinator's to choose). A plain build sees no read and not the region taken again, two cases pinning both; cases `run/fixedbugs-451-*`, six, each red on the base. The Windows arm (decommit, commit again) is unrun there.

**Closed 2026-10-09**, after the push's platform legs, this defect being at the C boundary or changing what clang gets on every platform (`.claude/rules/verification.md` § The batch): batch 15 closed on this Mac alone (`811f8398`) and the CI's legs ran its cases afterwards, run 37844345400, created at 23:06 on 2026-10-08 and its Windows leg finished at 01:31 on 2026-10-09. Darwin arm64 read the net 6,960 passed and 0 failed, Linux arm64 and Linux x86-64 6,941 each, and on those three legs the compiler's own tests 1,478, the module's 269 and the net's own tests 318, all passed; the Windows leg read the compiler's own tests 1,478 and the module's 269, all passed, and the net 6,811 passed and 1 failed, defect 322's sanitiser case alone (defect 509), its net's own tests not reached. This defect's cases are not among any leg's SKIP lines, read from the four logs, and passed on every leg; defect 447's vcpkg step ran on the Windows leg and its SDL3 case ran there, where batch 14's leg had skipped it. Defect 463's case is the hand-written one its lane ran on a Linux with SELinux enforcing, which no leg is.
