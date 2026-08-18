//! §4.1's adjacency rule, and blank lines as content (design.md §4.15).
//!
//! A comment directly above a declaration IS its documentation, and a blank
//! line between them demotes it to an ordinary remark — so the formatter may
//! neither open that gap nor close it. Every test here is a way of getting that
//! wrong, including the one that cost panel 014 a milestone.

use super::format;

/// The load-bearing blank line: adjacency is what makes a comment
/// documentation (§4.1), so the formatter may never open or close that gap.
#[test]
fn a_blank_line_between_comment_and_declaration_is_preserved() {
    let doc = "# The limit.\nconstant MAX: i64\n    64\n";
    assert_eq!(format(doc), doc);
    let remark = "# Just a remark.\n\nconstant MAX: i64\n    64\n";
    assert_eq!(format(remark), remark);
}

#[test]
fn comments_inside_a_body_are_kept_where_they_were() {
    let text = "\
function main()
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
        format("constant A: i64\n    1\nconstant B: i64\n    2\n"),
        "constant A: i64\n    1\n\nconstant B: i64\n    2\n"
    );
    assert_eq!(
        format("constant A: i64\n    1\n\n\n\nconstant B: i64\n    2\n"),
        "constant A: i64\n    1\n\nconstant B: i64\n    2\n"
    );
}

/// A blank line inside a body is content: it is the only grouping a body has,
/// so one survives and a run collapses.
#[test]
fn one_blank_line_inside_a_body_survives() {
    let text = "function main()\n    print(1)\n\n    print(2)\n";
    assert_eq!(format(text), text);
    assert_eq!(
        format("function main()\n    print(1)\n\n\n\n    print(2)\n"),
        text
    );
}

/// The doc comment a first prototype of panel 014 silently ate: the arm's span
/// reaches its terminator, so a `last_line` one line too far made
/// `trailing_comment` steal the *next* declaration's documentation and glue it
/// to the arm, where §4.1 adjacency then demoted it to a remark.
#[test]
fn an_arm_does_not_steal_the_next_declarations_doc_comment() {
    let text = "\
function f(k: i64) -> i64
    return match k
        _ => 2

# Doc for g.
function g() -> i64
    return 1
";
    assert_eq!(format(text), text);
}
