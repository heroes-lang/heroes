//! Literals, operators, `if`, and where a value is not allowed (§4.14, §4.3).

use super::{assert_clean, diagnostics, type_of_last};
use crate::types::{IntKind, Ty, TyId, Types};

#[test]
fn the_five_literal_kinds_get_their_types() {
    assert_eq!(type_of_last("constant MAX: i64\n    1\n"), "i64");
    assert_eq!(type_of_last("constant X: f64\n    1.5\n"), "f64");
    assert_eq!(type_of_last("constant X: bool\n    true\n"), "bool");
    assert_eq!(type_of_last("constant X: str\n    \"hi\"\n"), "str");
    // §4.3: a character literal *is* an `i64`, which is what makes
    // `c >= '0' && c <= '9'` ordinary arithmetic.
    assert_eq!(type_of_last("constant X: i64\n    'a'\n"), "i64");
}

#[test]
fn arithmetic_is_monomorphic_and_says_so() {
    assert_clean("function f(a: i64, b: i64) -> i64\n    return a + b\n");
    assert_clean("function f(a: f64, b: f64) -> f64\n    return a * b\n");
    assert_eq!(
        diagnostics("function f(a: i64, b: f64) -> i64\n    return a + b\n"),
        "test.hero:2:12: error[mixed_arithmetic]: `+` takes two of one type — two integers of the SAME width, or two `f64` — and found `i64` and `f64`\n"
    );
}

/// Panel 017 B: `+` joins two `str`, and nothing else does. design.md §4.20
/// commits the runtime to the concatenation; before this the language could not
/// build a message at all.
#[test]
fn plus_joins_two_strings_and_only_plus() {
    assert_clean("function f(a: str, b: str) -> str\n    return a + b\n");
    assert_clean(
        "function describe(n: i64) -> str\n    return \"n = \" + n.to_str()\n",
    );
    assert_eq!(
        diagnostics("function f(a: str, b: str) -> str\n    return a * b\n"),
        "test.hero:2:12: error[bad_operand]: `*` takes `i64` or `f64`, found `str`\n"
    );
    // The conversion is named in the message, because the repair is the point.
    assert_eq!(
        diagnostics("function f(a: str, n: i64) -> str\n    return a + n\n"),
        "test.hero:2:12: error[mixed_arithmetic]: `+` takes two of one type — two integers of the SAME width, or two `f64` — and found `str` and `i64`\n"
    );
}

#[test]
fn equality_is_structural_and_ordering_is_not() {
    assert_clean(
        "\
record Point
    x: i64

function same(a: Point, b: Point) -> bool
    return a == b
",
    );
    assert_clean("function f(a: [i64], b: [i64]) -> bool\n    return a != b\n");
    assert_eq!(
        diagnostics("function f(a: str, b: str) -> bool\n    return a < b\n"),
        "test.hero:2:12: error[bad_operand]: `<` takes `i64` or `f64`, found `str`\n"
    );
    assert_eq!(
        diagnostics("function f(a: i64, b: str) -> bool\n    return a == b\n"),
        "test.hero:2:12: error[type_mismatch]: expected `i64`, found `str`\n"
    );
}

#[test]
fn there_is_no_truthiness() {
    assert_eq!(
        diagnostics(
            "\
function f(n: i64) -> i64
    if n
        return 1
    return 0
"
        ),
        "test.hero:2:8: error[not_bool]: an `if` condition must be `bool`, found `i64` — there is no truthiness in this language\n"
    );
    assert_eq!(
        diagnostics(
            "\
function f(n: i64) -> i64
    total: i64 @ 0
    while n
        total @ total + 1
    return total
"
        ),
        "test.hero:3:11: error[not_bool]: a `while` condition must be `bool`, found `i64` — there is no truthiness in this language\n"
    );
}

#[test]
fn boolean_operators_take_bool_only() {
    assert_clean("function f(a: bool, b: bool) -> bool\n    return a && !b\n");
    assert_eq!(
        diagnostics("function f(a: i64) -> bool\n    return !a\n"),
        "test.hero:2:12: error[bad_operand]: `!` takes `bool`, found `i64`\n"
    );
}

