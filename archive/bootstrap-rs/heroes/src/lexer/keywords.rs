//! The two word tables: Heroes' own keywords, and the foreign words that
//! fail loudly with the repair pre-written.
//!
//! `keyword` is the closed list from spec/reserved-words.md § keywords —
//! these can never be identifiers. `foreign_word` is the registry's other
//! half and the language's first executable thesis feature: the likeliest
//! mistake from a model on autopilot is a keyword imported from another
//! language, so each one is a compile error carrying its own solution.

use super::token::TokenKind;

pub(super) fn keyword(text: &str) -> Option<TokenKind> {
    Some(match text {
        "constant" => TokenKind::KwConstant,
        "function" => TokenKind::KwFunction,
        "record" => TokenKind::KwRecord,
        "variant" => TokenKind::KwVariant,
        "match" => TokenKind::KwMatch,
        "if" => TokenKind::KwIf,
        "else" => TokenKind::KwElse,
        "for" => TokenKind::KwFor,
        "while" => TokenKind::KwWhile,
        "in" => TokenKind::KwIn,
        "break" => TokenKind::KwBreak,
        "continue" => TokenKind::KwContinue,
        "return" => TokenKind::KwReturn,
        "test" => TokenKind::KwTest,
        "assert" => TokenKind::KwAssert,
        "extern" => TokenKind::KwExtern,
        // §4.19: `ptr`'s only literal. A keyword rather than a built-in so that
        // it is a *value* a reader can write, not a call — and so the
        // foreign-word registry's answer for `null` can point at it.
        "nullptr" => TokenKind::KwNullPtr,
        // Panel 031. Until M-module-namespace this word was in `foreign_word` below, refused
        // with "modules do not exist yet"; the two tables are the reason the
        // change is one line in each.
        "use" => TokenKind::KwUse,
        "true" => TokenKind::KwTrue,
        "false" => TokenKind::KwFalse,
        "fail" => TokenKind::KwFail,
        _ => return None,
    })
}

/// Returns the prescribed message (spec/reserved-words.md — the registry is
/// the single source; the golden tests in check/ will pin the exact text)
/// and, where the repair is a pure word-for-word swap, the replacement to
/// attach as a `Certain`, machine-applicable fix (design.md §4.17).
/// Guidance-only cases (`let`, `null`, `try`…) carry no fix: guessing a
/// repair a model would blindly apply is worse than explaining.
pub(crate) fn foreign_word(text: &str) -> Option<(&'static str, Option<&'static str>)> {
    Some(match text {
        // `i64` is a foreign word now, and it is the one in this table a model is
        // likeliest to write. The author deleted it 2026-08-12 in favour of `i64`
        // — no alias, one spelling — on the ground that `i64` carries forty years
        // of conflicting widths (16 bits, 32, 64, arbitrary) while `i64` is
        // ambiguous to no reader. The repair is a pure word-for-word swap, so it
        // is `Certain` and machine-applicable, and `heroes check --in-place`
        // migrates a whole file.
        "int" => ("`int` is not a word in this language — an integer says its width: `i64`, and also `i8` `i16` `i32` `u8` `u16` `u32` `u64`", Some("i64")),
        "struct" => ("`struct` is not a word in this language — use `record`: `record Point`", Some("record")),
        "enum" => ("`enum` is not a word in this language — use `variant`: `variant Token`", Some("variant")),
        "union" => ("`union` is not a word in this language — use `variant`: `variant Token`", Some("variant")),
        "class" => ("`class` is not a word in this language — use `record` (there is no inheritance)", None),
        "fn" => ("`fn` is not a word in this language — use `function`: `function f(x: i64) -> i64`", Some("function")),
        "func" => ("`func` is not a word in this language — use `function`", Some("function")),
        "def" => ("`def` is not a word in this language — use `function`", Some("function")),
        "let" => ("`let` is not a word in this language — bind with `=`: `x = 5`", None),
        "var" => ("`var` is not a word in this language — declare a mutable with `@`: `v: i64 @ 0`", None),
        "const" => ("`const` is not a word in this language — use `constant`: `constant MAX: i64`", Some("constant")),
        "elif" => ("`elif` is not a word in this language — write `else if`", Some("else if")),
        "switch" => ("`switch` is not a word in this language — use `match`", Some("match")),
        "case" => ("`case` is not a word in this language — a `match` arm is `.name => expr`", None),
        // The FFI's `nullptr` is named here rather than left to be discovered,
        // because this message is otherwise *wrong advice at a `ptr` site*: a
        // binding author writing `db: ptr @ null` would be told to use `int?`,
        // which no C out-parameter can be (panel 036 rider 7, amended when the
        // spec gate found the collision).
        "null" | "nil" | "None" => ("there is no null in this language — absence is a fallible type: `i64?`, and a C pointer's zero is `nullptr`", None),
        "try" | "catch" | "throw" | "raise" => ("there are no exceptions in this language — errors are values: `fail(code, msg)`, propagate with `?`", None),
        // `use` left this table at panel 031 and became a keyword. Its two
        // neighbours stay, and they gain what they never had: the repair is now
        // a pure word-for-word swap, so it is `Certain` and machine-applicable.
        "import" | "include" => ("`import` and `include` are not words in this language — a module is named with `use`: `use geom`", Some("use")),
        _ => return None,
    })
}
