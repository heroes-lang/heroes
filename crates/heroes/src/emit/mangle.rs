//! Every name that reaches C, in one place (design.md §3.1, CLAUDE.md §7).
//!
//! The rule is `h_<module>_<name>[_<typehash>]`, and the reason is collisions in
//! two directions: a Heroes identifier can be a C keyword (`default`,
//! `register`) or a libc symbol (`index`, `y1`), and — the direction panel 019
//! found by compiling it — an *unmangled* Heroes `function open` silently replaces
//! libc's. The prefix answers both at once, which is why `extern` names, and only
//! they, pass through untouched.
//!
//! **The module component is sanitised to `[A-Za-z0-9]`**, and that one line is
//! what makes the scheme injective. Without it `h_<module>_<name>` is ambiguous
//! wherever both halves may contain `_`: module `print` with name
//! `inst_value_name` and module `print_inst` with name `value_name` are the same
//! symbol — and those are this compiler's own file names, so M8a would have met it
//! immediately. With the module alphanumeric, the first `_` after `h_` ends it.
//! Nim reserves `__` for the same purpose and can do so because its identifiers
//! forbid it; Heroes does not forbid `__`, so the separator cannot be widened and
//! the *component* is narrowed instead.
//!
//! Residual and recorded: `print_inst.hero` and `printinst.hero` are one module.
//! The failure is a duplicate-symbol error from the linker, which is loud — and at
//! M8a a module is a declared name rather than a file stem, so the shape stops
//! being a stem-sanitising question at all.
//!
//! There is no `_<typehash>` yet. It exists for monomorphisation (§4.12) and a
//! generic function does not reach this backend (`gate.rs`).

/// The module a source path stands for, until modules exist (M8a).
///
/// The file's stem, alphanumerics only. Deterministic from the path *as given* —
/// `./x.hero`, `x.hero` and `/tmp/x.hero` all yield `x`, which is what keeps
/// `--emit-c` byte-identical across invocations that name the same file
/// differently.
pub fn module_of(path: &str) -> String {
    let file = path.rsplit(['/', '\\']).next().unwrap_or(path);
    let stem = file.split('.').next().unwrap_or(file);
    let kept: String = stem.chars().filter(|c| c.is_ascii_alphanumeric()).collect();
    if kept.is_empty() {
        // A file called `.hero` or `---.hero`. It has to become *something*, and
        // one fixed word is better than an empty component, which would make
        // `h__main` collide with a module named `_`.
        return "anon".to_string();
    }
    kept
}

/// A Heroes function or constant.
pub fn function(module: &str, name: &str) -> String {
    format!("h_{module}_{name}")
}

/// A slot: a parameter, a local, a mutable cell, or one lowering invented.
///
/// The index is in the name because two slots may share a spelling — a synthetic
/// `$i` beside a user `i` — and because a C keyword must not be reachable by
/// accident. `$`, which no Heroes program contains, is dropped: `$i` in slot 5
/// becomes `h5_i`.
pub fn slot(index: u32, name: &str) -> String {
    let kept: String = name
        .chars()
        .map(|c| if c.is_ascii_alphanumeric() || c == '_' { c } else { '_' })
        .collect();
    format!("h{index}_{}", kept.trim_start_matches('_'))
}

/// The pointer a mutable parameter arrives through (§4.8's ABI, panel 020).
pub fn out_param(index: u32, name: &str) -> String {
    format!("p{}", slot(index, name))
}

/// A temporary. Assigned once by construction, so the name needs no scope.
pub fn value(index: u32) -> String {
    format!("t{index}")
}

/// A basic block's label. Not a user name, but it goes through here anyway so
/// that "every name that reaches C" has exactly one home.
pub fn block(index: usize) -> String {
    format!("bb{index}")
}
