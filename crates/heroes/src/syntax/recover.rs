//! Recovery: what to drop after a mistake, so that the next mistake is still
//! found and the same one is not reported twice (design.md §4.17 — the error
//! is the deliverable, and a model paying ten turns for ten mistakes is the
//! cost this file keeps down).
//!
//! Four moves, each with a landmark it trusts:
//!
//! | move | landmark |
//! |------|----------|
//! | `skip_line` | the line end, without leaving the block |
//! | `balanced_block` | `Indent` … matching `Dedent`, nesting included |
//! | `recover_to_next_decl` | the line end, then the block that hung off it |
//! | `recover_past_closer` | the bracket that matches the one consumed |
//!
//! The subtle one is `at_line_start`, and it exists because of panel 007: a
//! line ending in `record` or `variant` gets **no terminator**, so a line
//! boundary can be invisible in the token stream. Spans see it anyway.

use crate::lexer::TokenKind;
use crate::source::{Source, Span};

use super::cursor::Cursor;

impl Cursor {
    /// Consume an `Indent` … matching `Dedent` run and return the span of
    /// what sat between them. Nested blocks go with it, so the caller
    /// resumes on the next line of the *enclosing* block.
    pub(super) fn balanced_block(&mut self) -> Span {
        let open = self.bump(); // the Indent
        let mut depth = 1u32;
        let mut last = open.span;
        while depth > 0 && !self.at(TokenKind::Eof) {
            match self.kind() {
                TokenKind::Indent => {
                    depth += 1;
                    last = self.bump().span;
                }
                TokenKind::Dedent => {
                    depth -= 1;
                    let closer = self.bump();
                    if depth > 0 {
                        last = closer.span;
                    }
                }
                _ => last = self.bump().span,
            }
        }
        Span { start: open.span.end, end: last.end }
    }

    /// Drop the rest of a broken line, stopping before whatever ends or
    /// nests the block: the next line is then parsed on its own merits.
    pub(super) fn skip_line(&mut self) {
        while !matches!(
            self.kind(),
            TokenKind::Eof | TokenKind::Indent | TokenKind::Dedent
        ) {
            if self.bump().kind == TokenKind::Terminator {
                return;
            }
        }
    }

    /// Drop what is left of a broken declaration: the rest of its line and,
    /// if it had one, its whole body. One bad declaration must cost one
    /// diagnostic, not one per line inside it — and, just as importantly,
    /// must not cost the *next* declaration.
    pub(super) fn recover_to_next_decl(&mut self, src: &Source) {
        if self.at_line_start(src) {
            // Nothing left of the broken line. A block still goes with it:
            // it belonged to the declaration that failed.
            if self.at(TokenKind::Indent) {
                self.balanced_block();
            }
            return;
        }
        while !self.at(TokenKind::Eof) && !self.at(TokenKind::Indent) {
            if self.bump().kind == TokenKind::Terminator {
                break;
            }
        }
        if self.at(TokenKind::Indent) {
            self.balanced_block();
        }
    }

    /// Recover inside a bracketed list: drop tokens up to and including the
    /// closer matching the opener already consumed.
    ///
    /// Brackets are the one place where recovery has a landmark it cannot
    /// misread, and using it is what keeps a single misplaced separator in a
    /// signature from turning the rest of the line into "declarations" —
    /// four diagnostics for one mistake, which is the failure mode §4.17
    /// exists to prevent.
    pub(super) fn recover_past_closer(&mut self, opener: TokenKind, closer: TokenKind) {
        let mut depth = 1u32;
        while !self.at(TokenKind::Eof) {
            let kind = self.bump().kind;
            if kind == opener {
                depth += 1;
            } else if kind == closer {
                depth -= 1;
                if depth == 0 {
                    return;
                }
            }
        }
    }

    /// True when the current token opens a new source line.
    ///
    /// Spans decide it, not tokens, because a line ending in `record` or
    /// `variant` gets no terminator at all (panel 007: those words cannot
    /// end a statement). Without this, `Point = record` with its fields
    /// forgotten would swallow the declaration written below it — one
    /// mistake, two casualties.
    fn at_line_start(&self, src: &Source) -> bool {
        if self.pos == 0 {
            return true;
        }
        let previous = self.tokens[self.pos - 1];
        src.line_col(previous.span.end).0 != src.line_col(self.span().start).0
    }
}
