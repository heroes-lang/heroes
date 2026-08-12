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
//! | `escape.rs`   | escape sequences: the table, validation, decoding (panel 008) |
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

mod escape;
mod keywords;
mod layout;
mod scan;
mod token;

pub use escape::unescape;
pub use token::{kind_name, Token, TokenKind};

use crate::diagnostics::Diagnostic;
use crate::source::{Source, Span};

pub struct LexOutput {
    pub tokens: Vec<Token>,
    pub diagnostics: Vec<Diagnostic>,
}

/// Run the lexer over a compilation. Never fails: errors are in `diagnostics`,
/// and the token stream is always complete through `Eof`.
///
/// **One file at a time, and that is the repair.** This function used to walk to
/// `src.text.len()` under a doc comment saying "one source file" — true until
/// M8a made a `Source` N files concatenated, after which the indent stack and
/// the open-bracket list crossed the boundary between them. Measured: an
/// unclosed `(` in `main.hero` swallowed the whole of `geom.hero` and reported
/// itself **inside the library** at exit 2, so the commonest state a file is
/// ever in — a half-written line, `y =` — answered *the compiler is wrong*
/// (2026-08-12, sweep 001 audit L1).
///
/// Panel 007 already fixed the shape of the rule: continuation lives inside
/// brackets only, and an opener that never closes is a diagnostic rather than a
/// silent swallow. A file boundary is where that rule has to be applied, because
/// nothing in one file may continue a line in another.
pub fn lex(src: &Source) -> LexOutput {
    let mut tokens: Vec<Token> = Vec::new();
    let mut diagnostics: Vec<Diagnostic> = Vec::new();
    for index in 0..src.files().len() {
        let offset = src.files()[index].start;
        // Each file lexed as its own text, then the spans moved back into the
        // compilation's coordinates. Bounding the existing walk instead would
        // mean threading an end through every `as_bytes()` read in `scan.rs`,
        // and the interesting state — `level`, `open_brackets` — is exactly what
        // must not survive the boundary anyway.
        let one = Source::new(src.files()[index].name.clone(), src.text_of(index).to_string());
        let out = lex_one(&one);
        for mut token in out.tokens {
            if token.kind == TokenKind::Eof {
                continue;
            }
            token.span.start += offset;
            token.span.end += offset;
            tokens.push(token);
        }
        for mut diagnostic in out.diagnostics {
            diagnostic.span.start += offset;
            diagnostic.span.end += offset;
            diagnostics.push(diagnostic);
        }
    }
    // **`Eof` sits at the end of the ROOT file, not of the text.** Every parser
    // diagnostic that runs out of input anchors here, and the text ends inside
    // the appended library — so `expected_args_close` on an unclosed `(` landed
    // in a file the author cannot open and `library::misplaced` threw the whole
    // batch away as a compiler bug. Same rule as `no_entry_point`'s caret
    // (`Source::root_end`): a diagnostic about the input as a whole belongs to
    // the file the reader named.
    let at = src.root_end();
    tokens.push(Token { kind: TokenKind::Eof, span: Span { start: at, end: at } });
    LexOutput { tokens, diagnostics }
}

/// One file's tokens, in that file's own coordinates.
fn lex_one(src: &Source) -> LexOutput {
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

/// Whether a word is one the foreign-word registry rejects on sight
/// (`keywords.rs`). Public because the *spec* has to be checked against it: a
/// document that uses a word the lexer refuses is a document that briefs a model
/// wrongly, and `measure::spec` asserts it does not.
pub fn is_foreign_word(word: &str) -> bool {
    keywords::foreign_word(word).is_some()
}
