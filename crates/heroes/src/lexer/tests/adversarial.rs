//! M1's five adversarial cases (ROADMAP) — written to *break* the lexer,
//! not to exercise it. Ratified by the author in the debrief of 2026-08-04;
//! what each one guards against is stated on the case itself.
//!
//! They all defend one bet: **in Heroes, whitespace carries meaning**, so
//! every "nearly right" margin must be an error rather than an
//! interpretation (design.md §4.15). A language where indentation is
//! structural and ambiguity is tolerated is a language where the same file
//! means different programs on different screens.

use super::dump;

#[test]
fn adversarial_tab_in_indentation() {
    // Guards: a tab in the margin LOOKS like an indent but its width is an
    // editor setting, so the same file would mean different programs on
    // different screens. Structure may never depend on display.
    assert_eq!(
        dump("\tx = 1\n"),
        "\
1:2 ident x
1:4 eq =
1:6 int 1
1:7 terminator
2:1 eof
DIAG test.hero:1:1: error[tab_in_indentation]: a tab in indentation is a compile error — indent with spaces, exactly 4 per level
"
    );
}

#[test]
fn adversarial_five_space_indent() {
    // Guards: "nearly right" indentation. Four spaces is the only legal
    // step — five is not slightly crooked, it is wrong. This is the whole
    // rigid-indentation bet in one case.
    assert_eq!(
        dump("f = function: ()\n     x\n"),
        "\
1:1 ident f
1:3 eq =
1:5 kw_function function
1:13 colon :
1:15 lparen (
1:16 rparen )
1:17 terminator
2:6 ident x
2:7 terminator
3:1 eof
DIAG test.hero:2:1: error[indentation_not_multiple_of_4]: indentation must be exactly 4 spaces per level; found 5
"
    );
}

#[test]
fn adversarial_indentation_jump() {
    // Guards: the recovery invariant. Opening two levels at once is an
    // error, but the lexer must still hand the parser a coherent structure
    // (two indents, later two dedents) instead of a truncated file — one
    // bad margin must not hide every error after it.
    assert_eq!(
        dump("f = function: ()\n        x\n"),
        "\
1:1 ident f
1:3 eq =
1:5 kw_function function
1:13 colon :
1:15 lparen (
1:16 rparen )
1:17 terminator
2:1 indent
2:1 indent
2:9 ident x
2:10 terminator
3:1 dedent
3:1 dedent
3:1 eof
DIAG test.hero:2:1: error[indentation_jump]: indentation jumps from level 0 to level 2; a block opens one level at a time
"
    );
}

#[test]
fn adversarial_tab_between_tokens() {
    // Guards: the tab rule being only half a rule. If tabs were rejected
    // in the margin but tolerated between tokens, "spaces only" would be a
    // claim the compiler does not actually keep.
    assert_eq!(
        dump("x =\t1\n"),
        "\
1:1 ident x
1:3 eq =
1:5 int 1
1:6 terminator
2:1 eof
DIAG test.hero:1:4: error[tab_in_line]: a tab is a compile error — use spaces
"
    );
}

#[test]
fn adversarial_single_ampersand_is_not_a_token() {
    // Guards: the reserved-operator boundary. design.md §4.14 reserves
    // `&` for future bitwise use, so it must never quietly lex as
    // something else — and this is also the proof that an unknown
    // character costs exactly one skipped character, not the rest of the
    // file.
    assert_eq!(
        dump("x = 1 & 2\n"),
        "\
1:1 ident x
1:3 eq =
1:5 int 1
1:7 error &
1:9 int 2
1:10 terminator
2:1 eof
DIAG test.hero:1:7: error[unexpected_character]: `&` is not part of the language's syntax (ASCII-only)
"
    );
}
