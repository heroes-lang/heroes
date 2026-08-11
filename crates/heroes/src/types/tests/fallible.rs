//! `T?`: `ok`/`fail` in ⇐ mode, `?`, the error's two fields, and the four readers
//! (§4.6, panel 002).

use super::{assert_clean, diagnostics, type_of_last};

/// Panel 002: no implicit `T` → `T?`, ever. Both constructors are ⇐-only, so the
/// expected type is what decides which fallible they build.
#[test]
fn ok_and_fail_are_checked_against_the_expected_type() {
    assert_clean(
        "\
function half(n: int) -> int?
    if n < 0
        return fail(\"negative\", \"n must not be negative\")
    return ok(n / 2)
",
    );
    assert_eq!(
        diagnostics("function f(n: int) -> int?\n    return n\n"),
        "test.hero:2:12: error[type_mismatch]: expected `int?`, found `int`\n"
    );
    assert_eq!(
        diagnostics("function f(n: int) -> int\n    return ok(n)\n"),
        "test.hero:2:12: error[type_mismatch]: `ok(…)` builds a fallible value, and `int` is not one — no `T` is ever promoted to a `T?`\n"
    );
}

#[test]
fn ok_carries_the_inner_type_and_fail_two_strings() {
    assert_eq!(
        diagnostics("function f(s: str) -> int?\n    return ok(s)\n"),
        "test.hero:2:15: error[type_mismatch]: expected `int`, found `str`\n"
    );
    assert_eq!(
        diagnostics("function f() -> int?\n    return fail(\"code\", 42)\n"),
        "test.hero:2:25: error[type_mismatch]: expected `str`, found `int`\n"
    );
}

/// `?` needs two things, and each has its own message: the value must be
/// fallible, and the function must be able to fail.
#[test]
fn the_question_mark_needs_a_fallible_value_and_a_fallible_caller() {
    assert_clean(
        "\
function read(s: str) -> int?
    return ok(s.len())

function twice(s: str) -> int?
    n = read(s)?
    return ok(n * 2)
",
    );
    assert_eq!(
        diagnostics(
            "\
function read(s: str) -> int?
    return ok(s.len())

function twice(s: str) -> int
    n = read(s)?
    return n * 2
"
        ),
        "test.hero:5:9: error[try_in_infallible]: `?` hands the error to the caller, so this function's result must be fallible — it is `int`, not `int?`\n"
    );
    assert_eq!(
        diagnostics("function f(n: int) -> int?\n    return ok(n?)\n"),
        "test.hero:2:15: error[not_fallible]: `?` propagates an error, and `int` cannot fail — remove the `?`\n"
    );
}

/// §4.6's general way in: `match` over `.ok` and `.err`, exhaustive like any
/// other two-case variant.
#[test]
fn a_fallible_is_matched_on_ok_and_err() {
    assert_clean(
        "\
function read(s: str) -> int?
    return ok(s.len())

function report(s: str) -> str
    return match read(s)
        .ok v  => v.to_str()
        .err e => e.code + \": \" + e.msg
",
    );
    assert_eq!(
        diagnostics(
            "\
function read(s: str) -> int?
    return ok(s.len())

function report(s: str) -> str
    return match read(s)
        .ok v => v.to_str()
"
        ),
        "test.hero:5:12: error[non_exhaustive]: this `match` does not cover `.err`\n"
    );
}

/// §4.6 states the error's two fields and nothing more, so a third one is an
/// error that lists the two.
#[test]
fn the_error_has_exactly_two_fields() {
    assert_eq!(
        diagnostics(
            "\
function read(s: str) -> int?
    return ok(s.len())

function report(s: str) -> str
    return match read(s)
        .ok v  => v.to_str()
        .err e => e.detail
"
        ),
        "test.hero:7:21: error[no_such_field]: `the error` has no field `detail` — `the error` has `code`, `msg`\n"
    );
}

#[test]
fn the_three_readers_have_the_types_the_table_says() {
    assert_eq!(
        type_of_last(
            "\
function read(s: str) -> int?
    return ok(s.len())

function f(s: str) -> int
    return read(s).must()
"
        ),
        "int"
    );
    assert_eq!(
        type_of_last(
            "\
function read(s: str) -> int?
    return ok(s.len())

function f(s: str) -> int
    return read(s).default(0)
"
        ),
        "int"
    );
    assert_eq!(
        type_of_last(
            "\
function read(s: str) -> int?
    return ok(s.len())

function f(s: str) -> bool
    return read(s).is_err()
"
        ),
        "bool"
    );
    assert_eq!(
        diagnostics(
            "\
function read(s: str) -> int?
    return ok(s.len())

function f(s: str) -> int
    return read(s).default(\"none\")
"
        ),
        "test.hero:5:28: error[type_mismatch]: expected `int`, found `str`\n"
    );
}

/// §4.9: a map access is fallible, which is what makes a missing key a value
/// rather than a crash — and `keys` is the boolean test beside it.
#[test]
fn a_map_access_is_fallible() {
    assert_clean(
        "\
function lookup(env: {str: int}, key: str) -> int?
    if !env[key].is_err()
        return fail(\"unknown_name\", \"undefined: \" + key)
    return ok(env[key].must())
",
    );
    assert_eq!(
        diagnostics(
            "\
function f(env: {str: int}, key: str) -> int
    return env[key]
"
        ),
        "test.hero:2:12: error[type_mismatch]: expected `int`, found `int?`\n"
    );
}
