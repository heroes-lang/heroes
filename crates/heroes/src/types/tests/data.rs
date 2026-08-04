//! Records, variants, patterns, exhaustiveness, and `_`'s ban (§4.2, §4.7).

use super::{assert_clean, diagnostics, type_of_last};

#[test]
fn a_record_field_is_read_by_name() {
    assert_eq!(
        type_of_last(
            "\
Point = record
    x: int
    y: f64

f = function: (p: Point) -> f64
    return p.y
"
        ),
        "f64"
    );
    assert_eq!(
        diagnostics(
            "\
Point = record
    x: int
    y: int

f = function: (p: Point) -> int
    return p.z
"
        ),
        "test.hero:6:14: error[no_such_field]: `Point` has no field `z` — `Point` has `x`, `y`\n"
    );
}

/// Nominal, not structural: two records with the same fields are two types, which
/// is what makes `Point` and `Size` unconfusable.
#[test]
fn two_records_with_the_same_fields_are_different_types() {
    assert_eq!(
        diagnostics(
            "\
Point = record
    x: int

Size = record
    x: int

f = function: (p: Point) -> Size
    return p
"
        ),
        "test.hero:8:12: error[type_mismatch]: expected `Size`, found `Point`\n"
    );
}

/// §4.2: a case with fields *is* a small record, and the pattern's binding has
/// that case's type — a type nothing in the surface syntax can write.
#[test]
fn a_payload_binding_has_the_case_type() {
    assert_clean(
        "\
Shape = variant
    circle
        r: int
    square
        side: int

area = function: (s: Shape) -> int
    return match s
        .circle c => c.r * 3
        .square q => q.side * q.side
",
    );
    assert_eq!(
        diagnostics(
            "\
Shape = variant
    circle
        r: int
    square
        side: int

area = function: (s: Shape) -> int
    return match s
        .circle c => c.side
        .square q => q.side
"
        ),
        "test.hero:9:24: error[no_such_field]: `Shape.circle` has no field `side` — `Shape.circle` has `r`\n"
    );
}

#[test]
fn a_variants_fields_are_reached_through_match() {
    assert_eq!(
        diagnostics(
            "\
Shape = variant
    circle
        r: int

f = function: (s: Shape) -> int
    return s.r
"
        ),
        "test.hero:6:14: error[no_such_field]: `Shape` is a variant, so `.r` belongs to one of its cases — reach it through `match`\n"
    );
}

/// The rule `match` exists for. §4.17: the missing cases are named.
#[test]
fn a_match_over_a_variant_is_exhaustive_or_names_what_is_missing() {
    assert_eq!(
        diagnostics(
            "\
Token = variant
    num
        v: int
    plus
    times

f = function: (t: Token) -> int
    return match t
        .num n => n.v
"
        ),
        "test.hero:8:12: error[non_exhaustive]: this `match` does not cover `.plus`, `.times`\n"
    );
}

/// §4.7's ban, and the reason travels with it: with `_` allowed, adding a case
/// would stop breaking compilation.
#[test]
fn a_wildcard_arm_is_forbidden_on_a_variant() {
    assert_eq!(
        diagnostics(
            "\
Token = variant
    num
        v: int
    plus

f = function: (t: Token) -> int
    return match t
        .num n => n.v
        _      => 0
"
        ),
        "test.hero:9:9: error[wildcard_on_variant]: `_` is not allowed as an arm over `Token` — name every case, so that adding one breaks this `match` instead of silently falling through\n"
    );
}

/// …and on `int` or `str` it is the opposite: exhaustiveness is impossible, so
/// the wildcard is required.
#[test]
fn matching_an_int_needs_a_wildcard() {
    assert_clean(
        "\
f = function: (n: int) -> str
    return match n
        0 => \"zero\"
        1 => \"one\"
        _ => \"many\"
",
    );
    assert_eq!(
        diagnostics(
            "\
f = function: (n: int) -> str
    return match n
        0 => \"zero\"
        1 => \"one\"
"
        ),
        "test.hero:2:12: error[non_exhaustive]: matching on `int` or `str` cannot be exhaustive, so it needs a `_` arm\n"
    );
}

#[test]
fn an_unknown_case_lists_the_ones_that_exist() {
    assert_eq!(
        diagnostics(
            "\
Token = variant
    num
        v: int
    plus

f = function: (t: Token) -> int
    return match t
        .num n  => n.v
        .plus   => 0
        .minus  => 1
"
        ),
        "test.hero:10:10: error[no_such_case]: `Token` has no case `.minus` — it has `.num`, `.plus`\n"
    );
}

#[test]
fn a_case_covered_twice_is_reported() {
    assert_eq!(
        diagnostics(
            "\
Token = variant
    num
        v: int
    plus

f = function: (t: Token) -> int
    return match t
        .num n => n.v
        .plus  => 0
        .plus  => 1
"
        ),
        "test.hero:10:9: error[duplicate_arm]: `.plus` is already covered by an earlier arm\n"
    );
}

/// A literal arm must have the subject's type — `match n` with a `"zero"` arm is
/// a mistake the arm, not the subject, is blamed for.
#[test]
fn a_literal_arm_matches_the_subjects_type() {
    assert_eq!(
        diagnostics(
            "\
f = function: (n: int) -> int
    return match n
        \"zero\" => 0
        _      => 1
"
        ),
        "test.hero:3:9: error[type_mismatch]: expected `int`, found `str`\n"
    );
}

/// A case in ⇐ position builds a value; its fields are named like a record's.
#[test]
fn a_case_is_constructed_against_the_expected_type() {
    assert_clean(
        "\
Token = variant
    num
        v: int
    plus

make = function: (n: int) -> Token
    return .num(v: n)

plus_token = function: () -> Token
    return .plus
",
    );
    assert_eq!(
        diagnostics(
            "\
Token = variant
    num
        v: int

make = function: (n: int) -> Token
    return .num(value: n)
"
        ),
        "test.hero:6:17: error[wrong_label]: `Token.num` has no field `value` at this position — it is `v`\n"
    );
    assert_eq!(
        diagnostics(
            "\
Token = variant
    num
        v: int

make = function: (n: int) -> int
    return .num(v: n)
"
        ),
        "test.hero:6:12: error[type_mismatch]: expected `int`, found the case `.num`\n"
    );
}

/// A bare case with nothing expecting it has nothing to be: the message says so,
/// and names the three places the type could come from.
#[test]
fn a_case_with_no_expectation_says_what_is_missing() {
    assert_eq!(
        diagnostics(
            "\
Token = variant
    plus

main = function: ()
    t = .plus
    print(t == .plus)
"
        ),
        "test.hero:5:9: error[cannot_infer]: `.plus` does not say which variant it belongs to — the type has to come from the context (a signature, an annotation, or the value being returned)\n"
    );
}

/// `t == .plus` is the shape the appendix uses, and it works because equality
/// hands the other side's type to the case.
#[test]
fn equality_gives_a_case_its_type() {
    assert_clean(
        "\
Token = variant
    plus
    times

is_plus = function: (t: Token) -> bool
    return t == .plus
",
    );
}
