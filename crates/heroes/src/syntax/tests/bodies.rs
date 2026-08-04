//! Statements and expressions.
//!
//! Expressions are pinned through the dump's fully-parenthesised rendering,
//! which is the point: `2 + 3 * 4` printing as `(2 + (3 * 4))` *is* the
//! precedence test. Every assertion below reads as "this source means this
//! grouping".

use super::dump;

/// One statement in `main`, dumped. Keeps the tests about the line under
/// test instead of about the wrapper.
fn stmt(text: &str) -> String {
    let dumped = dump(&format!("main = function: ()\n    {text}\n"));
    dumped
        .strip_prefix("file test.hero\n  function main() -> ()\n")
        .unwrap_or(&dumped)
        .to_string()
}

// --- the §4.14 precedence table ---------------------------------------

#[test]
fn multiplication_binds_tighter_than_addition() {
    assert_eq!(stmt("x = 2 + 3 * 4"), "    bind x = (2 + (3 * 4))\n");
    assert_eq!(stmt("x = 2 * 3 + 4"), "    bind x = ((2 * 3) + 4)\n");
}

#[test]
fn arithmetic_is_left_associative() {
    assert_eq!(stmt("x = 1 - 2 - 3"), "    bind x = ((1 - 2) - 3)\n");
    assert_eq!(stmt("x = 8 / 4 / 2"), "    bind x = ((8 / 4) / 2)\n");
}

/// Comparisons sit *above* `&&`, so a compound condition needs no
/// parentheses — the property that makes `for a < b && c < d` readable.
#[test]
fn comparison_binds_tighter_than_and_which_binds_tighter_than_or() {
    assert_eq!(
        stmt("x = a < b && c < d"),
        "    bind x = ((a < b) && (c < d))\n"
    );
    assert_eq!(stmt("x = a && b || c"), "    bind x = ((a && b) || c)\n");
    assert_eq!(stmt("x = a || b && c"), "    bind x = (a || (b && c))\n");
}

#[test]
fn unary_binds_tighter_than_arithmetic_and_looser_than_a_call() {
    assert_eq!(stmt("x = -a + b"), "    bind x = ((-a) + b)\n");
    assert_eq!(stmt("x = !l.at_end()"), "    bind x = (!l.at_end())\n");
}

#[test]
fn parentheses_group_and_nothing_else() {
    assert_eq!(stmt("x = (2 + 3) * 4"), "    bind x = ((2 + 3) * 4)\n");
}

// --- the postfix chain -------------------------------------------------

/// `x.f` is a field, `x.f(y)` is UFCS — the `(` decides (§4.11). Both, and
/// indexing and `?`, chain left to right.
#[test]
fn fields_calls_indices_and_try_chain_in_order() {
    assert_eq!(stmt("x = l.pos"), "    bind x = l.pos\n");
    assert_eq!(stmt("x = l.here()"), "    bind x = l.here()\n");
    assert_eq!(stmt("x = l.text[l.pos]"), "    bind x = l.text[l.pos]\n");
    assert_eq!(stmt("x = f(y)?"), "    bind x = f(y)?\n");
    assert_eq!(
        stmt("x = s.children.sum_of(env)"),
        "    bind x = s.children.sum_of(env)\n"
    );
}

/// §4.9: named arguments, mandatory where two parameters share a type. §4.8:
/// `@` at the call site, so the mutation is visible on the line.
#[test]
fn arguments_carry_their_names_and_their_markers() {
    assert_eq!(
        stmt("x = copy(from: a, to: b)"),
        "    bind x = copy(from: a, to: b)\n"
    );
    assert_eq!(stmt("advance(@l)"), "    expr advance(@l)\n");
    assert_eq!(
        stmt("x = Point(x: 3, y: 4)"),
        "    bind x = Point(x: 3, y: 4)\n"
    );
}

// --- literals ----------------------------------------------------------

#[test]
fn containers_and_the_leading_dot_variant() {
    assert_eq!(stmt("x = [3, 1, 4]"), "    bind x = [3, 1, 4]\n");
    assert_eq!(stmt("x = []"), "    bind x = []\n");
    assert_eq!(
        stmt("x = { \"mario\": 30, \"anna\": 25 }"),
        "    bind x = {\"mario\": 30, \"anna\": 25}\n"
    );
    assert_eq!(stmt("x = .plus"), "    bind x = .plus\n");
    assert_eq!(stmt("x = .num(v: 12)"), "    bind x = .num(v: 12)\n");
}

