//! The `extern` group: a head line naming the header and the library, and
//! indented signatures under it (design.md §4.19, panel 036).
//!
//! Two properties are worth more than the rest of this file put together.
//!
//! **The header is mandatory**, because §4.19's mechanism is the `#include` and
//! nothing else: a signature with no header behind it would be emitted as a
//! prototype that is self-consistent by construction and verified by nothing.
//! Panel 030 measured the cost — `void *fopen(const char *, const char *)` links
//! by accident on arm64, silently — so the headerless spelling is refused with a
//! message that repairs *that* program rather than describing the grammar.
//!
//! **The group is flattened here.** Every test below asserts on N declarations,
//! never on a group node, because a declaration's index is function identity
//! across `Ref::Top`, `ir::Function::decl` and the mangler's name table. Nothing
//! after the parser knows the word "group" (`printer/fmt.rs` rebuilds one from
//! the runs, which is a rendering decision, not a tree).

use super::dump;

#[test]
fn a_group_gives_every_signature_its_header() {
    assert_eq!(
        dump("extern \"math.h\"\n    function sqrt(x: f64) -> f64\n    function pow(base: f64, exponent: f64) -> f64\n"),
        "\
file test.hero
  extern \"math.h\" function sqrt(x: f64) -> f64
  extern \"math.h\" function pow(base: f64, exponent: f64) -> f64
"
    );
}

#[test]
fn the_library_is_carried_beside_the_header() {
    assert_eq!(
        dump("extern \"sqlite3.h\" link \"sqlite3\"\n    function sqlite3_close(db: ptr) -> int\n"),
        "\
file test.hero
  extern \"sqlite3.h\" link \"sqlite3\" function sqlite3_close(db: ptr) -> int
"
    );
}

/// Two groups in one file, which is the ordinary case the moment a program uses
/// two libraries — and the case a per-declaration form makes drift-prone,
/// because the tail nobody reads is where the wrong library goes.
#[test]
fn two_groups_do_not_borrow_each_others_headers() {
    assert_eq!(
        dump("extern \"math.h\"\n    function sqrt(x: f64) -> f64\n\nextern \"stdio.h\"\n    function puts(s: cstr) -> int\n"),
        "\
file test.hero
  extern \"math.h\" function sqrt(x: f64) -> f64
  extern \"stdio.h\" function puts(s: cstr) -> int
"
    );
}

/// The old headerless spelling — and the one a model writes from memory of every
/// other language. The message names the repair, not the rule.
#[test]
fn an_extern_without_a_header_is_refused() {
    assert_eq!(
        dump("extern function sqrt(x: f64) -> f64\n"),
        "\
file test.hero
DIAG test.hero:1:8: error[expected_extern_header]: an `extern` names the header its signatures come from — `extern \"math.h\"`, then the signatures indented under it. Without the header there is no `#include`, and nothing checks the declaration (§4.19)
"
    );
}

#[test]
fn a_group_with_no_signatures_under_it_is_refused() {
    assert_eq!(
        dump("extern \"math.h\"\n\nfunction main()\n    print(1)\n"),
        "\
file test.hero
  function main() -> ()
    expr print(1)
DIAG test.hero:3:1: error[expected_extern_block]: an `extern` group's signatures are indented under it — `extern \"math.h\"` then `    function sqrt(x: f64) -> f64` (§4.19)
"
    );
}

#[test]
fn link_without_a_library_name_is_refused() {
    assert_eq!(
        dump("extern \"sqlite3.h\" link\n    function sqlite3_close(db: ptr) -> int\n"),
        "\
file test.hero
  extern \"sqlite3.h\" function sqlite3_close(db: ptr) -> int
DIAG test.hero:1:24: error[expected_link_name]: expected the library's name in quotes after `link`, found end of line — `link \"sqlite3\"`, which is `-lsqlite3` to the linker
"
    );
}

/// A member that is not a signature costs **one line**, not the rest of the
/// group: recovery inside a block is `skip_line`, the way `field_block` and
/// `case_block` already recover. The signature after the mistake is still read,
/// and that is what this asserts.
#[test]
fn a_stray_line_in_a_group_costs_one_line() {
    assert_eq!(
        dump("extern \"math.h\"\n    x = 1\n    function sqrt(x: f64) -> f64\n"),
        "\
file test.hero
  extern \"math.h\" function sqrt(x: f64) -> f64
DIAG test.hero:2:5: error[expected_extern_signature]: expected a `function` signature, found a name (`x`) — an `extern` group holds signatures and nothing else, one per line
"
    );
}
