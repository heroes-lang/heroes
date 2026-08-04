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
    assert_clean("function f(n: int) -> int\n    return n + 1\n");
}

/// §4.7: a jump is admissible as an arm body, and the `match`'s type comes from
/// the arms that can produce one.
#[test]
fn a_match_takes_its_type_from_the_arms_that_produce_a_value() {
    assert_clean(
        "\
variant Token
    num
        v: int
    eof

function value_of(t: Token) -> int
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
function f(c: bool) -> int
    x = if c
        1
    else
        return 0
    return x
"
        ),
        "int"
    );
}

/// Where every branch jumps there is nothing to bind, and that is the case
/// panel 014's value rule was really protecting — now loud in both constructs.
#[test]
fn a_construct_whose_every_branch_jumps_produces_no_value() {
    assert_eq!(
        diagnostics(
            "\
function f(c: bool) -> int
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
        v: int
    eof

function f(t: Token) -> int
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
        v: int
    eof

function f(t: Token) -> int
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
        v: int
    eof

function f(t: Token) -> int
    tag = match t
        .num n => n.v
        .eof   => \"none\"
    return tag
"
        ),
        "test.hero:9:9: error[type_mismatch]: expected `int`, found `str`\n"
    );
}

/// An `if` used as a value needs an `else`: without one it has no value whenever
/// the condition is false.
#[test]
fn an_if_used_as_a_value_needs_an_else() {
    assert_eq!(
        diagnostics(
            "\
function f(c: bool) -> int
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
function f(xs: [int]) -> int
    total: int @ 0
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
        diagnostics("function f() -> int\n    break\n"),
        "test.hero:2:5: error[jump_outside_loop]: `break` is only meaningful inside a `while` or a `for`\n"
    );
}

#[test]
fn return_agrees_with_the_signature_in_both_directions() {
    assert_eq!(
        diagnostics("function f() -> int\n    return \"x\"\n"),
        "test.hero:2:12: error[type_mismatch]: expected `int`, found `str`\n"
    );
    assert_eq!(
        diagnostics("function f() -> int\n    return\n"),
        "test.hero:2:5: error[missing_value]: this function returns `int`, so `return` needs a value\n"
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
function f(xs: [int]) -> int
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
