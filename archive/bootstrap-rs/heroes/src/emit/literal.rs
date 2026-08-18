//! A constant value as C text: the macro its width asks for, and a float in hex
//! (design.md §3.1, §4.14; panel 020, panel 042).
//!
//! Split out of `ops.rs` by the §11 sweep. The seam is a real one: everything here
//! turns a value the compiler already knows into the characters clang will read,
//! and nothing here can abort, branch or allocate — which is exactly what the rest
//! of `ops.rs` does. One rule lives here and it is measured:
//!
//! - **an `f64` literal is written as hex**, so the decimal round trip that would
//!   otherwise sit between the lexer and clang does not exist.

use crate::ir::Const;
use crate::types::IntKind;

/// A constant, in the C spelling its own width asks for.
///
/// A bare `-9223372036854775808` warns (`-Wimplicitly-unsigned-literal`) because
/// C parses it as a negation of an out-of-range positive, and a bare decimal
/// above `INT32_MAX` is only `long` on LP64 — so a macro is both the portable and
/// the warning-free spelling, and the macro has to match the width.
///
/// **`UINT64_C` for the unsigned widths is not tidiness.** `18446744073709551615`
/// through `INT64_C` is a constant C cannot represent, and the narrower unsigned
/// widths take it too so that the emitted text says what the Heroes type says
/// rather than relying on the assignment to convert it.
pub(super) fn constant(value: Const, kind: Option<IntKind>) -> String {
    match value {
        Const::Int(n) if n == i128::from(i64::MIN) => "INT64_MIN".to_string(),
        Const::Int(n) => match kind {
            Some(k) if !k.signed() => format!("UINT64_C({n})"),
            _ => format!("INT64_C({n})"),
        },
        Const::Bool(b) => (if b { "true" } else { "false" }).to_string(),
        // `NULL` would need a header; the cast needs none and is the same value.
        Const::NullPtr => "((void *)0)".to_string(),
        // **A hex float, not a decimal one.** `%a` is round-trip-exact by
        // construction, where `%.17g` is exact only in practice — and design.md §3.1
        // has said "`f64` literals emitted round-trip-exact (`%a`)" since M-day-zero. This is
        // the *literal*; how a value **prints** is `hero_print_f64`'s question and a
        // different answer (§4.9).
        Const::Float(x) => hex_float(x),
        // A static block, laid out by clang: refcount -1 means "never freed", so a
        // literal costs no allocation and decrefing one is a no-op.
        Const::Str(id) => format!("HERO_STR_LIT(hero_str_{})", id.0),
    }
}

/// A `double` as a C11 hexadecimal floating literal. Exact, warning-free, and
/// independent of every decimal-rendering question.
fn hex_float(x: f64) -> String {
    if x.is_nan() {
        return "(0.0 / 0.0)".to_string();
    }
    if x.is_infinite() {
        return if x > 0.0 { "HUGE_VAL".to_string() } else { "(-HUGE_VAL)".to_string() };
    }
    // Rust has no `{:a}`, so the digits are produced by the same route C reads them:
    // sign, mantissa in hex, binary exponent.
    let bits = x.to_bits();
    let negative = bits >> 63 == 1;
    let exponent = ((bits >> 52) & 0x7ff) as i64;
    let mantissa = bits & 0x000f_ffff_ffff_ffff;
    let sign = if negative { "-" } else { "" };
    if exponent == 0 && mantissa == 0 {
        return format!("{sign}0x0p+0");
    }
    let (lead, unbiased) = if exponent == 0 {
        (0, -1022) // subnormal
    } else {
        (1, exponent - 1023)
    };
    // 13 hex digits hold all 52 mantissa bits exactly.
    let digits = format!("{mantissa:013x}");
    let trimmed = digits.trim_end_matches('0');
    let fraction = if trimmed.is_empty() { String::new() } else { format!(".{trimmed}") };
    format!("{sign}0x{lead}{fraction}p{}{}", if unbiased < 0 { "-" } else { "+" }, unbiased.abs())
}
