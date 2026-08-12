//! `extern` groups through the formatter (§4.19, panel 036).
//!
//! The parser flattens a group into one declaration per signature, so **the
//! formatter is the only place that knows groups exist** — and the rule it uses
//! is the narrow one: *a run of consecutive declarations sharing a header and a
//! library prints under one head line.*
//!
//! Two rules were available and only one round-trips. Re-grouping **all**
//! members of a header, wherever they sit, would move declarations past each
//! other — §4.15 says any textual difference is semantic, and the formatter does
//! not reorder (`fmt.rs` refuses to sort `use` lines for the same reason).
//! Printing one head line **per member** would turn a two-signature group into
//! two groups, and `fmt(fmt(x)) == fmt(x)` would still hold while the program
//! grew a line every time anybody ran it.

use super::{assert_canonical, format};

#[test]
fn a_group_prints_as_a_group() {
    assert_eq!(
        format("extern \"math.h\"\n    function sqrt(x: f64) -> f64\n    function pow(base: f64, exponent: f64) -> f64\n"),
        "\
extern \"math.h\"
    function sqrt(x: f64) -> f64
    function pow(base: f64, exponent: f64) -> f64
"
    );
}

#[test]
fn the_library_survives_the_round_trip() {
    assert_canonical(
        "extern \"sqlite3.h\" link \"sqlite3\"\n    function sqlite3_open(path: cstr, out: ptr) -> i64\n    function sqlite3_close(db: ptr) -> i64\n",
    );
}

/// Two headers, two head lines, and a blank line between them — the run ends
/// where the header changes. Without the run rule this file would print four
/// head lines for four signatures.
#[test]
fn a_new_header_opens_a_new_group() {
    assert_eq!(
        format("extern \"math.h\"\n    function sqrt(x: f64) -> f64\n\nextern \"stdio.h\"\n    function puts(s: cstr) -> i64\n"),
        "\
extern \"math.h\"
    function sqrt(x: f64) -> f64

extern \"stdio.h\"
    function puts(s: cstr) -> i64
"
    );
}

/// **The head line is where a group's comments attach.** A member's own span
/// starts at its `function` keyword, one line below the head, so every rule that
/// keys on "is there a blank line above this?" would see a gap that only exists
/// because the head is in between — and `fmt` would insert a blank line the
/// author never wrote, once per run. §4.1 makes that a meaning change: a comment
/// separated by a blank line is a remark, not documentation.
#[test]
fn a_comment_above_a_group_stays_attached_to_it() {
    assert_eq!(
        format("# What the C library does.\nextern \"math.h\"\n    function sqrt(x: f64) -> f64\n"),
        "\
# What the C library does.
extern \"math.h\"
    function sqrt(x: f64) -> f64
"
    );
}

/// **The other side of that rule, and it was broken.** A comment above a group's
/// *first member* documents the member, not the group — and it was being moved
/// down onto the **second** member, because the caller keys a first member's
/// comment rules on the head's line (it has to, per the test above) and nothing
/// then flushed the comments between the head and the member. Whatever
/// declaration came next printed them, at the wrong place.
///
/// Pre-existing: this case is functions only. Found by panel 038's
/// compiler-engineer, and fixed before `examples/curl/main.hero` moved a four-line
/// comment into exactly this position.
#[test]
fn a_comment_above_a_groups_first_member_stays_on_it() {
    assert_canonical(
        "extern \"math.h\"\n    # The one the program actually calls.\n    function sqrt(x: f64) -> f64\n    function pow(base: f64, exponent: f64) -> f64\n",
    );
}

/// A `constant` member round-trips, and the head line is printed once for a group
/// that mixes the two kinds (§4.19, panel 038).
#[test]
fn a_group_mixing_constants_and_functions_round_trips() {
    assert_canonical(
        "extern \"sqlite3.h\" link \"sqlite3\"\n    # From the header, so it cannot be copied wrong.\n    constant SQLITE_OK: i64\n    constant SQLITE_ROW: i64\n    function sqlite3_close(db: ptr) -> i64\n",
    );
}

/// A group beside ordinary declarations: the run must not swallow what follows.
#[test]
fn a_function_after_a_group_is_outside_it() {
    assert_canonical(
        "extern \"math.h\"\n    function sqrt(x: f64) -> f64\n\nfunction hypotenuse(a: f64, b: f64) -> f64\n    return sqrt(a * a + b * b)\n",
    );
}
