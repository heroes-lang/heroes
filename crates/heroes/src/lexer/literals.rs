//! A value written between quotes (design.md §4.1; spec § Types; panel 008).
//!
//! Split out of `scan.rs` by the §11 sweep. `scan.rs` decides *which* token starts
//! here; this scans the two whose content is a value the author wrote out — a
//! string and a character — and they belong together because they share the rule
//! that panel 008 spent a sitting on: **the backslash is reserved**, five escapes
//! exist, and any other escape is a compile error rather than a literal backslash.
//!
//! **A bad escape does NOT poison the token.** It is reported and the literal stays
//! a `Str`, so one mistake inside a string costs one diagnostic rather than
//! derailing the rest of the line — the same rule the parser follows after a failed
//! statement.
//!
//! Both stop at the end of the line. A string is not multi-line in this language,
//! so an unterminated one is a mistake with a known extent, and saying so at the
//! newline gives a caret that fits on the screen.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::source::{Source, Span};

use super::escape::Quoted;
use super::token::TokenKind;
use super::LexState;

impl LexState {
    /// `"…"` to the closing quote on the same line, escapes validated as we
    /// go (`escape.rs`). A bad escape does NOT poison the token: it is
    /// reported and the literal stays a `Str`.
    pub(super) fn string(&mut self, src: &Source) {
        let text = src.text.as_bytes();
        let start = self.pos;
        self.pos += 1; // opening "
        loop {
            match text.get(self.pos) {
                Some(b'"') => {
                    self.pos += 1;
                    self.push(TokenKind::Str, start);
                    return;
                }
                Some(b'\\') => {
                    if !self.escape(src, Quoted::Str) {
                        break; // backslash at end of line
                    }
                }
                None | Some(b'\n') => break,
                Some(_) => self.pos += 1,
            }
        }
        let span = Span { start: start as u32, end: self.pos as u32 };
        self.error_token(Diagnostic::new(
            "unterminated_string",
            "this string never closes — strings are single-line, `\"` to `\"`".to_string(),
            span,
        ));
    }

    /// `'a'` — exactly ONE character: one ASCII byte, or one escape
    /// sequence. Its value is an `i64` (design.md §4.3). `'\n'` is four
    /// source bytes and one character — source length stopped being the
    /// rule when escapes landed (panel 008). Syntax is ASCII-only (§1.10),
    /// so a multi-byte character here is an error, not an interpretation.
    pub(super) fn char_lit(&mut self, src: &Source) {
        let text = src.text.as_bytes();
        let start = self.pos;
        let content_start = start + 1;
        self.pos = content_start;
        // One pass to the closing quote, counting units: a unit is one
        // character or one whole escape sequence. Scanning past the closer
        // in one go is what keeps `'ab'` a single diagnostic instead of an
        // unterminated literal followed by debris.
        let mut units = 0;
        let closed = loop {
            match text.get(self.pos).copied() {
                None | Some(b'\n') => break false,
                Some(b'\'') => break true,
                Some(b'\\') => {
                    if !self.escape(src, Quoted::Char) {
                        break false;
                    }
                    units += 1;
                }
                Some(_) => {
                    match src.text[self.pos..].chars().next() {
                        Some(ch) => self.pos += ch.len_utf8(),
                        None => break false,
                    }
                    units += 1;
                }
            }
        };
        if !closed {
            return self.unterminated_char(src, start);
        }
        let content = &src.text[content_start..self.pos];
        self.pos += 1; // closing '
        if units == 1 && (content.starts_with('\\') || content.is_ascii()) {
            self.push(TokenKind::Char, start);
        } else {
            let span = Span { start: start as u32, end: self.pos as u32 };
            self.error_token(Diagnostic::new(
                "char_literal",
                format!(
                    "a character literal holds exactly one character — one ASCII character ('a', '0', ' ') or one escape ('\\n', '\\t') — `'{content}'` does not"
                ),
                span,
            ));
        }
    }

    /// No closing quote on this line. `self.pos` is already where the scan
    /// stopped (on the newline, never past it).
    fn unterminated_char(&mut self, src: &Source, start: usize) {
        let span = Span { start: start as u32, end: self.pos as u32 };
        let mut diag = Diagnostic::new(
            "unterminated_char",
            "this character literal never closes — write one character between single quotes: 'a'"
                .to_string(),
            span,
        );
        // `'\'` is the classic: the backslash escaped the closing quote.
        if src.slice(span).contains('\\') {
            diag.fixes.push(Fix {
                title: "escape the backslash: `'\\\\'`".to_string(),
                replacement: "'\\\\'".to_string(),
                span,
                certainty: Certainty::Certain,
            });
        }
        self.error_token(diag);
    }}
