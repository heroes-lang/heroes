//! Written types against the three things a type name can be (§4.3, §4.12).

use crate::resolve::{Prim, TypeRef};
use crate::syntax::TypeId;

use super::{assert_clean, diagnostics, resolved};

#[test]
fn the_primitives_a_record_and_a_generic_all_resolve() {
    assert_clean(
        "\
record Point
    x: int
    y: f64

function first<A>(xs: [A], flag: bool, name: str) -> A
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
record Point
    x: int

function f(m: {str: [Point]?}) -> int
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
record Point
    x: int

function f(p: Poimt) -> int
    return p.x
"
        ),
        "test.hero:4:15: error[unknown_type]: no type named `Poimt` — did you mean `Point`?\n"
    );
}

/// The name exists and is not a type, so "unknown" would be a lie and a
/// did-you-mean would be noise.
#[test]
fn a_function_in_type_position_is_told_what_it_is() {
    assert_eq!(
        diagnostics(
            "\
function g() -> int
    return 1

function f(x: g) -> int
    print(x)
    return 1
"
        ),
        "test.hero:4:15: error[not_a_type]: `g` is a function, not a type\n"
    );
}

/// A type parameter is in scope for the whole signature and body, and only for
/// its own function (§4.12).
#[test]
fn a_type_parameter_does_not_escape_its_function() {
    assert_eq!(
        diagnostics(
            "\
function first<A>(xs: [A]) -> A
    return xs[0]

function second(ys: [A]) -> int
    print(ys)
    return 0
"
        ),
        "test.hero:4:22: error[unknown_type]: no type named `A`\n"
    );
}

/// §4.19's two FFI names are types like any other.
#[test]
fn ptr_and_cstr_are_primitives() {
    assert_clean("extern \"stdio.h\"\n    function puts(s: cstr) -> int\n");
    let (out, _) = resolved("extern \"stdlib.h\"\n    function malloc(n: int) -> ptr\n");
    assert_eq!(out.type_at(TypeId(1)), TypeRef::Prim(Prim::Ptr));
}

/// A function type is a type (panel 013's `(function(A) -> B)`), and its parts
/// are resolved like any other.
#[test]
fn a_function_type_resolves_its_parameters_and_result() {
    assert_clean(
        "\
record Point
    x: int

function twice(f: (function(Point) -> int), p: Point) -> int
    return f(p) + f(p)
",
    );
    assert_eq!(
        diagnostics(
            "\
function twice(f: (function(Poimt) -> int)) -> int
    return f(1)
"
        ),
        "test.hero:1:29: error[unknown_type]: no type named `Poimt`\n"
    );
}

