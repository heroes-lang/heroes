//! `use` lines through the formatter (panel 031).
//!
//! A `use` lives outside `Ast::decls`, so the formatter is the first place that
//! has to put the two streams back into one order. Three rules, and the second
//! is the one a reader will notice: a run of `use` lines is a **block**, like a
//! record's fields, not a sequence of declarations each demanding a blank line.

use super::{assert_canonical, format};

#[test]
fn a_use_line_survives_formatting_unchanged() {
    let text = "use geom\n\nfunction main()\n    print(0)\n";
    assert_eq!(format(text), text);
    assert_canonical(text);
}

/// No blank line between two `use`s, one before the declaration that follows.
#[test]
fn a_run_of_uses_is_one_block() {
    let text = "use lex\nuse parse\n\nfunction main()\n    print(0)\n";
    assert_eq!(format(text), text);
    assert_canonical(text);

    // A blank line the author put *between* two `use` lines closes: the run is
    // a block, and a block has no internal separators anywhere in this language.
    assert_eq!(format("use lex\n\nuse parse\n\nfunction main()\n    print(0)\n"), text);
}

/// **Source order, not sorted.** gofmt sorts imports; this formatter does not,
/// because it interleaves comments by line, and moving a line means deciding
/// what happens to the comment above it. Recorded as a choice, with a test, so
/// that changing it later is a decision rather than a drift.
#[test]
fn uses_keep_the_order_the_author_wrote() {
    let text = "use parse\nuse lex\n\nfunction main()\n    print(0)\n";
    assert_eq!(format(text), text);
    assert_canonical(text);
}

/// A comment above a `use` is kept where it was, and so is one at the end of
/// the line — the two positions the formatter has to know about.
#[test]
fn comments_around_a_use_are_kept() {
    let text = "\
# The lexer, and nothing else in it.
use lex  # tokens only
use parse

function main()
    print(0)
";
    assert_eq!(format(text), text);
    assert_canonical(text);
}

/// A file's header block sits above the first `use` exactly as it sits above a
/// first declaration: the blank line after it is what makes it a remark about
/// the file rather than documentation for the line below (§4.1).
#[test]
fn a_file_header_above_the_first_use_keeps_its_gap() {
    let text = "\
# A calculator, in three modules.

use lex
use parse

function main()
    print(0)
";
    assert_eq!(format(text), text);
    assert_canonical(text);
}

/// A `use` written *after* a declaration is not hoisted. Declaration order
/// carries no meaning (§4.2), so hoisting would be legal — and it would move
/// the comment above it, which is the thing this formatter never does.
#[test]
fn a_late_use_stays_where_it_was() {
    let text = "\
function main()
    print(0)

use geom
";
    assert_eq!(format(text), text);
    assert_canonical(text);
}
