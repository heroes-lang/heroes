//! How a token is named in a diagnostic.
//!
//! Parse errors read "expected X, found Y", and both halves come from here.
//! The names are prose, not source text, because the three tokens that
//! matter most have no spelling at all: a `Terminator` is an invisible line
//! end, an `Indent` is four spaces the author cannot see, a `Dedent` is
//! their absence. Naming them "end of line", "an indented block" and "the
//! end of the block" is the difference between an error a model can act on
//! and one it has to guess at (design.md §4.17).
//!
//! This is output surface: changing a phrase churns every golden that
//! contains it.

use crate::lexer::TokenKind;

pub(super) fn describe(kind: TokenKind) -> &'static str {
    match kind {
        TokenKind::Amp => "`&`",
        TokenKind::Caret => "`^`",
        TokenKind::Tilde => "`~`",
        TokenKind::Shl => "`<<`",
        TokenKind::Shr => "`>>`",
        TokenKind::Ident => "a name",
        TokenKind::Int => "a number",
        TokenKind::Float => "a number",
        TokenKind::Str => "a string",
        TokenKind::Char => "a character literal",
        TokenKind::KwConstant => "`constant`",
        TokenKind::KwFunction => "`function`",
        TokenKind::KwRecord => "`record`",
        TokenKind::KwVariant => "`variant`",
        TokenKind::KwMatch => "`match`",
        TokenKind::KwIf => "`if`",
        TokenKind::KwElse => "`else`",
        TokenKind::KwFor => "`for`",
        TokenKind::KwWhile => "`while`",
        TokenKind::KwIn => "`in`",
        TokenKind::KwBreak => "`break`",
        TokenKind::KwContinue => "`continue`",
        TokenKind::KwReturn => "`return`",
        TokenKind::KwTest => "`test`",
        TokenKind::KwAssert => "`assert`",
        TokenKind::KwExtern => "`extern`",
        TokenKind::KwNullPtr => "`nullptr`",
        TokenKind::KwUse => "`use`",
        TokenKind::KwTrue => "`true`",
        TokenKind::KwFalse => "`false`",
        TokenKind::KwFail => "`fail`",
        TokenKind::Eq => "`=`",
        TokenKind::At => "`@`",
        TokenKind::Colon => "`:`",
        TokenKind::Comma => "`,`",
        TokenKind::Dot => "`.`",
        TokenKind::Question => "`?`",
        TokenKind::Hole => "`???`",
        TokenKind::Arrow => "`->`",
        TokenKind::FatArrow => "`=>`",
        TokenKind::Pipe => "`|`",
        TokenKind::LParen => "`(`",
        TokenKind::RParen => "`)`",
        TokenKind::LBracket => "`[`",
        TokenKind::RBracket => "`]`",
        TokenKind::LBrace => "`{`",
        TokenKind::RBrace => "`}`",
        TokenKind::Plus => "`+`",
        TokenKind::Minus => "`-`",
        TokenKind::Star => "`*`",
        TokenKind::Slash => "`/`",
        TokenKind::Percent => "`%`",
        TokenKind::EqEq => "`==`",
        TokenKind::BangEq => "`!=`",
        TokenKind::Lt => "`<`",
        TokenKind::Le => "`<=`",
        TokenKind::Gt => "`>`",
        TokenKind::Ge => "`>=`",
        TokenKind::AndAnd => "`&&`",
        TokenKind::OrOr => "`||`",
        TokenKind::Bang => "`!`",
        TokenKind::Comment => "a comment",
        TokenKind::Terminator => "end of line",
        TokenKind::Indent => "an indented block",
        TokenKind::Dedent => "the end of the block",
        TokenKind::Eof => "end of file",
        TokenKind::Error => "an unreadable token",
    }
}
