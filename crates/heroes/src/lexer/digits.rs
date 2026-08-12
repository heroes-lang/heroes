//! What a literal's digits *mean*: the four bases, the one decoder, and the
//! two things a reader is owed about a number (design.md §4.3, §4.15; panel
//! 041, author ratification 2026-08-12).
//!
//! **There is exactly one decoder, and this is it.** Before panel 041 there were
//! two — `types/exprs.rs` and `ir/exprs.rs` each called `parse::<i64>()` on the
//! source slice, with the out-of-range sentence spelled out separately in each.
//! With one base that drift cost a duplicated message. With four it would cost a
//! **wrong value**: the frontend and the lowering could disagree about what
//! `0x10` is and nothing in the compiler would say so.
//!
//! The scanner next door reads the same `Base` table this decoder reads, which
//! is the property that matters rather than the file boundary: the two cannot
//! disagree about what a prefix means, because there is one place that says.

use crate::diagnostics::Diagnostic;
use crate::source::Span;

/// The four bases a literal may be written in. `i64` is one type; this is only
/// how its digits are spelled, which is why there is no `Base` anywhere outside
/// the lexer.
#[derive(Clone, Copy, PartialEq, Eq)]
pub(super) enum Base {
    Binary,
    Octal,
    Decimal,
    Hexadecimal,
}

impl Base {
    /// The lowercase marker that introduces this base, if it has one.
    pub(super) fn from_marker(marker: u8) -> Option<Base> {
        match marker {
            b'b' => Some(Base::Binary),
            b'o' => Some(Base::Octal),
            b'x' => Some(Base::Hexadecimal),
            _ => None,
        }
    }

    fn radix(self) -> u32 {
        match self {
            Base::Binary => 2,
            Base::Octal => 8,
            Base::Decimal => 10,
            Base::Hexadecimal => 16,
        }
    }

    pub(super) fn prefix(self) -> &'static str {
        match self {
            Base::Binary => "0b",
            Base::Octal => "0o",
            Base::Decimal => "",
            Base::Hexadecimal => "0x",
        }
    }

    pub(super) fn name(self) -> &'static str {
        match self {
            Base::Binary => "binary",
            Base::Octal => "octal",
            Base::Decimal => "decimal",
            Base::Hexadecimal => "hexadecimal",
        }
    }

    /// The name with its article, because "a octal digit" is the kind of thing
    /// a reader trips over in a message whose whole job is to be read.
    pub(super) fn a_name(self) -> &'static str {
        match self {
            Base::Binary => "a binary",
            Base::Octal => "an octal",
            Base::Decimal => "a decimal",
            Base::Hexadecimal => "a hexadecimal",
        }
    }

    /// The digits this base admits, written out for the diagnostic rather than
    /// derived, because the reader needs to see them.
    pub(super) fn digits(self) -> &'static str {
        match self {
            Base::Binary => "0 and 1",
            Base::Octal => "0 through 7",
            Base::Decimal => "0 through 9",
            Base::Hexadecimal => "0 through 9 and a through f",
        }
    }

    pub(super) fn admits(self, b: u8) -> bool {
        (b as char).is_digit(self.radix())
    }
}

/// The value of an `i64` literal, or `None` if the digits do not fit one.
///
/// **This is the only place that reads a literal's digits.** The frontend calls
/// it to decide whether to report `int_out_of_range`, and the lowering calls it
/// to get the number — so the two can no longer disagree about what `0x10` is.
///
/// The reading is by **value**, not by bit pattern: `0xffffffffffffffff` does
/// not fit an `i64` and is refused, exactly as its decimal twin
/// `18446744073709551615` is. Panel 041 vetoed the bit-pattern reading, on the
/// ground that it deletes a compile error — sixteen `f`s would silently become
/// `-1` and `0xFFFFFFFF00000000` would silently become `-4294967296`.
pub(crate) fn decode_int(text: &str) -> Option<i64> {
    let (base, digits) = split_base(text);
    let digits: String = digits.chars().filter(|c| *c != '_').collect();
    i64::from_str_radix(&digits, base.radix()).ok()
}

/// `int_out_of_range`, built where the decoder lives — the frontend raises it
/// (so `heroes check` and `heroes build` agree about what has diagnostics) and
/// the lowering keeps its own net, and before panel 041 the two spelled the
/// sentence out separately.
///
/// The note answers **in the notation the question was asked in**: a reader who
/// wrote `0xffffffffffffffff` is told the largest `i64` is `0x7fffffffffffffff`,
/// because being handed a decimal boundary for a hexadecimal mistake is the
/// second half of the same error (CLAUDE.md §8 — everything needed to fix the
/// program without opening another file).
pub(crate) fn int_out_of_range(text: &str, span: Span) -> Diagnostic {
    let (base, _) = split_base(text);
    let largest = match base {
        Base::Decimal => i64::MAX.to_string(),
        Base::Binary => format!("0b{:b}", i64::MAX),
        Base::Octal => format!("0o{:o}", i64::MAX),
        Base::Hexadecimal => format!("0x{:x}", i64::MAX),
    };
    let note = if base == Base::Decimal {
        "`i64` is a 64-bit signed integer and the only integer type, so it holds -9223372036854775808 through 9223372036854775807".to_string()
    } else {
        format!(
            "`i64` is a 64-bit signed integer and the only integer type, so the largest one {} literal can write is `{largest}` (9223372036854775807)",
            base.a_name()
        )
    };
    Diagnostic::new("int_out_of_range", format!("`{text}` does not fit in an `i64`"), span)
        .with_note(note)
}

fn split_base(text: &str) -> (Base, &str) {
    let lowered = text.as_bytes();
    if lowered.len() > 2 && lowered[0] == b'0' {
        if let Some(base) = Base::from_marker(lowered[1].to_ascii_lowercase()) {
            return (base, &text[2..]);
        }
    }
    (Base::Decimal, text)
}

/// The canonical spelling of an `i64` literal: what the author wrote, with the
/// digits lowercased.
///
/// A C header spells a mask `0xFF` and a reader copies it as written, so the
/// lexer accepts either case — refusing uppercase *digits* would defeat the
/// reason the notation exists at all. §4.15 still wants exactly one spelling of
/// any program, so `fmt` is where the two converge. That division is the only
/// precedent there is: no language's lexer enforces hex digit case, and the one
/// tool that canonicalises it is a formatter (rustfmt's `hex_literal_case`;
/// panel 041, historian, a sourced negative result).
///
/// The prefix needs no work — `0X` never reaches here, because the lexer
/// refuses it where it is written.
pub(crate) fn canonical_int(text: &str) -> String {
    text.to_ascii_lowercase()
}
