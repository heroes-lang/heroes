//! The lexer: source text → tokens (design.md §4.15).
//!
//! Rigid layout, by the letter of the design:
//! - spaces only — a tab in indentation is a compile error, not an
//!   interpretation;
//! - exactly 4 spaces per level (`Indent`/`Dedent` tokens); five spaces is
//!   an error, and an indent may grow by at most one level per line;
//! - Go-style terminator insertion: a `Terminator` is emitted at end of
//!   line only when the line's last significant token is an identifier, a
//!   literal, `return`, `)`, `]`, or `}`. NOTE: design.md's ender list
//!   omits `break`/`continue`/`???` — implemented by the letter; the gap is
//!   on record in docs/panel/OPEN-QUESTIONS.md for a session before M2.
//! - `#` comments run to end of line and are RETAINED as tokens (a comment
//!   directly above a declaration is its documentation; `##` is a section
//!   heading — telling them apart is the parser's job, the text is in the
//!   span).
//!
//! The lexer never stops: on an error it records a diagnostic, emits an
//! `Error` token if a character had to be skipped, and keeps going.
//!
//! Shape note (the Cyclone rule): `LexState` owns everything it carries;
//! the source text enters every function as a parameter, never lives in a
//! struct field.

#[cfg(test)]
mod tests;

use crate::diagnostics::Diagnostic;
use crate::source::{Source, Span};

#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum TokenKind {
    // Names and literals.
    Ident,
    Int,
    Float,
    // Heroes keywords (spec/reserved-words.md § keywords).
    KwConstant,
    KwFunction,
    KwRecord,
    KwVariant,
    KwMatch,
    KwIf,
    KwElse,
    KwFor,
    KwIn,
    KwBreak,
    KwContinue,
    KwReturn,
    KwTest,
    KwAssert,
    KwExtern,
    KwTrue,
    KwFalse,
    KwFail,
    // Punctuation.
    Eq,
    At,
    Colon,
    Comma,
    Dot,
    Question,
    Hole,     // ???
    Arrow,    // ->
    FatArrow, // =>
    Pipe,     // | (match-pattern join; bitwise use is reserved, design.md §4.14)
    LParen,
    RParen,
    LBracket,
    RBracket,
    LBrace,
    RBrace,
    // Operators.
    Plus,
    Minus,
    Star,
    Slash,
    Percent,
    EqEq,
    BangEq,
    Lt,
    Le,
    Gt,
    Ge,
    AndAnd,
    OrOr,
    Bang,
    // Trivia and layout.
    Comment,
    Terminator,
    Indent,
    Dedent,
    Eof,
    // Error recovery: a skipped character.
    Error,
}

#[derive(Clone, Copy, Debug)]
pub struct Token {
    pub kind: TokenKind,
    pub span: Span,
}

pub struct LexOutput {
    pub tokens: Vec<Token>,
    pub diagnostics: Vec<Diagnostic>,
}

pub fn lex(src: &Source) -> LexOutput {
    let mut st = LexState {
        pos: 0,
        level: 0,
        tokens: Vec::new(),
        diagnostics: Vec::new(),
        last_significant: None,
    };
    loop {
        st.line_start(src);
        if st.pos >= src.text.len() {
            break;
        }
        st.line_body(src);
    }
    // EOF: terminate the last line if it ended without a newline, then
    // close every open block.
    st.maybe_terminator();
    let end = Span { start: st.pos as u32, end: st.pos as u32 };
    for _ in 0..st.level {
        st.tokens.push(Token { kind: TokenKind::Dedent, span: end });
    }
    st.tokens.push(Token { kind: TokenKind::Eof, span: end });
    LexOutput { tokens: st.tokens, diagnostics: st.diagnostics }
}

/// design.md §4.15: the terminator is inserted when a line ends with an
/// identifier, a literal, `return`, `)`, `]`, or `}`.
fn is_line_ender(kind: TokenKind) -> bool {
    matches!(
        kind,
        TokenKind::Ident
            | TokenKind::Int
            | TokenKind::Float
            | TokenKind::KwTrue
            | TokenKind::KwFalse
            | TokenKind::KwReturn
            | TokenKind::RParen
            | TokenKind::RBracket
            | TokenKind::RBrace
    )
}

