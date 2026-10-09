---
kind: defect
area: emit
milestone: none
filed: 2026-10-08
commit: c7ca80583b3150f32e6126c3159b87bb7717e461
github: none
---

- [ ] **507 — clang warns that every path calls itself on a function a path of which ends in `exit`** | `function serve(n: i64) -> i64` whose body is `if n > 3` / `exit(code: 0)`, then `print(n)`, `return serve(next(n))` with `next(n)` returning `n + 1`: `check` exit 0; `build` exit 0 printing *serve2.hero:4:38: warning: all paths through this function will call itself [-Winfinite-recursion]*; the program prints 1, 2, 3 and exits 0 (run by the coordinator before 07:48 on 2026-10-08 on the trunk's compiler, `<scratchpad>/batch15/serve/serve2.hero`; found by panel 199's completeness critic): the emitted `exit(code:)` is a call followed by a jump, nothing marking it as never returning | `selfhost/emit/`, the call of the library's `exit` · defect 457 · panel 199 · **class: blocking**

    **Origin:** filed by the coordinator at 07:48 on 2026-10-08 from panel 199's completeness critic's first pass (`docs/panel/199-reports/completeness-critic-pass1.md`, finding 2), reproduced before filing.

    **Class: blocking**, 2026-10-08 (`.claude/rules/verification.md` § Bounded discovery): a clang warning on a correct program.

    Repaired at `67c94b96`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A call of the library's `exit` is followed by `hero_unreachable()` in the C (`emit/heroes_call.hero`), so a function whose one other path calls itself builds silently: the case's three shapes, `exit` in an arm, in an arm with an `else` and in a loop, warned 3 times at -O0 and at -O2 on the base and 0 times now, and 17 emissions moved by one such line after each call of `exit`, 56 in all. A termination through a callee that returns for other values (the spec-warden's `hidden2` and six shapes beside it) keeps clang's warning before and after; the one repair that closes that class is a flag word in `selfhost/cli/flags.hero`, reported as a patch to the coordinator for panel 199.

    Repaired at `c7ca8058`, 2026-10-09 (lane land199, panel 199's R3), its reasons condensed under `flags.hero`'s ceiling at `ce320d65`, gated by its cases and the compiler's own tests; the net is owed at the batch's close. `-Wno-infinite-recursion` is the last word of `flags.flags()`, after `-Wall`, the checker the witness since defect 457's repair: the spec-warden's `hidden2` and a recursion ending on the `exit` of a function handed to `map`, which warned falsely at both levels on the base, build silent now, and `warnings` reads 480 passed and 0 failed over the class's six correct programs (`calleeexit`, `callback`, `fieldcb`, `serve`, `hidden2` and the `map` one, each exit 0 at both levels) and defect 508's helper case, whose warning was true and is dropped where reading R4 lets the function through. Measured on Apple clang 21.0.0 and in the Linux arm64 container (Debian clang 22.1.8).
