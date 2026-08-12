//! Panel 017 A: a jump has no type, and every branch join uses one rule.
//!
//! These are the tests the panel's three options disagreed about. The
//! compiler-engineer built all three and measured that typing a jump `()` — RFC
//! 1216's recorded wrong turn — rejects every function whose body ends in
//! `return`; the historian's objection was that skipping a diverging branch must
//! be *one* rule rather than one per construct. Both are pinned here.

use super::{assert_clean, diagnostics, type_of_last};

/// The program A3 would reject. It is also every function in this compiler.
#[test]
fn a_function_may_end_in_return() {
    assert_clean("function f(n: i64) -> i64\n    return n + 1\n");
}

/// §4.7: a jump is admissible as an arm body, and the `match`'s type comes from
/// the arms that can produce one.
#[test]
fn a_match_takes_its_type_from_the_arms_that_produce_a_value() {
    assert_clean(
        "\
variant Token
    num
        v: i64
    eof

function value_of(t: Token) -> i64
    tag = match t
        .num n => n.v
        .eof   => return 0
    return tag + 1
",
    );
}

/// The same rule in the other joining construct, which is why it is one routine:
/// an `if` whose `else` diverges still has the type of its `then`.
#[test]
fn an_if_with_a_diverging_branch_still_has_a_type() {
    assert_eq!(
        type_of_last(
            "\
function f(c: bool) -> i64
    x = if c
        1
    else
        return 0
    return x
"
        ),
        "i64"
    );
}

/// Where every branch jumps there is nothing to bind, and that is the case
/// panel 014's value rule was really protecting — now loud in both constructs.
#[test]
fn a_construct_whose_every_branch_jumps_produces_no_value() {
    assert_eq!(
        diagnostics(
            "\
function f(c: bool) -> i64
    x = if c
        return 1
    else
        return 0
    return x
"
        ),
        "test.hero:2:9: error[no_value]: this `if` produces no value — every branch jumps, so there is nothing to bind\n"
    );
    assert_eq!(
        diagnostics(
            "\
variant Token
    num
        v: i64
    eof

function f(t: Token) -> i64
    tag = match t
        .num _ => return 1
        .eof   => return 0
    return tag
"
        ),
        "test.hero:7:11: error[no_value]: this `match` produces no value — every branch jumps, so there is nothing to bind\n"
    );
}

/// …and in *statement* position the same shape is legal, which is the sentence
/// Kotlin ships for a non-exhaustive `when` and panel 017 adopted.
#[test]
fn a_match_whose_arms_all_jump_is_legal_as_a_statement() {
    assert_clean(
        "\
variant Token
    num
        v: i64
    eof

function f(t: Token) -> i64
    match t
        .num n => return n.v
        .eof   => return 0
",
    );
}

/// The arms must agree with each other, and the message lands on the arm that
/// disagrees — not on the `match`, and not on the first arm for having chosen.
#[test]
fn arms_that_disagree_are_reported_where_they_disagree() {
    assert_eq!(
        diagnostics(
            "\
variant Token
    num
        v: i64
    eof

function f(t: Token) -> i64
    tag = match t
        .num n => n.v
        .eof   => \"none\"
    return tag
"
        ),
        "test.hero:9:9: error[type_mismatch]: expected `i64`, found `str`\n"
    );
}

/// An `if` used as a value needs an `else`: without one it has no value whenever
/// the condition is false.
#[test]
fn an_if_used_as_a_value_needs_an_else() {
    assert_eq!(
        diagnostics(
            "\
function f(c: bool) -> i64
    x = if c
        1
    return x
"
        ),
        "test.hero:2:9: error[if_without_else]: an `if` used as a value needs an `else`, or it has no value when the condition is false\n"
    );
}

#[test]
fn break_and_continue_belong_to_a_loop() {
    assert_clean(
        "\
function f(xs: [i64]) -> i64
    total: i64 @ 0
    for x in xs
        if x == 0
            continue
        if x > 100
            break
        total @ total + x
    return total
",
    );
    assert_eq!(
        diagnostics("function f() -> i64\n    break\n"),
        "test.hero:2:5: error[jump_outside_loop]: `break` is only meaningful inside a `while` or a `for`\n"
    );
}

