- [ ] **M-ir-lowering.1** | Four instructions: `add`, `add!`, `lt`, `index!`. Which can stop the program, and what makes `add` and `add!` different — the operation or the operands?

    **Where to look:** ir/print_inst.rs (aborts) · spec lines 126–128
    **Why it matters:** the reader who misses this writes a pass that assumes one exit per block
