//! The four entities and their two neighbours (`extern`, `test`) — one
//! keyword-first shape, `constant MAX: int` / `function f(…)` / `record R` /
//! `variant V`, seen six ways (design.md §4.2, §4.18, §4.19; panel 018).

use super::dump;

#[test]
fn empty_file_has_no_declarations() {
    assert_eq!(dump(""), "file test.hero\n");
}

#[test]
fn constant_carries_its_type_and_its_value() {
    assert_eq!(
        dump("constant MAX_DEPTH: int\n    64\n"),
        "\
file test.hero
  constant MAX_DEPTH: int
    expr 64
"
    );
}

#[test]
fn function_signature_reads_back_as_written() {
    assert_eq!(
        dump("function dist2(a: Point, b: Point) -> int\n    return 1\n"),
        "\
file test.hero
  function dist2(a: Point, b: Point) -> int
    return 1
"
    );
}

/// `function main()` — the entry point (§4.1). No `->` at all, and the
/// result still prints as `()`: the parser synthesises the node so no later
/// pass has to ask whether the arrow was written.
#[test]
fn no_arrow_means_the_result_is_unit() {
    assert_eq!(
        dump("function main()\n    print(1)\n"),
        "\
file test.hero
  function main() -> ()
    expr print(1)
"
    );
}

/// §4.8: `@` in the signature declares an in-out parameter. It is the one
/// marker that must survive into every later pass, so the dump shows it.
#[test]
fn mutable_parameter_keeps_its_marker() {
    assert_eq!(
        dump("function advance(@l: Lex)\n    print(1)\n"),
        "\
file test.hero
  function advance(@l: Lex) -> ()
    expr print(1)
"
    );
}

#[test]
fn generics_are_names_only() {
    assert_eq!(
        dump("function map<A, B>(xs: [A], f: (function(A) -> B)) -> [B]\n    return xs\n"),
        "\
file test.hero
  function map<A, B>(xs: [A], f: (function(A) -> B)) -> [B]
    return xs
"
    );
}

#[test]
fn record_is_one_field_per_line() {
    assert_eq!(
        dump("record Point\n    x: int\n    y: int\n"),
        "\
file test.hero
  record Point
    field x: int
    field y: int
"
    );
}

/// A variant case with fields *is* a small record (§4.2), which is why the
/// payload block and the record body are the same parser.
#[test]
fn variant_cases_carry_optional_payloads() {
    assert_eq!(
        dump("variant Token\n    num\n        v: int\n    plus\n"),
        "\
file test.hero
  variant Token
    case num
      field v: int
    case plus
"
    );
}

/// §4.19: the signature is the whole declaration — the code is in C.
#[test]
fn extern_has_a_signature_and_no_body() {
    assert_eq!(
        dump("extern function sqrt(x: f64) -> f64\n"),
        "file test.hero\n  extern function sqrt(x: f64) -> f64\n"
    );
}

/// §4.18: the name is a string, because it is a title.
#[test]
fn test_block_is_named_by_a_string() {
    assert_eq!(
        dump("test \"3-4-5 triangle\"\n    assert 1 == 1\n"),
        "\
file test.hero
  test \"3-4-5 triangle\"
    assert (1 == 1)
"
    );
}

/// Declaration order carries no meaning (§4.2) but the tree keeps it, so
/// errors and `fmt` speak in reading order. Blank lines between
/// declarations are layout-neutral.
#[test]
fn a_whole_file_keeps_its_reading_order() {
    assert_eq!(
        dump(
            "\
record Point
    x: int

function dist2(a: Point, b: Point) -> int
    return 1

constant MAX: int
    64
"
        ),
        "\
file test.hero
  record Point
    field x: int
  function dist2(a: Point, b: Point) -> int
    return 1
  constant MAX: int
    expr 64
"
    );
}
