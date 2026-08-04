//! The token cursor: the parser's only way to move, and the only place that
//! knows how to fail.
//!
//! Three jobs, all of them invariants the grammar files then get to ignore:
//!
//! 1. **Comments never reach the grammar.** The cursor's position never rests
//!    on a `Comment`, but the spans it passes are kept, because a comment
//!    directly above a declaration *is* its documentation (design.md §4.1).
//!    `take_docs` hands that run back when a declaration starts.
//! 2. **The parser never stops.** A failed expectation is a diagnostic plus
//!    a recovery move (`recover_to_next_decl`), so a broken first
//!    declaration cannot hide the nine after it.
//! 3. **Errors are reported once.** At an `Error` token the lexer has
//!    already spoken; the cursor stays silent and lets recovery run, which
//!    is what keeps one mistake from producing two diagnostics.
//!
//! The Cyclone rule (CLAUDE.md §5): the cursor *owns* its token vector and
//! takes `&Source` as a parameter wherever it needs to read text.

use crate::diagnostics::Diagnostic;
use crate::lexer::{Token, TokenKind};
use crate::source::{Source, Span};

use super::describe::describe;

pub(super) struct Cursor {
    tokens: Vec<Token>,
    pos: usize,
    pub(super) diagnostics: Vec<Diagnostic>,
    /// Comment spans passed over since the last `take_docs`.
    comments: Vec<Span>,
}

impl Cursor {
    /// The token vector always ends with `Eof` (the lexer guarantees it), so
    /// `pos` is always in range and the parser never checks for the end
    /// twice.
    pub(super) fn new(tokens: Vec<Token>) -> Cursor {
        let mut cur =
            Cursor { tokens, pos: 0, diagnostics: Vec::new(), comments: Vec::new() };
        cur.pass_comments();
        cur
    }

    fn pass_comments(&mut self) {
        while self.tokens[self.pos].kind == TokenKind::Comment {
            self.comments.push(self.tokens[self.pos].span);
            self.pos += 1;
        }
    }

    // --- reading -------------------------------------------------------

    pub(super) fn kind(&self) -> TokenKind {
        self.tokens[self.pos].kind
    }

    pub(super) fn span(&self) -> Span {
        self.tokens[self.pos].span
    }

    pub(super) fn at(&self, kind: TokenKind) -> bool {
        self.kind() == kind
    }

    /// True where the lexer already reported the problem: say nothing more.
    pub(super) fn at_reported_error(&self) -> bool {
        self.at(TokenKind::Error)
    }

    // --- moving --------------------------------------------------------

    /// Consume the current token and return it. Stops at `Eof`, forever:
    /// every recovery loop can then be written without an end-of-file arm.
    pub(super) fn bump(&mut self) -> Token {
        let token = self.tokens[self.pos];
        if token.kind != TokenKind::Eof {
            self.pos += 1;
            self.pass_comments();
        }
        token
    }

    pub(super) fn eat(&mut self, kind: TokenKind) -> bool {
        if self.at(kind) {
            self.bump();
            return true;
        }
        false
    }

    /// Consume `kind` or report what was found instead. Never consumes on
    /// failure: the caller decides whether to recover or carry on.
    pub(super) fn expect(
        &mut self,
        kind: TokenKind,
        code: &str,
        what: &str,
        src: &Source,
    ) -> bool {
        if self.eat(kind) {
            return true;
        }
        if !self.at_reported_error() {
            let message = format!("expected {what}, found {}", self.found(src));
            self.error(code, message, self.span());
        }
        false
    }

    /// Line ends between declarations, and inside brackets, where the lexer
    /// inserts terminators that carry no structure (panel 007).
    pub(super) fn skip_terminators(&mut self) {
        while self.at(TokenKind::Terminator) {
            self.bump();
        }
    }

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

    // --- documentation --------------------------------------------------

    /// The comment lines directly above `decl`, in source order (§4.1).
    ///
    /// Adjacency is the whole rule, and it is two comparisons: the comment
    /// must sit on the line immediately above, and it must start in the same
    /// column — which is what tells `# doc` on its own line from the
    /// trailing `x = 5  # note` on the line before. A `##` section heading
    /// ends the run: it documents the section, not the declaration under it.
    pub(super) fn take_docs(&mut self, src: &Source, decl: Span) -> Vec<Span> {
        let (decl_line, decl_col) = src.line_col(decl.start);
        let mut wanted = decl_line;
        let mut docs: Vec<Span> = Vec::new();
        for span in self.comments.iter().rev() {
            let (line, col) = src.line_col(span.start);
            if line >= decl_line {
                continue; // trailing comment on the declaration's own line
            }
            if line + 1 != wanted || col != decl_col {
                break;
            }
            if src.slice(*span).starts_with("##") {
                break;
            }
            docs.push(*span);
            wanted = line;
        }
        self.comments.clear();
        docs.reverse();
        docs
    }

    // --- failing --------------------------------------------------------

    pub(super) fn error(&mut self, code: &str, message: String, span: Span) {
        self.diagnostics.push(Diagnostic::new(code, message, span));
    }

    pub(super) fn push_diagnostic(&mut self, diag: Diagnostic) {
        self.diagnostics.push(diag);
    }

    /// How the current token is named in "expected X, found Y". Tokens whose
    /// text carries information show it; punctuation and keywords are their
    /// own text already, and layout tokens have none at all.
    pub(super) fn found(&self, src: &Source) -> String {
        let token = self.tokens[self.pos];
        match token.kind {
            TokenKind::Ident
            | TokenKind::Int
            | TokenKind::Float
            | TokenKind::Str
            | TokenKind::Char
            | TokenKind::Error => {
                format!("{} (`{}`)", describe(token.kind), src.slice(token.span))
            }
            _ => describe(token.kind).to_string(),
        }
    }
}
