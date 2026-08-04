//! §4.1's adjacency rule: which comment is documentation.
//!
//! Go's rule, and it costs two integer comparisons — same column, line
//! immediately above. No `///` form exists, so these tests are the whole
//! definition of "this comment documents that declaration".

use super::dump;

#[test]
fn the_comment_directly_above_is_documentation() {
    assert_eq!(
        dump("# Squared distance, without the square root.\nMAX = constant: int\n    1\n"),
        "\
file test.hero
  constant MAX: int
    doc # Squared distance, without the square root.
    body lines 3-3
"
    );
}

#[test]
fn a_run_of_comments_stays_in_source_order() {
    assert_eq!(
        dump("# First line.\n# Second line.\nMAX = constant: int\n    1\n"),
        "\
file test.hero
  constant MAX: int
    doc # First line.
    doc # Second line.
    body lines 4-4
"
    );
}

/// A blank line between makes it an ordinary comment (§4.1). This is the
/// half of the rule that lets a file carry prose that documents nothing.
#[test]
fn a_blank_line_ends_the_documentation() {
    assert_eq!(
        dump("# Just a note about the file.\n\nMAX = constant: int\n    1\n"),
        "\
file test.hero
  constant MAX: int
    body lines 4-4
"
    );
}

/// `##` is a section heading: it documents the section, not whatever
/// declaration happens to follow it.
#[test]
fn a_section_heading_is_not_documentation() {
    assert_eq!(
        dump("## Limits\nMAX = constant: int\n    1\n"),
        "\
file test.hero
  constant MAX: int
    body lines 3-3
"
    );
}

/// The column comparison earns its place here: a comment trailing the line
/// above is adjacent by line and still not documentation.
#[test]
fn a_trailing_comment_on_the_line_above_is_not_documentation() {
    assert_eq!(
        dump("A = constant: int\n    1  # the answer\nB = constant: int\n    2\n"),
        "\
file test.hero
  constant A: int
    body lines 2-2
  constant B: int
    body lines 4-4
"
    );
}

/// Fields are declarations too, so the same rule applies one level in.
#[test]
fn fields_carry_their_own_documentation() {
    assert_eq!(
        dump("Point = record\n    # Distance from the left edge.\n    x: int\n    y: int\n"),
        "\
file test.hero
  record Point
    field x: int
      doc # Distance from the left edge.
    field y: int
"
    );
}
