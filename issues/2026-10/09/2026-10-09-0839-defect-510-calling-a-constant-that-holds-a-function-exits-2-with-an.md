---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: 4d12ddacac9be125e979e0198047c00a35212a41
github: none
---

- [ ] **510 — calling a constant that holds a function exits 2 with an internal error** | `DOUBLE(4)`, where a constant `DOUBLE` holds a function: the generated C calls the constant's zero-argument accessor with an argument and the build exits 2, *internal error*; binding it first, `h = DOUBLE` then `h(4)`, works (lane b16-land199; its reproducer `.claude/worktrees/scratch-b15/land199/beside/constcall.hero`, ignored by git) | the call of a constant of function type, `selfhost/ir/` and `selfhost/emit/` · **class: blocking**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b16-land199's final report; the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 on a correct program.

    Repaired at `4d12ddac`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The lowering called the constant's zero-argument accessor with the arguments where the checker had sent the call through its value; it now reads the constant and calls what it holds. The qualified spelling `helper.DOUBLE(4)` was a second door on the same path, the checker answering the error type with no diagnostic, so `check` passed `helper.DOUBLE(4, 5)` at exit 0 and every build stopped at exit 2; it is checked through the constant's type now. Four `run/` cases and two surface rows over `constcall510/`, each red on the base; emission 1,074 with no blessed emission moved, the compiler's own tests 1,504.
