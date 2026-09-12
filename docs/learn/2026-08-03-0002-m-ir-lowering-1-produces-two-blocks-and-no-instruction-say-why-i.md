- [ ] **M-ir-lowering.1** | `a && b` produces two blocks and no `and` instruction. Say why in one sentence — and then say which spec line forces it

    **Where to look:** spec/heroes-spec.md § Operators · ir/control.rs (short_circuit)
    **Why it matters:** a construct that evaluates one side conditionally IS control flow, and calling it an operator hides that
