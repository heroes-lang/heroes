//! M-token-stream's five adversarial cases (ROADMAP) — written to *break* the lexer,
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
    //
    // The margin is still an error; what the recovery does is round it to the
    // nearest level, so the parser is handed the body the author obviously
    // meant instead of a function with no body (2026-08-04: that guess used to
    // cost three further diagnostics about a mistake already reported).
    assert_eq!(
        dump("function f()\n     x\n"),
        "\
1:1 kw_function function
1:10 ident f
1:11 lparen (
1:12 rparen )
1:13 terminator
2:1 indent
2:6 ident x
2:7 terminator
3:1 dedent
3:1 eof
DIAG test.hero:2:1: error[indentation_not_multiple_of_4]: indentation must be exactly 4 spaces per level; found 5
"
    );
}

#[test]
fn adversarial_indentation_jump() {
    // Guards: the recovery invariant. Opening two levels at once is an
    // error, and one bad margin must not hide every error after it.
    //
    // The recovery **clamps to one level** (2026-08-04, author instruction):
    // honouring the two levels the writer asked for produced a block that
    // nothing in the language opens, and the parser then said so — a second
    // diagnostic about the same mistake. Nothing opens two levels, so nothing
    // is invented by refusing to.
    assert_eq!(
        dump("function f()\n        x\n"),
        "\
1:1 kw_function function
1:10 ident f
1:11 lparen (
1:12 rparen )
1:13 terminator
2:1 indent
2:9 ident x
2:10 terminator
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
    //
    // The message changed at panel 036: the spec's sentence naming all six
    // reserved spellings was deleted, and this diagnostic is what replaced it,
    // so it now names the set rather than the character. `unexpected_character`
    // survives for a character that is genuinely not in the syntax.
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
DIAG test.hero:1:7: error[reserved_operator]: `&` is reserved for a future bitwise set and has no meaning yet — `& | ^ << >> ~` are all held, and none of them is an operator in this language
"
    );
}

/// `<<` reaches the lexer's two-character arm **only because no prefix operator
/// starts with `<` or `>`** — the premise `scan.rs` writes down, and this is the
/// test that fires when it dies (CLAUDE.md §11). If a later milestone gives `<`
/// a prefix meaning, `1 << 2` becomes two comparisons over a prefix expression
/// and the shift arm silently eats a legal program: this test fails first, and
/// its name says what depends on it.
#[test]
fn a_shift_is_never_two_comparisons() {
    // The prefix set, in full: `-` and `!` (syntax/expr.rs::unary). Neither is
    // `<` or `>`, so `<` immediately followed by `<` cannot be a comparison
    // against a prefixed operand — there is nothing for the second `<` to open.
    assert_eq!(
        dump("x = 1 << 2\n"),
        "\
1:1 ident x
1:3 eq =
1:5 int 1
1:7 error <<
1:10 int 2
1:11 terminator
2:1 eof
DIAG test.hero:1:7: error[reserved_operator]: `<<` is reserved for a future bitwise set and has no meaning yet — `& | ^ << >> ~` are all held, and none of them is an operator in this language
"
    );
    // The control: two comparisons that ARE legal stay legal, because they are
    // not adjacent. `a < b` and `a > b` never meet, so the arm is unreachable
    // from a well-formed program.
    assert_eq!(
        dump("x = a < b\n"),
        "\
1:1 ident x
1:3 eq =
1:5 ident a
1:7 lt <
1:9 ident b
1:10 terminator
2:1 eof
"
    );
}

/// **The premise `is_line_ender` rests on, and the test that fires when it dies**
/// (CLAUDE.md §11). The list is "every token that can end a statement", and a
/// *value* keyword is the class that keeps being forgotten: `nullptr` was added
/// to the language at M-ffi-ladder and missed here, so `p: ptr @ nullptr` planted
/// no terminator and the next line was swallowed as a continuation — panel 007's
/// trap, reopened by a new literal rather than by a new rule.
///
/// One line per value-producing spelling. A new one that is not added here will
/// pass this test; a new one that is added here and NOT added to `is_line_ender`
/// will fail it, which is the direction that matters — the list is what gets
/// forgotten, not the test.
#[test]
fn every_value_keyword_ends_a_line() {
    for literal in ["true", "false", "nullptr", "1", "1.5", "\"s\"", "'c'", "x", "???"] {
        let dumped = dump(&format!("a = {literal}\nb = 2\n"));
        assert!(
            dumped.contains("terminator"),
            "`{literal}` planted no terminator, so the next line continues this one:\n{dumped}"
        );
        // Two bindings, not one: the terminator has to fall BETWEEN them.
        assert_eq!(
            dumped.matches("terminator").count(),
            2,
            "`{literal}` did not end its own line:\n{dumped}"
        );
    }
}
