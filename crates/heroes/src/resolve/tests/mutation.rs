//! What may be written, and what the compiler says when it may not (§4.4's
//! third line shape, §4.8's `@` parameters).

use super::{assert_clean, diagnostics};

#[test]
fn a_cell_and_a_mutable_parameter_may_be_written() {
    assert_clean(
        "\
record Lex
    pos: int

function advance(@l: Lex)
    l.pos @ l.pos + 1

function count(n: int) -> int
    total: int @ 0
    while total < n
        total @ total + 1
    return total
",
    );
}

/// The error teaches the rule and hands over the shape of the repair, but the
/// type is the missing half and M3a has none: prose, not a fix a tool applies.
#[test]
fn an_immutable_binding_cannot_be_written() {
    assert_eq!(
        diagnostics(
            "\
function main()
    x = 1
    x @ 2
    print(x)
"
        ),
        "test.hero:3:5: error[not_mutable]: `x` was bound with `=`, which binds once, forever — a cell is declared with its type: `x: <type> @ <value>`\n"
    );
}

/// §4.8's whole rule in one message: the marker is on the parameter *and* on
/// every call site.
#[test]
fn a_parameter_without_the_marker_cannot_be_written() {
    assert_eq!(
        diagnostics(
            "\
function bump(n: int) -> int
    n @ n + 1
    return n
"
        ),
        "test.hero:2:5: error[not_mutable]: `n` is a parameter without `@`, so it cannot be written — mark it `@n` in the signature and at every call site (copy in, copy out)\n"
    );
}

#[test]
fn a_loop_variable_is_bound_afresh_and_is_not_a_cell() {
    assert_eq!(
        diagnostics(
            "\
function main(xs: [int])
    for x in xs
        x @ x + 1
"
        ),
        "test.hero:3:9: error[not_mutable]: `x` is the loop variable, and it is bound afresh for each element — write to a cell declared outside the loop\n"
    );
}

#[test]
fn there_are_no_mutable_globals() {
    assert_eq!(
        diagnostics(
            "\
constant MAX: int
    64

function main()
    MAX @ 1
    print(MAX)
"
        ),
        "test.hero:5:5: error[no_mutable_globals]: `MAX` is a top-level declaration, and there are no mutable globals — pass what changes as an `@` parameter\n"
    );
}

/// The index of an element write is read, not written — `xs[i] @ 0` uses `i`.
#[test]
fn the_index_of_an_element_write_is_a_read() {
    assert_clean(
        "\
function main(n: int)
    xs: [int] @ [0, 0]
    i = n
    xs[i] @ 1
    print(xs)
",
    );
}

/// A typo on the left of `@` is the case §4.4's mandatory type exists for: the
/// name was never declared, so it is not a silent new binding.
#[test]
fn a_typo_on_the_left_of_a_mutation_is_an_unknown_name() {
    assert_eq!(
        diagnostics(
            "\
function sum(xs: [int]) -> int
    total: int @ 0
    for x in xs
        totl @ total + x
    return total
"
        ),
        "test.hero:4:9: error[unknown_name]: nothing named `totl` is in scope — did you mean `total`?\n"
    );
}
