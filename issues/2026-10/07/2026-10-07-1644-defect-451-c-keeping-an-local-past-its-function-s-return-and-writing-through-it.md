---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **451 — C keeping an `@` local past its function's return and writing through it later is seen by no leg of the net** | a lent local lives in a reused mapped region (panel 196's R7), so ASan, its free fill and Guard Malloc are all silent when C writes through a pointer it kept past the call (lane b14-cli's case `<scratchpad>/batch14/cli/p321b/cell.hero`, not re-run by the coordinator) | `runtime/parts/lend.c` · defects 321 and 390 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-cli's final report (*found beside* 2).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach over a C library that breaks its own `lent` promise; no correct program is wrong for it.