/// Panel 003: the value of a statement is discarded, and discarding it silently
/// is the mistake. The fix is machine-applicable because it preserves meaning.
#[test]
fn a_discarded_value_is_an_error_with_a_certain_fix() {
    let (out, _) = super::checked(
        "\
function main(xs: [i64])
    xs.push(4)
",
    );
    assert_eq!(out.diagnostics.len(), 1);
    assert_eq!(out.diagnostics[0].code, "discarded_value");
    assert_eq!(out.diagnostics[0].fixes[0].replacement, "_ = ");
    assert_eq!(out.diagnostics[0].fixes[0].certainty, crate::diagnostics::Certainty::Certain);
}

/// Panel 017 C, and Nim's rule verbatim (the historian's row): `()` is a type in
/// parameter and result position, and no name may hold it.
#[test]
fn a_unit_value_cannot_be_bound() {
    assert_eq!(
        diagnostics(
            "\
function main()
    done = print(\"a\")
    print(done)
"
        ),
        "test.hero:2:5: error[bound_unit]: `done` would hold `()`, which is no value — keep the call and drop the binding\n"
    );
}

/// §4.5's two inference failures, and the compiler asks for the annotation by
/// name rather than reporting a mystery.
#[test]
fn an_empty_literal_needs_its_annotation() {
    assert_clean("function main()\n    xs: [i64] = []\n    print(xs.len())\n");
    assert_eq!(
        diagnostics("function main()\n    xs = []\n    print(xs.len())\n"),
        "test.hero:2:10: error[cannot_infer]: the array literal is empty, so there is nothing to infer its type from — write the annotation\n"
    );
    assert_eq!(
        diagnostics("function main()\n    m = {}\n    print(m.len())\n"),
        "test.hero:2:9: error[cannot_infer]: the map literal is empty, so there is nothing to infer its type from — write the annotation\n"
    );
}

#[test]
fn a_declared_cell_checks_its_initialiser_and_every_write() {
    assert_clean(
        "\
function main()
    total: i64 @ 0
    total @ total + 1
    print(total)
",
    );
    assert_eq!(
        diagnostics("function main()\n    total: i64 @ \"x\"\n    print(total)\n"),
        "test.hero:2:18: error[type_mismatch]: expected `i64`, found `str`\n"
    );
    assert_eq!(
        diagnostics(
            "\
function main()
    total: i64 @ 0
    total @ \"x\"
    print(total)
"
        ),
        "test.hero:3:13: error[type_mismatch]: expected `i64`, found `str`\n"
    );
}

#[test]
fn indexing_knows_its_three_containers() {
    // `s[i]` is a `u8` after M-sized-integers: the language's byte source says
    // byte, which is what gives `[u8]` a producer (panel 042, author ruling).
    assert_eq!(type_of_last("function f(s: str) -> u8\n    return s[0]\n"), "u8");
    assert_eq!(
        type_of_last("function f(xs: [str]) -> str\n    return xs[0]\n"),
        "str"
    );
    // §4.9: a map yields `V?`, which is what makes a missing key a value.
    assert_eq!(
        type_of_last("function f(m: {str: i64}) -> i64?\n    return m[\"a\"]\n"),
        "i64?"
    );
    assert_eq!(
        diagnostics("function f(n: i64) -> i64\n    return n[0]\n"),
        "test.hero:2:12: error[not_indexable]: `i64` cannot be indexed — `s[i]`, `xs[i]` and `m[k]` are for `str`, `[T]` and `{K: V}`\n"
    );
}

#[test]
fn a_for_in_walks_an_array_and_names_the_alternative() {
    assert_clean("function main(xs: [i64])\n    for x in xs\n        print(x)\n");
    assert_eq!(
        diagnostics("function main(s: str)\n    for c in s\n        print(c)\n"),
        "test.hero:2:14: error[not_iterable]: `for x in …` walks an array, and this is a `str` — a string's characters come from `s.chars()`, a count from `range(a, b)`\n"
    );
}

#[test]
fn an_assert_takes_a_bool() {
    assert_clean("test \"it holds\"\n    assert 1 + 1 == 2\n");
    assert_eq!(
        diagnostics("test \"it holds\"\n    assert 1 + 1\n"),
        "test.hero:2:12: error[not_bool]: an `assert` must be `bool`, found `i64` — there is no truthiness in this language\n"
    );
}

/// **The prelude's fixed ids, asserted against the prelude itself.**
///
/// `Types::new` interns nine scalars in a fixed order so their ids are constants
/// the whole pass can name without a lookup — `int()` is `TyId(2)`, `failure()`
/// is `TyId(8)`, written out as literals. That is a premise about the order of a
/// list twenty lines away, and `M-sized-integers` is about to add seven more
/// integers to this table: inserting one into the *middle* of that list would
/// silently re-point every accessor below it, and nothing here or anywhere else
/// would go red (panel 042's compiler-engineer named this site).
///
/// So the premise gets the test CLAUDE.md §11 owes it. The failure message names
/// what depends on it, because the symptom would otherwise be unrecognisable: a
/// program whose `str` had quietly become a `ptr`.
#[test]
fn the_prelude_ids_match_their_accessors() {
    let mut types = Types::new();
    let expected: [(&str, TyId, Ty); 8] = [
        ("error", types.error(), Ty::Error),
        ("unit", types.unit(), Ty::Unit),
        ("i64", types.int(), Ty::Int(IntKind::I64)),
        ("f64", types.f64(), Ty::F64),
        ("bool", types.bool(), Ty::Bool),
        ("str", types.str(), Ty::Str),
        ("failure", types.failure(), Ty::Failure),
        // `ptr` and `cstr` have no accessor and are interned on demand; they are
        // here so the list below is the whole prelude and a reader can check it.
        ("ptr", types.intern(Ty::Ptr), Ty::Ptr),
    ];
    for (name, id, ty) in expected {
        assert_eq!(
            types.get(id),
            ty,
            "`Types::{name}()` returns a hardcoded id and the prelude in `Types::new` no longer \
             puts {ty:?} there. Every accessor after the inserted entry is now off by one, and \
             nothing else in this compiler checks that — a new width belongs at the END of \
             `new()`'s list, never in the middle of it."
        );
    }
}

/// **`IntKind::contains` and `IntKind::range` must agree, at all 64 pairs.**
///
/// Two answers to one question live in `table.rs`: `contains` decides whether a
/// conversion can fail, and `range` decides what the emitted test compares. The
/// emitter trusts them to agree — it skips the test entirely when `contains` says
/// so, and `emit/ops.rs`'s fallback for "no test needed" is `hero_unreachable`
/// rather than `if (1)` precisely because a disagreement must not be survivable.
///
/// If this fires, three things depend on it: the `fit_` arm in `types/builtins.rs`
/// (which returns `T` or `T?` from `contains`), the emission branch in
/// `emit/ops.rs` (which builds its test from `range`), and every `.must()` a
/// Heroes programme did *not* write because a widening was infallible. A
/// `contains` that says yes where the ranges say no is a silent truncation.
#[test]
fn contains_agrees_with_range() {
    for target in crate::types::INT_KINDS {
        for source in crate::types::INT_KINDS {
            let (t_low, t_high) = target.range();
            let (s_low, s_high) = source.range();
            let by_range = s_low >= t_low && s_high <= t_high;
            assert_eq!(
                target.contains(source),
                by_range,
                "`{}::contains({})` says {} and the ranges say {} — {}..{} against {}..{}. \
                 `types/builtins.rs` decides `T` vs `T?` from the first and `emit/ops.rs` \
                 builds its range test from the second; where they part, a narrowing is \
                 emitted with no check and truncates in silence.",
                target.name(), source.name(), target.contains(source), by_range,
                s_low, s_high, t_low, t_high
            );
        }
    }
}
