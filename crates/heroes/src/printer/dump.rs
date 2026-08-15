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
        // **`partial` appears here because `fmt_decl.rs` prints it**, and that is
        // the rule rather than the instance (panel 061). `assert_canonical` asserts
        // `dump(text) == dump(fmt(text))` — *"fmt changed the tree"* — so a field
        // the formatter can print and this cannot is a **cancelling pair**: the
        // formatter drops it, the dump does not notice, and the test is green about
        // a program that changed meaning. Measured: with this arm absent, `fmt` on
        // `record Font partial` emitted `record Font` and every test passed.
        DeclKind::Record { fields, partial, .. } => {
            let marker = if *partial { " partial" } else { "" };
            out.push_str(&format!("  record {name}{marker}\n"));
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
pub fn render_signature(ast: &Ast, src: &Source, decl: u32) -> String {
    let declaration = &ast.decls[decl as usize];
    match &declaration.kind {
        DeclKind::Function(function) => {
            signature(ast, src, src.slice(declaration.name), function)
        }
        _ => src.slice(declaration.name).to_string(),
    }
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
