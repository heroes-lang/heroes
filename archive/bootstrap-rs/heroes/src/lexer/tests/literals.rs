//! Words that denote values: numbers, operators, strings, char literals,
//! and the escape rules panel 008 settled.
//!
//! Note the division of labour: the dump format prints RAW SOURCE SLICES,
//! so it can show that a literal lexed but never what it *means*. Escape
//! semantics are therefore tested against `unescape`, on byte values.

use super::dump;
use crate::lexer::{lex, unescape, TokenKind};
use crate::source::Source;

/// Decode the first `Str`/`Char` token of a snippet.
fn value_of(text: &str) -> String {
    let src = Source::new("test.hero".to_string(), text.to_string());
    let out = lex(&src);
    let t = out
        .tokens
        .iter()
        .find(|t| matches!(t.kind, TokenKind::Str | TokenKind::Char))
        .expect("a string or char token");
    unescape(&src, t.span)
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

// --- panel 008 (as amended by 066): the six escapes, split by context -------------------------

#[test]
fn escapes_decode_to_their_bytes() {
    // The whole point, and invisible to the token dump: these are VALUES.
    assert_eq!(value_of("s = \"a\\nb\"\n"), "a\nb");
    assert_eq!(value_of("s = \"a\\tb\"\n"), "a\tb");
    assert_eq!(value_of("s = \"C:\\\\temp\"\n"), "C:\\temp");
    assert_eq!(value_of("s = \"he said \\\"hi\\\"\"\n"), "he said \"hi\"");
    // A `'` needs no escape inside a string; a `"` needs none inside a char.
    assert_eq!(value_of("s = \"it's\"\n"), "it's");
    assert_eq!(value_of("c = '\"'\n"), "\"");
    // Char literals decode to one character each.
    assert_eq!(value_of("c = '\\n'\n"), "\n");
    assert_eq!(value_of("c = '\\t'\n"), "\t");
    assert_eq!(value_of("c = '\\\\'\n"), "\\");
    assert_eq!(value_of("c = '\\''\n"), "'");
    // UTF-8 in strings survives untouched (spec: strings may hold any UTF-8).
    assert_eq!(value_of("s = \"caffè ☕\"\n"), "caffè ☕");
}

#[test]
fn escaped_literals_still_lex_as_one_token() {
    assert_eq!(
        dump("s = \"a\\nb\"\nc = '\\t'\n"),
        "\
1:1 ident s
1:3 eq =
1:5 str \"a\\nb\"
1:11 terminator
2:1 ident c
2:3 eq =
2:5 char '\\t'
2:9 terminator
3:1 eof
"
    );
}

#[test]
fn unknown_escape_is_loud_but_does_not_poison_the_token() {
    // panel 008: the token stays `str`. Poisoning it to `error` would
    // delete the line's terminator (error is not an ender, panel 007) and
    // glue the next line on — one mistake becoming two.
    assert_eq!(
        dump("re = \"\\d+\"\nx = 1\n"),
        "\
1:1 ident re
1:4 eq =
1:6 str \"\\d+\"
1:11 terminator
2:1 ident x
2:3 eq =
2:5 int 1
2:6 terminator
3:1 eof
DIAG test.hero:1:7: error[unknown_escape]: `\\d` is not an escape sequence — the escapes are \\n \\t \\r \\\\ \\\"; write `\\\\` for a literal backslash
"
    );
}

#[test]
fn the_residual_windows_path_trap_is_on_the_record() {
    // Honest limit of panel 008, found while testing: reserving the
    // backslash makes `"\d+"` loud, but it CANNOT make `"C:\temp"` loud —
    // `\t` is a legal escape, so the path silently becomes `C:<TAB>emp`.
    // Every language with C-style escapes carries this; the fix is not
    // more escapes but raw strings (deferred, and out of scope for v1).
    assert_eq!(value_of("p = \"C:\\temp\"\n"), "C:\temp");
    let src = Source::new("test.hero".to_string(), "p = \"C:\\temp\"\n".to_string());
    assert!(lex(&src).diagnostics.is_empty());
    // The half that IS loud: an unknown escape after the backslash.
    let src2 = Source::new("test.hero".to_string(), "p = \"C:\\Users\"\n".to_string());
    assert_eq!(lex(&src2).diagnostics[0].code, "unknown_escape");
    // **The trap widened when `\r` landed** (panel 066, the compiler-engineer's
    // finding): `r` is a common path-initial letter, so `"C:\\results"` joins
    // `"C:\\temp"` in the silent set. The trap is exactly the legal escapes whose
    // letter starts a plausible word — `\n`, `\t`, `\r` — and the price was
    // weighed against a byte that had no spelling at all.
    let src3 = Source::new("test.hero".to_string(), "p = \"C:\\results\"\n".to_string());
    assert!(lex(&src3).diagnostics.is_empty());
    assert_eq!(value_of("p = \"C:\\results\"\n"), "C:\u{0d}esults");
}

#[test]
fn the_sixth_escape_decodes_in_both_contexts() {
    // Panel 066. The value is 13 in both, from the one table — and the
    // literals still lex as one token each, which is what the escape is for:
    // before it, byte 13 had no spelling at all.
    assert_eq!(value_of("s = \"a\\rb\"\n"), "a\rb");
    assert_eq!(value_of("c = '\\r'\n"), "\r");
    let src = Source::new("test.hero".to_string(), "s = \"a\\rb\"\nc = '\\r'\n".to_string());
    let out = lex(&src);
    assert!(out.diagnostics.is_empty());
    assert_eq!(out.tokens.iter().filter(|t| t.kind == TokenKind::Str).count(), 1);
    assert_eq!(out.tokens.iter().filter(|t| t.kind == TokenKind::Char).count(), 1);
}

#[test]
fn a_raw_carriage_return_in_a_literal_is_refused_with_the_escape_as_its_repair() {
    // The coherence condition of panel 066, and the reason it exists: this
    // program compiled at exit 0 before the sitting, rendered identically to
    // the same literal without the byte, and survived `fmt` — so retyping what
    // the screen shows changed the bytes and still compiled. One spelling per
    // character (§4.15) now cuts the invisible one.
    let src = Source::new("test.hero".to_string(), "s = \"a\rb\"\n".to_string());
    let out = lex(&src);
    assert_eq!(out.diagnostics.len(), 1);
    assert_eq!(out.diagnostics[0].code, "raw_carriage_return");
    assert_eq!(out.diagnostics[0].fixes[0].replacement, "\\r");
    assert_eq!(out.diagnostics[0].fixes[0].certainty, crate::diagnostics::Certainty::Certain);
    // The token keeps its kind — one mistake, one diagnostic (panel 007).
    assert!(out.tokens.iter().any(|t| t.kind == TokenKind::Str));
    // And in a character literal, where the byte is likewise content.
    let ch = Source::new("test.hero".to_string(), "c = '\r'\n".to_string());
    let out2 = lex(&ch);
    assert_eq!(out2.diagnostics[0].code, "raw_carriage_return");
    // A CRLF line ending is untouched: the CR is not inside a literal.
    let crlf = Source::new("test.hero".to_string(), "x = 1\r\ny = 2\r\n".to_string());
    assert!(lex(&crlf).diagnostics.is_empty());
}

#[test]
fn cross_context_escape_is_rejected_with_its_own_repair() {
    // Go's split, enforced: exactly one spelling for each character.
    let src = Source::new("test.hero".to_string(), "s = \"it\\'s\"\n".to_string());
    let out = lex(&src);
    let d = &out.diagnostics[0];
    assert_eq!(d.code, "escape_not_needed");
    assert_eq!(d.fixes[0].replacement, "'");
    let src2 = Source::new("test.hero".to_string(), "c = '\\\"'\n".to_string());
    let out2 = lex(&src2);
    assert_eq!(out2.diagnostics[0].code, "escape_not_needed");
    assert_eq!(out2.diagnostics[0].fixes[0].replacement, "\"");
}

#[test]
fn backslash_at_end_of_line_leaves_the_layout_intact() {
    // The escape must not swallow the newline: doing so would destroy
    // every indent/dedent that follows (§4.15).
    assert_eq!(
        dump("s = \"oops\\\n    x\n"),
        "\
1:1 ident s
1:3 eq =
1:5 error \"oops\\
2:1 indent
2:5 ident x
2:6 terminator
3:1 dedent
3:1 eof
DIAG test.hero:1:5: error[unterminated_string]: this string never closes — strings are single-line, `\"` to `\"`
"
    );
}

// --- broken literals -------------------------------------------------------

#[test]
fn a_multibyte_character_after_the_backslash_does_not_panic() {
    // Regression guard: the escape validator must advance by the escaped
    // character's UTF-8 LENGTH, not by one byte. Advancing by a fixed two
    // bytes would leave `pos` mid-character and the next slice would panic
    // on a non-char-boundary — a crash, not a diagnostic.
    //
    // The terminator's column reads **11 and not 12** from 2026-08-12: a column
    // counts characters, and `è` is two bytes. This test was the only thing in
    // the tree pinning the old answer, and it was pinning it by accident — it
    // is about the validator's stride, and it happened to also record a column
    // that no reader could have matched against the line above it.
    assert_eq!(
        dump("s = \"a\\èb\"\n"),
        "\
1:1 ident s
1:3 eq =
1:5 str \"a\\èb\"
1:11 terminator
2:1 eof
DIAG test.hero:1:7: error[unknown_escape]: `\\è` is not an escape sequence — the escapes are \\n \\t \\r \\\\ \\\"; write `\\\\` for a literal backslash
"
    );
    // And it decodes without losing the character.
    assert_eq!(value_of("s = \"a\\èb\"\n"), "a\\èb");
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
fn char_literal_holds_exactly_one_character() {
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
DIAG test.hero:1:5: error[char_literal]: a character literal holds exactly one character — one ASCII character ('a', '0', ' ') or one escape ('\\n', '\\t') — `''` does not
DIAG test.hero:2:5: error[char_literal]: a character literal holds exactly one character — one ASCII character ('a', '0', ' ') or one escape ('\\n', '\\t') — `'ab'` does not
DIAG test.hero:3:5: error[char_literal]: a character literal holds exactly one character — one ASCII character ('a', '0', ' ') or one escape ('\\n', '\\t') — `'é'` does not
"
    );
}

#[test]
fn lone_backslash_char_suggests_escaping_it() {
    // `'\'` — the backslash escaped the closing quote, so the literal never
    // closes. The repair is certain: `'\\'`.
    let src = Source::new("test.hero".to_string(), "c = '\\'\n".to_string());
    let out = lex(&src);
    let d = &out.diagnostics[0];
    assert_eq!(d.code, "unterminated_char");
    assert_eq!(d.fixes[0].replacement, "'\\\\'");
}
