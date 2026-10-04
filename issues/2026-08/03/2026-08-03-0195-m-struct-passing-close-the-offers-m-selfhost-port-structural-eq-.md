- [ ] **M-struct-passing close — the offers** | M-selfhost-port, structural eq/hash | **Why does a fixed array member unroll instead of comparing pointers or memcmp?** Two wrong shortcuts, two different silent failures — name both

    **Where to look:** emit_structural.hero::field_eq_expr's fixed arm
    **Why it matters:** a->f == b->f compiles clean and compares ADDRESSES; memcmp reads padding — both lie without a diagnostic