#[test]
fn return_agrees_with_the_signature_in_both_directions() {
    assert_eq!(
        diagnostics("function f() -> i64\n    return \"x\"\n"),
        "test.hero:2:12: error[type_mismatch]: expected `i64`, found `str`\n"
    );
    assert_eq!(
        diagnostics("function f() -> i64\n    return\n"),
        "test.hero:2:5: error[missing_value]: this function returns `i64`, so `return` needs a value\n"
    );
    assert_eq!(
        diagnostics("function f()\n    return 1\n"),
        "test.hero:2:5: error[returns_nothing]: this function returns nothing, so `return` takes no value\n"
    );
}

/// A loop may run zero times, so it never counts as diverging. The branch below
/// therefore falls through *without* a value — which is a different thing from a
/// branch that jumps, and the message says which. Collapsing the two would have
/// let this program bind `x` to nothing.
#[test]
fn a_branch_that_ends_on_a_statement_has_no_value() {
    assert_eq!(
        diagnostics(
            "\
function f(xs: [i64]) -> i64
    x = if xs.len() > 0
        for y in xs
            return y
    else
        0
    return x
"
        ),
        "test.hero:3:9: error[no_value]: this branch ends on a statement, so it produces no value — its last line has to be the value\n"
    );
}

// --- §4.7: a function that runs off its end (panel 020) ---------------

/// The class did not exist until M-scalars-run, and the reason it was invisible is that a
/// function body is checked with `Want::Nothing`: its value comes from `return`
/// statements, so nothing ever compared the *tail* against the declared result.
/// Verified before writing it — this exact program checked clean through all of M-ir-lowering.
#[test]
fn a_function_that_can_run_off_its_end_is_rejected() {
    let said = diagnostics("function sign(x: i64) -> i64\n    if x > 0\n        return 1\n");
    assert!(said.contains("missing_return"), "{said}");
    assert!(said.contains("`sign` must return `i64`"), "{said}");
    // The span is the written result type — rustc's choice, for rustc's reason: the
    // missing statement has no line to point at, and the promise does.
    assert!(said.contains(":1:26:"), "{said}");
}

#[test]
fn every_path_returning_is_accepted_however_it_is_written() {
    assert_clean("function sign(x: i64) -> i64\n    if x > 0\n        return 1\n    return 0\n");
    assert_clean("function sign(x: i64) -> i64\n    if x > 0\n        return 1\n    else\n        return 0\n");
    // And a `()` result owes nothing.
    assert_clean("function shout(n: i64)\n    if n > 0\n        print(n)\n");
}

/// The rule **over-rejects** `while true`, deliberately and in good company: Rust's
/// `while true` does not diverge either — that is what `loop` is for, and Heroes has
/// no `loop`. So a function that declares a result and never leaves its loop needs an
/// unreachable `return` after it. Recorded rather than fixed: the alternative is
/// special-casing a literal condition, which is a rule the spec would have to state
/// for one program shape nobody has written yet.
#[test]
fn the_rule_over_rejects_an_infinite_loop_as_rust_does() {
    let said = diagnostics("function forever() -> i64\n    while true\n        print(1)\n");
    assert!(said.contains("missing_return"), "{said}");
    // The repair is one line, and it is the same line Rust asks for.
    assert_clean("function forever() -> i64\n    while true\n        print(1)\n    return 0\n");
}

/// §4.16's file-wide hole exemption, which the appendix earned within an hour: a body
/// that is `???` falls off its end by construction, and that is what an unwritten
/// thing does. The exemption is file-wide because the unused rule's already is.
#[test]
fn a_file_with_a_hole_is_exempt_from_the_return_rule() {
    assert_clean("function later(x: i64) -> i64\n    ???\n");
    // …and the exemption is file-wide, not function-wide: one hole anywhere excuses
    // the whole file, exactly as it excuses an unused binding.
    assert_clean("function sign(x: i64) -> i64\n    if x > 0\n        return 1\n\nfunction later() -> i64\n    ???\n");
}
