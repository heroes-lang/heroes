---
kind: defect
area: ir
milestone: none
filed: 2026-10-03
commit: fcdddfa56caad12943bc56923f44d36773533b02
github: none
---

- [ ] **261 — the IR builder's `build.slot` and `build.param` copy a growing table through a struct field** | 320,499 copies on `one-return-many-800` (batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/emit/shapes/` and `<scratchpad>/round1003d/shapes2000/`), the pattern batch 8's 230 repaired in three other places | `selfhost/ir/build.hero:254` and `:259` (`param`, `slot`) · **class: improvement**

    **Origin:** batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the slots' square; no program refused or wrong.

    Repaired at `fcdddfa5`, 2026-10-07 (lane b14-ir), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The builder's slots, parameters, blocks and path steps are lent to helpers that grow them in place and a join's predecessor list is hoisted, so `--dump-ir` is byte-identical on 20 shapes while slot copies at one-return-many-800 fall from 325,450 to 7,501 and block copies at slots-returns-800 from 2,896,203 to 18,908 (instrumented copies of both compilers).
