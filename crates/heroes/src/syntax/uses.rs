//! `use geom` — the one declaration that names another file (design.md §3.1,
//! panel 031).
//!
//! It is its own module for the reason the two rejections below give: a `use`
//! line is where a model's memory of four other languages arrives, so the
//! interesting code here is not the happy path (one identifier) but the two
//! shapes that are refused and what each is met with.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::lexer::TokenKind;
use crate::source::Source;

use super::ast::{Ast, Use};
use super::cursor::Cursor;

/// `use geom` — the module named, and nothing else on the line (panel 031).
///
/// The whole of it is one identifier, and the two rejections below are why the
/// panel chose that spelling over a quoted path. A **string** is the shape four
/// other languages use and the shape a model on autopilot writes, so it is met
/// with a machine-applicable fix rather than a lecture: the repair is deleting
/// two characters, which is as `Certain` as a fix gets. A **dotted or slashed
/// path** is the design space the bare identifier forecloses — there is no
/// nesting, and saying so at the point of the attempt is cheaper than a spec
/// sentence nobody reads twice (R5's principle, applied to syntax).
pub(super) fn declaration(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
    let keyword = cur.span();
    cur.bump(); // `use`
    if cur.at(TokenKind::Str) {
        let span = cur.span();
        // The quotes are part of the literal's span, so the replacement is its
        // own text with them removed — and it is only `Certain` when what is
        // left is a name this language could have accepted.
        let quoted = src.slice(span);
        let inner = quoted.trim_matches('"');
        let mut diagnostic = Diagnostic::new(
            "use_wants_a_name",
            format!(
                "a module is named, not quoted — write `use {inner}`, and the file `{inner}.hero` beside this one is what it reads"
            ),
            span,
        );
        if is_module_name(inner) {
            diagnostic.fixes.push(Fix {
                title: format!("write `use {inner}`"),
                replacement: inner.to_string(),
                span,
                certainty: Certainty::Certain,
            });
        }
        cur.push_diagnostic(diagnostic);
        cur.recover_to_next_decl();
        return;
    }
    if !cur.at(TokenKind::Ident) {
        if !cur.at_reported_error() {
            // `use` is not in panel 007's ender list, so a `use` with nothing
            // after it inserts no terminator and the next token is the *next
            // line's* first word. Naming it would blame an innocent
            // declaration, so the caret goes on the keyword and the found
            // token is quoted only when it really is on this line.
            let keyword_line = src.line_of(keyword.start);
            let found_line = src.line_of(cur.span().start);
            let message = if keyword_line == found_line {
                format!(
                    "expected the module's name after `use`, found {} — `use geom`, which reads `geom.hero` beside this file",
                    cur.found(src)
                )
            } else {
                "`use` names one module and this line names none — `use geom`, which reads `geom.hero` beside this file".to_string()
            };
            cur.error("expected_module_name", message, keyword);
        }
        // Nothing is left of the broken line when the cursor has already
        // crossed onto the next one, and recovering anyway would eat an
        // innocent declaration whole — the failure panel 018's keyword-first
        // shape exists to prevent. It reappears here because `use` is not in
        // panel 007's ender list, so a bare `use` plants no terminator.
        if src.line_of(cur.span().start) == src.line_of(keyword.start) {
            cur.recover_to_next_decl();
        }
        return;
    }
    let name = cur.bump().span;
    // `use geom.shapes` and `use shapes/geom`: there is no hierarchy, and the
    // second token is where the author finds that out.
    if cur.at(TokenKind::Dot) || cur.at(TokenKind::Slash) {
        let span = cur.span();
        cur.error(
            "module_path_has_no_parts",
            format!(
                "a module name is one word — `use {}`. There is no nesting: every `.hero` a program reads sits beside the file that names it",
                src.slice(name)
            ),
            span,
        );
        cur.recover_to_next_decl();
        return;
    }
    ast.uses.push(Use { name, span: keyword.to(name) });
}

/// Could this text have been written after `use`? Exactly the identifier rule,
/// asked of a string's contents so that the fix above is offered only when
/// applying it produces a program that parses.
fn is_module_name(text: &str) -> bool {
    let mut chars = text.chars();
    match chars.next() {
        Some(first) if first.is_ascii_alphabetic() || first == '_' => {}
        _ => return false,
    }
    chars.all(|c| c.is_ascii_alphanumeric() || c == '_')
}
