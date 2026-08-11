//! `heroes check --dump-scopes`: what the resolver decided, seen.
//!
//! The dump exists for the same reason `--dump-ast` does — a pass whose output
//! nobody can look at is a pass nobody can debug — and it shows the two things
//! the resolver knows that no earlier stage did:
//!
//! 1. **The top-level table**, sorted. Declaration order carries no meaning
//!    (§4.2), and here that stops being a claim: the table is alphabetical, and
//!    a file whose functions call each other in any order produces the same one.
//! 2. **Every binding, nested by scope, with its read and write counts.** The
//!    indentation *is* the scope structure, so a loop variable visibly lives
//!    inside its loop, and §4.4's unused rule becomes readable: the bindings
//!    with `reads 0` are exactly the ones a hole-free file would be rejected
//!    for.
//!
//! Output surface, like every other dump: goldens and snapshots contain it.

use crate::resolve::{LocalKind, Resolved};
use crate::source::Source;
use crate::syntax::{Ast, DeclKind};


/// The library is part of every compilation and part of no dump.
///
/// A `--dump-<stage>` answers "what does the compiler know about **the file I
/// named**" (CLAUDE.md §10). The library is the same in every program, so it
/// carries no information about this one — and printing it would bury the
/// answer under a section the reader cannot change. The emitted C is the
/// exception, and necessarily: the binary needs the definitions.
pub fn dump_scopes(ast: &Ast, resolved: &Resolved, src: &Source) -> String {
    let mut out = format!("file {}\n", src.name);
    out.push_str("symbols\n");
    for (name, decl) in &resolved.top {
        if src.is_library(ast.decls[*decl as usize].name.start) {
            continue;
        }
        let (line, _) = src.line_col(ast.decls[*decl as usize].name.start);
        out.push_str(&format!("  {} {name} (line {line})\n", entity(ast, *decl)));
    }
    out.push_str("scopes\n");
    for (index, decl) in ast.decls.iter().enumerate() {
        if src.is_library(decl.name.start) {
            continue;
        }
        let locals: Vec<&crate::resolve::Local> =
            resolved.locals.iter().filter(|l| l.owner == index as u32).collect();
        if locals.is_empty() {
            continue;
        }
        out.push_str(&format!("  {} {}\n", entity(ast, index as u32), src.slice(decl.name)));
        for local in locals {
            // Two spaces per scope, like every other dump in this module.
            let indent = "  ".repeat(1 + local.depth as usize);
            let mutable = if local.mutable { " @" } else { "" };
            out.push_str(&format!(
                "  {indent}{} {}{mutable} (reads {}, writes {})\n",
                kind(local.kind),
                src.slice(local.name),
                local.reads,
                local.writes
            ));
        }
    }
    if resolved.has_hole {
        out.push_str("holes present: unused bindings are not reported\n");
    }
    out
}

fn entity(ast: &Ast, decl: u32) -> &'static str {
    match &ast.decls[decl as usize].kind {
        DeclKind::Constant { .. } => "constant",
        DeclKind::Function(function) => {
            if function.is_extern {
                "extern"
            } else {
                "function"
            }
        }
        DeclKind::Record { .. } => "record",
        DeclKind::Variant { .. } => "variant",
        DeclKind::Test { .. } => "test",
    }
}

fn kind(kind: LocalKind) -> &'static str {
    match kind {
        LocalKind::Param => "param",
        LocalKind::Bind => "bind",
        LocalKind::Cell => "cell",
        LocalKind::Loop => "loop",
        LocalKind::Payload => "payload",
    }
}
