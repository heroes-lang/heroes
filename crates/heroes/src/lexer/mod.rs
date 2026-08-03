//! The lexer: source text → tokens (design.md §4.15).
//!
//! First station of the assembly line. It cuts the text into words and,
//! because Heroes has no braces and no semicolons, it also mints the two
//! invisible currencies structure travels in: `Indent`/`Dedent` (the
//! margin) and `Terminator` (the line end that *can* end a statement).
//!
//! The module is split by concern — each file is one idea:
//!
//! | file          | idea |
//! |---------------|------|
//! | `token.rs`    | the vocabulary: `TokenKind`, `Token`, `kind_name` |
//! | `keywords.rs` | the word tables: Heroes keywords, foreign words with prescribed errors |
//! | `layout.rs`   | shape: rigid indentation, terminator insertion (panel 007) |
//! | `scan.rs`     | words: comments, literals, identifiers, punctuation |
//! | `tests/`      | snapshot tests, grouped the same way |
//!
//! Two invariants hold everywhere:
//!
//! - **The lexer never stops.** Errors become a diagnostic (+ an `Error`
//!   token when something was skipped) and lexing continues — one bad
//!   literal must not hide every error after it.
//! - **The Cyclone rule.** `LexState` owns everything it carries; the
//!   source text enters every function as a parameter and never lives in
//!   a struct field. Tokens point into the source by byte offset.

#[cfg(test)]
mod tests;

mod keywords;
mod layout;
mod scan;
mod token;

pub use token::{kind_name, Token, TokenKind};

use crate::diagnostics::Diagnostic;
use crate::source::{Source, Span};

pub struct LexOutput {
    pub tokens: Vec<Token>,
    pub diagnostics: Vec<Diagnostic>,
}

/// Run the lexer over one source file. Never fails: errors are in
/// `diagnostics`, and the token stream is always complete through `Eof`.
pub fn lex(src: &Source) -> LexOutput {
    let mut st = LexState {
        pos: 0,
        level: 0,
        open_brackets: Vec::new(),
        tokens: Vec::new(),
        diagnostics: Vec::new(),
        last_significant: None,
    };
    loop {
        st.line_start(src); // shape: margin → Indent/Dedent (layout.rs)
        if st.pos >= src.text.len() {
            break;
        }
        st.line_body(src); // words: one physical line of tokens (scan.rs)
    }
    // EOF: terminate the last line if the file ended without a newline,
    // report every opener that never closed (panel 007: otherwise one
    // missing `)` silently swallows the rest of the file's layout), then
    // close every block still open.
    st.maybe_terminator();
    for span in &st.open_brackets {
        st.diagnostics.push(Diagnostic::new(
            "unclosed_bracket",
            format!("`{}` opened here is never closed", src.slice(*span)),
            *span,
        ));
    }
    let end = Span { start: st.pos as u32, end: st.pos as u32 };
    for _ in 0..st.level {
        st.tokens.push(Token { kind: TokenKind::Dedent, span: end });
    }
    st.tokens.push(Token { kind: TokenKind::Eof, span: end });
    LexOutput { tokens: st.tokens, diagnostics: st.diagnostics }
}

/// Owned lexing state — position, layout, output. The source text is a
/// parameter everywhere (the Cyclone rule), so this is all the state
/// there is.
struct LexState {
    pos: usize,
    /// Current indentation level (one level == 4 spaces).
    level: u32,
    /// Spans of the openers `( [ {` not yet closed; its length is the
    /// bracket depth. Inside brackets, indentation is not structural
    /// (panel 007).
    open_brackets: Vec<Span>,
    tokens: Vec<Token>,
    diagnostics: Vec<Diagnostic>,
    /// Kind of the current line's last significant (non-comment) token —
    /// the one datum terminator insertion needs (layout.rs).
    last_significant: Option<TokenKind>,
}

impl LexState {
    /// Emit one token and remember it for terminator insertion. Bracket
    /// depth is tracked here, where the kind is known — so brackets inside
    /// string literals can never touch it.
    fn push(&mut self, kind: TokenKind, start: usize) {
        let span = Span { start: start as u32, end: self.pos as u32 };
        match kind {
            TokenKind::LParen | TokenKind::LBracket | TokenKind::LBrace => {
                self.open_brackets.push(span);
            }
            TokenKind::RParen | TokenKind::RBracket | TokenKind::RBrace => {
                // Saturating pop: a stray closer never corrupts the
                // following layout; the parser reports it.
                self.open_brackets.pop();
            }
            _ => {}
        }
        self.tokens.push(Token { kind, span });
        self.last_significant = Some(kind);
    }

    /// Error recovery in one move: record why, emit an `Error` token over
    /// the same span, keep lexing.
    fn error_token(&mut self, diag: Diagnostic) {
        let span = diag.span;
        self.diagnostics.push(diag);
        self.tokens.push(Token { kind: TokenKind::Error, span });
        self.last_significant = Some(TokenKind::Error);
    }
}
