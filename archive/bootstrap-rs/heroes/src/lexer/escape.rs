//! Escape sequences — the table, the validator, the decoder (panel 008, `\r`
//! added by panel 066).
//!
//! Six escapes, **split by context** (Go's rule), so every character has
//! exactly one spelling and §4.15's "exactly one correct way" survives:
//!
//! | in `"…"` | in `'…'` | value |
//! |----------|----------|-------|
//! | `\n`     | `\n`     | 10    |
//! | `\t`     | `\t`     | 9     |
//! | `\r`     | `\r`     | 13    |
//! | `\\`     | `\\`     | 92    |
//! | `\"`     | —        | 34 — a `"` needs no escape inside a char literal |
//! | —        | `\'`     | 39 — a `'` needs no escape inside a string |
//!
//! **`\r` is why `raw_carriage_return` exists** (panel 066). One spelling per
//! character cuts both ways: the escape gave byte 13 a spelling, and the same
//! sitting found that a raw CR pasted into a literal compiled silently, survived
//! `fmt`, and rendered invisibly — so retyping the visible text dropped the byte
//! and still compiled. That is the thesis inverted, and it is a diagnostic here
//! rather than in `literals.rs` because this file owns what a literal's bytes
//! may be.
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
            Quoted::Str => r#"\n \t \r \\ \""#,
            Quoted::Char => r"\n \t \r \\ \'",
        }
    }

    /// How the diagnostic names this context, for a message that has to read.
    fn word(self) -> &'static str {
        match self {
            Quoted::Str => "a string",
            Quoted::Char => "a character literal",
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
        'r' => Some(b'\r'),
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

    /// A raw carriage return inside a literal — refused, with `\r` as the repair
    /// (panel 066, the coherence condition).
    ///
    /// The escape is what makes this diagnostic possible: before it, byte 13 had
    /// **no** spelling, so refusing the raw byte would have left the character
    /// unwritable. Now there is exactly one spelling, and the second — invisible
    /// — one is what §4.15 forbids anywhere else in the language.
    ///
    /// Why it is worth a diagnostic at all: measured on the compiler before this
    /// landed, a raw CR pasted mid-literal compiled at exit 0, survived
    /// `heroes fmt` byte-for-byte, and rendered identically to a literal without
    /// it — so retyping what the screen shows silently changed the program's
    /// bytes and still compiled. A plausible mistake that was not a compile
    /// error.
    ///
    /// Scanning is unaffected: `self.pos` is left where it was, the caller
    /// consumes the byte as ordinary content, and the literal keeps its kind —
    /// one mistake, one diagnostic, and the rest of the line still lexes.
    pub(super) fn raw_carriage_return(&mut self, at: usize, ctx: Quoted) {
        let span = Span { start: at as u32, end: at as u32 + 1 };
        let mut diag = Diagnostic::new(
            "raw_carriage_return",
            format!(
                "a carriage return written as itself inside {} — it is invisible on screen, so the next reader deletes it by retyping the line",
                ctx.word()
            ),
            span,
        );
        diag.fixes.push(Fix {
            title: "write it as `\\r`".to_string(),
            replacement: "\\r".to_string(),
            span,
            certainty: Certainty::Certain,
        });
        self.diagnostics.push(diag);
    }
}
