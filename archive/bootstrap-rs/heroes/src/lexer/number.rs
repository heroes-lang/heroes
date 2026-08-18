//! Which characters *are* a number: the scanner for the four bases and the
//! `_` separator (design.md §4.3, §4.15; panel 041, author ratification
//! 2026-08-12).
//!
//! What those characters *mean* is `digits.rs`, and the two share one `Base`
//! table so they cannot disagree about what a prefix is.
//!
//! Three rules shape every refusal below, and they are the same three:
//!
//! - **The test is on the characters in hand**, never on what the parser is
//!   expecting (CLAUDE.md §11, and panel 035 wrote it into the exponent refusal
//!   at the bottom of this file). A fact about the value cannot expire.
//! - **A refusal teaches.** `0xFFFF` used to die as *"expected the end of the
//!   line, found a name (`xFFFF`)"* — a message about a name, for a number.
//! - **The fallback is loud.** A character that cannot continue a based literal
//!   ends it with a diagnostic, never by silently handing `2` back to the
//!   parser as a separate token.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::source::{Source, Span};

use super::digits::Base;
use super::token::TokenKind;
use super::LexState;

/// What a run of digits turned out to be. `faulted` exists so a run that has
/// already reported one mistake does not report a second one about the same
/// characters — two diagnostics for one mistake is a defect this compiler has
/// fixed three times.
struct Run {
    digits: usize,
    faulted: bool,
}

impl LexState {
    /// Digits, in one of four bases, with `_` between any two of them; then
    /// optionally `.` digits, which only a decimal literal may have.
    ///
    /// `1.5` is one token; `1.str()` is Int Dot Ident, because the dot makes a
    /// float only when a digit follows — UFCS must keep working on numbers, and
    /// that is as true of `0xff.str()` as it was of `1.str()`.
    pub(super) fn number(&mut self, src: &Source) {
        let text = src.text.as_bytes();
        let start = self.pos;

        if text.get(self.pos) == Some(&b'0') {
            match text.get(self.pos + 1).copied() {
                // `0x` / `0o` / `0b`.
                Some(marker) if Base::from_marker(marker).is_some() => {
                    let base = Base::from_marker(marker).expect("just matched");
                    self.pos += 2;
                    return self.based(src, base, start);
                }
                // `0X` / `0O` / `0B`. Refused rather than accepted, and this is
                // the one rule here that no other language has: §4.15 wants one
                // spelling, and of the three uppercase markers `0O` is genuinely
                // misreadable as `00`. The digits themselves stay case-blind —
                // a mask copied out of a C header arrives as `0xFF`, and
                // refusing *that* would defeat the reason the notation exists.
                Some(marker) if Base::from_marker(marker.to_ascii_lowercase()).is_some() => {
                    let base = Base::from_marker(marker.to_ascii_lowercase()).expect("just matched");
                    self.pos += 2;
                    let run = self.digit_run(src, base);
                    let span = Span { start: start as u32, end: self.pos as u32 };
                    let lowered = format!("{}{}", base.prefix(), &src.text[start + 2..self.pos]);
                    self.error_token(
                        Diagnostic::new(
                            "base_prefix_case",
                            format!(
                                "`{}` writes the base in uppercase — this language spells it `{}`",
                                src.slice(span),
                                base.prefix()
                            ),
                            span,
                        )
                        .with_note(format!(
                            "there is exactly one way to write any program (§4.15). The {} digits themselves may be either case, so `{lowered}` is the same value written correctly",
                            base.name()
                        ))
                        .with_fix(Fix {
                            title: format!("write the prefix as `{}`", base.prefix()),
                            replacement: lowered,
                            span,
                            certainty: Certainty::Certain,
                        }),
                    );
                    let _ = run;
                    return;
                }
                _ => {}
            }
        }

        let run = self.digit_run(src, Base::Decimal);
        let mut kind = TokenKind::Int;

        // A leading zero is **not** octal here, and the whole reason this
        // diagnostic exists is that it is octal almost everywhere else. Measured
        // before the rule was written: `print(0700)` printed `700`, so a model
        // writing the most-copied constant form in Unix wrote a program that
        // compiled, ran, and tested green against itself (panel 041 § D1). The
        // reading is genuinely ambiguous, so **neither fix is `certain`** — the
        // note names both and the author picks.
        let leading_zero = run.digits > 1
            && !run.faulted
            && src.text.as_bytes().get(start) == Some(&b'0')
            && text.get(self.pos) != Some(&b'.');
        if leading_zero {
            let span = Span { start: start as u32, end: self.pos as u32 };
            let written = src.slice(span).to_string();
            let stripped = written.trim_start_matches('0');
            let stripped = if stripped.is_empty() { "0" } else { stripped };
            self.error_token(
                Diagnostic::new(
                    "leading_zero",
                    format!("`{written}` starts with a zero, and this language reads no meaning into that"),
                    span,
                )
                .with_note(format!(
                    "C, and every language that copied it, reads a leading zero as octal — so `{written}` is a number a reader will size wrong. Write `0o{stripped}` for the octal value, or `{stripped}` for the decimal one"
                ))
                .with_fix(Fix {
                    title: format!("octal: `0o{stripped}`"),
                    replacement: format!("0o{stripped}"),
                    span,
                    certainty: Certainty::Guess,
                })
                .with_fix(Fix {
                    title: format!("decimal: `{stripped}`"),
                    replacement: stripped.to_string(),
                    span,
                    certainty: Certainty::Guess,
                }),
            );
            return;
        }

        if text.get(self.pos) == Some(&b'.')
            && text.get(self.pos + 1).is_some_and(|b| b.is_ascii_digit())
        {
            self.pos += 1;
            self.digit_run(src, Base::Decimal);
            kind = TokenKind::Float;
        }

        // **An exponent is refused, and it is refused where it is written**
        // (panel 035). `1e300` used to lex as `1` then the identifier `e300`, so
        // the reader was told `expected ')' … found a name (e300)` — a message
        // about a parenthesis, for a number. Panel 035 refused exponent literals;
        // this is the other half of refusing them, and CLAUDE.md §8's standard is
        // that the error carries what is needed without opening another file.
        //
        // The test is on the characters in hand — a digit, then `e`/`E`, then an
        // optional sign, then a digit — and never on what the parser is expecting.
        let exponent = matches!(text.get(self.pos), Some(b'e') | Some(b'E'))
            && match text.get(self.pos + 1) {
                Some(b'+') | Some(b'-') => text.get(self.pos + 2).is_some_and(|b| b.is_ascii_digit()),
                Some(b) => b.is_ascii_digit(),
                None => false,
            };
        if exponent {
            let digits = &src.text[start..self.pos];
            self.pos += 1;
            if matches!(text.get(self.pos), Some(b'+') | Some(b'-')) {
                self.pos += 1;
            }
            while text.get(self.pos).is_some_and(|b| b.is_ascii_digit()) {
                self.pos += 1;
            }
            let span = Span { start: start as u32, end: self.pos as u32 };
            let written = &src.text[start..self.pos];
            self.error_token(Diagnostic::new(
                "exponent_literal",
                format!(
                    "`{written}` is not a number in this language — there are no exponents. Write the digits out, or compute it: `{digits} * pow(base: 10.0, exponent: …)` through the FFI (§4.19)"
                ),
                span,
            ));
            return;
        }

        if run.faulted {
            self.error_token_at(start);
            return;
        }
        self.push(kind, start);
    }

