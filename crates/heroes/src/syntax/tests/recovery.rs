//! Broken files: one diagnostic per mistake, and the declarations after it
//! still parse.
//!
//! Recovery is not politeness — a model fixing ten mistakes in ten turns
//! instead of one is the cost this file exists to keep down (§4.17).

use super::dump;

/// The `=` is the whole declaration form; without it there is nothing to
/// read. The body goes with the broken header, not to a second diagnostic.
#[test]
fn a_missing_eq_costs_one_diagnostic() {
    assert_eq!(
        dump("MAX constant: int\n    1\n"),
        "\
file test.hero
DIAG test.hero:1:5: error[expected_eq]: expected `=` — a top-level line binds a name to one of the four entities, found `constant`
"
    );
}

#[test]
fn an_unknown_entity_word_names_the_four() {
    assert_eq!(
        dump("Point = thing\n    x: int\n"),
        "\
file test.hero
DIAG test.hero:1:9: error[expected_entity]: expected `constant`, `function`, `record` or `variant`, found a name (`thing`) — those four are everything a name can be
"
    );
}

/// The lexer already said what `struct` is (`reserved_word`, with a certain
/// fix). The parser adds nothing: one mistake, one diagnostic.
#[test]
fn a_foreign_entity_word_is_reported_once() {
    assert_eq!(
        dump("Point = struct\n    x: int\n"),
        "\
file test.hero
DIAG test.hero:1:9: error[reserved_word]: `struct` is not a word in this language — use `record`: `Point = record`
"
    );
}

#[test]
fn a_declaration_without_a_body_says_so() {
    assert_eq!(
        dump("MAX = constant: int\n"),
        "\
file test.hero
DIAG test.hero:2:1: error[missing_body]: a `constant` needs an indented body — one level deeper, exactly 4 spaces (found end of file)
"
    );
}

/// The recovery case that needed spans instead of tokens: `record` plants no
/// terminator, so a header with its fields forgotten must not swallow the
/// declaration below it.
#[test]
fn an_empty_record_does_not_eat_the_next_declaration() {
    assert_eq!(
        dump("Point = record\nMAX = constant: int\n    1\n"),
        "\
file test.hero
  constant MAX: int
    body lines 3-3
DIAG test.hero:2:1: error[empty_record]: a `record` needs at least one field, indented one level below it
"
    );
}

#[test]
fn an_empty_variant_says_what_is_missing() {
    assert_eq!(
        dump("Token = variant\n"),
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
        dump("Point = record\n    x int\n    y: int\n"),
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
        dump("extern sqrt = function: (x: f64) -> f64\n    return x\n"),
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
        dump("copy = function: (\n    from: str\n    to: str\n) -> bool\n    return true\n"),
        "\
file test.hero
  function copy(from: str) -> bool
    body lines 5-5
DIAG test.hero:3:5: error[expected_params_close]: expected `)`, or `,` and another parameter, found a name (`to`)
"
    );
}

/// A stray indented line at top level: consumed as a block, one diagnostic,
/// and the file after it still parses.
#[test]
fn a_stray_indented_block_does_not_derail_the_file() {
    assert_eq!(
        dump("    x = 1\nMAX = constant: int\n    1\n"),
        "\
file test.hero
  constant MAX: int
    body lines 3-3
DIAG test.hero:1:1: error[expected_declaration]: expected a declaration, found an indented block — every top-level line names something: `name = constant|function|record|variant`, `test \"…\"`, or `extern`
"
    );
}
