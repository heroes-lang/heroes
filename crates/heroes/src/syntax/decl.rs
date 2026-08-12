//! The top level: the six declaration heads, one shape (design.md §4.1,
//! §4.2, §4.18, §4.19; panel 018).
//!
//! Every top-level line starts with its kind — `constant`, `function`,
//! `record`, `variant`, `test`, `extern` — so the dispatcher is a switch on
//! the first token of the line, and an identifier at top level is never a
//! declaration. That closed keyword set is also recovery's anchor: one
//! broken declaration can never swallow the next one.
//!
//! A useful consequence of the keyword-first shape: every header line now
//! ends in a token that can end a statement (a name, `)`, a type, a
//! string), so every header gets a terminator and the panel-007 trap —
//! headers ending in non-enders — is gone. Bodies still skip terminators
//! and ask only for the `Indent`.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::lexer::TokenKind;
use crate::source::{Source, Span};

use super::ast::{Ast, Decl, DeclKind, Function, TypeKind, TypeNode, Use};
use super::cursor::Cursor;
use super::data::{record, variant};
use super::members::{generics, params};
use super::stmt::block;
use super::types::parse_type;

/// Read declarations until end of file. Every path either builds a
/// declaration or recovers past one, so this loop always makes progress.
pub(super) fn file(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
    loop {
        cur.skip_terminators();
        match cur.kind() {
            TokenKind::Eof => return,
            TokenKind::KwConstant => constant(cur, ast, src),
            TokenKind::KwFunction => function_decl(cur, ast, src),
            TokenKind::KwRecord => record(cur, ast, src),
            TokenKind::KwVariant => variant(cur, ast, src),
            TokenKind::KwExtern => extern_group(cur, ast, src),
            TokenKind::KwTest => test(cur, ast, src),
            TokenKind::KwUse => use_decl(cur, ast, src),
            _ => {
                if !cur.at_reported_error() {
                    let message = format!(
                        "expected a declaration, found {} — every top-level line starts with its kind: `use`, `constant`, `function`, `record`, `variant`, `test \"…\"`, or `extern`",
                        cur.found(src)
                    );
                    cur.error("expected_declaration", message, cur.span());
                }
                // The one arm that has consumed nothing yet, so it consumes
                // here: recovery only drops the *rest* of a broken line, and
                // this loop must always move. An indented block at top level
                // belongs to no declaration, so it is dropped whole — and
                // nothing else is, because the next line is innocent.
                if cur.at(TokenKind::Indent) {
                    cur.balanced_block();
                } else {
                    cur.bump();
                    cur.recover_to_next_decl();
                }
            }
        }
    }
}

/// The shared head of every named declaration: the kind keyword —
/// documentation attaches to it, since it opens the line — then the declared
/// name. Returns `None` (already recovered) when the name is missing.
pub(super) fn head(cur: &mut Cursor, src: &Source, example: &str) -> Option<(Span, Vec<Span>, Span)> {
    let keyword = cur.span();
    let doc = cur.take_docs(src, keyword);
    let kind = cur.found(src);
    cur.bump();
    if !cur.at(TokenKind::Ident) {
        if !cur.at_reported_error() {
            let message = format!(
                "expected the declared name after {kind}, found {} — `{example}`",
                cur.found(src)
            );
            cur.error("expected_name", message, cur.span());
        }
        cur.recover_to_next_decl();
        return None;
    }
    let name = cur.bump().span;
    Some((keyword, doc, name))
}

/// `constant MAX_DEPTH: int` + the value, indented (§4.2). Constants use
/// SCREAMING_CASE by convention; the parser does not police it.
fn constant(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
    let Some((keyword, doc, name)) = head(cur, src, "constant MAX_DEPTH: int") else {
        return;
    };
    if !cur.expect(
        TokenKind::Colon,
        "expected_constant_type",
        "`:` and the constant's type — a `constant` declares a value, so it has one",
        src,
    ) {
        cur.recover_to_next_decl();
        return;
    }
    let ty = parse_type(cur, ast, src);
    let Some(body) = block(cur, ast, src, "a `constant`") else {
        cur.recover_to_next_decl();
        return;
    };
    let span = keyword.to(body.span);
    ast.decls.push(Decl { name, doc, span, kind: DeclKind::Constant { ty, body } });
}

/// `function dist2(a: Point, b: Point) -> int` + body (§4.2). The parameter
/// list attaches to the name, as at the call site.
fn function_decl(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
    let Some((keyword, doc, name)) =
        head(cur, src, "function dist2(a: Point, b: Point) -> int")
    else {
        return;
    };
    function_tail(cur, ast, src, keyword, doc, name, Linkage::Heroes);
}

/// Where a signature's implementation comes from, and — when it comes from C —
/// the two strings the group's head line carried (§4.19, panel 036).
///
/// A copy per member rather than a shared index: the group exists only in the
/// source text, and every later pass sees N ordinary declarations.
#[derive(Clone, Copy)]
enum Linkage {
    Heroes,
    Extern { header: Span, link: Option<Span> },
}

