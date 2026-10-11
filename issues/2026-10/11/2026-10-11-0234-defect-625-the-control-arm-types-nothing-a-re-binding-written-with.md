---
kind: defect
area: check
milestone: none
filed: 2026-10-11
commit: none
github: none
---

- [ ] **625 — the control arm types nothing a re-binding written with `@=` holds** | under `check --permissive`, which drops `cell_redeclared`, `v @= 0` then `v @= "text"` then `print(v + 1)` exits 0, and so does `v @ v + 1` after it, where the re-binding meant, `v @ "text"`, is refused `type_mismatch` in both arms: the resolver reads the line as `v`'s re-binding (`resolve/meant.meant_rebinding` counts a write on the `v` above and declares nothing), and the checker reads it as a cell born, its value typed against no local, so the value is never held to the cell's type | `selfhost/check/walk.hero`'s `birth` (the `.declare` arm, `born(…, cell: true)` then `state.bind_local`, which finds no local at the name); `selfhost/resolve/meant.hero` `meant_rebinding`, which records no use at the name a checker could read the re-bound local from · **class: adjacent**

    **Origin:** found by lane b21-misc at 02:34 on 2026-10-11, attacking the shapes beside defect 614's reproducer (`_ @= nullptr` under `--permissive`) with the lane's compiler after that repair (`519ac63b`): the named cell's `@=` where `@` was meant is the one other `.declare` the resolver declares nothing for. Another cause than 614's: the wildcard's line has one reading, the discard, while this one needs the checker to type the value against the local the resolver re-binds, which it has no reference to.

    **Class: adjacent**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): found beside the work in the control arm alone, as defects 193, 251 and 614 were; the normal arm refuses the line at `cell_redeclared` before the checker runs, and the arm's count reads a program accepted that every reading of the language refuses.
