//! Written types against the three things a type name can be (§4.3, §4.12).

use crate::resolve::{Prim, TypeRef};
use crate::syntax::TypeId;

use super::{assert_clean, diagnostics, resolved};

#[test]
fn the_primitives_a_record_and_a_generic_all_resolve() {
    assert_clean(
        "\
Point = record
    x: int
    y: f64

first = function<A>: (xs: [A], flag: bool, name: str) -> A
    print(flag)
    print(name)
    return xs[0]
",
    );
}

/// One entry per node in the arena, and the nesting is visible in the entries:
/// `{str: [Point]?}` is five nodes and five answers.
#[test]
fn every_node_of_a_nested_type_is_answered() {
    let (out, _) = resolved(
        "\
Point = record
    x: int

f = function: (m: {str: [Point]?}) -> int
    print(m)
    return 0
",
    );
    let answers: Vec<TypeRef> =
        out.type_uses.iter().copied().filter(|t| *t != TypeRef::Unresolved).collect();
    // `int` (the field), then str · Point inside the map, then the record and
    // the `-> int`: every *named* node, and only those.
    assert!(answers.contains(&TypeRef::Prim(Prim::Int)));
    assert!(answers.contains(&TypeRef::Prim(Prim::Str)));
    assert!(answers.contains(&TypeRef::Top(0)));
    assert!(out.diagnostics.is_empty());
}

#[test]
fn an_unknown_type_names_the_nearest_candidate() {
    assert_eq!(
        diagnostics(
            "\
Point = record
    x: int

f = function: (p: Poimt) -> int
    return p.x
"
        ),
        "test.hero:4:19: error[unknown_type]: no type named `Poimt` — did you mean `Point`?\n"
    );
}

/// The name exists and is not a type, so "unknown" would be a lie and a
/// did-you-mean would be noise.
#[test]
fn a_function_in_type_position_is_told_what_it_is() {
    assert_eq!(
        diagnostics(
            "\
g = function: () -> int
    return 1

f = function: (x: g) -> int
    print(x)
    return 1
"
        ),
        "test.hero:4:19: error[not_a_type]: `g` is a function, not a type\n"
    );
}

/// A type parameter is in scope for the whole signature and body, and only for
/// its own function (§4.12).
#[test]
fn a_type_parameter_does_not_escape_its_function() {
    assert_eq!(
        diagnostics(
            "\
first = function<A>: (xs: [A]) -> A
    return xs[0]

second = function: (ys: [A]) -> int
    print(ys)
    return 0
"
        ),
        "test.hero:4:26: error[unknown_type]: no type named `A`\n"
    );
}

/// §4.19's two FFI names are types like any other.
#[test]
fn ptr_and_cstr_are_primitives() {
    assert_clean("extern puts = function: (s: cstr) -> int\n");
    let (out, _) = resolved("extern malloc = function: (n: int) -> ptr\n");
    assert_eq!(out.type_at(TypeId(1)), TypeRef::Prim(Prim::Ptr));
}

/// A function type is a type (panel 013's `(function(A) -> B)`), and its parts
/// are resolved like any other.
#[test]
fn a_function_type_resolves_its_parameters_and_result() {
    assert_clean(
        "\
Point = record
    x: int

twice = function: (f: (function(Point) -> int), p: Point) -> int
    return f(p) + f(p)
",
    );
    assert_eq!(
        diagnostics(
            "\
twice = function: (f: (function(Poimt) -> int)) -> int
    return f(1)
"
        ),
        "test.hero:1:33: error[unknown_type]: no type named `Poimt`\n"
    );
}
