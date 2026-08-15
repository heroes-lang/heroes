//! `heroes parse --dump-ast`: the tree as text.
//!
//! Not the formatter (that is `heroes fmt`, M-syntax-tree step 4) — this is the tree
//! *seen*, one node per line, so that "what the parser understood" is
//! readable without a debugger. Two deliberate differences from Heroes
//! source keep the two apart at a glance: the dump indents by **2** spaces,
//! and it names node kinds (`field`, `case`, `body`) that no program
//! contains.
//!
//! The format is output surface: goldens and snapshots contain it, so a
//! change to a word here churns them all.
//!
//! **Every field of a `DeclKind` is destructured and used here, and `..` is
//! banned in this file's arms** (panels 060, 061, 062). The reason is that this
//! module is half of a guard: `printer::tests::assert_canonical` asserts
//! `dump(text) == dump(fmt(text))` — *"fmt changed the tree"* — so a field the
//! formatter can print and this dump elides is a **cancelling pair**. The
//! formatter drops it, the dump does not notice, and the test is green about a
//! program that changed meaning. It happened twice: `header`/`library` on a
//! `record` (panel 060) and `partial` (panel 061), both swallowed by a `..`.
//!
//! The rule is mechanical rather than a convention, and rustc is what enforces
//! it: with no `..`, a new field on a `DeclKind` variant is a *compile error*
//! here and in `fmt_decl.rs` at once, and a field bound but not read is
//! `unused_variables`, which CI denies. What a reviewer still owes is the
//! decision itself — a field deliberately not printed must be bound to `_` in
//! the open, where the diff shows it.

use crate::source::{Source, Span};
use crate::syntax::{Ast, Block, Case, Decl, DeclKind, Field, Function, Library};

use super::bodies::write_block;
use super::types::render_type;


/// The library is part of every compilation and part of no dump.
///
/// A `--dump-<stage>` answers "what does the compiler know about **the file I
/// named**" (CLAUDE.md §10). The library is the same in every program, so it
/// carries no information about this one — and printing it would bury the
/// answer under a section the reader cannot change. The emitted C is the
/// exception, and necessarily: the binary needs the definitions.
pub fn dump_ast(ast: &Ast, src: &Source) -> String {
    let mut out = format!("file {}\n", src.name);
    for used in &ast.uses {
        if !src.is_root(used.span.start) {
            continue;
        }
        out.push_str(&format!("  use {}\n", src.slice(used.name)));
    }
    for decl in &ast.decls {
        if !src.is_root(decl.name.start) {
            continue;
        }
        declaration(ast, src, decl, &mut out);
    }
    out
}

fn declaration(ast: &Ast, src: &Source, decl: &Decl, out: &mut String) {
    let name = src.slice(decl.name);
    match &decl.kind {
        DeclKind::Constant { ty, body, header, library } => {
            // The group's head line, exactly as a signature carries it: a reader of
            // `--dump-ast` must be able to tell a constant whose value is in a
            // header from one whose value is in this file, and the widened
            // `DeclKind` is a place that could have said nothing about it.
            let group = extern_prefix(src, *header, *library);
            out.push_str(&format!("  {group}constant {name}: {}\n", render_type(ast, *ty, src)));
            docs(src, &decl.doc, out);
            // An `extern constant` has no body: the value is in the header
            // (§4.19), exactly as an `extern function`'s code is.
            if let Some(body) = body {
                write_body(ast, src, body, out);
            }
        }
        DeclKind::Function(function) => {
            out.push_str(&format!("  {}\n", signature(ast, src, name, function)));
            docs(src, &decl.doc, out);
            // An `extern` has no body: the code is in C (§4.19).
            if let Some(block) = &function.body {
                write_body(ast, src, block, out);
            }
        }
        // **`partial` and the group head appear here because `fmt_decl.rs` prints
        // them** — the rule stated in this file's doc, and this arm is where it was
        // broken twice. `partial` was repaired at panel 061; `header`/`library`
        // survived until panel 062's audit, so `fmt` hoisting a `record` out of its
        // group, folding it into a neighbouring one, or turning `link` into
        // `package` all produced identical dumps and a green test. Measured, with
        // this arm's `..` restored: `dump` of `extern "raylib.h" package "raylib"`
        // + `record Font` and of a top-level `record Font` are the same string.
        DeclKind::Record { fields, header, library, partial } => {
            let group = extern_prefix(src, *header, *library);
            let marker = if *partial { " partial" } else { "" };
            out.push_str(&format!("  {group}record {name}{marker}\n"));
            docs(src, &decl.doc, out);
            for field in fields {
                field_line(ast, src, field, "    ", out);
            }
        }
        DeclKind::Variant { cases } => {
            out.push_str(&format!("  variant {name}\n"));
            docs(src, &decl.doc, out);
            for case in cases {
                case_lines(ast, src, case, out);
            }
        }
        DeclKind::Test { body: block } => {
            // `name` is the string literal, quotes included: a title.
            out.push_str(&format!("  test {name}\n"));
            docs(src, &decl.doc, out);
            write_body(ast, src, block, out);
        }
    }
}

