//! What a formatter has to prove.
//!
//! Three properties, in order of importance:
//!
//! 1. **It preserves meaning.** The tree of the formatted text is the tree of
//!    the original — checked by comparing the two dumps, which is the one
//!    rendering that cannot silently agree with itself (it prints every
//!    parenthesis, and `fmt` prints the fewest).
//! 2. **It is idempotent.** `fmt(fmt(x)) == fmt(x)`, or "canonical" means
//!    nothing and §4.15's "any textual difference is semantic" is false.
//! 3. **It loses nothing.** Comments are not in the tree; a formatter that
//!    drops them is a formatter nobody runs.

use crate::source::Source;
use crate::syntax::parse;

use super::{dump_ast, format_file};

fn format(text: &str) -> String {
    let src = Source::new("test.hero".to_string(), text.to_string());
    let out = parse(&src);
    let rendered: Vec<String> = out.diagnostics.iter().map(|d| d.render_line(&src)).collect();
    assert_eq!(rendered, Vec::<String>::new(), "the input must parse clean");
    format_file(&out.ast, &out.comments, &src)
}

fn dump(text: &str) -> String {
    let src = Source::new("test.hero".to_string(), text.to_string());
    let out = parse(&src);
    dump_ast(&out.ast, &src)
}

/// Formatting twice changes nothing, and formatting does not change the tree.
fn assert_canonical(text: &str) {
    let once = format(text);
    let twice = format(&once);
    assert_eq!(once, twice, "fmt is not idempotent");
    assert_eq!(dump(text), dump(&once), "fmt changed the tree");
}

/// The acceptance program: 317 lines, every construct in the language, read
/// out of design.md's appendix (its single source — see
/// `syntax::tests::acceptance`).
#[test]
fn the_acceptance_program_is_canonical_and_unchanged_by_formatting() {
    let design = std::fs::read_to_string(concat!(
        env!("CARGO_MANIFEST_DIR"),
        "/../../design.md"
    ))
    .expect("design.md must exist");
    let appendix = design
        .find("## Appendix — A complete example program")
        .expect("the appendix must exist");
    let tail = &design[appendix..];
    let open = tail.find("```\n").expect("the appendix has a code fence") + 4;
    let close = tail[open..].find("\n```").expect("the fence closes") + open;
    assert_canonical(&tail[open..=close]);
}

/// `examples/first.hero` is already written in canonical form, and stays that
/// way: this test fails the day the formatter's policies change under it.
#[test]
fn the_first_program_is_already_canonical() {
    let text = std::fs::read_to_string(concat!(
        env!("CARGO_MANIFEST_DIR"),
        "/../../examples/first.hero"
    ))
    .expect("examples/first.hero must exist");
    assert_eq!(format(&text), text);
}

/// Minimal parentheses: the table already says what `2 + 3 * 4` means, so the
/// canonical form does not repeat it — and says what it cannot leave out.
#[test]
fn parentheses_survive_exactly_where_they_carry_meaning() {
    assert_eq!(
        format("main = function: ()\n    x = 2 + (3 * 4)\n"),
        "main = function: ()\n    x = 2 + 3 * 4\n"
    );
    assert_eq!(
        format("main = function: ()\n    x = (2 + 3) * 4\n"),
        "main = function: ()\n    x = (2 + 3) * 4\n"
    );
    // Left-associative, so the right operand of an equal-power operator keeps
    // its parentheses: `a - (b - c)` is not `a - b - c`.
    assert_eq!(
        format("main = function: ()\n    x = a - (b - c)\n"),
        "main = function: ()\n    x = a - (b - c)\n"
    );
    assert_eq!(
        format("main = function: ()\n    x = (a - b) - c\n"),
        "main = function: ()\n    x = a - b - c\n"
    );
}

/// A function that returns nothing writes no arrow — `main = function: ()` is
/// the form §4.2 shows, so `-> ()` has one spelling and it is the empty one.
#[test]
fn the_unit_result_is_never_written() {
    assert_eq!(
        format("main = function: () -> ()\n    print(1)\n"),
        "main = function: ()\n    print(1)\n"
    );
}

/// The load-bearing blank line: adjacency is what makes a comment
/// documentation (§4.1), so the formatter may never open or close that gap.
#[test]
fn a_blank_line_between_comment_and_declaration_is_preserved() {
    let doc = "# The limit.\nMAX = constant: int\n    64\n";
    assert_eq!(format(doc), doc);
    let remark = "# Just a remark.\n\nMAX = constant: int\n    64\n";
    assert_eq!(format(remark), remark);
}

#[test]
fn comments_inside_a_body_are_kept_where_they_were() {
    let text = "\
main = function: ()
    # Why this order matters.
    print(1)
    print(2)  # the second one
";
    assert_eq!(format(text), text);
}

/// One blank line always separates top-level declarations, and a run of them
/// collapses to one.
#[test]
fn declarations_are_separated_by_exactly_one_blank_line() {
    assert_eq!(
        format("A = constant: int\n    1\nB = constant: int\n    2\n"),
        "A = constant: int\n    1\n\nB = constant: int\n    2\n"
    );
    assert_eq!(
        format("A = constant: int\n    1\n\n\n\nB = constant: int\n    2\n"),
        "A = constant: int\n    1\n\nB = constant: int\n    2\n"
    );
}

/// A blank line inside a body is content: it is the only grouping a body has,
/// so one survives and a run collapses.
#[test]
fn one_blank_line_inside_a_body_survives() {
    let text = "main = function: ()\n    print(1)\n\n    print(2)\n";
    assert_eq!(format(text), text);
    assert_eq!(
        format("main = function: ()\n    print(1)\n\n\n\n    print(2)\n"),
        text
    );
}

/// §4.9: a list too long for one line breaks by **newline**, not by comma —
/// and only inside its brackets, because panel 007 allows no other
/// continuation.
#[test]
fn a_list_too_long_for_one_line_breaks_by_newline() {
    let long = "main = function: ()\n    xs = [\"aaaaaaaaaaaaaaaa\", \"bbbbbbbbbbbbbbbb\", \"cccccccccccccccc\", \"dddddddddddddddd\"]\n";
    assert_eq!(
        format(long),
        "\
main = function: ()
    xs = [
        \"aaaaaaaaaaaaaaaa\"
        \"bbbbbbbbbbbbbbbb\"
        \"cccccccccccccccc\"
        \"dddddddddddddddd\"
    ]
"
    );
    assert_canonical(long);
}

/// An argument list keeps its commas when it breaks: §4.9's newline rule is
/// about literals, and the parser requires `,` in a call.
#[test]
fn an_argument_list_too_long_for_one_line_breaks_by_comma() {
    let long = "main = function: ()\n    copy(from: \"/tmp/aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\", to: \"/tmp/bbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\")\n";
    assert_eq!(
        format(long),
        "\
main = function: ()
    copy(
        from: \"/tmp/aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\",
        to: \"/tmp/bbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\"
    )
"
    );
    assert_canonical(long);
}

/// `if` as a value, `match` with its arms, a record and a variant: the shapes
/// whose canonical layout is not obvious.
#[test]
fn control_forms_and_type_declarations_round_trip() {
    let text = "\
Token = variant
    num
        v: int
    plus

state = function: (t: Token) -> str
    label = if t == .plus
        \"+\"
    else
        \"?\"
    return match t
        .num n => n.v.str()
        .plus => label
";
    assert_eq!(format(text), text);
    assert_canonical(text);
}
