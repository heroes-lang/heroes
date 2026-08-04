//! What a name resolves to, and what the compiler says when nothing does.

use crate::diagnostics::Certainty;
use crate::resolve::{Ref, BUILTINS};

use super::{assert_clean, diagnostics, resolved};

/// The three kinds of name a body can hold, in one function — which is what
/// `Ref`'s three resolved variants are for. The list is read straight out of the
/// dense table, in arena order.
#[test]
fn a_local_a_top_level_name_and_a_builtin_each_record_their_own_kind() {
    let (out, _) = resolved(
        "\
constant MAX: int
    64

function main()
    x = MAX
    print(x)
",
    );
    let resolved_refs: Vec<Ref> =
        out.uses.iter().copied().filter(|r| *r != Ref::Unresolved).collect();
    assert_eq!(
        resolved_refs,
        vec![Ref::Top(0), Ref::Builtin(builtin("print")), Ref::Local(0)]
    );
    assert!(out.diagnostics.is_empty());
}

/// Every expression gets an entry, name or not: the table is dense because the
/// array is Heroes' only indirection (§4.10), and a later pass indexes it
/// directly rather than searching.
#[test]
fn the_use_table_has_one_entry_per_expression() {
    let (out, src) = resolved(
        "\
function main()
    print(1 + 2)
",
    );
    let parsed = crate::syntax::parse(&src);
    assert_eq!(out.uses.len(), parsed.ast.exprs.len());
    assert_eq!(out.type_uses.len(), parsed.ast.types.len());
}

#[test]
fn an_unknown_name_names_the_nearest_candidate_and_offers_a_certain_fix() {
    let program = "\
function main()
    total = 1
    print(totl)
";
    assert_eq!(
        diagnostics(program),
        "test.hero:3:11: error[unknown_name]: nothing named `totl` is in scope — did you mean `total`?\n"
    );
    let (out, _) = resolved(program);
    let fixes = &out.diagnostics[0].fixes;
    assert_eq!(fixes.len(), 1);
    assert_eq!(fixes[0].replacement, "total");
    assert_eq!(fixes[0].certainty, Certainty::Certain);
}

/// More than one candidate is prose, never a fix: applying one of three is
/// guessing, and only `certain` fixes may ever be applied (§4.17).
#[test]
fn several_candidates_are_listed_and_carry_no_fix() {
    let (out, src) = resolved(
        "\
function main()
    cat = 1
    cut = 2
    bat = 3
    print(cot + cat + cut + bat)
",
    );
    let rendered = out.diagnostics[0].render_line(&src);
    assert!(rendered.contains("the closest names in scope are"), "{rendered}");
    assert!(out.diagnostics[0].fixes.is_empty());
}

/// A difference in case only is the likeliest slip and the cheapest repair.
#[test]
fn a_case_only_difference_is_a_candidate() {
    assert_eq!(
        diagnostics(
            "\
record Point
    x: int

function main()
    p = point(x: 1)
    print(p.x)
"
        ),
        "test.hero:5:9: error[unknown_name]: nothing named `point` is in scope — did you mean `Point`?\n"
    );
}

/// `x.f(y)` is `f(x, y)`, so the diagnostic teaches the rewrite (§4.11). The
/// historian's condition in panel 015: do not copy the UFCS languages' error
/// shape, which names the receiver's type — Heroes has no methods to name.
#[test]
fn an_unknown_ufcs_name_teaches_the_rewrite() {
    assert_eq!(
        diagnostics(
            "\
function main()
    m: {str: int} @ {}
    m.set(\"a\", 1)
"
        ),
        "test.hero:3:7: error[unknown_function]: no function named `set` — `x.set(…)` means `set(x, …)`, so `set` has to be a top-level function or a built-in\n"
    );
}

/// The compiler-engineer's veto in panel 015, now a test: a field can hold a
/// function value (§4.13) and §4.11 resolves the dot by looking at the
/// receiver's *type* first. M3a has no types, so it must stay silent.
#[test]
fn a_field_that_could_hold_a_function_is_left_to_the_checker() {
    assert_clean(
        "\
record Holder
    cb: (function(int) -> int)

function run(h: Holder, n: int) -> int
    return h.cb(n)
",
    );
}

#[test]
fn a_top_level_name_that_is_not_a_function_cannot_be_called_through_a_dot() {
    assert_eq!(
        diagnostics(
            "\
constant MAX: int
    64

function main()
    x = 1
    print(x.MAX())
"
        ),
        "test.hero:6:13: error[not_a_function]: `MAX` is a constant, so `x.MAX(…)` cannot call it\n"
    );
}

#[test]
fn a_variant_name_in_value_position_says_where_the_values_are() {
    assert_eq!(
        diagnostics(
            "\
variant Token
    plus
    minus

function main()
    t = Token
    print(t)
"
        ),
        "test.hero:6:9: error[variant_in_value_position]: `Token` is a variant, so it names a type and not a value — write one of its cases, `.case` or `.case(field: value)`\n"
    );
}

#[test]
fn a_record_is_called_by_name_because_construction_is_a_call() {
    assert_clean(
        "\
record Point
    x: int
    y: int

function main()
    p = Point(x: 1, y: 2)
    print(p.x)
",
    );
}

/// `fail` is a lexer keyword *and* a built-in name: it reaches the resolver as
/// an ordinary name (`syntax/primary.rs`), so it has to resolve to something.
#[test]
fn fail_and_ok_resolve_as_builtins() {
    assert_clean(
        "\
function half(n: int) -> int?
    if n < 0
        return fail(\"negative\", \"n must not be negative\")
    return ok(n / 2)
",
    );
}

/// The int-to-string conversion is on §4.20's inventory and was missing from the
/// spec's list — found by resolving the acceptance program, which calls it. Panel
/// 017 renamed it `to_str`, so the three conversions share one scheme.
#[test]
fn to_str_is_a_builtin_because_the_inventory_says_so() {
    assert_clean(
        "\
function describe(n: int) -> str
    return \"n = \" + n.to_str()
",
    );
}

#[test]
fn a_name_used_before_its_own_binding_is_not_defined_yet() {
    assert_eq!(
        diagnostics(
            "\
function main()
    x = x + 1
    print(x)
"
        ),
        "test.hero:2:9: error[unknown_name]: nothing named `x` is in scope\n"
    );
}

/// A hole is a name-shaped nothing: it resolves to no reference, is not an
/// error, and suspends the unused rule (§4.16).
#[test]
fn a_hole_resolves_to_nothing_and_is_not_an_error() {
    let (out, _) = resolved(
        "\
function f() -> int
    return ???
",
    );
    assert!(out.has_hole);
    assert!(out.diagnostics.is_empty());
}

fn builtin(name: &str) -> u32 {
    BUILTINS.iter().position(|b| b.name == name).expect("a built-in") as u32
}
