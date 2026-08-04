//! Broken files: one diagnostic per mistake, and the declarations after it
//! still parse.
//!
//! Recovery is not politeness — a model fixing ten mistakes in ten turns
//! instead of one is the cost this file exists to keep down (§4.17).

use super::dump;

/// The pre-panel-018 shape, written from habit: a name where the kind
/// keyword belongs. The dispatcher is a switch on the closed keyword set, so
/// it says so once — and the body goes with the broken header, not to a
/// second diagnostic. (Replaces the `expected_eq` test: with no `=` in the
/// grammar there is no missing `=` to report.)
#[test]
fn the_old_shape_costs_one_diagnostic() {
    assert_eq!(
        dump("MAX = constant: int\n    1\n"),
        "\
file test.hero
DIAG test.hero:1:1: error[expected_declaration]: expected a declaration, found a name (`MAX`) — every top-level line starts with its kind: `constant`, `function`, `record`, `variant`, `test \"…\"`, or `extern`
"
    );
}

/// An invented kind word gets the whole list: the keyword set is closed, so
/// the message can enumerate everything a top-level line may start with.
/// (Replaces the `expected_entity` test — there is no `= entity` position
/// left for an unknown word to sit in.)
#[test]
fn an_unknown_kind_word_names_the_six() {
    assert_eq!(
        dump("widget Point\n    x: int\n"),
        "\
file test.hero
DIAG test.hero:1:1: error[expected_declaration]: expected a declaration, found a name (`widget`) — every top-level line starts with its kind: `constant`, `function`, `record`, `variant`, `test \"…\"`, or `extern`
"
    );
}

/// The anchor property panel 018 bought: recovery trusts the closed keyword
/// set, so one broken declaration head never swallows the declaration below
/// it — the next line starts with `record`, and `record` is never body.
#[test]
fn a_broken_head_does_not_swallow_the_next_declaration() {
    assert_eq!(
        dump("constant MAX\nrecord Point\n    x: int\n"),
        "\
file test.hero
  record Point
    field x: int
DIAG test.hero:1:13: error[expected_constant_type]: expected `:` and the constant's type — a `constant` declares a value, so it has one, found end of line
"
    );
}

/// The lexer already said what `struct` is (`reserved_word`, with a certain
/// fix). The parser adds nothing: one mistake, one diagnostic.
#[test]
fn a_foreign_entity_word_is_reported_once() {
    assert_eq!(
        dump("struct Point\n    x: int\n"),
        "\
file test.hero
DIAG test.hero:1:1: error[reserved_word]: `struct` is not a word in this language — use `record`: `record Point`
"
    );
}

#[test]
fn a_declaration_without_a_body_says_so() {
    assert_eq!(
        dump("constant MAX: int\n"),
        "\
file test.hero
DIAG test.hero:2:1: error[missing_body]: a `constant` needs an indented body — one level deeper, exactly 4 spaces (found end of file)
"
    );
}

/// This used to be the recovery case that needed spans instead of tokens:
/// `Point = record` ended in a non-ender, so its line boundary was invisible.
/// Panel 018 flipped the invariant — `record Point` ends in the declared
/// name and earns a terminator like any other line — but what the test pins
/// survives the flip: a header with its fields forgotten must not swallow
/// the declaration below it.
#[test]
fn an_empty_record_does_not_eat_the_next_declaration() {
    assert_eq!(
        dump("record Point\nconstant MAX: int\n    1\n"),
        "\
file test.hero
  constant MAX: int
    expr 1
DIAG test.hero:2:1: error[empty_record]: a `record` needs at least one field, indented one level below it
"
    );
}

#[test]
fn an_empty_variant_says_what_is_missing() {
    assert_eq!(
        dump("variant Token\n"),
        "\
file test.hero
DIAG test.hero:2:1: error[empty_variant]: a `variant` needs at least one case, indented one level below it
"
    );
}

