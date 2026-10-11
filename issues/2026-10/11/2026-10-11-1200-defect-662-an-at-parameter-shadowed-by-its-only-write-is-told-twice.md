---
kind: defect
area: resolve
milestone: none
filed: 2026-10-11
commit: a2167890c0036d0341a4aac2eba1fdf83fc4fa2d
github: none
---

- [ ] **662 — an `@` parameter whose only write is mis-spelled `=` is told twice, never written and shadowed** | `function grow(@rows: [i64])` over `rows = rows.push(1)`: `check` tells `shadowed_binding` at line 2, the one mistake, and since panel 210 (batch 21) also `never_rebound` at the parameter, *`rows` is an `@` parameter and nothing writes it*, because the shadowing refusal declares nothing and counts no write, so the sweep finds the parameter written nowhere. The write was meant, and spelled with the wrong symbol; one message is owed, the shadowing one, whose words already say the name is bound above. Measured by panel 187's R2 over the plan cut on 2026-10-11 (defect 661), the trunk at `341bebcd` against `397381f1`: six `assign-equals` mutants told `1 -> 2`, in `selfhost/name_rows.hero`, `tests/golden/emit/copy-out.hero` and four `tests/golden/run/` programs; reproduced by hand with the four lines above. The resolver already keeps a cell meant to be written out of `never_rebound`'s sweep (`resolve/meant.hero`); a shadowing binding whose name is an `@` parameter in scope is the same intent, and counting that write on the parameter where the shadowing is refused is the likely repair | `selfhost/resolve/state.hero` `declare` (the shadowing refusal), `selfhost/resolve/written_nowhere.hero` (panel 210's sweep and its message), `selfhost/resolve/meant.hero` (the writes counted where they were meant) · **class: adjacent**

    **Origin:** found by the landing session of panel 209 at 11:53 on 2026-10-11, reading panel 187's R2 over the recovery plan cut again for defect 661, the first R2 run after batch 21 landed panel 210.

    **Class: adjacent**, 2026-10-11: a second message for one mistake, found beside the work, the one message it repeats being true and its fix certain.

    Repaired at `a2167890c0036d0341a4aac2eba1fdf83fc4fa2d`, 2026-10-11 (lane b22-662), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `resolve/meant.bind` counts a `=` over a mutable local in scope, an `@` parameter or a cell, as that local's write, and the shadow is the one message, offering `@` at the `=` as a `guess` where no type is written: a shadow inside a block meant a name the block alone sees, and `@` writes the local past it. The cell shape beside, `v @= 0` over `v = v + 1`, lost the same second message and its `certain` `=` fix, which left the shadow standing. Shapes measured, the trunk's compiler against the lane's: the reproducer, a branch, a loop and a typed or untyped cell each 2 messages to 1; a real `@` write beside the shadow and an `@`-less parameter shadowed 1 and 1; a field written with `=` the parser's one message, no shadow. Five `tests/golden/check/fixedbugs-662-*` cases; check 695/0 whole, annotations and fixes on fixedbugs-662 5/0, never-written 3/0 and fixedbugs-63 5/0, full 38/0, layout on both modules 4/0, own tests 1617 passed.
