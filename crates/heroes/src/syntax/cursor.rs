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
//!
//! The recovery moves — what to drop after a mistake — live next door in
//! `recover.rs`, on the same struct.

use crate::diagnostics::Diagnostic;
use crate::lexer::{Token, TokenKind};
use crate::source::{Source, Span};

use super::describe::describe;

pub(super) struct Cursor {
    /// `tokens` and `pos` are `pub(super)` for one reason: `recover.rs`
    /// implements the recovery moves on this same struct. Nothing else in
    /// the parser may touch them — going through `bump` is what keeps the
    /// "never rests on a comment" invariant true.
    pub(super) tokens: Vec<Token>,
    pub(super) pos: usize,
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

    /// The kind `ahead` significant tokens on. Comments are skipped here too,
    /// so lookahead sees what the grammar sees. Two places need it: telling
    /// `name: value` from a bare argument, and telling `for x in xs` from
    /// `for cond`.
    pub(super) fn peek(&self, ahead: usize) -> TokenKind {
        let mut left = ahead;
        let mut at = self.pos;
        while at + 1 < self.tokens.len() && left > 0 {
            at += 1;
            if self.tokens[at].kind != TokenKind::Comment {
                left -= 1;
            }
        }
        self.tokens[at].kind
    }

    /// The kind of the token just consumed. One caller, one reason: a
    /// statement that ended with a block has already eaten its `Dedent`, so
    /// it must not also be asked for a line end.
    pub(super) fn previous_kind(&self) -> TokenKind {
        let at = if self.pos == 0 { 0 } else { self.pos - 1 };
        self.tokens[at].kind
    }

    /// The span of the token just consumed — how a node that ends in a
    /// closer (`)`, `]`) states its own extent without carrying the closer
    /// around.
    pub(super) fn previous_span(&self) -> Span {
        let at = if self.pos == 0 { 0 } else { self.pos - 1 };
        self.tokens[at].span
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