/// A broken field line is one diagnostic and one lost field; the fields
/// below it are read normally.
#[test]
fn a_broken_field_line_loses_only_that_field() {
    assert_eq!(
        dump("record Point\n    x int\n    y: int\n"),
        "\
file test.hero
  record Point
    field y: int
DIAG test.hero:2:7: error[expected_field_type]: expected `:` and the field's type, found a name (`int`)
"
    );
}

/// §4.19: an `extern` body would never be compiled, so it is loud rather
/// than ignored.
#[test]
fn an_extern_with_a_body_is_an_error() {
    assert_eq!(
        dump("extern function sqrt(x: f64) -> f64\n    return x\n"),
        "\
file test.hero
  extern function sqrt(x: f64) -> f64
DIAG test.hero:2:1: error[extern_has_body]: an `extern` declaration has no body — it names a C function, and C provides the code
"
    );
}

/// §4.9 gives newline separation to multi-line *literals*; nothing extends
/// it to signatures, so the parser requires `,` — the narrower reading, and
/// the one `heroes fmt` can widen later if the formatter needs it.
///
/// The interesting half is the recovery: the closing `)` is a landmark, so
/// the result type and the body are still read and the mistake costs exactly
/// one diagnostic. Without it this one line produced four, three of them
/// about "declarations" that do not exist.
#[test]
fn parameters_are_separated_by_commas_not_newlines() {
    assert_eq!(
        dump("function copy(\n    from: str\n    to: str\n) -> bool\n    return true\n"),
        "\
file test.hero
  function copy(from: str) -> bool
    return true
DIAG test.hero:3:5: error[expected_params_close]: expected `)`, or `,` and another parameter, found a name (`to`)
"
    );
}

/// A stray indented line at top level: consumed as a block, one diagnostic,
/// and the file after it still parses.
#[test]
fn a_stray_indented_block_does_not_derail_the_file() {
    assert_eq!(
        dump("    x = 1\nconstant MAX: int\n    1\n"),
        "\
file test.hero
  constant MAX: int
    expr 1
DIAG test.hero:1:1: error[expected_declaration]: expected a declaration, found an indented block — every top-level line starts with its kind: `constant`, `function`, `record`, `variant`, `test \"…\"`, or `extern`
"
    );
}

/// The one that never terminated. `skip_line` stops *before* an `Indent` — it
/// must not leave the block it is cleaning — so a failed arm followed by an
/// indented body consumed nothing and the arm loop spun forever. Found by the
/// compiler-engineer while costing panel 014.
///
/// The input had to change when panel 014 landed: `1 => for x in xs` is now a
/// *legal* arm body, so the failure has to come from the pattern. If this test
/// ever hangs instead of failing, the arm loop lost its `Indent` case again.
#[test]
fn a_failed_arm_with_a_body_terminates() {
    assert_eq!(
        dump("function f(k: int) -> int\n    return match k\n        + => 1\n            print(k)\n        _ => 0\n"),
        "file test.hero\n  function f(k: int) -> int\n    return match k\n      _ => expr 0\nDIAG test.hero:3:9: error[expected_pattern]: expected a pattern, found `+` — `.case`, `.case name`, a literal, or `_` (on `int`/`str` only)\n"
    );
}

/// One mistake, one diagnostic. Kept from the era when `=> assert false` was
/// itself the mistake: it reported `expected_expression` *and* a spurious
/// `expected_pattern`, because the unconsumed token was read as the next arm's
/// pattern. Panel 014 made the form legal, so the case now guards the
/// diagnostic-count signal on a body that is still wrong.
#[test]
fn a_broken_arm_body_reports_once() {
    assert_eq!(
        dump("function f(k: int) -> int\n    return match k\n        1 => x = = 2\n        _ => 0\n"),
        "file test.hero\n  function f(k: int) -> int\n    return match k\n      1 => bind x = <?>\n      _ => expr 0\nDIAG test.hero:3:18: error[expected_expression]: expected an expression, found `=` — a value, a name, a call, `[`, `{`, `.case`, `if`, `match`, or `???`\n"
    );
}
