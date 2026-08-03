//! Crate-internal snapshot tests for token dumps (the plan's M1 test
//! strategy: goldens are for rendered diagnostics and program output; token
//! dumps are pinned here).
//!
//! Dump format, one token per line: `line:col kind [text]` — layout tokens
//! (terminator/indent/dedent/eof) carry no text. Diagnostics follow, each
//! prefixed `DIAG `.

use super::{kind_name, lex, TokenKind};
use crate::source::Source;

fn dump(text: &str) -> String {
    let src = Source::new("test.hero".to_string(), text.to_string());
    let out = lex(&src);
    let mut s = String::new();
    for t in &out.tokens {
        let (line, col) = src.line_col(t.span.start);
        s.push_str(&format!("{line}:{col} {}", kind_name(t.kind)));
        let layout = matches!(
            t.kind,
            TokenKind::Terminator | TokenKind::Indent | TokenKind::Dedent | TokenKind::Eof
        );
        if !layout && t.span.start != t.span.end {
            s.push(' ');
            s.push_str(src.slice(t.span));
        }
        s.push('\n');
    }
    for d in &out.diagnostics {
        s.push_str(&format!("DIAG {}\n", d.render_line(&src)));
    }
    s
}

#[test]
fn empty_file_is_just_eof() {
    assert_eq!(dump(""), "1:1 eof\n");
}

