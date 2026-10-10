---
kind: defect
area: check
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **578 — an operator over literals passed to a generic function's parameter takes no type from the call's context** | on batch 18's round (`e0aeb991`, defects 541 and 564 landed), `y: u8 = id(a: 2 + 3)` with `id<T>(a: T) -> T` is refused *expected `u8`, found `i64`*, while `y: u8 = id(a: 5)` builds and prints 5; panel 206's critic reads the same of `first(a: 2 + 3, b: 1)` and `first(a: 1, b: 2 + 3)` while `first(a: 2, b: 1)` is accepted; spec § 9's V3T, landed in the round, says a type parameter takes its type *else from the type the context asks for, else from a generic call or a literal among them*, so a reader accepts all three; reproduced by the coordinator at 05:01 with the round's compiler (`.claude/worktrees/scratch-b15/r578/`) | defect 541's waiting forms (`selfhost/check/waiting.hero` in batch 18's round, 2026-10-10) and 564's operand typing: an operator whose operands are all literals waits as a literal does · defects 541 and 564 · **class: blocking**

    **Origin:** found by panel 206's completeness critic, first pass (its shapes `u01` to `u03` and `t01`, `.claude/worktrees/scratch-b15/critic-206/shapes/`, ignored by git), reproduced and filed by the coordinator at 05:02 on 2026-10-10.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a correct program refused, against the spec the same batch lands.
