- [ ] **panel? M-rich-diagnostics** | `to_i64` on a `str` is not offered (parsing can fail and the spec does not say what it returns), and ordering on `str` is not offered either (`a < b` on strings is an error, because a collation is a language decision). Both are rejections, so both are relaxable

    **Where to look:** archive/bootstrap-rs/heroes/src/types/builtins.rs, ops.rs
    **Why it matters:** two absences a model will reach for, chosen rather than overlooked
