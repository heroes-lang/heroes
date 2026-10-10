---
kind: defect
area: resolve
milestone: none
filed: 2026-10-11
commit: none
github: none
---

- [ ] **608 — `_ @= e` is accepted, a cell nothing can re-bind** | `_ @= f(x)` checks and runs as the discard `_ = f(x)` does: `resolve/state.declare` answers the wildcard with a silent `fail(code: "wildcard")` and declares nothing, so the never-re-bound rule, which reads the locals, never sees a cell; yet spec § 5 says `_` binds nothing, so there is no cell to declare and the line is the rule's own shape, a `@=` that is a `=` in disguise. Measured after M-inferred-cell's push by `heroes mutate examples --operator bind-as-cell --survivors`: 833 mutants, 785 killed (94%), 48 surviving, **47 of them `_ @= …`** and the one other in a module holding a hole, which the sweep exempts on purpose. The repair is one message at the symbol, `never_rebound` with its certain fix writing `=`, told where the resolver meets the wildcard's `@=` (`resolve/walk.hero`'s `.declare` arm), and a golden beside `tests/golden/check/never-rebound-cell.hero` | `selfhost/resolve/walk.hero` (the `.declare` arm, before `state.declare`), `selfhost/resolve/state.hero` `declare` (the wildcard's silent answer), `selfhost/resolve/errors.hero` `never_rebound`; the `bind-as-cell` row of `docs/metrics/operators.md`, whose rate this is · **class: blocking**

    **Origin:** found by the landing session of panel 209 at 00:11 on 2026-10-11, reading the two new operators' kill rates over `examples/` after the push of `m-inferred-cell` (the optimistic chain's panel 187 R2 and the operator table's own measurement); the spec-warden's prediction (3) at the sitting, *the new row kills 100%*, scored false by this shape alone.

    **Class: blocking**, 2026-10-11: a wrong program accepted, by `.claude/rules/verification.md` § Bounded discovery's list; the next batch's first item.
