---
kind: defect
area: resolve
milestone: none
filed: 2026-10-11
commit: none
github: none
---

- [ ] **662 — an `@` parameter whose only write is mis-spelled `=` is told twice, never written and shadowed** | `function grow(@rows: [i64])` over `rows = rows.push(1)`: `check` tells `shadowed_binding` at line 2, the one mistake, and since panel 210 (batch 21) also `never_rebound` at the parameter, *`rows` is an `@` parameter and nothing writes it*, because the shadowing refusal declares nothing and counts no write, so the sweep finds the parameter written nowhere. The write was meant, and spelled with the wrong symbol; one message is owed, the shadowing one, whose words already say the name is bound above. Measured by panel 187's R2 over the plan cut on 2026-10-11 (defect 661), the trunk at `341bebcd` against `397381f1`: six `assign-equals` mutants told `1 -> 2`, in `selfhost/name_rows.hero`, `tests/golden/emit/copy-out.hero` and four `tests/golden/run/` programs; reproduced by hand with the four lines above. The resolver already keeps a cell meant to be written out of `never_rebound`'s sweep (`resolve/meant.hero`); a shadowing binding whose name is an `@` parameter in scope is the same intent, and counting that write on the parameter where the shadowing is refused is the likely repair | `selfhost/resolve/state.hero` `declare` (the shadowing refusal), `selfhost/resolve/written_nowhere.hero` (panel 210's sweep and its message), `selfhost/resolve/meant.hero` (the writes counted where they were meant) · **class: adjacent**

    **Origin:** found by the landing session of panel 209 at 11:53 on 2026-10-11, reading panel 187's R2 over the recovery plan cut again for defect 661, the first R2 run after batch 21 landed panel 210.

    **Class: adjacent**, 2026-10-11: a second message for one mistake, found beside the work, the one message it repeats being true and its fix certain.
