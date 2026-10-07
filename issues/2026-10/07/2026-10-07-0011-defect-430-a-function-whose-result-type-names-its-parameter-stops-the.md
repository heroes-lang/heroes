---
kind: defect
area: ir
milestone: none
filed: 2026-10-07
commit: b594a130dd5e83a003b0b3e95531ca869a4cfe65
github: none
---

- [ ] **430 — a function whose result type names its parameter stops the build** | `function pick() -> (function(x: i64) -> i64)` returning `inc`: `check` 0, then `run` exit 2, *internal error: the lowered program is not well formed* and *the verifier refused* (the coordinator, 00:11); the unlabelled `(function(i64) -> i64)` runs, and so does a labelled binding type (the lane's) | the lowering of a function value returned against a labelled function type, `selfhost/ir/` · **class: blocking**

    **Origin:** filed by the coordinator at 00:11 on 2026-10-07, from lane b13-zero401's report of the evening before (*found beside*); reproduced by the coordinator on round b13's compiler.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told, `check` accepting what the build cannot make.

    Repaired at `b594a130`, 2026-10-07 (lane b13-gen402): the verifier's two return checks compare function types that differ only in their parameters' names as one (`ir/agree.hero`, used by `ir/verify.hero` and `ir/one_exit.hero`), the checker untouched; cases `run/` and `check/fixedbugs-430-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; the card filled by the coordinator.
