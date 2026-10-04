---
kind: learn
area: runtime
milestone: M-argv-execution
filed: 2026-08-03
commit: none
github: none
---

- [ ] **M-argv-execution** | walkthrough offer: `runtime/parts/run.c` end to end — why Windows has no argv, what `win_quote` defends, where the watchdog lives

    **Where to look:** runtime/parts/run.c
    **Why it matters:** the compiler's whole contact with the OS is this one file now