fn keyword(text: &str) -> Option<TokenKind> {
    Some(match text {
        "constant" => TokenKind::KwConstant,
        "function" => TokenKind::KwFunction,
        "record" => TokenKind::KwRecord,
        "variant" => TokenKind::KwVariant,
        "match" => TokenKind::KwMatch,
        "if" => TokenKind::KwIf,
        "else" => TokenKind::KwElse,
        "for" => TokenKind::KwFor,
        "in" => TokenKind::KwIn,
        "break" => TokenKind::KwBreak,
        "continue" => TokenKind::KwContinue,
        "return" => TokenKind::KwReturn,
        "test" => TokenKind::KwTest,
        "assert" => TokenKind::KwAssert,
        "extern" => TokenKind::KwExtern,
        "true" => TokenKind::KwTrue,
        "false" => TokenKind::KwFalse,
        "fail" => TokenKind::KwFail,
        _ => return None,
    })
}

/// Stable lowercase name, used by `heroes lex --json` and the snapshots.
pub fn kind_name(kind: TokenKind) -> &'static str {
    match kind {
        TokenKind::Ident => "ident",
        TokenKind::Int => "int",
        TokenKind::Float => "float",
        TokenKind::KwConstant => "kw_constant",
        TokenKind::KwFunction => "kw_function",
        TokenKind::KwRecord => "kw_record",
        TokenKind::KwVariant => "kw_variant",
        TokenKind::KwMatch => "kw_match",
        TokenKind::KwIf => "kw_if",
        TokenKind::KwElse => "kw_else",
        TokenKind::KwFor => "kw_for",
        TokenKind::KwIn => "kw_in",
        TokenKind::KwBreak => "kw_break",
        TokenKind::KwContinue => "kw_continue",
        TokenKind::KwReturn => "kw_return",
        TokenKind::KwTest => "kw_test",
        TokenKind::KwAssert => "kw_assert",
        TokenKind::KwExtern => "kw_extern",
        TokenKind::KwTrue => "kw_true",
        TokenKind::KwFalse => "kw_false",
        TokenKind::KwFail => "kw_fail",
        TokenKind::Eq => "eq",
        TokenKind::At => "at",
        TokenKind::Colon => "colon",
        TokenKind::Comma => "comma",
        TokenKind::Dot => "dot",
        TokenKind::Question => "question",
        TokenKind::Hole => "hole",
        TokenKind::Arrow => "arrow",
        TokenKind::FatArrow => "fat_arrow",
        TokenKind::Pipe => "pipe",
        TokenKind::LParen => "lparen",
        TokenKind::RParen => "rparen",
        TokenKind::LBracket => "lbracket",
        TokenKind::RBracket => "rbracket",
        TokenKind::LBrace => "lbrace",
        TokenKind::RBrace => "rbrace",
        TokenKind::Plus => "plus",
        TokenKind::Minus => "minus",
        TokenKind::Star => "star",
        TokenKind::Slash => "slash",
        TokenKind::Percent => "percent",
        TokenKind::EqEq => "eq_eq",
        TokenKind::BangEq => "bang_eq",
        TokenKind::Lt => "lt",
        TokenKind::Le => "le",
        TokenKind::Gt => "gt",
        TokenKind::Ge => "ge",
        TokenKind::AndAnd => "and_and",
        TokenKind::OrOr => "or_or",
        TokenKind::Bang => "bang",
        TokenKind::Comment => "comment",
        TokenKind::Terminator => "terminator",
        TokenKind::Indent => "indent",
        TokenKind::Dedent => "dedent",
        TokenKind::Eof => "eof",
        TokenKind::Error => "error",
    }
}

/// Owned lexing state. The source text is a parameter everywhere.
struct LexState {
    pos: usize,
    /// Current indentation level (one level == 4 spaces).
    level: u32,
    tokens: Vec<Token>,
    diagnostics: Vec<Diagnostic>,
    /// Kind of the current line's last significant (non-comment) token.
    last_significant: Option<TokenKind>,
}

