- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the lowering skeleton | **A negative literal is folded at lowering, not negated at runtime.** What goes wrong with `-128` against an i8 if the minus stays an instruction? (The answer is a clang warning class and one unreachable value.)

    **Where to look:** ir_lower.hero::fold_negative
    **Why it matters:** the two passes must agree on WHERE the sign lives, or the range check and the emitted C check different numbers
