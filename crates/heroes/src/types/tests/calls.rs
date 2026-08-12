//! Calls, built-ins, UFCS, the `@` marker, and generics (§4.9, §4.11, §4.12,
//! §4.13, §4.8).

use super::{assert_clean, diagnostics, type_of_last};

#[test]
fn a_call_checks_its_arguments_and_its_arity() {
    assert_clean(
        "\
function repeat(text: str, times: int) -> str
    out: str @ \"\"
    left: int @ times
    while left > 0
        out @ out + text
        left @ left - 1
    return out

function main()
    print(repeat(\"ab\", 2))
",
    );
    assert_eq!(
        diagnostics(
            "\
function repeat(text: str, times: int) -> str
    print(times)
    return text

function main()
    print(repeat(\"ab\"))
"
        ),
        "test.hero:6:11: error[wrong_arity]: `repeat` takes 2 argument(s), found 1\n"
    );
    assert_eq!(
        diagnostics(
            "\
function repeat(text: str, times: int) -> str
    print(times)
    return text

function main()
    print(repeat(\"ab\", \"two\"))
"
        ),
        "test.hero:6:24: error[type_mismatch]: expected `int`, found `str`\n"
    );
}

/// §4.9's same-typed-argument rule: the one place the language spends tokens on
/// purpose, because `save(user.name, user.id)` type-checks perfectly and does the
/// wrong thing. It is also what metric 3's `swap-args` operator exists to produce.
#[test]
fn two_parameters_of_one_type_must_be_named_at_the_call_site() {
    assert_clean(
        "\
function add(a: int, b: int) -> int
    return a + b

function main()
    print(add(a: 1, b: 2))
",
    );
    assert_eq!(
        diagnostics(
            "\
function add(a: int, b: int) -> int
    return a + b

function main()
    print(add(1, 2))
"
        ),
        "test.hero:5:15: error[needs_label]: two of `add`'s parameters are `int`, so every one of them is named at the call site — this one is `a`\ntest.hero:5:18: error[needs_label]: two of `add`'s parameters are `int`, so every one of them is named at the call site — this one is `b`\n"
    );
    // …and where the types differ, a label is optional — the rule spends nothing
    // where nothing can be inverted.
    assert_clean(
        "\
function at(text: str, index: int) -> int
    return text[index]

function main()
    print(at(\"ab\", 1))
    print(at(text: \"ab\", index: 1))
",
    );
    // The receiver of a UFCS call names nothing: the dot *is* its position.
    assert_clean(
        "\
record Point
    x: int

function dist(a: Point, b: Point) -> int
    return a.x - b.x

function main(p: Point, q: Point)
    print(p.dist(b: q))
",
    );
}

/// §4.11: `x.f(y)` is `f(x, y)`, so the receiver is the first argument and the
/// signature is checked exactly as it would be at a plain call.
#[test]
fn ufcs_is_argument_reordering_and_nothing_else() {
    assert_clean(
        "\
record Point
    x: int
    y: int

function sum_of(p: Point) -> int
    return p.x + p.y

function main(p: Point)
    print(p.sum_of())
    print(sum_of(p))
",
    );
}

/// §4.8: the marker is repeated at the call site, and the dotted form is refused
/// on a `@` first parameter — the whole point of the marker is that it is visible
/// on the line that mutates.
#[test]
fn the_mutable_marker_is_checked_on_both_sides() {
    assert_clean(
        "\
record Lex
    pos: int

function advance(@l: Lex)
    l.pos @ l.pos + 1

function main()
    l: Lex @ Lex(pos: 0)
    advance(@l)
    print(l.pos)
",
    );
    assert_eq!(
        diagnostics(
            "\
record Lex
    pos: int

function advance(@l: Lex)
    l.pos @ l.pos + 1

function main()
    l: Lex @ Lex(pos: 0)
    advance(l)
    print(l.pos)
"
        ),
        "test.hero:9:13: error[marker_mismatch]: `advance` changes this argument, so the call site writes `@` too\n"
    );
    assert_eq!(
        diagnostics(
            "\
record Lex
    pos: int

function advance(@l: Lex)
    l.pos @ l.pos + 1

function main()
    l: Lex @ Lex(pos: 0)
    l.advance()
    print(l.pos)
"
        ),
        "test.hero:9:7: error[ufcs_on_mutable]: `advance` changes its first argument, so it is called `advance(@x, …)` — the dotted form would hide the `@`\n"
    );
}

/// §4.9: record construction is a call, and every field is named, always.
#[test]
fn a_record_is_built_with_every_field_named() {
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
    assert_eq!(
        diagnostics(
            "\
record Point
    x: int
    y: int

function main()
    p = Point(x: 1)
    print(p.x)
"
        ),
        "test.hero:6:9: error[missing_fields]: `Point` is built with every field, named: x:, y: — all of them, always\n"
    );
    assert_eq!(
        diagnostics(
            "\
record Point
    x: int
    y: int

function main()
    p = Point(1, 2)
    print(p.x)
"
        ),
        "test.hero:6:15: error[missing_label]: `Point`'s fields are always named — this one is `x`\ntest.hero:6:18: error[missing_label]: `Point`'s fields are always named — this one is `y`\n"
    );
}

#[test]
fn a_wrong_label_names_the_right_one_and_offers_the_rename() {
    let (out, _) = super::checked(
        "\
record Point
    x: int
    y: int

function main()
    p = Point(x: 1, z: 2)
    print(p.y)
",
    );
    assert_eq!(out.diagnostics.len(), 1);
    assert_eq!(out.diagnostics[0].code, "wrong_label");
    // The bare name: the span is the label's identifier and the `:` is not in
    // it, which is what the `.fixed` goldens caught — the first version of this
    // fix produced `y:: 2`.
    assert_eq!(out.diagnostics[0].fixes[0].replacement, "y");
    assert_eq!(
        out.diagnostics[0].fixes[0].certainty,
        crate::diagnostics::Certainty::Certain
    );
}

/// §4.12: generics are inferred from the arguments and never written at the call
/// site. One pass, no unification variables.
#[test]
fn generics_are_inferred_from_the_arguments() {
    assert_eq!(
        type_of_last(
            "\
function first<A>(xs: [A]) -> A
    return xs[0]

function head() -> int
    return first([1, 2, 3])
"
        ),
        "int"
    );
    assert_clean(
        "\
function apply<A, B>(xs: [A], f: (function(A) -> B)) -> [B]
    out: [B] @ []
    for x in xs
        out @ out.push(f(x))
    return out

function label(n: int) -> str
    return n.to_str()

function main()
    print(apply([1, 2], label).len())
",
    );
}

/// A type parameter that meets two different types is the error, and it lands on
/// the argument that disagreed.
#[test]
fn a_type_parameter_must_agree_with_itself() {
    assert_eq!(
        diagnostics(
            "\
function pair<A>(a: A, b: A) -> A
    _ = b
    return a

function main()
    print(pair(a: 1, b: \"two\"))
"
        ),
        "test.hero:6:25: error[type_mismatch]: expected `int`, found `str`\n"
    );
}

/// §4.13: a top-level function is a value, and its type is written with the word
/// that declares one (panel 013).
#[test]
fn a_function_is_a_value_of_its_signature_type() {
    assert_eq!(
        type_of_last(
            "\
function double(n: int) -> int
    return n * 2

function twice() -> int
    f = double
    return f(2)
"
        ),
        "int"
    );
    assert_eq!(
        diagnostics(
            "\
function double(n: int) -> int
    return n * 2

function takes(f: (function(str) -> int)) -> int
    return f(\"a\")

function main()
    print(takes(double))
"
        ),
        "test.hero:8:17: error[type_mismatch]: expected `(function(str) -> int)`, found `(function(int) -> int)`\n"
    );
}

/// The compiler-engineer's panel-015 veto, now typed: a field holding a function
/// is called through the dot, and §4.11 looks for the field *first*.
#[test]
fn a_function_valued_field_is_called_through_the_dot() {
    assert_clean(
        "\
record Holder
    cb: (function(int) -> int)

function run(h: Holder, n: int) -> int
    return h.cb(n)
",
    );
}

/// …and when it is neither a field of *this* receiver nor a function, one message
/// says both — which is what the veto bought. The resolver stayed silent because
/// `handler` is a field *somewhere* in the file, which is exactly the type-free
/// approximation panel 015 D settled on.
#[test]
fn neither_a_field_nor_a_function_is_one_message() {
    assert_eq!(
        diagnostics(
            "\
record Point
    x: int

record Widget
    handler: (function(int) -> int)

function f(p: Point) -> int
    return p.handler(1)
"
        ),
        "test.hero:8:14: error[unknown_function]: `Point` has no field `handler`, and no function is named `handler` — `x.handler(…)` means `handler(x, …)`\n"
    );
}

#[test]
fn the_builtins_know_their_shapes() {
    assert_eq!(type_of_last("function f(s: str) -> int\n    return s.len()\n"), "int");
    assert_eq!(
        type_of_last("function f(s: str) -> [str]\n    return s.chars()\n"),
        "[str]"
    );
    assert_eq!(
        type_of_last("function f(xs: [int]) -> [int]\n    return xs.push(4)\n"),
        "[int]"
    );
    assert_eq!(
        type_of_last("function f(m: {str: int}) -> bool\n    return !m[\"a\"].is_err()\n"),
        "bool"
    );
    assert_eq!(
        type_of_last("function f(xs: [str]) -> str\n    return xs.join(\", \")\n"),
        "str"
    );
    assert_eq!(
        diagnostics("function f(n: int) -> int\n    return n.len()\n"),
        "test.hero:2:12: error[bad_operand]: `len` takes `str`, `[T]` or `{K: V}`, found `int`\n"
    );
}

/// `print` is a compiler form, not a function: any number of arguments, four
/// types, canonical rendering (panel 006).
#[test]
fn print_takes_any_number_of_the_four_printable_types() {
    assert_clean(
        "function main(n: int, x: f64, b: bool, s: str)\n    print(s, n, x, b, \"\\n\")\n",
    );
    assert_eq!(
        diagnostics(
            "\
record Point
    x: int

function main(p: Point)
    print(p)
"
        ),
        "test.hero:5:5: error[bad_operand]: `print` takes `int`, `f64`, `bool` or `str`, found `Point`\n"
    );
}

/// A built-in's argument is *checked*, not synthesised, from the second one on —
/// which is what lets a `.case` be written inside `push` (design.md's appendix
/// does it six times).
#[test]
fn a_builtin_argument_can_be_a_contextual_form() {
    assert_clean(
        "\
variant Token
    num
        v: int
    plus

function main()
    out: [Token] @ []
    out @ out.push(.plus)
    out @ out.push(.num(v: 1))
    print(out.len())
",
    );
}

#[test]
fn an_extern_is_called_like_any_other_function() {
    assert_clean(
        "\
extern \"math.h\"
    function sqrt(x: f64) -> f64

function hypotenuse(a: f64, b: f64) -> f64
    return sqrt(a * a + b * b)
",
    );
    assert_eq!(
        diagnostics(
            "\
extern \"math.h\"
    function sqrt(x: f64) -> f64

function f(n: int) -> f64
    return sqrt(n)
"
        ),
        "test.hero:5:17: error[type_mismatch]: expected `f64`, found `int`\n"
    );
}

/// **What a generic call instantiated at, recorded rather than recomputed.**
///
/// The checker binds these to type the call and used to drop them; the
/// monomorphisation pass needs exactly them, and recomputing them at IR level
/// would be a second answer to one question (panel 029 R2). One entry per
/// generic call, keyed by the call's span; a monomorphic program records none.
#[test]
fn a_generic_call_records_what_it_instantiated_at() {
    let (out, src) = super::checked(
        "\
function first<T>(xs: [T]) -> T
    return xs[0]

function main()
    print(first([1, 2]))
    print(first([\"a\"]))
    print(len(\"plain\"))
",
    );
    let parsed = crate::syntax::parse(&src);
    let mut seen: Vec<String> = out
        .instantiations
        .values()
        .map(|args| {
            args.iter()
                .map(|t| crate::types::render_ty(&out.types, &parsed.ast, &src, *t, &[]))
                .collect::<Vec<String>>()
                .join(", ")
        })
        .collect();
    seen.sort();
    assert_eq!(seen, vec!["int".to_string(), "str".to_string()]);
}
