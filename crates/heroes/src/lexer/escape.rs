//! Escape sequences — the table, the validator, the decoder (panel 008).
//!
//! Five escapes, **split by context** (Go's rule), so every character has
//! exactly one spelling and §4.15's "exactly one correct way" survives:
//!
//! | in `"…"` | in `'…'` | value |
//! |----------|----------|-------|
//! | `\n`     | `\n`     | 10    |
//! | `\t`     | `\t`     | 9     |
//! | `\\`     | `\\`     | 92    |
//! | `\"`     | —        | 34 — a `"` needs no escape inside a char literal |
//! | —        | `\'`     | 39 — a `'` needs no escape inside a string |
//!
//! The backslash is **reserved**: any other character after it is a compile
//! error carrying a `certain` fix. That half is what turns `"C:\temp"` and
//! `"\d+"` from silent bytes into diagnostics — the settled modern position
//! (Go, Rust and Zig all error from day one; C's permissiveness is what
//! Python has been unwinding since 2016).
//!
//! The set is **frozen**: `\0`, `\xNN`, `\u{…}` and octal escapes reconvene
//! the panel — they can produce an interior NUL, which silently truncates
//! every C call and voids §4.20's free `.cstr()`.
//!
//! Validation happens while lexing; **decoding happens on demand** through
//! `unescape`, a pure function of (source, span). That keeps `Token` `Copy`
//! and adds no lexer state — the alternatives (a side-table, or an owned
//! `String` per token) both cost more and port worse.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::source::{Source, Span};

use super::LexState;

/// Which literal the escape appears in. The legal set differs between them.
#[derive(Clone, Copy, PartialEq, Eq)]
pub(super) enum Quoted {
    Str,
    Char,
}

impl Quoted {
    /// The escapes legal here, for the diagnostic that lists them.
    fn legal(self) -> &'static str {
        match self {
            Quoted::Str => r#"\n \t \\ \""#,
            Quoted::Char => r"\n \t \\ \'",
        }
    }

    /// The character that needs no escape here because it is not the
    /// delimiter — the other context's escape, which we must reject.
    fn bare_here(self) -> char {
        match self {
            Quoted::Str => '\'',
            Quoted::Char => '"',
        }
    }
}

/// The one table. Both the validator and the decoder read it, so they
/// cannot drift apart.
fn escape_value(after_backslash: char, ctx: Quoted) -> Option<u8> {
    match after_backslash {
        'n' => Some(b'\n'),
        't' => Some(b'\t'),
        '\\' => Some(b'\\'),
        '"' if ctx == Quoted::Str => Some(b'"'),
        '\'' if ctx == Quoted::Char => Some(b'\''),
        _ => None,
    }
}

/// Decode a `Str` or `Char` token into the bytes it denotes. `span` is the
/// token's own span, delimiters included.
///
/// Total by construction: an invalid escape (already reported by the lexer)
/// decodes to the backslash itself, so a broken literal never panics a
/// later pass.
pub fn unescape(src: &Source, span: Span) -> String {
    let text = src.slice(span);
    let ctx = if text.starts_with('\'') { Quoted::Char } else { Quoted::Str };
    let inner = &text[1..text.len().saturating_sub(1)];
    let mut out = String::with_capacity(inner.len());
    let mut chars = inner.chars();
    while let Some(c) = chars.next() {
        if c != '\\' {
            out.push(c);
            continue;
        }
        let mut rest = chars.clone();
        match rest.next().and_then(|n| escape_value(n, ctx)) {
            Some(value) => {
                chars = rest;
                out.push(value as char);
            }
            None => out.push('\\'),
        }
    }
    out
}

impl LexState {
    /// Validate one escape sequence, `self.pos` sitting on the backslash,
    /// and advance past it.
    ///
    /// Returns `false` when the backslash is the last thing on the line —
    /// the caller then reports the literal unterminated. `self.pos` is left
    /// ON the newline in that case: consuming it would swallow the line
    /// break and destroy every Indent/Dedent that follows (§4.15).
    pub(super) fn escape(&mut self, src: &Source, ctx: Quoted) -> bool {
        let backslash = self.pos;
        let Some(escaped) = src.text[backslash + 1..].chars().next() else {
            self.pos = backslash + 1; // EOF
            return false;
        };
        if escaped == '\n' {
            self.pos = backslash + 1; // stay on the newline
            return false;
        }
        self.pos = backslash + 1 + escaped.len_utf8();
        if escape_value(escaped, ctx).is_some() {
            return true;
        }
        let span = Span { start: backslash as u32, end: self.pos as u32 };
        // Two different mistakes, two different repairs — both certain.
        let diag = if escaped == ctx.bare_here() {
            let mut d = Diagnostic::new(
                "escape_not_needed",
                format!(
                    "`\\{escaped}` is not an escape here — `{escaped}` needs no escape inside {}, write it bare",
                    match ctx {
                        Quoted::Str => "a string",
                        Quoted::Char => "a character literal",
                    }
                ),
                span,
            );
            d.fixes.push(Fix {
                title: format!("drop the backslash: `{escaped}`"),
                replacement: escaped.to_string(),
                span,
                certainty: Certainty::Certain,
            });
            d
        } else {
            let mut d = Diagnostic::new(
                "unknown_escape",
                format!(
                    "`\\{escaped}` is not an escape sequence — the escapes are {}; write `\\\\` for a literal backslash",
                    ctx.legal()
                ),
                span,
            );
            d.fixes.push(Fix {
                title: format!("escape the backslash: `\\\\{escaped}`"),
                replacement: format!("\\\\{escaped}"),
                span,
                certainty: Certainty::Certain,
            });
            d
        };
        self.diagnostics.push(diag);
        // The token keeps its kind: poisoning it to `Error` would delete the
        // line's Terminator (Error is not an ender, panel 007) and turn one
        // mistake into two.
        true
    }
}
