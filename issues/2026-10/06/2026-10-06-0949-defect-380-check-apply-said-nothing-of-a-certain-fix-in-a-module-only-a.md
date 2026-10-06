---
kind: defect
area: cli
milestone: none
filed: 2026-10-06
commit: de2e9770dea52393fa9cf4d1b0bcb3f0284b7fb0
github: none
---

- [ ] **380 — check --apply said nothing of a certain fix in a module only a later round reads** | `check main.hero --apply` over a `main.hero` that quotes its `use` (`use "geom"`) and a `geom.hero` holding `fn` writes `use geom` at exit 0 and says nothing on stderr; `check` of the program it wrote exits 1 on `geom.hero`'s `fn`, its fix `certain` (the trunk's compiler at `0f48f9f9`, run by the coordinator at 09:49 on 2026-10-06 over `tests/golden/surface-fixtures/applylater/`) | `selfhost/cli/check.hero`, `settle` reading the program again each round and `elsewhere` gathered from the first round alone · panel 193's R4, `not_applied` · **class: adjacent**

    **Origin:** lane b12-cli12, 2026-10-06, found beside defect 371 (its commit `de2e9770`'s body and its final report); numbered and filed by the coordinator at 09:49, after the machine's restart, under the author's instruction of 2026-10-05: any defect found that is not an improvement goes into the batch.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a note missing, the program written right; `--apply` says a certain fix it did not write in another module when the first round finds it, and not when a later round does.

    Repaired at `de2e9770`, 2026-10-06 (lane cli12), gated by its cases: `tests/golden/surface-fixtures/applylater/` and one `surface` row, red on the base's compiler; every round's fixes in another module are gathered once each and said after the last round. The table count it moved was repaired at `debc3068`. The net is owed at the batch's close.