/// Everything after a function's name: generics, the signature, and — for a
/// non-extern — the body. Shared by `function` and a group's members.
fn function_tail(
    cur: &mut Cursor,
    ast: &mut Ast,
    src: &Source,
    keyword: Span,
    doc: Vec<Span>,
    name: Span,
    linkage: Linkage,
) {
    let is_extern = matches!(linkage, Linkage::Extern { .. });
    let generic_names = generics(cur, src);
    if !cur.at(TokenKind::LParen) {
        params(cur, ast, src); // reports the missing `(` with its own message
        cur.recover_to_next_decl();
        return;
    }
    let parameters = params(cur, ast, src);
    // No `->` means no value. The `Unit` node is synthesised at the end of
    // the header so later passes never have to ask whether the arrow was
    // written (§4.3: `()` is a type like any other).
    let result = if cur.eat(TokenKind::Arrow) {
        parse_type(cur, ast, src)
    } else {
        let here = cur.span();
        ast.push_type(TypeNode { kind: TypeKind::Unit, span: Span { start: here.start, end: here.start } })
    };
    let body = if is_extern {
        extern_body_check(cur);
        None
    } else {
        match block(cur, ast, src, "a `function`") {
            Some(body) => Some(body),
            None => {
                cur.recover_to_next_decl();
                return;
            }
        }
    };
    let end = match &body {
        Some(block) => block.span,
        None => cur.previous_span(),
    };
    let (header, link) = match linkage {
        Linkage::Heroes => (None, None),
        Linkage::Extern { header, link } => (Some(header), link),
    };
    let kind = DeclKind::Function(Function {
        generics: generic_names,
        params: parameters,
        result,
        body,
        is_extern,
        header,
        link,
    });
    ast.decls.push(Decl { name, doc, span: keyword.to(end), kind });
}

/// ```text
/// extern "sqlite3.h" link "sqlite3"
///     function sqlite3_open(path: cstr, out: ptr) -> int
///     function sqlite3_close(db: ptr) -> int
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
fn extern_group(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
    let keyword = cur.span();
    let doc = cur.take_docs(src, keyword);
    cur.bump(); // `extern`
    let Some(header) = extern_header(cur, src) else { return };
    let link = extern_link(cur, src);
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
    extern_members(cur, ast, src, doc, Linkage::Extern { header, link });
}

/// The head line's header string. A missing one is where the old headerless
/// spelling lands, so the message repairs *that* program rather than describing
/// the grammar.
fn extern_header(cur: &mut Cursor, src: &Source) -> Option<Span> {
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
fn extern_link(cur: &mut Cursor, src: &Source) -> Option<Span> {
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
fn extern_members(
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
            TokenKind::KwFunction => {
                let keyword = cur.span();
                // The group's own doc comment documents its first signature;
                // after that each line takes its own, exactly as a record's
                // fields do.
                let doc = if first { group_doc.clone() } else { cur.take_docs(src, keyword) };
                first = false;
                cur.bump(); // `function`
                if !cur.at(TokenKind::Ident) {
                    if !cur.at_reported_error() {
                        let message = format!(
                            "expected the C function's name, found {} — `function sqrt(x: f64) -> f64`",
                            cur.found(src)
                        );
                        cur.error("expected_name", message, cur.span());
                    }
                    cur.skip_line();
                    continue;
                }
                let name = cur.bump().span;
                function_tail(cur, ast, src, keyword, doc, name, linkage);
            }
            _ => {
                if !cur.at_reported_error() {
                    let message = format!(
                        "expected a `function` signature, found {} — an `extern` group holds signatures and nothing else, one per line",
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
fn extern_body_check(cur: &mut Cursor) {
    cur.skip_terminators();
    if !cur.at(TokenKind::Indent) {
        return;
    }
    let span = cur.span();
    cur.error(
        "extern_has_body",
        "an `extern` declaration has no body — it names a C function, and C provides the code"
            .to_string(),
        span,
    );
    cur.balanced_block();
}

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
fn use_decl(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
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

/// `test "3-4-5 triangle"` + body (§4.18). The name is a string, not an
/// identifier: it is a title, and it is printed when the test fails.
fn test(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
    let keyword = cur.span();
    let doc = cur.take_docs(src, keyword);
    cur.bump(); // `test`
    if !cur.at(TokenKind::Str) {
        if !cur.at_reported_error() {
            let message = format!(
                "expected the test's name as a string, found {} — `test \"3-4-5 triangle\"`",
                cur.found(src)
            );
            cur.error("expected_test_name", message, cur.span());
        }
        cur.recover_to_next_decl();
        return;
    }
    let name = cur.bump().span;
    let Some(body) = block(cur, ast, src, "a `test`") else {
        cur.recover_to_next_decl();
        return;
    };
    let span = keyword.to(body.span);
    ast.decls.push(Decl { name, doc, span, kind: DeclKind::Test { body } });
}
