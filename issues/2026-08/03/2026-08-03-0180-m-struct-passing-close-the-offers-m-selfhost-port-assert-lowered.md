- [ ] **M-struct-passing close — the offers** | M-selfhost-port, assert lowered (ir_lower.hero) | **A str side of an assert rides a $assert slot; an i64 side does not.** Why is the block edge the dangerous place for a counted value, and which pass makes it so?

    **Where to look:** ir_lower.hero::lower_assert's comment, the two assert tests
    **Why it matters:** the ownership pass releases an owning temporary at the end of its defining block — a block-crossing read is a use-after-free the type system cannot see
