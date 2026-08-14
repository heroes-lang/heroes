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

use super::ast::{Ast, Library};
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
    let library = library(cur, src);
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
    members(cur, ast, src, doc, Linkage::Extern { header, library });
}

/// The head line's header string. A missing one is where the old headerless
/// spelling lands, so the message repairs *that* program rather than describing
/// the grammar.
fn header(cur: &mut Cursor, src: &Source) -> Option<Span> {
    if cur.at(TokenKind::Str) {
        let span = cur.bump().span;
        machine_locked(cur, src, span, "header");
        return Some(span);
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

/// **A string in a group head may not name an absolute path** (panel 055).
///
/// It could, and nobody had noticed: `extern "/opt/foo/include/foo.h"` compiled
/// and ran, because the string is passed through to `#include` and C accepts a
/// path there. The slot is inherited rather than designed — Nim's `header` pragma
/// has the same permissiveness and CLAUDE.md §6 says copy Nim's surface — so §6
/// worked and bit in the same act. `link` has the worse version: GNU `ld` reads
/// `-l:filename` as *a file*, so `link ":/opt/foo/libfoo.a"` names one on the legs
/// whose linker supports it, against a driver comment claiming no `.hero` file can
/// hand clang an arbitrary argument.
///
/// **Only the absolute form is refused, and the narrowing is the point.**
/// `extern "sub/bar.h"` resolves against the `.hero` file's own directory
/// (panel 036's `-I <source dir>`), verified from two working directories, so a
/// vendored header beside the program is portable and correct. `curl/curl.h` is
/// path-shaped and right. *"This string begins with a filesystem root"* is a fact
/// about the value; *"nobody needs a relative path"* would be a premise about the
/// world (CLAUDE.md §11).
///
/// The repair is not text, so there is no `Fix`: the answer is `CPATH` or a `.pc`
/// on `PKG_CONFIG_PATH`, and a `certain` fix that rewrote the string would be a
/// lie.
fn machine_locked(cur: &mut Cursor, src: &Source, span: Span, what: &str) {
    let text = src.slice(span).trim_matches('"');
    // A leading root, a Windows drive letter, or — for a library only — GNU `ld`'s
    // `-l:filename` form, which is how a `link` string names a **file** rather
    // than a library. The last is the one the historian measured: it is refused
    // whatever follows the colon, because the whole point of the form is that the
    // rest is a filename.
    let rooted = text.starts_with('/')
        || text.starts_with('\\')
        || text.as_bytes().get(1) == Some(&b':')
        || text.starts_with(':');
    if !rooted {
        return;
    }
    let route = if what == "header" {
        "set `CPATH` to the directory holding it, or name a `package` and let `pkg-config` answer; a header beside this file is written relative, as `sub/foo.h`"
    } else {
        "set `LIBRARY_PATH` to the directory holding it, or name a `package` and let `pkg-config` answer; `link` takes the library's **name**, as `link \"sqlite3\"`"
    };
    cur.error(
        "machine_locked_path",
        format!(
            "`{text}` names a path, and a {what} in a group head says what the machine has, not where this machine keeps it — the next machine keeps it elsewhere\n  {route}"
        ),
        span,
    );
}

/// `link "sqlite3"` or `package "raylib"` — optional, one or the other, and both
/// matched by text rather than lexed as keywords, for the reason above.
///
/// **They are alternatives because they are different questions.** `link` tells
/// the linker a name the program already knows; `package` asks the machine, and
/// gets a different answer per platform from one spelling. Writing both would
/// mean the program both knew and did not know, so the second is refused with
/// the first one named.
fn library(cur: &mut Cursor, src: &Source) -> Option<Library> {
    let taken = one_library(cur, src)?;
    machine_locked(cur, src, span_of(taken), "library");
    // A second clause on the same head line. The message names what is already
    // there, because the repair is to delete one and the author must be told
    // which one they wrote first.
    if let Some(extra) = one_library(cur, src) {
        let (first, second) = (word_of(taken), word_of(extra));
        if !cur.at_reported_error() {
            cur.error(
                "one_library_per_group",
                format!(
                    "this group already says `{first}`, so `{second}` is a second answer to the same question — a group names a library or asks for a package, never both"
                ),
                span_of(extra),
            );
        }
    }
    Some(taken)
}

fn word_of(library: Library) -> &'static str {
    match library {
        Library::Link(_) => "link",
        Library::Package(_) => "package",
    }
}

fn span_of(library: Library) -> Span {
    match library {
        Library::Link(span) | Library::Package(span) => span,
    }
}

fn one_library(cur: &mut Cursor, src: &Source) -> Option<Library> {
    if !cur.at(TokenKind::Ident) {
        return None;
    }
    let word = src.slice(cur.span());
    let is_package = match word {
        "link" => false,
        "package" => true,
        _ => return None,
    };
    cur.bump();
    if !cur.at(TokenKind::Str) {
        if !cur.at_reported_error() {
            let message = if is_package {
                format!(
                    "expected the package's name in quotes after `package`, found {} — `package \"raylib\"`, which is what the machine is asked about",
                    cur.found(src)
                )
            } else {
                format!(
                    "expected the library's name in quotes after `link`, found {} — `link \"sqlite3\"`, which is `-lsqlite3` to the linker",
                    cur.found(src)
                )
            };
            cur.error("expected_link_name", message, cur.span());
        }
        return None;
    }
    let span = cur.bump().span;
    Some(if is_package { Library::Package(span) } else { Library::Link(span) })
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
