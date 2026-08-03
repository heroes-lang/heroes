//! M1's five adversarial cases (ROADMAP). They live here until the golden
//! runner executes check/ cases through the CLI, then move there.
//!
//! # UNVERIFIED — pending debrief: the author has not yet said what each
//! case guards against (docs/debrief/QUEUE.md).

use super::dump;

#[test]
fn adversarial_tab_in_indentation() {
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
    // design.md §4.14 reserves single `&` for future bitwise use; the
    // boolean operator is `&&`.
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