#[test]
fn first_hero_end_to_end() {
    // The real examples/first.hero, byte for byte.
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
7:1 ident main
7:6 eq =
7:8 kw_function function
7:16 colon :
7:18 lparen (
7:19 rparen )
7:20 terminator
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
fn terminator_only_after_enders() {
    // Line 1 ends with `+` (no terminator: the expression continues);
    // line 2 ends with `)` (terminator). design.md §4.15, Go's rule.
    assert_eq!(
        dump("b = (2 +\n3)\n"),
        "\
1:1 ident b
1:3 eq =
1:5 lparen (
1:6 int 2
1:8 plus +
2:1 int 3
2:2 rparen )
2:3 terminator
3:1 eof
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

#[test]
fn nesting_indents_and_dedents() {
    assert_eq!(
        dump("f = function: ()\n    if x\n        g()\n    h()\n"),
        "\
1:1 ident f
1:3 eq =
1:5 kw_function function
1:13 colon :
1:15 lparen (
1:16 rparen )
1:17 terminator
2:1 indent
2:5 kw_if if
2:8 ident x
2:9 terminator
3:1 indent
3:9 ident g
3:10 lparen (
3:11 rparen )
3:12 terminator
4:1 dedent
4:5 ident h
4:6 lparen (
4:7 rparen )
4:8 terminator
5:1 dedent
5:1 eof
"
    );
}

#[test]
fn comments_are_retained_and_layout_neutral() {
    // The doc comment gets no terminator; the trailing comment does not
    // block the terminator earned by `1`.
    assert_eq!(
        dump("# doc\nx = 1 # five\n"),
        "\
1:1 comment # doc
2:1 ident x
2:3 eq =
2:5 int 1
2:7 comment # five
2:13 terminator
3:1 eof
"
    );
}

#[test]
fn operators_multichar_and_floats() {
    assert_eq!(
        dump("a == 1.5 != <= >= && || ! -> => ??? ? . |\n"),
        "\
1:1 ident a
1:3 eq_eq ==
1:6 float 1.5
1:10 bang_eq !=
1:13 le <=
1:16 ge >=
1:19 and_and &&
1:22 or_or ||
1:25 bang !
1:27 arrow ->
1:30 fat_arrow =>
1:33 hole ???
1:37 question ?
1:39 dot .
1:41 pipe |
2:1 eof
"
    );
}

// --- strings and char literals (M1 step 3) ---------------------------------

#[test]
fn strings_and_chars() {
    assert_eq!(
        dump("greeting = \"hello, world\"\nc = 'a'\nsp = ' '\n"),
        "\
1:1 ident greeting
1:10 eq =
1:12 str \"hello, world\"
1:26 terminator
2:1 ident c
2:3 eq =
2:5 char 'a'
2:8 terminator
3:1 ident sp
3:4 eq =
3:6 char ' '
3:9 terminator
4:1 eof
"
    );
}

#[test]
fn backslash_is_an_ordinary_byte() {
    // No escape sequences exist (design.md is silent; gap on record in
    // OPEN-QUESTIONS): "a\nb" is FOUR characters, backslash included.
    assert_eq!(
        dump("s = \"a\\nb\"\n"),
        "\
1:1 ident s
1:3 eq =
1:5 str \"a\\nb\"
1:11 terminator
2:1 eof
"
    );
}

#[test]
fn unterminated_string_is_loud() {
    assert_eq!(
        dump("s = \"oops\nx = 1\n"),
        "\
1:1 ident s
1:3 eq =
1:5 error \"oops
2:1 ident x
2:3 eq =
2:5 int 1
2:6 terminator
3:1 eof
DIAG test.hero:1:5: error[unterminated_string]: this string never closes — strings are single-line, `\"` to `\"`
"
    );
}

#[test]
fn char_literal_is_one_ascii_character() {
    assert_eq!(
        dump("a = ''\nb = 'ab'\nc = 'é'\n"),
        "\
1:1 ident a
1:3 eq =
1:5 error ''
2:1 ident b
2:3 eq =
2:5 error 'ab'
3:1 ident c
3:3 eq =
3:5 error 'é'
4:1 eof
DIAG test.hero:1:5: error[char_literal]: a character literal holds exactly one ASCII character ('a', '0', ' ') — `''` does not
DIAG test.hero:2:5: error[char_literal]: a character literal holds exactly one ASCII character ('a', '0', ' ') — `'ab'` does not
DIAG test.hero:3:5: error[char_literal]: a character literal holds exactly one ASCII character ('a', '0', ' ') — `'é'` does not
"
    );
}

// --- reserved foreign words (M1 step 4, spec/reserved-words.md) ------------

#[test]
fn foreign_keywords_fail_with_the_fix_prewritten() {
    assert_eq!(
        dump("struct Point\nlet x = 5\n"),
        "\
1:1 error struct
1:8 ident Point
1:13 terminator
2:1 error let
2:5 ident x
2:7 eq =
2:9 int 5
2:10 terminator
3:1 eof
DIAG test.hero:1:1: error[reserved_word]: `struct` is not a word in this language — use `record`: `Point = record`
DIAG test.hero:2:1: error[reserved_word]: `let` is not a word in this language — bind with `=`: `x = 5`
"
    );
}

#[test]
fn null_family_and_exceptions_share_their_messages() {
    assert_eq!(
        dump("x = None\ntry\n"),
        "\
1:1 ident x
1:3 eq =
1:5 error None
2:1 error try
3:1 eof
DIAG test.hero:1:5: error[reserved_word]: there is no null in this language — absence is a fallible type: `int?`
DIAG test.hero:2:1: error[reserved_word]: there are no exceptions in this language — errors are values: `fail(code, msg)`, propagate with `?`
"
    );
}

#[test]
fn certain_fix_travels_with_the_diagnostic() {
    // design.md §4.17 discipline: `while` → `for` is a pure word swap, so
    // the Fix is Certain and machine-applicable.
    let src = Source::new("test.hero".to_string(), "while x > 0\n".to_string());
    let out = lex(&src);
    let d = &out.diagnostics[0];
    assert_eq!(d.code, "reserved_word");
    assert_eq!(d.fixes.len(), 1);
    assert_eq!(d.fixes[0].replacement, "for");
    assert!(matches!(
        d.fixes[0].certainty,
        crate::diagnostics::Certainty::Certain
    ));
    // `let` has guidance but no mechanical repair: no fix attached.
    let src2 = Source::new("test.hero".to_string(), "let x = 5\n".to_string());
    let out2 = lex(&src2);
    assert_eq!(out2.diagnostics[0].fixes.len(), 0);
}

// --- panel 007: completed ender list + brackets-only continuation ---------

#[test]
fn enders_break_continue_hole_question() {
    // panel 007: break/continue/???/postfix ? now terminate their line.
    assert_eq!(
        dump("for x in xs\n    break\ny = f(s)?\nz = ???\n"),
        "\
1:1 kw_for for
1:5 ident x
1:7 kw_in in
1:10 ident xs
1:12 terminator
2:1 indent
2:5 kw_break break
2:10 terminator
3:1 dedent
3:1 ident y
3:3 eq =
3:5 ident f
3:6 lparen (
3:7 ident s
3:8 rparen )
3:9 question ?
3:10 terminator
4:1 ident z
4:3 eq =
4:5 hole ???
4:8 terminator
5:1 eof
"
    );
}

#[test]
fn brackets_suspend_indentation() {
    // panel 007: inside [ ] the element lines produce no Indent/Dedent;
    // terminators still separate the elements (§4.9 multi-line literals).
    assert_eq!(
        dump("fixed = [\n    3\n    1\n]\n"),
        "\
1:1 ident fixed
1:7 eq =
1:9 lbracket [
2:5 int 3
2:6 terminator
3:5 int 1
3:6 terminator
4:1 rbracket ]
4:2 terminator
5:1 eof
"
    );
}

#[test]
fn unclosed_opener_is_reported_at_eof() {
    // panel 007 (engineer's condition): without this diagnostic, one
    // missing `)` silently swallows the rest of the file's layout.
    assert_eq!(
        dump("x = (2 +\n3\n"),
        "\
1:1 ident x
1:3 eq =
1:5 lparen (
1:6 int 2
1:8 plus +
2:1 int 3
2:2 terminator
3:1 eof
DIAG test.hero:1:5: error[unclosed_bracket]: `(` opened here is never closed
"
    );
}

#[test]
fn stray_closer_does_not_corrupt_layout() {
    // Saturating pop: the `)` is the parser's problem; the next line's
    // indentation still works.
    assert_eq!(
        dump("x = 1)\n    y\n"),
        "\
1:1 ident x
1:3 eq =
1:5 int 1
1:6 rparen )
1:7 terminator
2:1 indent
2:5 ident y
2:6 terminator
3:1 dedent
3:1 eof
"
    );
}

// ---------------------------------------------------------------------------
// M1's five adversarial cases (ROADMAP). They live here until the golden
// runner executes check/ cases through the CLI, then move there.
// # UNVERIFIED — pending debrief: the author has not yet said what each
// case guards against (docs/debrief/QUEUE.md).
// ---------------------------------------------------------------------------

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
