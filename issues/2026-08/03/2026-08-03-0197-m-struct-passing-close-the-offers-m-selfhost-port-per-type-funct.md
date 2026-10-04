- [ ] **M-struct-passing close — the offers** | M-selfhost-port, per-type functions complete | **A T?'s retain does NOT go through the descriptor's copy — why?** What two things would going through it cost?

    **Where to look:** emit_perfn.hero::one_reference's comment
    **Why it matters:** a descriptor has copy(dst,src) and no retain-in-place: the wrong reuse means copying a value onto itself and casting away a const
