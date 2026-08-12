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

    /// The current token's span, **unless it is the library's** — in which case
    /// the declaration this diagnostic is about.
    ///
    /// Three declarations report a missing body "at the NEXT declaration's
    /// line", which reads well and locates the mistake exactly: the body should
    /// have been between them. But the library is appended to every `Source`
    /// (§1.11, `crate::library`), so a file ENDING in a bodyless declaration
    /// would point at a line the author cannot open — and the pipeline would
    /// then refuse the whole compilation as a compiler bug, which it is not.
    /// Falling back to the declaration's own keyword keeps the message
    /// actionable at the one place the nice form stops working.
    pub(super) fn here_or(&self, src: &Source, fallback: Span) -> Span {
        let span = self.span();
        if src.is_library(span.start) {
            return fallback;
        }
        span
    }

    pub(super) fn at(&self, kind: TokenKind) -> bool {
        self.kind() == kind
    }

    /// The kind `ahead` significant tokens on. Comments are skipped here too,
    /// so lookahead sees what the grammar sees. Two places need it: telling
    /// `name: value` from a bare argument, and telling `for x in xs` from a
    /// condition mistakenly written after `for` (the `while` repair).
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

    /// The last span a **reader can see** — walking back over the tokens that
    /// are layout or annotation rather than program text.
    ///
    /// A statement's own extent is built from this, and the extent is what
    /// §4.17's caret underlines. Built from `previous_span` it reached the
    /// line's `Terminator`, whose position is past any trailing comment: `f()
    /// # a note` was underlined twenty-three columns wide for a three-column
    /// statement, so the part of the message that says *where* pointed at
    /// something the author cannot fix (2026-08-12, carried open since M5a).
    pub(super) fn previous_significant_span(&self) -> Span {
        let mut at = if self.pos == 0 { 0 } else { self.pos - 1 };
        while at > 0
            && matches!(
                self.tokens[at].kind,
                TokenKind::Comment
                    | TokenKind::Terminator
                    | TokenKind::Indent
                    | TokenKind::Dedent
            )
        {
            at -= 1;
        }
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

    /// How many diagnostics have been reported so far. The statement parser
    /// compares it before and after: a statement that produced one has already
    /// said what is wrong, wherever inside itself the mistake was.
    pub(super) fn diagnostic_count(&self) -> usize {
        self.diagnostics.len()
    }

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
