//! `???` output: what the compiler already knows (design.md §4.16).
//!
//! A hole is **not an error**. The compiler reaches it, finds it has nothing to
//! compute, and prints what it was about to need instead: the expected type, and
//! what is in scope to build one from. §4.16's claim is that this costs nothing —
//! the checker holds all of it for its ordinary work and normally throws it away.
//! This file is the throwing-away, undone.
//!
//! Two rules are normative and both are here:
//!
//! - **capped at 5, in deterministic order.** Unbounded "nearby functions" would
//!   flood a model's context on a 5k-line file and vary with layout, so identical
//!   holes would give different guidance. The cap is stated in the spec, so it is
//!   stated in the code that implements it.
//! - **a file with holes type-checks everything else** and produces no binary.
//!   The report is therefore *output*, not a diagnostic: it goes to stdout and
//!   the exit code stays 0.

use crate::resolve::Resolved;
use crate::source::Source;
use crate::syntax::{Ast, DeclKind};

use super::table::Ty;
use super::Checked;

/// §4.16's cap, and the reason it exists: identical holes must give identical
/// guidance, whatever the file's size.
const SUGGESTIONS: usize = 5;

/// The report for every hole in the file, in source order.
pub fn report(ast: &Ast, resolved: &Resolved, checked: &Checked, src: &Source) -> String {
    let mut out = String::new();
    for hole in &checked.holes {
        // `Source::locate`, never `line_col` plus `src.name`: the hole report
        // is not a `Diagnostic`, which is how it survived M-module-namespace's sweep still
        // naming the root file and a line from the concatenated text
        // (panel 033 D2).
        let (file, line, col) = src.locate(hole.span.start);
        out.push_str(&format!("hole at {file}:{line}:{col}\n"));
        let expected = checked.types.get(hole.expected);
        if expected == Ty::Error {
            // A hole in statement position: nothing is expected of it, and
            // saying "expected `?`" would be worse than saying nothing. What the
            // model needs instead is what the *function* owes, which is the one
            // thing a hole standing for a whole body is missing.
            out.push_str("  position: statement — any type, or none\n");
            if let Some(owner) = owner_of(ast, hole.span.start) {
                if let Some(result) = checked.result_type(owner) {
                    if checked.types.get(result) != Ty::Unit {
                        out.push_str(&format!(
                            "  this function returns: {}\n",
                            super::render_ty(&checked.types, ast, src, result, &[])
                        ));
                    }
                }
            }
        } else {
            out.push_str(&format!(
                "  expected type: {}\n",
                super::render_ty(&checked.types, ast, src, hole.expected, &[])
            ));
        }
        in_scope(ast, resolved, checked, src, hole.span.start, &mut out);
        fields_of(ast, checked, src, hole.expected, &mut out);
        nearby(ast, resolved, checked, src, hole.expected, hole.span.start, &mut out);
    }
    out
}

/// The locals visible at the hole, with their types — the list §4.16's example
/// prints. "Visible" is approximated by *the declaration the hole is in*, which
/// is what the resolver recorded and is exact for every shape the language has:
/// a local cannot be read outside the declaration that binds it.
fn in_scope(
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    at: u32,
    out: &mut String,
) {
    let Some(owner) = owner_of(ast, at) else { return };
    let mut shown: Vec<String> = Vec::new();
    for (index, local) in resolved.locals.iter().enumerate() {
        if local.owner != owner || local.name.start > at {
            continue;
        }
        let ty = checked.local_type(index);
        let rendered = super::render_ty(&checked.types, ast, src, ty, &[]);
        let mutable = if local.mutable { " (mutable)" } else { "" };
        shown.push(format!("{}: {rendered}{mutable}", src.slice(local.name)));
    }
    if shown.is_empty() {
        return;
    }
    out.push_str("  in scope:\n");
    for line in shown.iter().take(SUGGESTIONS) {
        out.push_str(&format!("    {line}\n"));
    }
    if shown.len() > SUGGESTIONS {
        out.push_str(&format!("    … and {} more\n", shown.len() - SUGGESTIONS));
    }
}

