//! §4.1's adjacency rule: which comment is documentation.
//!
//! Go's rule, and it costs two integer comparisons — same column, line
//! immediately above. No `///` form exists, so these tests are the whole
//! definition of "this comment documents that declaration".

use super::dump;

#[test]
fn the_comment_directly_above_is_documentation() {
    assert_eq!(
        dump("# Squared distance, without the square root.\nconstant MAX: i64\n    1\n"),
        "\
file test.hero
  constant MAX: i64
    doc # Squared distance, without the square root.
    expr 1
"
    );
}

#[test]
fn a_run_of_comments_stays_in_source_order() {
    assert_eq!(
        dump("# First line.\n# Second line.\nconstant MAX: i64\n    1\n"),
        "\
file test.hero
  constant MAX: i64
    doc # First line.
    doc # Second line.
    expr 1
"
    );
}

/// A blank line between makes it an ordinary comment (§4.1). This is the
/// half of the rule that lets a file carry prose that documents nothing.
#[test]
fn a_blank_line_ends_the_documentation() {
    assert_eq!(
        dump("# Just a note about the file.\n\nconstant MAX: i64\n    1\n"),
        "\
file test.hero
  constant MAX: i64
    expr 1
"
    );
}

/// `##` is a section heading: it documents the section, not whatever
/// declaration happens to follow it.
#[test]
fn a_section_heading_is_not_documentation() {
    assert_eq!(
        dump("## Limits\nconstant MAX: i64\n    1\n"),
        "\
file test.hero
  constant MAX: i64
    expr 1
"
    );
}

/// The column comparison earns its place here: a comment trailing the line
/// above is adjacent by line and still not documentation.
#[test]
fn a_trailing_comment_on_the_line_above_is_not_documentation() {
    assert_eq!(
        dump("constant A: i64\n    1  # the answer\nconstant B: i64\n    2\n"),
        "\
file test.hero
  constant A: i64
    expr 1
  constant B: i64
    expr 2
"
    );
}

/// Fields are declarations too, so the same rule applies one level in.
#[test]
fn fields_carry_their_own_documentation() {
    assert_eq!(
        dump("record Point\n    # Distance from the left edge.\n    x: i64\n    y: i64\n"),
        "\
file test.hero
  record Point
    field x: i64
      doc # Distance from the left edge.
    field y: i64
"
    );
}
