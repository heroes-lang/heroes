//! §4.16: what the compiler already knew, and used to throw away.
//!
//! The hole is not an error and never was. What these tests pin is that the
//! *expected type* reaches it — which is the whole deliverable, since M-rich-diagnostics's
//! output is built from `Checked::holes` and can only say what the checker
//! recorded here.

use super::{assert_clean, checked};

#[test]
fn a_hole_records_the_type_the_context_expects() {
    let (out, _) = checked(
        "\
function f(n: int) -> str
    return ???
",
    );
    assert!(out.diagnostics.is_empty(), "a hole is not an error");
    assert_eq!(out.holes.len(), 1);
    let ty = super::super::render_ty(&out.types, &parse_of("function f(n: int) -> str\n    return ???\n").0, &parse_of("function f(n: int) -> str\n    return ???\n").1, out.holes[0].expected, &[]);
    assert_eq!(ty, "str");
}

/// One hole per position, and each one knows its own expectation: an argument's
/// hole expects the parameter's type, not the function's result.
#[test]
fn each_hole_knows_its_own_expectation() {
    let (out, src) = checked(
        "\
function add(a: int, b: str) -> int
    print(b)
    return a

function main()
    print(add(???, ???))
",
    );
    assert_eq!(out.holes.len(), 2);
    let shown: Vec<String> = out
        .holes
        .iter()
        .map(|hole| {
            let parsed = crate::syntax::parse(&src);
            super::super::render_ty(&out.types, &parsed.ast, &src, hole.expected, &[])
        })
        .collect();
    assert_eq!(shown, vec!["int".to_string(), "str".to_string()]);
}

/// A file with a hole still checks everything else — which is the sentence §4.16
/// ends on, and the reason skeleton-first works at all.
#[test]
fn the_rest_of_the_file_is_still_checked() {
    let (out, _) = checked(
        "\
function half(n: int) -> int
    return n / 2

function later(n: int) -> str
    ???
",
    );
    assert!(out.diagnostics.is_empty());
    assert_eq!(out.holes.len(), 1);
}

/// A hole in a record's field position takes the field's type, because
/// construction is a call and its arguments are checked (§4.9).
#[test]
fn a_hole_in_a_field_position_expects_the_fields_type() {
    assert_clean(
        "\
record Point
    x: int
    y: int

function origin() -> Point
    return Point(x: 0, y: ???)
",
    );
}

fn parse_of(text: &str) -> (crate::syntax::Ast, crate::source::Source) {
    let src = crate::source::Source::new("test.hero".to_string(), text.to_string());
    let parsed = crate::syntax::parse(&src);
    (parsed.ast, src)
}
