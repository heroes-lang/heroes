---
kind: defect
area: resolve
milestone: none
filed: 2026-10-07
commit: da3193a8471f89865f75f0761e1214b418254c04
github: none
---

- [ ] **439 — an `@` passed to an unknown callee keeps its mark** | an `@` passed to a call whose callee name is unknown, or past the last parameter, keeps its mark, so with a name bound by `=` it can be told `not_mutable` beside `unknown_name` or the arity error | `selfhost/resolve/built_marks.hero` · **class: improvement**

    **Origin:** filed by the coordinator at 04:40 on 2026-10-07, from lane b13-zero401's report (*found beside*); the lane's reading, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a second message beside a mistake already told, at an edge.

    Repaired at `da3193a8`, 2026-10-07 (lane b14-resolve), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The mark now stays only on an argument whose parameter, at its position, takes an `@` (a function's, or `end_lease`'s one), so past the last parameter, at a name nothing binds, a module, a constant, a variant or a record called UFCS-style it is set aside untold, and the callee's own mistake is the one message: nine shapes of the base's `not_mutable` (and one `aliased_mutable_arguments`) now read `wrong_arity`, `not_callable`, `unknown_name`, `module_is_not_a_value` or `variant_in_value_position` alone; `check` 585, `full` 19, `permissive` 10, the compiler's own tests 1,358, all passed.
