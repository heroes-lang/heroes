- [ ] **M-ir-lowering.2** | `g.rows[r].cells[c] @ v` printed as `store g.rows[$t1].0[$t2]` until the fix. Say what the printer had lost, and why only the *second* field name was affected — not the first

    **Where to look:** ir/print_names.rs (field_type) · tests/golden/ir/place-paths.expected
    **Why it matters:** a path is a fold, and dropping the accumulator only shows up from the second step on
