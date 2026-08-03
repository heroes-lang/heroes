//! Layout: the part of the lexer that reads *shape* instead of words.
//!
//! Heroes has no braces and no semicolons (design.md §4.15), so structure
//! travels in two invisible currencies, both minted here:
//!
//! 1. **Indentation** — `line_start` measures each line's margin and turns
//!    changes into `Indent`/`Dedent` tokens. The rules are rigid on
//!    purpose (every "nearly right" margin is an error, never an
//!    interpretation): spaces only, exactly 4 per level, one new level at
//!    a time. Blank and comment-only lines are layout-neutral. Inside
//!    `( [ {` the margin means nothing at all (panel 007): multi-line
//!    calls and literals indent freely, and the level is frozen until the
//!    bracket closes.
//! 2. **Terminators** — `maybe_terminator` plants the invisible semicolon
//!    at a line end, but only when the line's last significant token can
//!    *end a statement* (`is_line_ender`, Go's rule). A line ending in
//!    `+` or `(` is visibly unfinished, so it quietly continues.

use crate::diagnostics::Diagnostic;
use crate::source::{Source, Span};

use super::token::{Token, TokenKind};
use super::LexState;

/// design.md §4.15 (panel 007): the terminator is inserted when a line ends
/// with an identifier, a literal, `return`, `break`, `continue`, `???`,
/// postfix `?`, `)`, `]`, or `}`. `?` is the analogue of Go's `++`/`--`;
/// panel 007 records why it is required, not merely safe.
pub(super) fn is_line_ender(kind: TokenKind) -> bool {
    matches!(
        kind,
        TokenKind::Ident
            | TokenKind::Int
            | TokenKind::Float
            | TokenKind::Str
            | TokenKind::Char
            | TokenKind::KwTrue
            | TokenKind::KwFalse
            | TokenKind::KwReturn
            | TokenKind::KwBreak
            | TokenKind::KwContinue
            | TokenKind::Hole
            | TokenKind::Question
            | TokenKind::RParen
            | TokenKind::RBracket
            | TokenKind::RBrace
    )
}

impl LexState {
    /// Consume the line's leading whitespace and emit `Indent`/`Dedent`.
    /// Every error path recovers by keeping the current level: the line
    /// still lexes, the diagnostic tells the author what the margin meant.
    pub(super) fn line_start(&mut self, src: &Source) {
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
            return;
        }
        // Inside brackets the margin is not structural (panel 007).
        if !self.open_brackets.is_empty() {
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
            return;
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
            // Recovery: honour the nesting the writer asked for, so the
            // parser sees a consistent block structure downstream.
            for _ in self.level..new_level {
                self.tokens.push(Token { kind: TokenKind::Indent, span });
            }
            self.level = new_level;
        } else if new_level == self.level + 1 {
            self.tokens.push(Token { kind: TokenKind::Indent, span });
            self.level = new_level;
        } else {
            // A dedent may close several levels at once; each gets its own
            // token so blocks nest and unnest one at a time for the parser.
            for _ in new_level..self.level {
                self.tokens.push(Token { kind: TokenKind::Dedent, span });
            }
            self.level = new_level;
        }
    }

    /// Plant the invisible semicolon — only if the line can be *over*.
    pub(super) fn maybe_terminator(&mut self) {
        if self.last_significant.is_some_and(is_line_ender) {
            let span = Span { start: self.pos as u32, end: self.pos as u32 };
            self.tokens.push(Token { kind: TokenKind::Terminator, span });
        }
    }
}
