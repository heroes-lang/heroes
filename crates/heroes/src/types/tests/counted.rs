//! Which types own a reference the compiler must count (§4.10, §4.20).
//!
//! The question is transitive, and that is the whole reason it has a table: a
//! `record` of two `int`s owns nothing and needs no zero-initialisation, no store
//! incref and no exit sweep, while the same record with one `str` in it needs all
//! three. Wrong in the cheap direction is a leak; in the other, a use-after-free.
//!
//! The lookup goes through `Types::lookup(Ty::Named(decl))` rather than by name,
//! because a declaration is only *interned* when something writes its type — which
//! is itself worth knowing: an unused record has no `TyId` at all, and a table over
//! interned types therefore has nothing to say about it.

use crate::ir::is_refcounted;
use crate::resolve::resolve;
use crate::source::Source;
use crate::syntax::{parse, DeclKind};
use crate::types::{check, Ty};

/// Does the record or variant declared under this name own a counted reference?
///
/// Panics if the type was never interned, which in these programs means the name is
/// wrong — every record below is used by a function signature for exactly that
/// reason.
fn owns(text: &str, wanted: &str) -> bool {
    let src = Source::new("test.hero".to_string(), text.to_string());
    let parsed = parse(&src);
    assert!(parsed.diagnostics.is_empty(), "the input must parse clean");
    let resolved = resolve(&parsed.ast, &src);
    assert!(resolved.diagnostics.is_empty(), "the input must resolve clean");
    let out = check(&parsed.ast, &resolved, &src);
    assert!(out.diagnostics.is_empty(), "the input must check clean");

    let decl = parsed
        .ast
        .decls
        .iter()
        .position(|d| {
            matches!(d.kind, DeclKind::Record { .. } | DeclKind::Variant { .. })
                && src.slice(d.name) == wanted
        })
        .unwrap_or_else(|| panic!("no declaration called {wanted}"));
    let id = out
        .types
        .lookup(Ty::Named(decl as u32))
        .unwrap_or_else(|| panic!("{wanted} was never interned — nothing writes its type"));
    is_refcounted(&out, id)
}

/// Every record is a parameter of something, so every one of them is interned.
const PROGRAM: &str = "\
record Plain
    x: int
    y: int

record Holder
    name: str

record Nested
    inner: Holder
    n: int

function a(p: Plain) -> int
    return p.x

function b(h: Holder) -> str
    return h.name

function c(n: Nested) -> int
    return n.n

function main()
    print(a(Plain(x: 1, y: 2)))
    print(b(Holder(name: \"x\")))
    print(c(Nested(inner: Holder(name: \"y\"), n: 3)))
";

#[test]
fn a_record_of_scalars_owns_nothing() {
    assert!(!owns(PROGRAM, "Plain"));
}

#[test]
fn a_record_with_a_str_owns_a_reference() {
    assert!(owns(PROGRAM, "Holder"));
}

/// The transitive case, and the one a per-field check gets wrong: `Nested` has no
/// `str` field of its own.
#[test]
fn owning_is_transitive_through_a_field() {
    assert!(owns(PROGRAM, "Nested"));
}

/// A variant is counted when any *case* carries something counted — the cases are
/// alternatives, so one is enough, and the drop has to switch on the tag.
#[test]
fn a_variant_is_counted_when_one_case_carries_a_reference() {
    let text = "\
variant Token
    num
        v: int
    word
        text: str

function kind(t: Token) -> int
    return match t
        .num n  => n.v
        .word _ => 0

function main()
    print(kind(.num(v: 1)))
";
    assert!(owns(text, "Token"));
}

/// And not counted when no case does. The whole variant is then a plain tagged
/// union that copies with `=` and needs no drop at all.
#[test]
fn a_variant_of_scalars_owns_nothing() {
    let text = "\
variant Shape
    dot
    line
        len: int

function size(s: Shape) -> int
    return match s
        .dot    => 0
        .line l => l.len

function main()
    print(size(.dot))
";
    assert!(!owns(text, "Shape"));
}
