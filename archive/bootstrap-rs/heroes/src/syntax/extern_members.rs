//! What is **inside** an `extern` group: its members, and the block that must not
//! be there.
//!
//! Split from `externs.rs` by the §11 ceiling, 2026-08-15, along the seam the file
//! already had. `externs.rs` reads the group's **head line** — the header string,
//! the `link`/`package` clause, the machine-locked path it refuses — and answers
//! *what does this group name*. This answers *what does it declare*, and the two
//! fail differently: a wrong head line is one diagnostic about the whole group, a
//! wrong member is one diagnostic per member with the rest of the block still
//! parsed.
//!
//! That difference is why recovery here is `skip_line` and never
//! `recover_to_next_decl` — inside a block, dropping "the rest of the declaration"
//! would eat the members that follow.

use crate::lexer::TokenKind;
use crate::source::{Source, Span};

use super::ast::Ast;
use super::cursor::Cursor;
use super::decl::{function_tail, Linkage};

/// The indented signatures. Every one becomes its own declaration carrying the
/// group's header and link — the flattening panel 036 made a condition.
///
/// Recovery here is `skip_line`, never `recover_to_next_decl`: inside a block,
/// dropping "the rest of the declaration" would eat the members that follow, and
/// `field_block` and `case_block` skip a line for the same reason.
pub(super) fn members(
    cur: &mut Cursor,
    ast: &mut Ast,
    src: &Source,
    group_doc: Vec<Span>,
    linkage: Linkage,
) {
    cur.bump(); // the Indent
    let mut first = true;
    loop {
        cur.skip_terminators();
        match cur.kind() {
            TokenKind::Dedent => {
                cur.bump();
                return;
            }
            TokenKind::Eof => return,
            // **A `record` is a member like the other two** (panel 060). It takes
            // the group's doc on the first line exactly as they do, and it is
            // flattened into its own `Decl` carrying the header — so nothing
            // downstream has to ask "was this parsed inside a group?", it asks the
            // declaration.
            TokenKind::KwRecord => {
                let keyword = cur.span();
                let doc = if first { group_doc.clone() } else { cur.take_docs(src, keyword) };
                first = false;
                super::data::record_with_doc(cur, ast, src, doc, linkage);
            }
            TokenKind::KwFunction | TokenKind::KwConstant => {
                let is_constant = cur.at(TokenKind::KwConstant);
                let keyword = cur.span();
                // The group's own doc comment documents its first signature;
                // after that each line takes its own, exactly as a record's
                // fields do.
                let doc = if first { group_doc.clone() } else { cur.take_docs(src, keyword) };
                first = false;
                cur.bump(); // `function` or `constant`
                if !cur.at(TokenKind::Ident) {
                    if !cur.at_reported_error() {
                        let shape = if is_constant {
                            "`constant SQLITE_OK: i64`"
                        } else {
                            "`function sqrt(x: f64) -> f64`"
                        };
                        let what = if is_constant { "constant's" } else { "function's" };
                        let message = format!(
                            "expected the C {what} name, found {} — {shape}",
                            cur.found(src)
                        );
                        cur.error("expected_name", message, cur.span());
                    }
                    cur.skip_line();
                    continue;
                }
                let name = cur.bump().span;
                if is_constant {
                    super::decl::constant_tail(cur, ast, src, keyword, doc, name, linkage);
                } else {
                    function_tail(cur, ast, src, keyword, doc, name, linkage);
                }
            }
            _ => {
                if !cur.at_reported_error() {
                    let message = format!(
                        "expected a `function`, a `constant` or a `record`, found {} — an `extern` group holds what the header declares, one per line",
                        cur.found(src)
                    );
                    cur.error("expected_extern_signature", message, cur.span());
                }
                if cur.at(TokenKind::Indent) {
                    cur.balanced_block();
                } else {
                    cur.skip_line();
                }
            }
        }
    }
}

/// An `extern` with a body is a mistake worth naming: the body would never
/// be compiled, and silently ignoring it is the class of thing this language
/// exists to make loud.
///
/// `kind` is what the member is, because the repair differs: a `function`'s code
/// comes from C, a `constant`'s *value* does — and telling a reader who wrote
/// `constant SQLITE_OK: i64` with `0` under it that "it names a C function" sends
/// them looking for a function they never wrote (§4.17).
pub(super) fn body_check(cur: &mut Cursor, kind: &str) {
    cur.skip_terminators();
    if !cur.at(TokenKind::Indent) {
        return;
    }
    let span = cur.span();
    let what = if kind == "constant" {
        "an `extern` declaration has no body — this `constant` names one the header already defines, so the value is the header's and never yours"
    } else {
        "an `extern` declaration has no body — it names a C function, and C provides the code"
    };
    cur.error("extern_has_body", what.to_string(), span);
    cur.balanced_block();
}
