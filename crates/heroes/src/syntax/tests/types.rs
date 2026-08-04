//! The type grammar: every shape in the §4.3 table, nested, plus the
//! function type panel 013 settled.
//!
//! Types are pinned through a record field, which is the shortest
//! declaration that holds exactly one type.

use super::dump;

fn field(ty: &str) -> String {
    dump(&format!("T = record\n    f: {ty}\n"))
}

#[test]
fn the_primitive_names_pass_through() {
    for name in ["int", "f64", "bool", "str", "ptr", "cstr", "Point"] {
        assert_eq!(field(name), format!("file test.hero\n  record T\n    field f: {name}\n"));
    }
}

#[test]
fn containers_nest() {
    assert_eq!(field("[int]"), "file test.hero\n  record T\n    field f: [int]\n");
    assert_eq!(field("{str: int}"), "file test.hero\n  record T\n    field f: {str: int}\n");
    assert_eq!(
        field("{str: [{int: bool}]}"),
        "file test.hero\n  record T\n    field f: {str: [{int: bool}]}\n"
    );
}

/// §4.6: `T?` is a `T` or an error, and it is the only postfix type.
#[test]
fn fallible_applies_to_whatever_precedes_it() {
    assert_eq!(field("int?"), "file test.hero\n  record T\n    field f: int?\n");
    assert_eq!(field("[int]?"), "file test.hero\n  record T\n    field f: [int]?\n");
    assert_eq!(field("{str: int}?"), "file test.hero\n  record T\n    field f: {str: int}?\n");
}

/// §4.6 refuses the `T??` level ambiguity in the language, so the parser
/// refuses it in the surface. Rejecting can be relaxed later; accepting
/// could not be withdrawn.
#[test]
fn fallible_does_not_nest() {
    assert_eq!(
        field("int??"),
        "\
file test.hero
  record T
    field f: int?
DIAG test.hero:2:12: error[nested_fallible]: a fallible type cannot be fallible twice — `T?` already carries the error case (§4.6)
"
    );
}

/// Panel 013: the marker is the word that declares a function, and `(`
/// opens either `()` or a function type — one token of lookahead.
#[test]
fn function_types_take_any_arity() {
    assert_eq!(
        field("(function(int) -> bool)"),
        "file test.hero\n  record T\n    field f: (function(int) -> bool)\n"
    );
    assert_eq!(
        field("(function(int, int) -> bool)"),
        "file test.hero\n  record T\n    field f: (function(int, int) -> bool)\n"
    );
    assert_eq!(
        field("(function() -> int)"),
        "file test.hero\n  record T\n    field f: (function() -> int)\n"
    );
    assert_eq!(
        field("(function(int) -> ())"),
        "file test.hero\n  record T\n    field f: (function(int) -> ())\n"
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
        field("(function(x: int) -> int)"),
        "\
file test.hero
  record T
    field f: (function(int) -> int)
DIAG test.hero:2:18: error[named_parameter_in_function_type]: a function type lists types, not names — write `(function(int) -> int)`
"
    );
}

/// `fn` never lexes (panel 013): the reserved-word error fires first and the
/// parser adds nothing — one mistake, one diagnostic.
#[test]
fn fn_is_still_a_reserved_word_inside_a_type() {
    assert_eq!(
        dump("T = record\n    f: (fn(int) -> int)\n"),
        "\
file test.hero
  record T
    field f: <?>
DIAG test.hero:2:9: error[reserved_word]: `fn` is not a word in this language — use `function`: `f = function: (x: int) -> int`
"
    );
}
