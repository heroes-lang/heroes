---
kind: feature
area: check
milestone: M-inferred-cell
filed: 2026-10-10
commit: e2165913357c4d2594fb7df6f881c078e9918d72
github: https://github.com/heroes-lang/heroes/issues/1967
---

- [x] **M-inferred-cell** | the type optional on `@=` and inferred from the value as `=`'s is (panel 209's R1, the landing's third commit): `declare.ty` an `i64?`, the `declare` arm of `check/walk.hero` mirroring the `bind` arm; `cannot_infer` where the value cannot say the type, `[]`, `{}`, `fail(`, `ok(`, a case name and a bare `nullptr`, told at the declaration with a `guess` fix naming the annotation (`db: <Handle> @= nullptr`) and the cell poisoned so the call below says nothing more; the fix text's `what` so it writes `@=` for a cell; the width class (a cell born of a literal and later written a width type, 15 sites in 4 files of `selfhost/` by the engineer's stripped copy) told at the mutation with a note naming the declaration and a `guess` fix annotating it; `v @= v + 1` and `v @= 1` in an inner block on a declared `v` one message with a certain fix `v @ v + 1`. No annotation the tree holds is stripped | the synthesis § The resolution; the engineer's report § 3, § 6 and § 7, `docs/panel/209-reports/compiler-engineer.md`; the ffi seat's conditions, `docs/panel/209-reports/ffi-pragmatist.md` § 10

    **Origin:** panel 209's synthesis, 2026-10-10, ratified 17:57; filed at the milestone's opening, 19:21.

    **Closed** at step 3, `e2165913`, 2026-10-10: `declare.ty` optional, `born` in `check/walk.hero`, `cannot_infer` at the birth with the cell poisoned, the `nullptr` rule, the width class's note and fix, `cell_redeclared` and `declared_place` each with a certain fix; nine goldens and two questions of `docs/learn/`.

## What was done

The type became optional on `@=` and inferred from the value as `=`'s is:
`declare.ty` optional, `born` in the checker's walk, `cannot_infer` told at
the birth with a `guess` fix naming the annotation and the cell poisoned, the
`nullptr` rule, the width class's note and fix, and `cell_redeclared` and
`declared_place` each with a certain fix; nine goldens and two questions of
`docs/learn/`, and no annotation the tree holds was stripped. Landed at
`e2165913`, "M-inferred-cell step 3: a cell's type is optional and inferred
from its value, with what the value cannot say told at the birth".
