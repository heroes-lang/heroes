- [ ] **M-struct-passing close — the offers** | M-selfhost-port, calls as C | **print is a compiler form, not a function.** Why does hero_print_end own the newline rather than the emitter? (The answer is a header and a collision surface.)

    **Where to look:** emit_ops.hero::print_call's doc
    **Why it matters:** one #include in every generated unit is a cost paid on every FFI program forever
