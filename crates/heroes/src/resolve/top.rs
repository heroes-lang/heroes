//! The top level, collected before any body is looked at — which is the whole
//! of "declaration order never matters" (§4.2).
//!
//! Mutual recursion needs no forward declarations because of this file: by the
//! time `factor` is resolved, `group` is already in the table, and vice versa.
//! Nothing else in the compiler ever needs a two-pass trick again.
//!
//! One namespace, not two. A `record` puts its name in the table *and* gives the
//! language its constructor (`Point(x: 3, y: 4)` is a call, §4.9), while a
//! `variant` name is only ever a type — its cases are written `.num`, and which
//! variant they belong to comes from context (§4.5's ⇐ mode). Keeping both in
//! one table is what makes `variant Token` and `record Token` in one file a
//! collision instead of a puzzle.
//!
//! The same walk collects every **field name** in the file. That set exists for
//! one narrow job, described in `exprs::method`: keeping the unknown-function
//! error off a field that holds a function value.

use crate::source::Source;
use crate::syntax::{Ast, DeclKind};

use super::builtins::index_of;
use super::errors;
use super::Resolver;

pub(super) fn collect(r: &mut Resolver, ast: &Ast, src: &Source) {
    for (index, decl) in ast.decls.iter().enumerate() {
        match &decl.kind {
            DeclKind::Record { fields } => {
                for field in fields {
                    let module = src.file(field.name.start).module.clone();
                    r.fields.insert((module, src.slice(field.name).to_string()));
                }
            }
            DeclKind::Variant { cases } => {
                for case in cases {
                    for field in &case.fields {
                        let module = src.file(field.name.start).module.clone();
                        r.fields.insert((module, src.slice(field.name).to_string()));
                    }
                }
            }
            _ => {}
        }
        // A `test`'s name is its title — a string literal, quotes included
        // (§4.18). It names nothing, and nothing can refer to it.
        if matches!(decl.kind, DeclKind::Test { .. }) {
            continue;
        }
        let name = src.slice(decl.name).to_string();
        let module = src.module_at(decl.name.start).to_string();
        let in_library = src.is_library(decl.name.start);
        // Every built-in name is taken, in both of §1.11's tiers — the tier says
        // where the implementation comes from, not whether the name is free
        // (see `builtins.rs`). …except in the library itself, which is where
        // those names get their implementations: it declares `range` because
        // `range` is its job.
        //
        // **This runs BEFORE the duplicate check, and the order is the message.**
        // Tier 2's implementations are real declarations now, so a user file
        // declaring `range` collides with one — and reported as a duplicate it
        // would say "the declaration at line 30", naming a line in a file the
        // author cannot open. `builtin_name_taken` says the true thing: the
        // language spent that name, wherever its body happens to live.
        if index_of(&name).is_some() && !in_library {
            let diagnostic = errors::builtin_name_taken(&name, decl.name);
            r.push_diagnostic(diagnostic);
            // The table keeps the library's entry rather than the author's, so
            // every *call* in the file still resolves to something with the right
            // signature: one mistake, one diagnostic.
            continue;
        }
        // Keyed by module: two modules may each declare `Point`, and they are
        // two types. Only a *second* declaration in the SAME module is the
        // duplicate spec line 76 refuses.
        if let Some(previous) = r.out.top.get(&(module.clone(), name.clone())) {
            let (line, _) = src.line_col(ast.decls[*previous as usize].name.start);
            let diagnostic = errors::declared_twice(&name, line, decl.name);
            r.push_diagnostic(diagnostic);
            continue;
        }
        r.out.top.insert((module, name), index as u32);
    }
    uses(r, ast, src);
}

/// Every `use` line, and the two things that make one wrong on its own.
///
/// **`use` binds** (panel 031 R3), so a module name is an ordinary name in its
/// file: declaring something with that name is the shadowing error spec line 76
/// already has, and never reading it is the unused error spec line 74 already
/// has. Neither needed a word of specification; both need a table, and this is
/// where it is built.
fn uses(r: &mut Resolver, ast: &Ast, src: &Source) {
    for (index, used) in ast.uses.iter().enumerate() {
        let from = src.module_at(used.span.start).to_string();
        let named = src.slice(used.name).to_string();
        if from == named {
            // A module using itself. `modules::graph` calls it a cycle, which it
            // is; nothing more is owed here, and a second message would be a
            // second diagnostic for one mistake.
            continue;
        }
        if r.out.module_uses.contains_key(&(from.clone(), named.clone())) {
            let diagnostic = errors::used_twice(&named, used.name);
            r.push_diagnostic(diagnostic);
            continue;
        }
        if let Some(&decl) = r.out.top.get(&(from.clone(), named.clone())) {
            let (line, _) = src.line_col(ast.decls[decl as usize].name.start);
            let diagnostic = errors::use_shadows_a_declaration(&named, line, used.name);
            r.push_diagnostic(diagnostic);
            continue;
        }
        r.out.module_uses.insert((from, named), index as u32);
    }
}

/// What no expression and no written type ever named. Runs after every body,
/// because a `use` may be read anywhere in its file.
pub(super) fn unused_uses(r: &mut Resolver, ast: &Ast, src: &Source) {
    // §4.16's exemption applies here for the same reason it applies to a
    // binding: a `???` is a program that is not finished, and the module it was
    // going to reach may be the one the hole would have used. It is asked per
    // *writing* module — `key.0` — because the suppression is file-wide.
    let unread: Vec<(String, String)> = r
        .out
        .module_uses
        .keys()
        .filter(|key| !r.module_reads.contains(*key))
        .filter(|key| !r.out.holes_in.contains(&key.0))
        .cloned()
        .collect();
    for key in unread {
        let index = r.out.module_uses[&key];
        let used = &ast.uses[index as usize];
        if src.is_library(used.span.start) {
            continue;
        }
        let diagnostic = errors::unused_use(&key.1, used.name);
        r.push_diagnostic(diagnostic);
    }
}
