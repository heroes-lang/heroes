---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **488 — a generic function value handed to a generic function's parameter is refused `cannot_infer`, where spec § 9 read plainly would admit it** | `app(f: ident, x: 20)` with `ident<A>` and `app<A>(f: (function(A) -> A), x: A)`, `ns.map(ident)` and `ns.fold(19, keep)` are refused `cannot_infer`; the message names the narrowing itself (*hand it to a non-generic function's parameter*), defect 402's design; spec § 9 says a type parameter takes its type *from the type the context asks for* (lane b14-emit; reproduced by the coordinator at 20:01 on the trunk and the round) | `selfhost/check/`, the inference of a function value · spec § 9 · defect 402 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-emit's final report; the lane's measurement, not re-run by the coordinator. Reproduced by the coordinator before filing.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a narrowing the message itself states, so a sitting's question whether the spec's sentence or the compiler is the rule.
