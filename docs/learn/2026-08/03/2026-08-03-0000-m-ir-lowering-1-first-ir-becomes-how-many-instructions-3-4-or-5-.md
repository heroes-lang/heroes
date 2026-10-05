- [ ] **M-ir-lowering.1** | First IR. `x = 2 + 3 * 4` becomes how many instructions — 3, 4, or 5? And which of them proves the precedence table, given the tree is gone by then?

    **Where to look:** archive/bootstrap-rs/heroes/src/ir/tests/scalars.rs (an_expression_becomes_a_line_per_operation)
    **Why it matters:** flattening is the whole job of the pass, and it is what makes the C emitter a printer