    /// The body of a `0x` / `0o` / `0b` literal: the prefix is already consumed.
    fn based(&mut self, src: &Source, base: Base, start: usize) {
        let run = self.digit_run(src, base);
        let span = Span { start: start as u32, end: self.pos as u32 };
        if run.digits == 0 {
            if !run.faulted {
                self.error_token(
                    Diagnostic::new(
                        "empty_base_literal",
                        format!("`{}` names a base and then stops — a number needs digits", src.slice(span)),
                        span,
                    )
                    .with_note(format!(
                        "{} literal is written `{}` followed by {}",
                        base.a_name(),
                        base.prefix(),
                        base.digits()
                    )),
                );
            } else {
                self.error_token_at(start);
            }
            return;
        }
        if run.faulted {
            self.error_token_at(start);
            return;
        }
        self.push(TokenKind::Int, start);
    }

    /// Consume digits of `base`, allowing `_` **between two digits** and nothing
    /// else. Advances past everything it consumed, including the characters it
    /// refused, so lexing continues past a bad literal (this module's contract).
    ///
    /// A character that is alphanumeric but not a digit of `base` is a
    /// diagnostic **only for a prefixed base**. In decimal it must fall through
    /// untouched, because `1e300` belongs to the exponent refusal above and
    /// `1.str()` belongs to UFCS — a `digit_not_in_base` on `e` would take a
    /// message panel 035 designed and replace it with a worse one.
    fn digit_run(&mut self, src: &Source, base: Base) -> Run {
        let text = src.text.as_bytes();
        let mut run = Run { digits: 0, faulted: false };
        loop {
            match text.get(self.pos).copied() {
                Some(b) if base.admits(b) => {
                    self.pos += 1;
                    run.digits += 1;
                }
                Some(b'_') => {
                    let after_digit = run.digits > 0
                        && text.get(self.pos - 1).is_some_and(|&b| base.admits(b));
                    let before_digit = text.get(self.pos + 1).is_some_and(|&b| base.admits(b));
                    if after_digit && before_digit {
                        self.pos += 1;
                        continue;
                    }
                    let start = self.pos;
                    while text.get(self.pos) == Some(&b'_') {
                        self.pos += 1;
                    }
                    let span = Span { start: start as u32, end: self.pos as u32 };
                    if !run.faulted {
                        self.diagnostics.push(
                            Diagnostic::new(
                                "misplaced_separator",
                                "`_` groups digits, so it goes between two of them".to_string(),
                                span,
                            )
                            .with_note(
                                "`1_000_000` and `0xff_ff` are the shape; a separator that leads, trails or doubles is a typo rather than a grouping".to_string(),
                            ),
                        );
                    }
                    run.faulted = true;
                }
                Some(b) if base != Base::Decimal && b.is_ascii_alphanumeric() => {
                    let start = self.pos;
                    while text.get(self.pos).is_some_and(|b| b.is_ascii_alphanumeric() || *b == b'_') {
                        self.pos += 1;
                    }
                    let span = Span { start: start as u32, end: self.pos as u32 };
                    if !run.faulted {
                        self.diagnostics.push(
                            Diagnostic::new(
                                "digit_not_in_base",
                                format!(
                                    "`{}` is not {} digit",
                                    src.slice(span),
                                    base.a_name()
                                ),
                                span,
                            )
                            .with_note(format!(
                                "{} literal admits {}",
                                base.a_name(),
                                base.digits()
                            )),
                        );
                    }
                    run.faulted = true;
                }
                _ => return run,
            }
        }
    }
}