impl LexState {
    /// Measure indentation and emit Indent/Dedent. Blank and comment-only
    /// lines are layout-neutral: they neither open nor close blocks.
    fn line_start(&mut self, src: &Source) {
        let text = src.text.as_bytes();
        let ws_start = self.pos;
        let mut spaces = 0u32;
        let mut saw_tab = false;
        while let Some(&b) = text.get(self.pos) {
            match b {
                b' ' => {
                    spaces += 1;
                    self.pos += 1;
                }
                b'\t' => {
                    saw_tab = true;
                    self.pos += 1;
                }
                _ => break,
            }
        }
        if saw_tab {
            let span = Span { start: ws_start as u32, end: self.pos as u32 };
            self.diagnostics.push(Diagnostic::new(
                "tab_in_indentation",
                "a tab in indentation is a compile error — indent with spaces, exactly 4 per level"
                    .to_string(),
                span,
            ));
            // Recovery: keep the current level; the line still lexes.
            return;
        }
        // Layout-neutral lines: empty, or comment-only.
        match text.get(self.pos) {
            None | Some(b'\n') | Some(b'\r') | Some(b'#') => return,
            _ => {}
        }
        if !spaces.is_multiple_of(4) {
            let span = Span { start: ws_start as u32, end: self.pos as u32 };
            self.diagnostics.push(Diagnostic::new(
                "indentation_not_multiple_of_4",
                format!("indentation must be exactly 4 spaces per level; found {spaces}"),
                span,
            ));
            return; // recovery: keep the current level
        }
        let new_level = spaces / 4;
        let span = Span { start: ws_start as u32, end: self.pos as u32 };
        if new_level > self.level + 1 {
            self.diagnostics.push(Diagnostic::new(
                "indentation_jump",
                format!(
                    "indentation jumps from level {} to level {new_level}; a block opens one level at a time",
                    self.level
                ),
                span,
            ));
            // Recovery: honour the nesting the writer asked for.
            for _ in self.level..new_level {
                self.tokens.push(Token { kind: TokenKind::Indent, span });
            }
            self.level = new_level;
        } else if new_level == self.level + 1 {
            self.tokens.push(Token { kind: TokenKind::Indent, span });
            self.level = new_level;
        } else {
            for _ in new_level..self.level {
                self.tokens.push(Token { kind: TokenKind::Dedent, span });
            }
            self.level = new_level;
        }
    }

    /// Lex tokens until the end of the physical line (inclusive).
    fn line_body(&mut self, src: &Source) {
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
                Some(b) if b.is_ascii_digit() => self.number(src),
                Some(b) if b.is_ascii_alphabetic() || b == b'_' => self.ident(src),
                Some(_) => self.punct(src),
            }
        }
    }

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
        // Comments never influence terminator insertion.
        self.tokens.push(Token {
            kind: TokenKind::Comment,
            span: Span { start: start as u32, end: end as u32 },
        });
    }

    fn number(&mut self, src: &Source) {
        let text = src.text.as_bytes();
        let start = self.pos;
        while text.get(self.pos).is_some_and(|b| b.is_ascii_digit()) {
            self.pos += 1;
        }
        let mut kind = TokenKind::Int;
        // A dot makes it a float only when digits follow: `1.5` is one
        // token; `1.str()` is Int Dot Ident.
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
        let kind = keyword(word).unwrap_or(TokenKind::Ident);
        self.push(kind, start);
    }

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
                // Not a token of the language. Skip one whole character
                // (UTF-8-safe), record, move on.
                let ch = src.text[start..].chars().next().unwrap_or('\u{FFFD}');
                self.pos += ch.len_utf8();
                let span = Span { start: start as u32, end: self.pos as u32 };
                self.diagnostics.push(Diagnostic::new(
                    "unexpected_character",
                    format!("`{ch}` is not part of the language's syntax (ASCII-only)"),
                    span,
                ));
                self.tokens.push(Token { kind: TokenKind::Error, span });
                self.last_significant = Some(TokenKind::Error);
                return;
            }
        };
        self.pos = start + len;
        self.push(kind, start);
    }

    fn maybe_terminator(&mut self) {
        if self.last_significant.is_some_and(is_line_ender) {
            let span = Span { start: self.pos as u32, end: self.pos as u32 };
            self.tokens.push(Token { kind: TokenKind::Terminator, span });
        }
    }

    fn push(&mut self, kind: TokenKind, start: usize) {
        self.tokens.push(Token {
            kind,
            span: Span { start: start as u32, end: self.pos as u32 },
        });
        self.last_significant = Some(kind);
    }
}
