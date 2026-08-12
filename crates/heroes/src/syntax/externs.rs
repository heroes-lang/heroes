//! The `extern` group: a head line naming the header and the library, then the
//! signatures indented under it (design.md §4.19, panel 036).
//!
//! It lives beside `decl.rs` rather than in it because it is the one declaration
//! head whose members are **other declarations** — every other kind reads one
//! line and a block of fields or statements. Splitting it out was predicted
//! rather than discovered: panel 036's compiler-engineer said the group form
//! would put `decl.rs` over 420 lines and force a §11 split, and it did, at 479.
//!
//! **The group is flattened here and only here.** One `Decl` per signature, each
//! carrying the head line's two spans, because a declaration's index is function
//! identity for `Ref::Top`, `ir::Function::decl`, `checked.result_type` and the
//! mangler's name table. Nothing after the parser knows the word "group";
//! `printer/fmt.rs` rebuilds one from a run of members, which is a rendering
//! decision rather than a tree.

use crate::lexer::TokenKind;
use crate::source::{Source, Span};

use super::ast::Ast;
use super::cursor::Cursor;
use super::decl::{function_tail, Linkage};

/// ```text
/// extern "sqlite3.h" link "sqlite3"
///     function sqlite3_open(path: cstr, out: ptr) -> i64
///     function sqlite3_close(db: ptr) -> i64
/// ```
///
/// The head line names the header and, optionally, the library; the members are
/// signatures with no bodies, because C provides the code (§4.19, panel 036).
///
/// **The header is not optional, and that is the point of the form.** §4.19's
/// whole mechanism is the `#include` — clang checking a signature against the
/// real declaration — so a headerless `extern` would emit a prototype that is
/// self-consistent by construction and verified by nothing. Panel 030 measured
/// what that costs: `void *fopen(const char *, const char *)` links by accident
/// on arm64, silently.
///
/// `link` is matched as a word, not lexed as a keyword: reserving it would put
/// it in the foreign-word registry, where §4.19's own open question already
/// records that C headers use ordinary words as identifiers 571 times over.
pub(super) fn group(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
    let keyword = cur.span();
    let doc = cur.take_docs(src, keyword);
    cur.bump(); // `extern`
    let Some(header) = header(cur, src) else { return };
    let link = link(cur, src);
    cur.skip_terminators();
    if !cur.at(TokenKind::Indent) {
        if !cur.at_reported_error() {
            cur.error(
                "expected_extern_block",
                "an `extern` group's signatures are indented under it — `extern \"math.h\"` then `    function sqrt(x: f64) -> f64` (§4.19)"
                    .to_string(),
                cur.span(),
            );
        }
        return;
    }
    members(cur, ast, src, doc, Linkage::Extern { header, link });
}

/// The head line's header string. A missing one is where the old headerless
/// spelling lands, so the message repairs *that* program rather than describing
/// the grammar.
fn header(cur: &mut Cursor, src: &Source) -> Option<Span> {
    if cur.at(TokenKind::Str) {
        return Some(cur.bump().span);
    }
    if !cur.at_reported_error() {
        let message = if cur.at(TokenKind::KwFunction) {
            "an `extern` names the header its signatures come from — `extern \"math.h\"`, then the signatures indented under it. Without the header there is no `#include`, and nothing checks the declaration (§4.19)".to_string()
        } else {
            format!(
                "expected the header's name in quotes after `extern`, found {} — `extern \"sqlite3.h\"`",
                cur.found(src)
            )
        };
        cur.error("expected_extern_header", message, cur.span());
    }
    cur.recover_to_next_decl();
    None
}

/// `link "sqlite3"` — optional, and matched by text.
fn link(cur: &mut Cursor, src: &Source) -> Option<Span> {
    if !(cur.at(TokenKind::Ident) && src.slice(cur.span()) == "link") {
        return None;
    }
    cur.bump(); // `link`
    if !cur.at(TokenKind::Str) {
        if !cur.at_reported_error() {
            let message = format!(
                "expected the library's name in quotes after `link`, found {} — `link \"sqlite3\"`, which is `-lsqlite3` to the linker",
                cur.found(src)
            );
            cur.error("expected_link_name", message, cur.span());
        }
        return None;
    }
    Some(cur.bump().span)
}

/// The indented signatures. Every one becomes its own declaration carrying the
/// group's header and link — the flattening panel 036 made a condition.
///
/// Recovery here is `skip_line`, never `recover_to_next_decl`: inside a block,
/// dropping "the rest of the declaration" would eat the members that follow, and
/// `field_block` and `case_block` skip a line for the same reason.
fn members(
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
                        "expected a `function` or a `constant`, found {} — an `extern` group holds what the header declares, one per line",
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
