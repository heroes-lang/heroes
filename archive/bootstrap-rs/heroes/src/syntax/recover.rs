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
//! There used to be a subtle fifth move here — `at_line_start`, span
//! arithmetic to see the line boundary panel 007's rule made invisible
//! after `Point = record`. The keyword-first shape (panel 018) retired it:
//! every header now ends in a token that can end a statement, so the
//! Terminator is always in the stream and tokens carry the answer.

use crate::lexer::TokenKind;
use crate::source::Span;

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
    pub(super) fn recover_to_next_decl(&mut self) {
        // Since panel 018 every header line ends in a token that can end a
        // statement, so a Terminator (or a layout token) behind the cursor
        // means a fresh line: nothing is left of the broken declaration but
        // a possible orphaned block. This used to need span arithmetic
        // (`at_line_start`, panel 007) when `Point = record` ended in a
        // non-ender; the keyword-first shape made the token stream carry it.
        let at_line_start = self.pos == 0
            || matches!(
                self.tokens[self.pos - 1].kind,
                TokenKind::Terminator | TokenKind::Indent | TokenKind::Dedent
            );
        if !at_line_start {
            while !self.at(TokenKind::Eof) && !self.at(TokenKind::Indent) {
                if self.bump().kind == TokenKind::Terminator {
                    break;
                }
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

}
