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
//! one table is what makes `Token = variant` and `Token = record` in one file a
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
                    r.fields.insert(src.slice(field.name).to_string());
                }
            }
            DeclKind::Variant { cases } => {
                for case in cases {
                    for field in &case.fields {
                        r.fields.insert(src.slice(field.name).to_string());
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
        if let Some(previous) = r.out.top.get(&name) {
            let (line, _) = src.line_col(ast.decls[*previous as usize].name.start);
            let diagnostic = errors::declared_twice(&name, line, decl.name);
            r.push_diagnostic(diagnostic);
            continue;
        }
        // Every built-in name is taken, in both of §1.11's tiers — the tier says
        // where the implementation comes from, not whether the name is free
        // (see `builtins.rs`). The table is filled anyway, so the rest of the
        // file resolves against what the author wrote rather than against a name
        // it was just told not to use: one mistake, one diagnostic.
        if index_of(&name).is_some() {
            let diagnostic = errors::builtin_name_taken(&name, decl.name);
            r.push_diagnostic(diagnostic);
        }
        r.out.top.insert(name, index as u32);
    }
}
