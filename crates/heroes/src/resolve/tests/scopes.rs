//! Shadowing, sibling scopes, and the order-free top level (§4.2, §4.4).

use super::{assert_clean, diagnostics, scopes};

/// Mutual recursion with no forward declarations — the property the top-level
/// table exists for, and the one the appendix's four parser functions rely on.
#[test]
fn declaration_order_never_matters() {
    assert_clean(
        "\
function even(n: int) -> bool
    if n == 0
        return true
    return odd(n - 1)

function odd(n: int) -> bool
    if n == 0
        return false
    return even(n - 1)
",
    );
}

#[test]
fn two_declarations_of_one_name_collide() {
    assert_eq!(
        diagnostics(
            "\
function f() -> int
    return 1

function f() -> int
    return 2
"
        ),
        "test.hero:4:10: error[declared_twice]: `f` is already declared at line 1 — one file is one program, and a name means one thing in it\n"
    );
}

/// A record and a variant share one namespace: the collision is the point.
#[test]
fn a_record_and_a_variant_of_the_same_name_collide() {
    assert_eq!(
        diagnostics(
            "\
record Token
    x: int

variant Token
    plus
"
        ),
        "test.hero:4:9: error[declared_twice]: `Token` is already declared at line 1 — one file is one program, and a name means one thing in it\n"
    );
}

#[test]
fn a_local_may_not_shadow_an_enclosing_binding() {
    assert_eq!(
        diagnostics(
            "\
function main()
    x = 1
    while x > 0
        x = 2
        print(x)
"
        ),
        "test.hero:4:9: error[shadowed_binding]: `x` is already in scope, bound at line 2 — shadowing is an error here: a name means one thing for as long as it is visible\n"
    );
}

#[test]
fn a_local_may_not_shadow_a_parameter() {
    assert_eq!(
        diagnostics(
            "\
function f(n: int) -> int
    n = 2
    return n
"
        ),
        "test.hero:2:5: error[shadowed_binding]: `n` is already in scope, bound at line 1 — shadowing is an error here: a name means one thing for as long as it is visible\n"
    );
}

/// A top-level name is in scope everywhere, so it is shadowable by nothing —
/// the gap the compiler-engineer asked panel 015 to close explicitly.
#[test]
fn a_local_may_not_shadow_a_top_level_declaration() {
    assert_eq!(
        diagnostics(
            "\
constant MAX: int
    64

function main()
    MAX = 1
    print(MAX)
"
        ),
        "test.hero:5:5: error[shadowed_binding]: `MAX` is the name of the declaration at line 1, which is in scope everywhere — pick another name\n"
    );
}

/// One rule, one message, everywhere: a built-in's name is taken at the top
/// level, as a parameter and as a local (panel 015 question C, one tier).
#[test]
fn a_builtin_name_is_taken_in_every_position() {
    assert_eq!(
        diagnostics(
            "\
function len() -> int
    return 1
"
        ),
        "test.hero:1:10: error[builtin_name_taken]: `len` is a built-in of the language, so the name is taken everywhere — pick another name\n"
    );
    assert_eq!(
        diagnostics(
            "\
function f(ok: int) -> int
    return ok
"
        ),
        "test.hero:1:12: error[builtin_name_taken]: `ok` is a built-in of the language, so the name is taken everywhere — pick another name\n"
    );
    assert_eq!(
        diagnostics(
            "\
function main()
    range = 1
    print(range)
"
        ),
        "test.hero:2:5: error[builtin_name_taken]: `range` is a built-in of the language, so the name is taken everywhere — pick another name\n"
    );
}

/// Two blocks that do not contain one another are not in scope in one another:
/// `for x in xs` twice in one function is the commonest shape in imperative
/// code, and Java (JLS §6.4) and C# (CS0136) both say so in as many words.
#[test]
fn sibling_scopes_may_reuse_a_name() {
    assert_clean(
        "\
function sum_both(xs: [int], ys: [int]) -> int
    total: int @ 0
    for x in xs
        total @ total + x
    for x in ys
        total @ total + x
    return total
",
    );
}

/// The check is sequential, and this is the case that proves it: the second `w`
/// is *below* the block that binds the first. Java and C# reject this
/// regardless of order; Heroes accepts it, because §1.3 says a line below must
/// not change whether the line above compiles.
#[test]
fn a_binding_below_a_block_does_not_reach_back_into_it() {
    assert_clean(
        "\
function f(flag: bool) -> int
    if flag
        w = 1
        print(w)
    w = 2
    return w
",
    );
}

/// Two arms are two scopes, and one arm's patterns may not bind one name twice.
#[test]
fn each_arm_is_its_own_scope() {
    assert_clean(
        "\
variant Shape
    circle
        r: int
    square
        side: int

function area(s: Shape) -> int
    return match s
        .circle n => n.r * 3
        .square n => n.side * n.side
",
    );
    assert_eq!(
        diagnostics(
            "\
variant Shape
    circle
        r: int
    square
        r: int

function first(s: Shape) -> int
    return match s
        .circle n | .square n => n.r
"
        ),
        "test.hero:9:29: error[duplicate_pattern_binding]: `n` is bound by two patterns of the same arm — one arm binds each name once\n"
    );
}

/// A type parameter is a name too (§4.12), in the type namespace.
#[test]
fn a_type_parameter_may_not_repeat_or_take_a_taken_name() {
    assert_eq!(
        diagnostics(
            "\
function first<A, A>(xs: [A]) -> A
    return xs[0]
"
        ),
        "test.hero:1:19: error[shadowed_binding]: `A` is already in scope, bound at line 1 — shadowing is an error here: a name means one thing for as long as it is visible\n"
    );
    assert_eq!(
        diagnostics(
            "\
function first<int>(xs: [int]) -> int
    return xs[0]
"
        ),
        "test.hero:1:16: error[builtin_name_taken]: `int` is a built-in of the language, so the name is taken everywhere — pick another name\n"
    );
}

/// The dump is the artifact: the table is alphabetical, the indentation is the
/// scope nesting, and the counts are §4.4's rule made readable.
#[test]
fn the_dump_shows_the_table_and_the_nesting() {
    assert_eq!(
        scopes(
            "\
function main(xs: [int])
    total: int @ 0
    for x in xs
        total @ total + x
    print(total)
"
        ),
        "\
file test.hero
module test
symbols
  function main (line 1)
scopes
  function main
    param xs (reads 1, writes 0)
      cell total @ (reads 2, writes 1)
        loop x (reads 1, writes 0)
"
    );
}
