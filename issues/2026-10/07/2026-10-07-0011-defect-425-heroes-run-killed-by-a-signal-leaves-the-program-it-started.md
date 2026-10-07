---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **425 — `heroes run` killed by a signal leaves the program it started running** | `timeout 3 heroes run forever.hero`, the program a loop with no end: `timeout` exits 124 and the built program runs on with parent 1, an orphan, until killed by hand; the runtime gives the child its own process group (`runtime/parts/run.c:810`, so the terminal's Ctrl-C reaches it) and `heroes` forwards no signal when it is itself ended (the coordinator, 2026-10-07 00:09); how the disk filled on 2026-10-06, 146 GB written by panel 196's critic's libuv probe under `timeout 60 heroes run` in 44 minutes | `runtime/parts/run.c` (`hero_run_go`, the child's group), the `run` verb in `selfhost/cli/` · **class: blocking**

    **Origin:** the coordinator, 2026-10-07, finding why panel 196's critic's probe outlived its `timeout`; reproduced at 00:09.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a process the author believes stopped keeps running and writing (robustness, design.md §1.12): it filled the data volume and stopped every lane.