/// The fields of the expected type, where it is a record — because the hole is
/// most often a value that has to be built, and the fields are what it is built
/// from (§4.9: every field named, always).
fn fields_of(
    ast: &Ast,
    checked: &Checked,
    src: &Source,
    expected: super::TyId,
    out: &mut String,
) {
    let Ty::Named(decl) = checked.types.get(expected) else { return };
    match &ast.decls[decl as usize].kind {
        DeclKind::Record { fields } => {
            let shown: Vec<String> = fields
                .iter()
                .map(|f| {
                    format!(
                        "{}: {}",
                        src.slice(f.name),
                        crate::printer::render_type(ast, f.ty, src)
                    )
                })
                .collect();
            out.push_str(&format!(
                "  fields of {}: {}\n",
                src.slice(ast.decls[decl as usize].name),
                shown.join(", ")
            ));
        }
        DeclKind::Variant { cases } => {
            let shown: Vec<String> =
                cases.iter().map(|c| format!(".{}", src.slice(c.name))).collect();
            out.push_str(&format!(
                "  cases of {}: {}\n",
                src.slice(ast.decls[decl as usize].name),
                shown.join(", ")
            ));
        }
        _ => {}
    }
}

/// Functions whose result is the expected type, with their documentation —
/// §4.16's "nearby functions", ranked by type-relevance and capped.
///
/// Ranking by relevance is the whole reason this is useful rather than noisy: on
/// a file with forty functions, the five that *return what is needed here* are
/// the answer, and the other thirty-five are context flooding.
fn nearby(
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    expected: super::TyId,
    at: u32,
    out: &mut String,
) {
    // The hole's own module: a name from anywhere else can only be written
    // qualified, so that is how it is offered.
    let here = src.module_at(at).to_string();
    if checked.types.get(expected) == Ty::Error {
        return;
    }
    let mut shown: Vec<String> = Vec::new();
    // `Resolved::top` is sorted by name, so the order is deterministic and does
    // not vary with where the declarations sit in the file.
    for ((module, name), decl) in &resolved.top {
        let DeclKind::Function(function) = &ast.decls[*decl as usize].kind else { continue };
        if checked.result_type(*decl) != Some(expected) {
            continue;
        }
        let params: Vec<String> = function
            .params
            .iter()
            .map(|p| {
                format!(
                    "{}{}: {}",
                    if p.mutable { "@" } else { "" },
                    src.slice(p.name),
                    crate::printer::render_type(ast, p.ty, src)
                )
            })
            .collect();
        // A name from another module is offered the only way it can be
        // written: qualified. Offering `f` where only `geom.f` compiles is a
        // suggestion the reader has to repair (§4.16 is output, not a guess).
        //
        // **And only from a module this file can name.** The principle was
        // applied halfway: a hole in `geom.hero` was offered `main.tally(x: i64)`,
        // which `geom` cannot `use` without a module cycle — so writing the
        // suggestion is three errors, and §4.16's whole promise is that you are
        // handed the answer rather than made to guess (2026-08-12).
        let reachable = module == &here
            || module == crate::source::LIBRARY_MODULE
            || resolved.is_used_module(&here, module);
        if !reachable {
            continue;
        }
        let written = if module == &here { name.clone() } else { format!("{module}.{name}") };
        let mut line = format!("{written}({})", params.join(", "));
        if let Some(doc) = ast.decls[*decl as usize].doc.first() {
            line.push_str(&format!("\n      {}", src.slice(*doc)));
        }
        shown.push(line);
        if shown.len() == SUGGESTIONS {
            break;
        }
    }
    if shown.is_empty() {
        return;
    }
    out.push_str("  functions that return it:\n");
    for line in shown {
        out.push_str(&format!("    {line}\n"));
    }
}

/// Which declaration a byte offset falls inside.
fn owner_of(ast: &Ast, at: u32) -> Option<u32> {
    ast.decls
        .iter()
        .position(|decl| decl.span.start <= at && at < decl.span.end)
        .map(|index| index as u32)
}
