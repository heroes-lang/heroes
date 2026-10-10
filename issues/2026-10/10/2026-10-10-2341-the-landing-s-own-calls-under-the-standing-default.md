---
kind: decision
area: compiler
milestone: M-inferred-cell
filed: 2026-10-10
commit: self
github: none
---

**The landing's own calls, taken under the standing default** (the author's *5a* of 2026-10-04: a batch's questions take the recommended answer, said once in the record). Panel 209's resolution bound the landing in its § The landing; where the lane had to choose, it chose as follows, each measured in its step's commit body, and the author can turn any of them.

- **A cell born of an empty literal is poisoned, and so is a binding's** (step 3, `e2165913`): the `cannot_infer` told at the birth is the one message, and a write into the cell below says nothing; the `=` binding was already quiet on its reads and is unchanged in what it says, the poison reaching it only where a literal is checked against it. Reason: design.md §4.17, one mistake one message; measured by `inferred-cell-empty-literal-names-the-cell` and by the whole `check` form, 668 and 0.
- **`f() @= 1` is `not_a_place` with the symbol named, not a new code** (step 3): the class is the one `f() @ 1` belongs to, and a second code for one class would be a second rule to witness. Reason: spec § 5's `Place` is one production; measured by `no-place-before-the-cell-symbol`.
- **`x @= 2` over an `x` bound with `=` stays `shadowed_binding`** (step 3): a `=` binds once, so no certain fix writes `@`, and the message is the shadowing one. Reason: a certain fix repairs the defect the diagnostic names, and `x @ 2` would be `not_mutable`; measured by `cell-redeclared-over-a-binding-is-shadowing`.
- **The width class note fires on an integer literal's birth and an integer width's write, and on nothing else** (step 3, `check/width_class.hero`): a `str` written into such a cell takes the plain mismatch. Reason: the note's fix annotates the cell with the width the write holds, which is a repair only for a width.
- **The messages that write a declaration's shape write `v @= 0` with no type** (step 4, `7a0a7b2e`): `not_mutable`, `marker_mismatch`, `unknown_name`'s note, `var`. Reason: R9 says they move with the shape, and the shape is now the inferred one; the site's two chapters show the same.
- **The two mutation operators are `inverse-at` and `bind-as-cell`** (step 4): `v @ e` to `v @= e` on a bare name, and `x = e` to `x @= e` on a binding with no written type. Reason: each is the one keystroke the new symbol makes possible, and each has a rule that kills it; their kill rates are panel 187's R2 after the push.
- **`selfhost/parse/`'s budget rose 8699 to 8735, written beside the row** (step 2): the two readers of the symbol are read beside their neighbours, never moved to fit a count. Reason: the row's own rule for a raise, and the sitting's engineer bound (*or a DECIDED row saying why the budget moves*).
- **The gallery's example refused at step 1 is repaired in step 2 and the gap filed, not repaired here** (defect 606, `adjacent`): a suite over `examples/gallery/*.hero` is a harness change outside this milestone's files. Reason: `.claude/rules/verification.md` § Bounded discovery, one class per item.
