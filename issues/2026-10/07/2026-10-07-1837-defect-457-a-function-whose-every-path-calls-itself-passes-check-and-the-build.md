---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **457 — a function whose every path calls itself passes `check`, and the build shows clang's warning over the generated C** | `function forever(n: i64) -> i64` returning `forever(n)`: `check` exit 0; `build` exit 0 printing clang's *warning: all paths through this function will call itself [-Winfinite-recursion]* over the emitted C; the run aborts *stack exhausted*, exit 134 (lane b14-resolve; the same shape behind defect 455's `bytes`, run by the coordinator before 16:48: `<scratchpad>/batch14/shadow/`) | the checker, which has no rule for it; the build, which passes clang's warning through in C words · defect 455 · **class: systemic**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-resolve's final report; the lane's measurement, not re-run by the coordinator. Its `bytes` form reproduced by the coordinator before filing defect 455.

    **Class: systemic**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a wrong program accepted and a clang warning in C words reaching the author (`blocking` by the letter), whose repair is a new checker rule or a new reading of clang's warning: a ruling no rule reaches, a sitting's (CLAUDE.md § 4).
