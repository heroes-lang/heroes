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
//! symbol — and those are this compiler's own file names, so M-module-namespace met it
//! immediately. With the module alphanumeric, the first `_` after `h_` ends it.
//! Nim reserves `__` for the same purpose and can do so because its identifiers
//! forbid it; Heroes does not forbid `__`, so the separator cannot be widened and
//! the *component* is narrowed instead.
//!
//! **This paragraph used to end "at M-module-namespace a module is a declared name rather than
//! a file stem, so the shape stops being a stem-sanitising question at all", and
//! that was false** — panel 031 had it compiled. A module name is an identifier,
//! identifiers admit `_`, and modules `geo_m` and `geom` still produce one
//! component: `error: redefinition of 'h_geom_Point'`, which CLAUDE.md §7 turns
//! into exit 2 — *the compiler is wrong* — on a program the author was entitled
//! to write. The residual is real and it is now refused as a Heroes diagnostic
//! naming both files (panel 031 R10), because the collision is a property of a
//! set of modules and belongs where that set is known.
//!
//! **`module_of` and `LIBRARY_MODULE` live in `crate::source`** since M-module-namespace. A
//! module is a language-level thing now — it is what a qualified name names —
//! and the C symbol is downstream of it, so the file table owns the answer and
//! this file spends it.
//!
//! `_<typehash>` arrives at M-generics-library step 6 with monomorphisation: two copies of one
//! generic declaration are two C functions, so the type arguments have to be in
//! the name. `instance` below carries the reasoning, which three judges shaped.

use crate::source::LIBRARY_MODULE;

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
/// and the M-selfhost-fixpoint fixpoint compares generated C byte for byte — a `TyId`-derived hash
/// would require the Rust bootstrap and the Heroes port to intern in *identical*
/// order. `source::module_of` already refuses a path-dependent name for the same
/// reason.
///
/// **A hash of the rendering**, therefore: reproducible by any implementation from
/// the same public spelling, which is exactly the deficiency RFC 2603 records
/// against Rust's legacy scheme. The information the hash loses is given back for
/// free — the emitter writes `/* map<int, str> */` above each instance.
///
/// **And it must be reproducible in Heroes, which FNV-1a was not** (author decision
/// 2026-08-12). This function used to be FNV-1a, and its own comment said the port
/// must reproduce it exactly — while all three of FNV's ingredients are
/// inexpressible here, measured: the offset basis `14695981039346656037` is
/// `int_out_of_range` because Heroes' `i64` is signed, `hash.wrapping_mul(…)` is
/// `panic: integer overflow` because §4.3 makes overflow an abort, and `^` is
/// `reserved_operator`. The closure list (§1.0) has no hashing row, no bitwise row
/// and no wrapping row, so measurement 003's audit could not see this — the second
/// time that blind spot has fired, after `system()` at panel 036.
///
/// So: a **polynomial hash, modulus 2^31 − 1**, whose every intermediate fits an
/// `i64` with three orders of magnitude to spare — `(M − 1) × B + 255 ≈ 2.8e11`
/// against `i64`'s `9.2e18`. It is written the way the port will write it, and
/// `tests/golden/run/premise-mangler-hash-in-heroes.hero` computes the same value
/// in Heroes and asserts it, so the day this stops being portable is a red test
/// rather than a discovery at the fixpoint (CLAUDE.md §11).
///
/// A collision is **loud**: two instances mangling alike emit two C definitions of
/// one name, which is a clang error and exit 2. That is what makes 31 bits enough
/// where a silent collision would not be.
pub fn instance(rendered: &str) -> String {
    let mut hash: i64 = 0;
    for byte in rendered.bytes() {
        hash = (hash * HASH_BASE + i64::from(byte)) % HASH_MODULUS;
    }
    format!("{hash:x}")
}

/// The two numbers `premise-mangler-hash-in-heroes.hero` copies. Named rather than
/// inlined because a test in another language is the only thing holding them, and a
/// literal it had to guess at would be the wrong kind of duplication.
const HASH_BASE: i64 = 131;
const HASH_MODULUS: i64 = 2_147_483_647;

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

/// One `test` block, by its position in the file.
///
/// **By index, not by title.** A title is a string literal — `test "3-4-5
/// triangle"` — so it contains spaces, digits at the front, and any UTF-8 the
/// author likes, none of which a C identifier admits. Sanitising it would need a
/// second escape scheme and would still not be injective, which is the argument
/// `instance` above makes for the type hash. The index is already unique and
/// already stable in source order.
pub fn test(module: &str, index: usize) -> String {
    format!("h_{module}_test{index}")
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
    field_of(false, name)
}

/// A field's C member name, given whether the record it belongs to is the
/// **header's** (panel 060).
///
/// A group's `record` is a struct this compiler never declared, so its members
/// must be spelled exactly as the header spells them — `.r`, not `.f_r`. The
/// `f_` prefix exists to keep a Heroes field name off C's keywords (`default`,
/// `register`); a header's field name is already valid C by construction, because
/// a C compiler accepted the header.
///
/// **`foreign` is asked of the declaration, never inferred from the field name.**
/// "A field with no `f_` is a header's" would be a premise about the world; "this
/// record carries a header" is a fact about the declaration in hand (CLAUDE.md
/// §11).
pub fn field_of(foreign: bool, name: &str) -> String {
    if foreign { name.to_string() } else { format!("f_{name}") }
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
