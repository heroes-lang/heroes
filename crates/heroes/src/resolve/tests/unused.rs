//! §4.4's unused rule and §4.16's file-wide exemption.
//!
//! Every case here is a panel-015 decision, and the interesting ones are the
//! *acceptances*: the code the rule must not reject.

use crate::diagnostics::Certainty;

use super::{assert_clean, diagnostics, resolved};

#[test]
fn a_binding_nobody_reads_is_an_error() {
    assert_eq!(
        diagnostics(
            "\
function main()
    x = 1
    print(2)
"
        ),
        "test.hero:2:5: error[unused_binding]: `x` is bound and never read — remove the binding, or read it\n"
    );
}

/// Question B: only a read is a use. A cell that is written and never read is a
/// value nobody consumes — Go's rule, tightened in Go 1.18 (golang/go#49214).
#[test]
fn a_cell_that_is_written_and_never_read_is_unused() {
    assert_eq!(
        diagnostics(
            "\
function main()
    total: i64 @ 0
    total @ 1
    print(2)
"
        ),
        "test.hero:2:5: error[unused_binding]: `total` is written and never read — a cell nothing reads is a value nobody uses\n"
    );
}

/// The declaration's own initialiser is not a write. Counting it would make the
/// rule blind to every cell in the language — the sub-question the
/// llm-ergonomist found undecided in both variants.
#[test]
fn the_initialiser_is_not_a_write() {
    assert_eq!(
        diagnostics(
            "\
function main()
    total: i64 @ 0
    print(2)
"
        ),
        "test.hero:2:5: error[unused_binding]: `total` is declared and never read — remove it, or read it\n"
    );
}

/// Writing through a field or an index writes the *root*, and with no aliasing
/// anywhere (§4.10) a write nobody reads back is unobservable. Go left this hole
/// open (golang/go#20802); Heroes closes it because it has no aliases.
#[test]
fn a_field_write_is_a_write_and_not_a_read() {
    assert_eq!(
        diagnostics(
            "\
record Counter
    n: i64

function main()
    c: Counter @ Counter(n: 0)
    c.n @ 1
    print(2)
"
        ),
        "test.hero:5:5: error[unused_binding]: `c` is written and never read — a cell nothing reads is a value nobody uses\n"
    );
}

/// Question A: parameters are covered.
#[test]
fn a_parameter_nobody_reads_is_an_error() {
    assert_eq!(
        diagnostics(
            "\
function f(a: i64, b: i64) -> i64
    return a
"
        ),
        "test.hero:1:20: error[unused_binding]: the parameter `b` is never read — remove it from the signature, or write `???` in the body while the function is unfinished\n"
    );
}

/// The llm-ergonomist's program 5, which the first draft of question B
/// rejected: the whole observable effect of this function is the write, because
/// §4.8's copy-out always happens.
#[test]
fn a_write_through_a_mutable_parameter_is_a_use() {
    assert_clean(
        "\
function reset(@counts: {str: i64})
    counts @ {}
",
    );
}

/// An `extern` has no body, so its parameters can never be read (§4.19). The
/// ffi-pragmatist's condition: a 40-extern binding file must compile.
#[test]
fn an_extern_parameter_is_never_unused() {
    assert_clean("extern \"math.h\"\n    function sqrt(x: f64) -> f64\n");
}

/// `_` is the escape valve, and every language that enforces an unused rule
/// ships one (panel 015, historian: six for six). It binds nothing, so it can
/// repeat in one parameter list.
#[test]
fn the_wildcard_binds_nothing_anywhere() {
    assert_clean(
        "\
function render(page: str, _: i64, _: bool) -> str
    return page

function main()
    for _ in range(0, 3)
        print(\"tick\")
",
    );
}

/// Renaming to `_` preserves meaning, so it is the one repair the compiler may
/// apply by itself (§4.17).
#[test]
fn a_loop_variable_and_a_payload_offer_a_certain_rename() {
    let (out, _) = resolved(
        "\
function main()
    for i in range(0, 3)
        print(\"tick\")
",
    );
    assert_eq!(out.diagnostics.len(), 1);
    assert_eq!(out.diagnostics[0].code, "unused_binding");
    assert_eq!(out.diagnostics[0].fixes[0].replacement, "_");
    assert_eq!(out.diagnostics[0].fixes[0].certainty, Certainty::Certain);

    let (payload, _) = resolved(
        "\
variant Token
    num
        v: i64
    plus

function describe(t: Token) -> i64
    return match t
        .num n => 1
        .plus => 2
",
    );
    assert_eq!(payload.diagnostics.len(), 1);
    assert_eq!(payload.diagnostics[0].fixes[0].replacement, "_");
}

/// §4.16, normative and file-wide: the hole suspends the rule for the whole
/// file, including parameters, and lifts when the last hole is filled. The
/// ffi-pragmatist's condition — the M-generics-library witness's `simplify` depends on it.
#[test]
fn one_hole_anywhere_suspends_the_rule_for_the_whole_file() {
    assert_clean(
        "\
function f(a: i64, b: i64) -> i64
    unused = 1
    return a

function simplify(e: i64) -> i64
    ???
",
    );
}

/// The hole suspends the *unused* rule and nothing else: an unknown name and a
/// shadow stay errors, or a file with one hole would stop being checked.
#[test]
fn a_hole_suspends_nothing_but_the_unused_rule() {
    assert_eq!(
        diagnostics(
            "\
function f(a: i64) -> i64
    x = 1
    print(nope)
    return a

function g() -> i64
    ???
"
        ),
        "test.hero:3:11: error[unknown_name]: nothing named `nope` is in scope\n"
    );
}

/// A declaration nobody calls is not an error: there is no export concept, and
/// `test` blocks reference functions the program itself never calls.
#[test]
fn an_uncalled_declaration_is_not_unused() {
    assert_clean(
        "\
function helper() -> i64
    return 1

function main()
    print(2)
",
    );
}
