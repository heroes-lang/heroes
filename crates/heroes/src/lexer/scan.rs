//! Scanning: the part of the lexer that reads *words* instead of shape.
//!
//! `line_body` walks one physical line and dispatches on the first byte of
//! each token: `#` comment, `"` string, `'` char, digit, letter, or
//! punctuation. Every scanner follows the same contract:
//!
//! - it advances `self.pos` past exactly what it consumed;
//! - on success it calls `push` (which also tracks bracket depth);
//! - on failure it calls `error_token` — a diagnostic plus an `Error`
//!   token — and *keeps lexing*. The lexer never stops: one bad literal
//!   must not hide every error after it.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::source::{Source, Span};

use super::escape::Quoted;
use super::keywords::{foreign_word, keyword};
use super::token::TokenKind;
use super::LexState;

impl LexState {
    /// Lex tokens until the end of the physical line (inclusive).
    pub(super) fn line_body(&mut self, src: &Source) {
        let text = src.text.as_bytes();
        loop {
            match text.get(self.pos).copied() {
                None => return,
                Some(b' ') => self.pos += 1,
                Some(b'\t') => {
                    // A tab between tokens is as illegal as one in the margin.
                    let span = Span { start: self.pos as u32, end: self.pos as u32 + 1 };
                    self.diagnostics.push(Diagnostic::new(
                        "tab_in_line",
                        "a tab is a compile error — use spaces".to_string(),
                        span,
                    ));
                    self.pos += 1;
                }
                Some(b'\r') => self.pos += 1, // tolerated only as part of \r\n
                Some(b'\n') => {
                    self.maybe_terminator();
                    self.pos += 1;
                    self.last_significant = None;
                    return;
                }
                Some(b'#') => self.comment(src),
                Some(b'"') => self.string(src),
                Some(b'\'') => self.char_lit(src),
                Some(b) if b.is_ascii_digit() => self.number(src),
                Some(b) if b.is_ascii_alphabetic() || b == b'_' => self.ident(src),
                Some(_) => self.punct(src),
            }
        }
    }

    /// `# …` to end of line, retained as a token (doc comments and `##`
    /// section headings are the parser's reading of the same span).
    /// Comments never influence terminator insertion, so this bypasses
    /// `push` and leaves `last_significant` alone.
    fn comment(&mut self, src: &Source) {
        let text = src.text.as_bytes();
        let start = self.pos;
        while let Some(&b) = text.get(self.pos) {
            if b == b'\n' {
                break;
            }
            self.pos += 1;
        }
        let mut end = self.pos;
        // Don't let a trailing \r into the comment's text.
        if end > start && text[end - 1] == b'\r' {
            end -= 1;
        }
        self.tokens.push(super::Token {
            kind: TokenKind::Comment,
            span: Span { start: start as u32, end: end as u32 },
        });
    }

