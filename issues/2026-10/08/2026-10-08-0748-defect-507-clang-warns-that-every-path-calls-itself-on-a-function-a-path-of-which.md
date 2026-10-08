---
kind: defect
area: emit
milestone: none
filed: 2026-10-08
commit: none
github: none
---

- [ ] **507 — clang warns that every path calls itself on a function a path of which ends in `exit`** | `function serve(n: i64) -> i64` whose body is `if n > 3` / `exit(code: 0)`, then `print(n)`, `return serve(next(n))` with `next(n)` returning `n + 1`: `check` exit 0; `build` exit 0 printing *serve2.hero:4:38: warning: all paths through this function will call itself [-Winfinite-recursion]*; the program prints 1, 2, 3 and exits 0 (run by the coordinator before 07:48 on 2026-10-08 on the trunk's compiler, `<scratchpad>/batch15/serve/serve2.hero`; found by panel 199's completeness critic): the emitted `exit(code:)` is a call followed by a jump, nothing marking it as never returning | `selfhost/emit/`, the call of the library's `exit` · defect 457 · panel 199 · **class: blocking**

    **Origin:** filed by the coordinator at 07:48 on 2026-10-08 from panel 199's completeness critic's first pass (`docs/panel/199-reports/completeness-critic-pass1.md`, finding 2), reproduced before filing.

    **Class: blocking**, 2026-10-08 (`.claude/rules/verification.md` § Bounded discovery): a clang warning on a correct program.
