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
        "in" => TokenKind::KwIn,
        "break" => TokenKind::KwBreak,
        "continue" => TokenKind::KwContinue,
        "return" => TokenKind::KwReturn,
        "test" => TokenKind::KwTest,
        "assert" => TokenKind::KwAssert,
        "extern" => TokenKind::KwExtern,
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
        "struct" => ("`struct` is not a word in this language — use `record`: `Point = record`", Some("record")),
        "enum" => ("`enum` is not a word in this language — use `variant`: `Token = variant`", Some("variant")),
        "union" => ("`union` is not a word in this language — use `variant`: `Token = variant`", Some("variant")),
        "class" => ("`class` is not a word in this language — use `record` (there is no inheritance)", None),
        "fn" => ("`fn` is not a word in this language — use `function`: `f = function: (x: int) -> int`", Some("function")),
        "func" => ("`func` is not a word in this language — use `function`", Some("function")),
        "def" => ("`def` is not a word in this language — use `function`", Some("function")),
        "let" => ("`let` is not a word in this language — bind with `=`: `x = 5`", None),
        "var" => ("`var` is not a word in this language — declare a mutable with `@`: `v: int @ 0`", None),
        "const" => ("`const` is not a word in this language — use `constant`: `MAX = constant: int`", Some("constant")),
        "while" => ("`while` is not a word in this language — use `for`: `for x > 0`", Some("for")),
        "elif" => ("`elif` is not a word in this language — write `else if`", Some("else if")),
        "switch" => ("`switch` is not a word in this language — use `match`", Some("match")),
        "case" => ("`case` is not a word in this language — a `match` arm is `.name => expr`", None),
        "null" | "nil" | "None" => ("there is no null in this language — absence is a fallible type: `int?`", None),
        "try" | "catch" | "throw" | "raise" => ("there are no exceptions in this language — errors are values: `fail(code, msg)`, propagate with `?`", None),
        "import" | "use" | "include" => ("modules do not exist yet — one file is one program (v1)", None),
        _ => return None,
    })
}
