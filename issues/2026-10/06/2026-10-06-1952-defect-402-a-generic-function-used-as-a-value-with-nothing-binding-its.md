---
kind: defect
area: ir
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **402 — a generic function used as a value with nothing binding its type stops the build** | `function ident<A>(x: A) -> A` and `_ = ident` in `main`: `check` 0, then `run` exit 2 with *internal error: monomorphisation produced an ill-formed program* (the trunk's compiler at `6a03c488`, run by the coordinator at 19:52 on 2026-10-06; lane b13-unit measured the same on Linux arm64 and x86-64) | a function value's type parameters left unbound, `selfhost/check/` and `selfhost/ir/mono.hero` · spec § 9 (function values, generics) · **class: blocking**

    **Origin:** lane b13-unit, 2026-10-06 (its report, *found beside*), beside defect 398; reproduced by the coordinator.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told; `check` accepts what the build cannot make. Into batch 13.
