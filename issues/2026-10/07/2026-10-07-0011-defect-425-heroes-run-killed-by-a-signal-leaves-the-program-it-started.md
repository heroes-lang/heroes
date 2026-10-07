---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: 558056c01921bb0e366e264c26f7ff73b235a5da
github: none
---

- [ ] **425 — `heroes run` killed by a signal leaves the program it started running** | `timeout 3 heroes run forever.hero`, the program a loop with no end: `timeout` exits 124 and the built program runs on with parent 1, an orphan, until killed by hand; the runtime gives the child its own process group (`runtime/parts/run.c:810`, so the terminal's Ctrl-C reaches it) and `heroes` forwards no signal when it is itself ended (the coordinator, 2026-10-07 00:09); how the disk filled on 2026-10-06, 146 GB written by panel 196's critic's libuv probe under `timeout 60 heroes run` in 44 minutes | `runtime/parts/run.c` (`hero_run_go`, the child's group), the `run` verb in `selfhost/cli/` · **class: blocking**

    **Origin:** the coordinator, 2026-10-07, finding why panel 196's critic's probe outlived its `timeout`; reproduced at 00:09.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a process the author believes stopped keeps running and writing (robustness, design.md §1.12): it filled the data volume and stopped every lane.

    Repaired at `558056c0`, 2026-10-07 (lane b13-tmpl407): while `hero_run_go` waits on a child it catches SIGTERM, SIGINT, SIGHUP and SIGQUIT where they were at their default, sends the same signal to every child's process group, waits 2 seconds, SIGKILLs any group still standing, hands the terminal back and dies of that signal (`runtime/parts/run.c`, the POSIX side, async-signal-safe, the ABI unchanged); the watchdog sends SIGTERM then SIGKILL after 5 seconds; the child's dispositions and mask restored before exec; Ctrl-C still reaches the program in front; Windows already correct by its job object's `KILL_ON_JOB_CLOSE`. Case `run/fixedbugs-425-a-program-ended-by-its-watchdog-ends-the-one-it-runs`, three generations of a program, no survivor; probes on this Mac, Linux arm64 and the Windows box, each followed by `pgrep`, 0 survivors (the lane's). A SIGKILL to `heroes` still orphans its child, no process being able to catch it; a C-boundary defect, so it closes after the batch's platform legs; the card filled by the coordinator.