/// **THE PREMISE: a declared type cannot contain a type parameter.**
///
/// §4.12's positive rule — *"Generics on functions only, not on types"* — and
/// this test exists because **three functions in three other modules quietly
/// depend on it and none of them says so**:
///
/// | who | what it does with the premise |
/// |---|---|
/// | `emit/ctype.rs::mentions_generic` | walks `Array\|Fallible\|Map\|Func` and answers `false` for everything else, so a `Ty::Named` is assumed parameter-free. If it were not, the emitter would give a template type a C declaration: `error: unknown type name 'HeroValue'`, exit 2 |
/// | `ir/mono.rs::contains` | same walk, same fallback. If it were wrong, monomorphisation would miss a substitution site and emit a body still mentioning a parameter |
/// | `types/table.rs::poisoned` | same walk again. If it were wrong, an `Error` inside a declared type would stop suppressing the cascade it exists to suppress |
///
/// All three fail **silently** if the premise dies: no diagnostic says "this
/// walk did not descend". Nothing in the compiler connects them to the rule
/// they rest on — so this is that connection, and it is a test rather than a
/// comment because two defects on 2026-08-12 were protected by comments that
/// argued correctly for a world that had changed underneath them
/// (`docs/defects/001-the-post-m8a-sweep.md`).
///
/// **The premise is currently enforced, and by an accident worth knowing about.**
/// `resolve/types.rs` resolves a type parameter against the *enclosing
/// function's* list, and a `record` has no such list — so `A` inside a field is
/// simply an unknown name. Nobody wrote a rule saying "a declaration may not be
/// generic"; the rule is a consequence of where the parameter list lives. An
/// accident is exactly the kind of enforcement that disappears in a refactor
/// nobody thinks is related.
///
/// **If this test goes red, the three functions above are wrong** and the walks
/// have to descend into a declaration's fields before anything else lands. Part 8
/// wart 5 is the live route to that: typed errors name generics-on-types as their
/// first prerequisite.
#[test]
fn a_declared_type_cannot_contain_a_type_parameter() {
    let said = diagnostics(
        "\
record Box
    v: A

function main()
    print(1)
",
    );
    assert!(
        said.contains("unknown_type"),
        "the premise three walks depend on has died — read this test's doc: {said}"
    );

    let said = diagnostics(
        "\
variant T
    one
        v: A

function main()
    print(1)
",
    );
    assert!(
        said.contains("unknown_type"),
        "a case payload is a small record and the premise covers it too: {said}"
    );

    // And the other way in: a declaration may not carry a parameter list at all,
    // which is what keeps `A` from ever being resolvable in a field. This one is
    // refused by the **parser**, one pass earlier — `record` takes a name and
    // nothing else (§4.2) — so it is asked of `parse` rather than of `resolve`.
    let src = crate::source::Source::new(
        "test.hero".to_string(),
        "record Box<A>\n    v: int\n\nfunction main()\n    print(1)\n".to_string(),
    );
    let parsed = crate::syntax::parse(&src);
    assert!(
        !parsed.diagnostics.is_empty(),
        "`record Box<A>` must not parse as a generic declaration"
    );
}

/// A `constant` may not be defined in terms of itself — directly, through
/// another constant, or through a function (`resolve/cycles.rs`).
///
/// The control is in the same test on purpose: `CHAIN` reading `START` and
/// `even`/`odd` recurring through each other are both legal, and a cycle check
/// that fired on either would be worse than the defect it replaced.
#[test]
fn a_constant_defined_in_terms_of_itself_is_refused() {
    let codes = |text: &str| -> Vec<String> {
        let src = crate::source::Source::new("t.hero".to_string(), text.to_string());
        let parsed = crate::syntax::parse(&src);
        assert!(parsed.diagnostics.is_empty(), "the fixture parses: {:?}", parsed.diagnostics);
        crate::resolve::resolve(&parsed.ast, &src)
            .diagnostics
            .iter()
            .map(|d| d.code.clone())
            .collect()
    };

    let direct = "constant A: int\n    A\n\nfunction main()\n    print(A)\n";
    assert_eq!(codes(direct), vec!["constant_cycle".to_string()]);

    let pair = "constant A: int\n    B\n\nconstant B: int\n    A\n\nfunction main()\n    print(A)\n";
    assert_eq!(codes(pair), vec!["constant_cycle".to_string()], "one diagnostic per cycle, not per constant");

    let through =
        "constant A: int\n    f()\n\nfunction f() -> int\n    return A\n\nfunction main()\n    print(A)\n";
    assert_eq!(codes(through), vec!["constant_cycle".to_string()], "the cycle escapes through a function");

    let legal = "constant START: int\n    2\n\nconstant CHAIN: int\n    START + 1\n\n\
                 function even(n: int) -> bool\n    if n == 0\n        return true\n    return odd(n - 1)\n\n\
                 function odd(n: int) -> bool\n    if n == 0\n        return false\n    return even(n - 1)\n\n\
                 function main()\n    print(CHAIN)\n    print(even(4))\n";
    assert!(codes(legal).is_empty(), "a chain and mutual recursion are both legal (§4.2)");
}
