- [ ] **panel 029** | A readable mangled suffix is **not injective**: `pair<int, str_x>` and `pair<int_str, x>` both spell `h_m_pair_int_str_x`. Task: construct the third pair of Heroes declarations that collides, then say what `mangle.rs`'s existing injectivity argument for `module_of` has to do with it

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/mangle.rs · docs/panel/029 R5
    **Why it matters:** the readable name was the obvious choice and it rejects a legal program
