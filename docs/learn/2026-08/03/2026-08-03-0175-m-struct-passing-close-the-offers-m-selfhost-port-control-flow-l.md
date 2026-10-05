- [ ] **M-struct-passing close — the offers** | M-selfhost-port, control flow lowered | **&& is not an instruction anywhere in the IR.** Find the branch polarity in short_circuit: which edge evaluates the right side for &&, and which for \|\|? Then say why `f() && g()` MUST be lowered this way (§4.14)

    **Where to look:** ir_lower.hero::short_circuit, the polarity test
    **Why it matters:** if && were an instruction, g() would run when f() is false — visible side effects in the wrong world
