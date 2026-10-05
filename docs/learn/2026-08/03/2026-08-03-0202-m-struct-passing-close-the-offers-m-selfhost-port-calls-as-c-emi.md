- [ ] **M-struct-passing close — the offers** | M-selfhost-port, calls as C (emit_ops.hero) | **Four callee kinds, four C spellings — and only ONE goes through unmangled.** Say which, why, and what the cstr guard wraps around its arguments (and which parameter kind is deliberately excluded)

    **Where to look:** emit_ops.hero, the extern test
    **Why it matters:** the linkage recorded in the IR pays off here: the emitter never re-derives who is being called