/// §4.9: a multi-line literal separates by newline. Inside brackets the
/// lexer keeps planting terminators (panel 007) — here they are separators.
#[test]
fn a_multi_line_list_separates_by_newline() {
    assert_eq!(
        dump("main = function: ()\n    cases = [\n        \"a\"\n        \"b\"\n    ]\n"),
        "\
file test.hero
  function main() -> ()
    bind cases = [\"a\", \"b\"]
"
    );
}

#[test]
fn a_hole_is_an_expression_anywhere() {
    assert_eq!(stmt("x = ???"), "    bind x = ???\n");
    assert_eq!(stmt("???"), "    expr ???\n");
}

// --- the three line shapes of §4.4 ------------------------------------

#[test]
fn binding_declaration_and_mutation_are_told_apart_by_one_token() {
    assert_eq!(stmt("x = 5"), "    bind x = 5\n");
    assert_eq!(stmt("v: int @ 0"), "    declare v: int @ 0\n");
    assert_eq!(stmt("v @ v + 1"), "    mutate v @ (v + 1)\n");
    // An empty literal cannot say what it holds, so it needs the annotation
    // (§4.5) — and that line is a binding, not a declaration.
    assert_eq!(stmt("xs: [int] = []"), "    bind xs: [int] = []\n");
}

/// A field or an element is a place too: `l.pos @ l.pos + 1` is how the
/// appendix threads a position through the lexer (§4.8).
#[test]
fn a_place_may_be_a_field_or_an_element() {
    assert_eq!(stmt("l.pos @ l.pos + 1"), "    mutate l.pos @ (l.pos + 1)\n");
    assert_eq!(stmt("xs[0] @ 9"), "    mutate xs[0] @ 9\n");
}

#[test]
fn return_break_continue_and_assert() {
    assert_eq!(stmt("return dx * dx"), "    return (dx * dx)\n");
    assert_eq!(stmt("return"), "    return\n");
    assert_eq!(stmt("break"), "    break\n");
    assert_eq!(stmt("continue"), "    continue\n");
    assert_eq!(stmt("assert a == 25"), "    assert (a == 25)\n");
}

// --- control flow ------------------------------------------------------

/// One keyword, two loops (§4.7). `in` is the only difference the parser
/// needs to see.
#[test]
fn both_loops_are_for() {
    assert_eq!(
        dump("main = function: ()\n    for i < 3\n        print(i)\n"),
        "\
file test.hero
  function main() -> ()
    for (i < 3)
      expr print(i)
"
    );
    assert_eq!(
        dump("main = function: ()\n    for x in xs\n        print(x)\n"),
        "\
file test.hero
  function main() -> ()
    for x in xs
      expr print(x)
"
    );
}

#[test]
fn if_else_if_else_is_one_expression() {
    assert_eq!(
        dump(
            "\
main = function: ()
    if c == ' '
        advance(@l)
    else if c == '+'
        push(.plus)
    else
        fail(\"unknown\", \"?\")
"
        ),
        "\
file test.hero
  function main() -> ()
    if (c == ' ')
      expr advance(@l)
    else if (c == '+')
      expr push(.plus)
    else
      expr fail(\"unknown\", \"?\")
"
    );
}

/// §4.7: blocks are expressions, so an `if` can be the value of a binding —
/// one rule seen in two places, not a second form.
#[test]
fn an_if_can_be_a_value() {
    assert_eq!(
        dump("main = function: ()\n    state = if t.done\n        \"[x]\"\n    else\n        \"[ ]\"\n"),
        "\
file test.hero
  function main() -> ()
    bind state = if t.done
      expr \"[x]\"
    else
      expr \"[ ]\"
"
    );
}

/// Patterns: a case, a case that binds its payload, `_` as a payload name,
/// `|` joining patterns, and a block arm (§4.7).
///
/// The `expr` label on the inline bodies is panel 014 visible in the dump: an
/// arm's body is a *statement*, and an expression statement is what carries
/// the arm's value.
#[test]
fn match_arms_carry_patterns_and_bodies() {
    assert_eq!(
        dump(
            "\
factor = function: (t: Token) -> int
    return match t
        .num n                   => n.v
        .name _                  => 0
        .plus | .times | .rparen => fail(\"expected_factor\", \"operator\")
        .lparen =>
            print(t)
            return 1
"
        ),
        "\
file test.hero
  function factor(t: Token) -> int
    return match t
      .num n => expr n.v
      .name _ => expr 0
      .plus | .times | .rparen => expr fail(\"expected_factor\", \"operator\")
      .lparen =>
        expr print(t)
        return 1
"
    );
}

#[test]
fn match_can_stand_as_a_statement() {
    assert_eq!(
        dump(
            "\
main = function: ()
    match calculate(c, env)
        .ok v  => print(v)
        .err e => print(e.code)
"
        ),
        "\
file test.hero
  function main() -> ()
    match calculate(c, env)
      .ok v => expr print(v)
      .err e => expr print(e.code)
"
    );
}

/// `_` is a catch-all arm, legal only where exhaustiveness is impossible
/// (`int`, `str`). The parser records it; M3c decides whether it was allowed.
#[test]
fn a_wildcard_arm_is_recorded_not_judged() {
    assert_eq!(
        dump("f = function: (n: int) -> int\n    return match n\n        0 => 1\n        _ => 2\n"),
        "\
file test.hero
  function f(n: int) -> int
    return match n
      0 => expr 1
      _ => expr 2
"
    );
}

// --- statements that go wrong -----------------------------------------

/// §4.4: `=` binds, `@` declares. Neither, after a type, is a mistake worth
/// naming — and both shapes appear in the message.
#[test]
fn an_annotation_without_a_binding_symbol_is_loud() {
    assert_eq!(
        stmt("v: int 0"),
        "    error\nDIAG test.hero:2:12: error[expected_binding_symbol]: expected `@` to declare a mutable, or `=` to bind, found a number (`0`) — `v: int @ 0` declares a cell, `xs: [int] = []` binds once\n"
    );
}

/// Only a place can be mutated: `f(x) @ 1` names nowhere to put the value.
#[test]
fn mutating_something_that_is_not_a_place_is_loud() {
    assert_eq!(
        stmt("f(x) @ 1"),
        "    mutate f(x) @ 1\nDIAG test.hero:2:5: error[not_a_place]: only a name, a field or an element can be mutated — the left of `@` must name where the value goes\n"
    );
}

/// One statement per line, no semicolons (§4.15). The rest of the line is
/// dropped, so the mistake costs one diagnostic.
#[test]
fn two_statements_on_one_line_are_loud() {
    assert_eq!(
        stmt("x = 1 y = 2"),
        "    bind x = 1\nDIAG test.hero:2:11: error[expected_end_of_line]: expected the end of the line, found a name (`y`) — one statement per line, no semicolons\n"
    );
}

/// `f(a @ n)` — the two rules compose as `f(a: @n)`: `:` names the
/// parameter, `@` marks the argument. One diagnostic, a certain fix, and the
/// tree is the one that was meant.
#[test]
fn a_mutable_marker_where_a_colon_belongs_is_repaired() {
    assert_eq!(
        stmt("shift(a @ n)"),
        "    expr shift(a: @n)\nDIAG test.hero:2:13: error[misplaced_mutable_marker]: a named mutable argument is written `name: @value` — `:` names the parameter, `@` marks the argument\n"
    );
}

/// Panel 017 D: a declaration is not an arm body. `x` would be bound where
/// nothing can read it — the arm *is* one statement, so its scope ends with it.
/// The rejection lives in the parser because in the checker the resolver would
/// speak first, with a repair the author cannot apply.
#[test]
fn a_declaration_is_not_an_arm_body() {
    assert_eq!(
        dump("f = function: (k: int) -> int\n    return match k\n        0 => x = 5\n        _ => 1\n"),
        "\
file test.hero
  function f(k: int) -> int
    return match k
      0 => bind x = 5
      _ => expr 1
DIAG test.hero:3:14: error[declaration_in_arm]: an arm's body may not declare a name — `x` would be bound where nothing can read it; write the value as the arm's body, or open a block
"
    );
}

/// …and a *mutation* stays legal, because it changes a cell that already exists
/// somewhere a reader can see.
#[test]
fn a_mutation_is_still_a_legal_arm_body() {
    assert_eq!(
        dump("f = function: (k: int)\n    v: int @ 0\n    match k\n        0 => v @ 1\n        _ => print(v)\n"),
        "\
file test.hero
  function f(k: int) -> ()
    declare v: int @ 0
    match k
      0 => mutate v @ 1
      _ => expr print(v)
"
    );
}