    /// `"…"` to the closing quote on the same line, escapes validated as we
    /// go (`escape.rs`). A bad escape does NOT poison the token: it is
    /// reported and the literal stays a `Str`.
    fn string(&mut self, src: &Source) {
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
    /// sequence. Its value is an `int` (design.md §4.3). `'\n'` is four
    /// source bytes and one character — source length stopped being the
    /// rule when escapes landed (panel 008). Syntax is ASCII-only (§1.10),
    /// so a multi-byte character here is an error, not an interpretation.
    fn char_lit(&mut self, src: &Source) {
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
    }

    /// Digits, then optionally `.` digits. The dot only makes a float when
    /// digits follow: `1.5` is one token; `1.str()` is Int Dot Ident (UFCS
    /// must keep working on number literals).
    fn number(&mut self, src: &Source) {
        let text = src.text.as_bytes();
        let start = self.pos;
        while text.get(self.pos).is_some_and(|b| b.is_ascii_digit()) {
            self.pos += 1;
        }
        let mut kind = TokenKind::Int;
        if text.get(self.pos) == Some(&b'.')
            && text.get(self.pos + 1).is_some_and(|b| b.is_ascii_digit())
        {
            self.pos += 1;
            while text.get(self.pos).is_some_and(|b| b.is_ascii_digit()) {
                self.pos += 1;
            }
            kind = TokenKind::Float;
        }
        self.push(kind, start);
    }

    /// A word: Heroes keyword, foreign reserved word (loud failure with the
    /// repair pre-written — the thesis, executable), or plain identifier.
    fn ident(&mut self, src: &Source) {
        let text = src.text.as_bytes();
        let start = self.pos;
        while text
            .get(self.pos)
            .is_some_and(|b| b.is_ascii_alphanumeric() || *b == b'_')
        {
            self.pos += 1;
        }
        let word = &src.text[start..self.pos];
        if let Some(kind) = keyword(word) {
            self.push(kind, start);
            return;
        }
        if let Some((message, replacement)) = foreign_word(word) {
            let span = Span { start: start as u32, end: self.pos as u32 };
            let mut diag = Diagnostic::new("reserved_word", message.to_string(), span);
            if let Some(fix) = replacement {
                diag.fixes.push(Fix {
                    title: format!("replace `{word}` with `{fix}`"),
                    replacement: fix.to_string(),
                    span,
                    certainty: Certainty::Certain,
                });
            }
            self.error_token(diag);
            return;
        }
        self.push(TokenKind::Ident, start);
    }

    /// Punctuation and operators, longest match first (`???` before `?`,
    /// `->` before `-`, `==` before `=`…). Anything unmatched is skipped
    /// one whole character at a time (UTF-8-safe) with a diagnostic.
    fn punct(&mut self, src: &Source) {
        let text = src.text.as_bytes();
        let start = self.pos;
        let at = |ahead: usize| text.get(start + ahead).copied();
        let (kind, len) = match (at(0).unwrap_or(0), at(1), at(2)) {
            (b'?', Some(b'?'), Some(b'?')) => (TokenKind::Hole, 3),
            (b'-', Some(b'>'), _) => (TokenKind::Arrow, 2),
            (b'=', Some(b'>'), _) => (TokenKind::FatArrow, 2),
            (b'=', Some(b'='), _) => (TokenKind::EqEq, 2),
            (b'!', Some(b'='), _) => (TokenKind::BangEq, 2),
            (b'<', Some(b'='), _) => (TokenKind::Le, 2),
            (b'>', Some(b'='), _) => (TokenKind::Ge, 2),
            (b'&', Some(b'&'), _) => (TokenKind::AndAnd, 2),
            (b'|', Some(b'|'), _) => (TokenKind::OrOr, 2),
            (b'=', _, _) => (TokenKind::Eq, 1),
            (b'@', _, _) => (TokenKind::At, 1),
            (b':', _, _) => (TokenKind::Colon, 1),
            (b',', _, _) => (TokenKind::Comma, 1),
            (b'.', _, _) => (TokenKind::Dot, 1),
            (b'?', _, _) => (TokenKind::Question, 1),
            (b'|', _, _) => (TokenKind::Pipe, 1),
            (b'(', _, _) => (TokenKind::LParen, 1),
            (b')', _, _) => (TokenKind::RParen, 1),
            (b'[', _, _) => (TokenKind::LBracket, 1),
            (b']', _, _) => (TokenKind::RBracket, 1),
            (b'{', _, _) => (TokenKind::LBrace, 1),
            (b'}', _, _) => (TokenKind::RBrace, 1),
            (b'+', _, _) => (TokenKind::Plus, 1),
            (b'-', _, _) => (TokenKind::Minus, 1),
            (b'*', _, _) => (TokenKind::Star, 1),
            (b'/', _, _) => (TokenKind::Slash, 1),
            (b'%', _, _) => (TokenKind::Percent, 1),
            (b'<', _, _) => (TokenKind::Lt, 1),
            (b'>', _, _) => (TokenKind::Gt, 1),
            (b'!', _, _) => (TokenKind::Bang, 1),
            _ => {
                let ch = src.text[start..].chars().next().unwrap_or('\u{FFFD}');
                self.pos += ch.len_utf8();
                let span = Span { start: start as u32, end: self.pos as u32 };
                self.error_token(Diagnostic::new(
                    "unexpected_character",
                    format!("`{ch}` is not part of the language's syntax (ASCII-only)"),
                    span,
                ));
                return;
            }
        };
        self.pos = start + len;
        self.push(kind, start);
    }
}
