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

use super::{assert_canonical, dump, format};

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

/// **fixedbugs, panel 062's audit, 2026-08-15.** The guard was blind in the place
/// it guards, and this test is what makes that falsifiable.
///
/// Symptom: `assert_canonical` asserts `dump(text) == dump(fmt(text))` —
/// *"fmt changed the tree"* — and `printer/dump.rs` printed a `record`'s name with
/// no group head. So **`fmt` hoisting a record out of its group, folding it into a
/// neighbouring one, or turning `link` into `package` all produced identical dumps
/// and a green test.** Measured before the repair: `extern "raylib.h" package
/// "raylib"` + `record Color partial` dumped as `record Color partial`, the same
/// string a top-level record gives.
///
/// Cause: a `..` in the dump's `Record` arm, which is also how `header`/`library`
/// were lost at panel 060 and `partial` at panel 061 — three fields, one hole,
/// repaired an arm at a time. The **structural** repair is in the two modules'
/// docs: no `..` and no unread binding in either printer's `DeclKind` arms, so
/// rustc fails the build for the next field instead of a test going green about a
/// program that changed meaning.
///
/// This case is the part rustc cannot do: it pins that the dump **discriminates**
/// the three edits a formatter could make to a group's head.
#[test]
fn fixedbugs_the_dump_separates_every_group_head_a_record_can_carry() {
    let in_group = "extern \"raylib.h\" package \"raylib\"\n    record Color\n        r: u8\n";
    assert_eq!(
        dump(in_group),
        "\
file test.hero
  extern \"raylib.h\" package \"raylib\" record Color
    field r: u8
"
    );

    // Hoisted out of the group: a different program, and now a different dump.
    let hoisted = "record Color\n    r: u8\n";
    assert_ne!(dump(in_group), dump(hoisted), "a record's group must reach the dump");

    // Folded into a neighbouring group: same record, other header.
    let other = "extern \"other.h\" package \"raylib\"\n    record Color\n        r: u8\n";
    assert_ne!(dump(in_group), dump(other), "the header must reach the dump");

    // `link` is the program telling the linker a name; `package` asks the machine
    // (§4.19, panel 049). One is not the other, and the dump must say which.
    let linked = "extern \"raylib.h\" link \"raylib\"\n    record Color\n        r: u8\n";
    assert_ne!(dump(in_group), dump(linked), "link and package are two groups");

    // `partial` — repaired at panel 061, pinned here beside its two siblings.
    let partial = "extern \"raylib.h\" package \"raylib\"\n    record Color partial\n        r: u8\n";
    assert_ne!(dump(in_group), dump(partial), "partial must reach the dump");
}

/// A `record` member round-trips inside a group that also holds a function, which
/// is raylib's shape and the one the round-trip guard could not see until the case
/// above. `assert_canonical` is doing real work here for the first time.
#[test]
fn a_group_holding_a_record_round_trips() {
    assert_canonical(
        "extern \"raylib.h\" package \"raylib\"\n    record Color partial\n        r: u8\n    function ColorToInt(color: Color) -> i32\n",
    );
}
