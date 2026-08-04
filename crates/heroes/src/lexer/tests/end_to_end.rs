//! Whole-file snapshots: the degenerate case and the repo's first program.

use super::dump;

#[test]
fn empty_file_is_just_eof() {
    assert_eq!(dump(""), "1:1 eof\n");
}

#[test]
fn first_hero_end_to_end() {
    // The real examples/first.hero, byte for byte — if its header comments
    // change, this snapshot churns on purpose (it pins the whole file).
    let text = std::fs::read_to_string(concat!(
        env!("CARGO_MANIFEST_DIR"),
        "/../../examples/first.hero"
    ))
    .expect("examples/first.hero must exist");
    assert_eq!(
        dump(&text),
        "\
1:1 comment ## First
2:1 comment #
3:1 comment # The smallest program worth compiling — the M5a acceptance input.
4:1 comment # Its hand-written C target is tools/spike/01-first.c; in M5a the emitter's
5:1 comment # output is compared against that file by eye.
7:1 kw_function function
7:10 ident main
7:14 lparen (
7:15 rparen )
7:16 terminator
8:1 indent
8:5 ident print
8:10 lparen (
8:11 lparen (
8:12 int 2
8:14 plus +
8:16 int 3
8:17 rparen )
8:19 star *
8:21 int 4
8:22 rparen )
8:23 terminator
9:1 dedent
9:1 eof
"
    );
}

#[test]
fn missing_final_newline_still_terminates() {
    assert_eq!(
        dump("x = 1"),
        "\
1:1 ident x
1:3 eq =
1:5 int 1
1:6 terminator
1:6 eof
"
    );
}
