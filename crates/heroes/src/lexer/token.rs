//! The token vocabulary: every word-shape the language has, and nothing else.
//!
//! A `Token` is deliberately tiny — a kind plus a byte span into the
//! `Source`. The text is never copied out: whoever needs it slices the
//! source (indices, not references — the Cyclone rule keeps tokens `Copy`).
//! The parser consumes kinds; `heroes lex --json` and the snapshot tests
//! render them through `kind_name`.

use crate::source::Span;

#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum TokenKind {
    // --- Names and literals -------------------------------------------
    Ident,
    Int,
    Float,
    /// `"…"` — any bytes except `"` and newline, single-line. NO escape
    /// sequences exist yet: design.md is silent on them and the appendix
    /// never uses one, so the backslash is an ordinary byte. The gap
    /// ("how does a program write a tab or newline character?") is on
    /// record in docs/panel/OPEN-QUESTIONS.md for a panel before M6.
    Str,
    /// `'a'` — exactly one ASCII character; its value is an `int`
    /// (design.md §4.3: char literals exist to kill magic numbers).
    Char,

    // --- Heroes keywords (spec/reserved-words.md § keywords) ----------
    KwConstant,
    KwFunction,
    KwRecord,
    KwVariant,
    KwMatch,
    KwIf,
    KwElse,
    KwFor,
    KwIn,
    KwBreak,
    KwContinue,
    KwReturn,
    KwTest,
    KwAssert,
    KwExtern,
    KwTrue,
    KwFalse,
    KwFail,

    // --- Punctuation ---------------------------------------------------
    Eq,
    At,
    Colon,
    Comma,
    Dot,
    Question,
    /// `???` — the typed hole (design.md §4.16); an expression, not an error.
    Hole,
    /// `->` — return type arrow.
    Arrow,
    /// `=>` — match-arm arrow.
    FatArrow,
    /// `|` — match-pattern join; single `&`/`^`/`~` are NOT tokens
    /// (reserved for future bitwise use, design.md §4.14).
    Pipe,
    LParen,
    RParen,
    LBracket,
    RBracket,
    LBrace,
    RBrace,

    // --- Operators -----------------------------------------------------
    Plus,
    Minus,
    Star,
    Slash,
    Percent,
    EqEq,
    BangEq,
    Lt,
    Le,
    Gt,
    Ge,
    AndAnd,
    OrOr,
    Bang,

    // --- Trivia and layout ---------------------------------------------
    /// `# …` to end of line, RETAINED: a comment directly above a
    /// declaration is its documentation, `##` is a section heading —
    /// telling them apart is the parser's job, the text is in the span.
    Comment,
    /// The invisible semicolon (design.md §4.15, panel 007): emitted at a
    /// line end only when the line can be *over* — see `layout.rs`.
    Terminator,
    /// The line indented one level deeper: a block opens.
    Indent,
    /// One level closes. A single line can emit several.
    Dedent,
    Eof,

    /// Error recovery: something was skipped, a diagnostic says why.
    /// The lexer never stops.
    Error,
}

#[derive(Clone, Copy, Debug)]
pub struct Token {
    pub kind: TokenKind,
    pub span: Span,
}

/// Stable lowercase name, used by `heroes lex --json` and the snapshots.
/// These names are output surface: changing one churns every snapshot.
pub fn kind_name(kind: TokenKind) -> &'static str {
    match kind {
        TokenKind::Ident => "ident",
        TokenKind::Int => "int",
        TokenKind::Float => "float",
        TokenKind::Str => "str",
        TokenKind::Char => "char",
        TokenKind::KwConstant => "kw_constant",
        TokenKind::KwFunction => "kw_function",
        TokenKind::KwRecord => "kw_record",
        TokenKind::KwVariant => "kw_variant",
        TokenKind::KwMatch => "kw_match",
        TokenKind::KwIf => "kw_if",
        TokenKind::KwElse => "kw_else",
        TokenKind::KwFor => "kw_for",
        TokenKind::KwIn => "kw_in",
        TokenKind::KwBreak => "kw_break",
        TokenKind::KwContinue => "kw_continue",
        TokenKind::KwReturn => "kw_return",
        TokenKind::KwTest => "kw_test",
        TokenKind::KwAssert => "kw_assert",
        TokenKind::KwExtern => "kw_extern",
        TokenKind::KwTrue => "kw_true",
        TokenKind::KwFalse => "kw_false",
        TokenKind::KwFail => "kw_fail",
        TokenKind::Eq => "eq",
        TokenKind::At => "at",
        TokenKind::Colon => "colon",
        TokenKind::Comma => "comma",
        TokenKind::Dot => "dot",
        TokenKind::Question => "question",
        TokenKind::Hole => "hole",
        TokenKind::Arrow => "arrow",
        TokenKind::FatArrow => "fat_arrow",
        TokenKind::Pipe => "pipe",
        TokenKind::LParen => "lparen",
        TokenKind::RParen => "rparen",
        TokenKind::LBracket => "lbracket",
        TokenKind::RBracket => "rbracket",
        TokenKind::LBrace => "lbrace",
        TokenKind::RBrace => "rbrace",
        TokenKind::Plus => "plus",
        TokenKind::Minus => "minus",
        TokenKind::Star => "star",
        TokenKind::Slash => "slash",
        TokenKind::Percent => "percent",
        TokenKind::EqEq => "eq_eq",
        TokenKind::BangEq => "bang_eq",
        TokenKind::Lt => "lt",
        TokenKind::Le => "le",
        TokenKind::Gt => "gt",
        TokenKind::Ge => "ge",
        TokenKind::AndAnd => "and_and",
        TokenKind::OrOr => "or_or",
        TokenKind::Bang => "bang",
        TokenKind::Comment => "comment",
        TokenKind::Terminator => "terminator",
        TokenKind::Indent => "indent",
        TokenKind::Dedent => "dedent",
        TokenKind::Eof => "eof",
        TokenKind::Error => "error",
    }
}
