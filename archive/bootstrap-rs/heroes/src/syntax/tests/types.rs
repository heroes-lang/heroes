//! The type grammar: every shape in the §4.3 table, nested, plus the
//! function type panel 013 settled.
//!
//! Types are pinned through a record field, which is the shortest
//! declaration that holds exactly one type.

use super::dump;

fn field(ty: &str) -> String {
    dump(&format!("record T\n    f: {ty}\n"))
}

#[test]
fn the_primitive_names_pass_through() {
    for name in ["i64", "f64", "bool", "str", "ptr", "cstr", "Point"] {
        assert_eq!(field(name), format!("file test.hero\n  record T\n    field f: {name}\n"));
    }
}

#[test]
fn containers_nest() {
    assert_eq!(field("[i64]"), "file test.hero\n  record T\n    field f: [i64]\n");
    assert_eq!(field("{str: i64}"), "file test.hero\n  record T\n    field f: {str: i64}\n");
    assert_eq!(
        field("{str: [{i64: bool}]}"),
        "file test.hero\n  record T\n    field f: {str: [{i64: bool}]}\n"
    );
}

/// §4.6: `T?` is a `T` or an error, and it is the only postfix type.
#[test]
fn fallible_applies_to_whatever_precedes_it() {
    assert_eq!(field("i64?"), "file test.hero\n  record T\n    field f: i64?\n");
    assert_eq!(field("[i64]?"), "file test.hero\n  record T\n    field f: [i64]?\n");
    assert_eq!(field("{str: i64}?"), "file test.hero\n  record T\n    field f: {str: i64}?\n");
}

/// §4.6 refuses the `T??` level ambiguity in the language, so the parser
/// refuses it in the surface. Rejecting can be relaxed later; accepting
/// could not be withdrawn.
#[test]
fn fallible_does_not_nest() {
    assert_eq!(
        field("i64??"),
        "\
file test.hero
  record T
    field f: i64?
DIAG test.hero:2:12: error[nested_fallible]: a fallible type cannot be fallible twice — `T?` already carries the error case (§4.6)
"
    );
}

/// Panel 013: the marker is the word that declares a function, and `(`
/// opens either `()` or a function type — one token of lookahead.
#[test]
fn function_types_take_any_arity() {
    assert_eq!(
        field("(function(i64) -> bool)"),
        "file test.hero\n  record T\n    field f: (function(i64) -> bool)\n"
    );
    assert_eq!(
        field("(function(i64, i64) -> bool)"),
        "file test.hero\n  record T\n    field f: (function(i64, i64) -> bool)\n"
    );
    assert_eq!(
        field("(function() -> i64)"),
        "file test.hero\n  record T\n    field f: (function() -> i64)\n"
    );
    assert_eq!(
        field("(function(i64) -> ())"),
        "file test.hero\n  record T\n    field f: (function(i64) -> ())\n"
    );
}

#[test]
fn unit_is_a_type_like_any_other() {
    assert_eq!(field("()"), "file test.hero\n  record T\n    field f: ()\n");
}

/// The mistake this diagnostic exists for: the signature was copied into the
/// type and its parameter name came along. The fix is `certain` — drop the
/// name — so it is machine-applicable (§4.17).
#[test]
fn a_named_parameter_in_a_function_type_is_loud() {
    assert_eq!(
        field("(function(x: i64) -> i64)"),
        "\
file test.hero
  record T
    field f: (function(i64) -> i64)
DIAG test.hero:2:18: error[named_parameter_in_function_type]: a function type lists types, not names — write `(function(i64) -> i64)`
"
    );
}

/// `fn` never lexes (panel 013): the reserved-word error fires first and the
/// parser adds nothing — one mistake, one diagnostic.
#[test]
fn fn_is_still_a_reserved_word_inside_a_type() {
    assert_eq!(
        dump("record T\n    f: (fn(i64) -> i64)\n"),
        "\
file test.hero
  record T
    field f: <?>
DIAG test.hero:2:9: error[reserved_word]: `fn` is not a word in this language — use `function`: `function f(x: i64) -> i64`
"
    );
}

/// One node per written constructor, and `?` *wraps* rather than annotates:
/// `{str: [i64]}?` is five nodes, not three. Pinned because the count is the
/// whole reason the arena exists — a recursive type needs no pointers, only
/// indices, which is what lets it port to Heroes unchanged (§4.10).
#[test]
fn the_arena_holds_one_node_per_written_constructor() {
    let src = crate::source::Source::new(
        "test.hero".to_string(),
        "record T\n    f: {str: [i64]}?\n".to_string(),
    );
    let out = crate::syntax::parse(&src);
    assert_eq!(out.diagnostics.len(), 0);
    // str · int · [i64] · {str: [i64]} · {str: [i64]}?
    assert_eq!(out.ast.types.len(), 5);
}
