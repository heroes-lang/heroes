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
//! `_<typehash>` arrives at M6 step 6 with monomorphisation: two copies of one
//! generic declaration are two C functions, so the type arguments have to be in
//! the name. `instance` below carries the reasoning, which three judges shaped.

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

/// The module component the library's own functions get, whatever file is being
/// compiled. `range` is `h_library_range` in every program.
///
/// The reason is M8a's, decided at panel 028 R3b with the evidence in hand: one
/// `.c` per module, every Heroes function emitted with external linkage, and two
/// modules that both call `range` are `ld: duplicate symbol` at every
/// optimisation level. The rule is one definition and many prototypes — which
/// only works if the definition has the *same* name everywhere, so the module
/// component cannot be the user's file stem.
pub const LIBRARY_MODULE: &str = "library";

/// The suffix a monomorphised instance carries: a hash of the **canonically
/// rendered** type arguments (CLAUDE.md §7's `_<typehash>`, panel 029 R5).
///
/// Three judges constrained this and the resolution takes all three.
///
/// **Not a readable suffix**, which was the obvious choice: it is **not
/// injective**. With `record int_str`, `record str_x` and `record x` in one file,
/// `pair<int, str_x>` and `pair<int_str, x>` both spell `h_m_pair_int_str_x`, and
/// clang answers `error: conflicting types` on a **legal Heroes program**. A false
/// rejection is worse than an opaque name. It also buys nothing where it would
/// matter: clang prints the `aka` expansion on every type error, so the full
/// instantiated signature is already in the message.
///
/// **Not a hash of the `TyId` sequence.** `TyId` is an interning-order artifact,
/// and the M8c fixpoint compares generated C byte for byte — a `TyId`-derived hash
/// would require the Rust bootstrap and the Heroes port to intern in *identical*
/// order. `module_of` above already refuses a path-dependent name for the same
/// reason.
///
/// **A hash of the rendering**, therefore: reproducible by any implementation from
/// the same public spelling, which is exactly the deficiency RFC 2603 records
/// against Rust's legacy scheme. The information the hash loses is given back for
/// free — the emitter writes `/* map<int, str> */` above each instance.
pub fn instance(rendered: &str) -> String {
    // FNV-1a over the rendering: short, stable, and written out rather than taken
    // from a crate, because the port must reproduce it exactly.
    let mut hash: u64 = 0xcbf2_9ce4_8422_2325;
    for byte in rendered.bytes() {
        hash ^= u64::from(byte);
        hash = hash.wrapping_mul(0x100_0000_01b3);
    }
    format!("{:x}", hash & 0xffff_ffff)
}

/// A Heroes function or constant.
///
/// **A function whose name is in the built-in inventory belongs to the library**,
/// and that is not a heuristic: `resolve/top.rs` refuses a user declaration of
/// any reserved name, and the library declares nothing else — a helper there
/// would take a name a user program is entitled to. Both halves are asserted by
/// `library::tests`, so the coupling is checked rather than assumed.
pub fn function(module: &str, name: &str) -> String {
    if crate::resolve::index_of(name).is_some() {
        return format!("h_{LIBRARY_MODULE}_{name}");
    }
    format!("h_{module}_{name}")
}

/// The same, for one monomorphised instance: the base name plus `instance`'s hash
/// of what it was instantiated at.
pub fn instance_of(module: &str, name: &str, rendered: &str) -> String {
    format!("{}_{}", function(module, name), instance(rendered))
}

/// A `record` or `variant` type, as a C struct tag and typedef name.
///
/// Same scheme as a function, and deliberately in the same namespace: C has one
/// ordinary identifier namespace for both, so `record print` and `function print`
/// would collide — and they collide *here*, at compile time, rather than at link
/// time. Heroes forbids the collision itself (a name is declared once), so this is
/// belt to that braces.
pub fn ty(module: &str, name: &str) -> String {
    format!("h_{module}_{name}")
}

/// A field, as a C struct member. Mangled too (CLAUDE.md §7 names fields
/// explicitly), because a field may be spelled `default` or `register`.
pub fn field(name: &str) -> String {
    format!("f_{name}")
}

/// One of a variant's cases: the union member that carries its payload.
pub fn case(name: &str) -> String {
    format!("c_{name}")
}

/// The payload of one case, as a C type of its own.
///
/// It needs a name rather than an anonymous `struct { … }` inside the union, because
/// `Ty::Case` is a real type in the IR — `$t5: Token.num = payload $t4 .num` puts one
/// in a temporary, and a temporary needs a declaration.
pub fn case_type(ty: &str, case_name: &str) -> String {
    format!("{ty}_{}", case(case_name))
}

/// The enum a variant's tag is drawn from.
pub fn tag_type(ty: &str) -> String {
    format!("{ty}_tag")
}

/// One enumerator of that enum.
pub fn tag_of(ty: &str, case_name: &str) -> String {
    format!("{}_{}", tag_type(ty), case_name)
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