/// `function map<A, B>(xs: [A], f: (function(A) -> B)) -> [B]` — the header
/// in one line, which is how a signature is read.
///
/// **It takes the function, not the declaration's index** (panel 062's audit).
/// This read `ast.decls[decl]` and matched, with `_ => <the name alone>` for a
/// record or a test — an arm that could not be reached, because its one caller
/// (`types/apply.rs::user_call`, building an `arity` diagnostic) has already
/// destructured `DeclKind::Function` and holds the function. The catch-all was
/// therefore not a fallback but a *second answer to a question nobody asks*, and
/// the way to make an unreachable case loud is to make it unrepresentable: the
/// caller passes what it has and there is no arm left to get wrong.
pub fn render_signature(ast: &Ast, src: &Source, name: &str, function: &Function) -> String {
    signature(ast, src, name, function)
}

/// **The group is not in the tree, so the dump does not print one.** The parser
/// flattens `extern "math.h"` into one declaration per signature, each carrying
/// the header (§4.19, panel 036) — and a dump that re-grouped them would show a
/// structure no later pass can see. `heroes fmt` re-groups, because it hands the
/// author their own program back; this prints what the tree holds.
/// `extern "sqlite3.h" link "sqlite3" ` — the head line a group's member carries,
/// or nothing. **One writer for both member kinds**, so a `constant` and a
/// `function` in the same group cannot print it two ways.
fn extern_prefix(src: &Source, header: Option<Span>, library: Option<Library>) -> String {
    let Some(header) = header else { return String::new() };
    let mut out = String::from("extern ");
    out.push_str(src.slice(header));
    if let Some(library) = library {
        out.push_str(match library {
            Library::Link(_) => " link ",
            Library::Package(_) => " package ",
        });
        out.push_str(src.slice(match library {
            Library::Link(span) | Library::Package(span) => span,
        }));
    }
    out.push(' ');
    out
}

fn signature(ast: &Ast, src: &Source, name: &str, function: &Function) -> String {
    let mut out = extern_prefix(src, function.header, function.library);
    out.push_str("function ");
    out.push_str(name);
    if !function.generics.is_empty() {
        out.push('<');
        for (i, generic) in function.generics.iter().enumerate() {
            if i > 0 {
                out.push_str(", ");
            }
            out.push_str(src.slice(*generic));
        }
        out.push('>');
    }
    out.push('(');
    for (i, param) in function.params.iter().enumerate() {
        if i > 0 {
            out.push_str(", ");
        }
        if param.mutable {
            out.push('@');
        }
        out.push_str(src.slice(param.name));
        out.push_str(": ");
        out.push_str(&render_type(ast, param.ty, src));
    }
    out.push_str(") -> ");
    out.push_str(&render_type(ast, function.result, src));
    out
}

fn case_lines(ast: &Ast, src: &Source, case: &Case, out: &mut String) {
    out.push_str(&format!("    case {}\n", src.slice(case.name)));
    docs_at(src, &case.doc, "      ", out);
    for field in &case.fields {
        field_line(ast, src, field, "      ", out);
    }
}

fn field_line(ast: &Ast, src: &Source, field: &Field, indent: &str, out: &mut String) {
    out.push_str(&format!(
        "{indent}field {}: {}\n",
        src.slice(field.name),
        render_type(ast, field.ty, src)
    ));
    docs_at(src, &field.doc, &format!("{indent}  "), out);
}

fn docs(src: &Source, doc: &[Span], out: &mut String) {
    docs_at(src, doc, "    ", out);
}

/// Doc comments are printed verbatim, `#` included: they are markdown, and
/// the compiler reuses them in `???` output and `outline` (§4.1).
fn docs_at(src: &Source, doc: &[Span], indent: &str, out: &mut String) {
    for span in doc {
        out.push_str(&format!("{indent}doc {}\n", src.slice(*span)));
    }
}

/// A declaration's body: its statements, one per line, indented under the
/// declaration they belong to.
fn write_body(ast: &Ast, src: &Source, block: &Block, out: &mut String) {
    write_block(ast, src, block, 4, out);
}
