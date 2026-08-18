//! Expressions, patterns, and the root of a mutated place (§4.9 calls, §4.11
//! UFCS, §4.14 operators, §4.16 holes).
//!
//! Three questions are answered here and nowhere else:
//!
//! - **A bare name** is a local, a top-level declaration or a built-in, in
//!   that order — innermost first, which is the only ordering that makes
//!   "shadowing is an error" and "a file's own declaration wins over a library
//!   function" consistent with each other.
//! - **A name after a dot with an argument list** is a *function*: `x.f(y)` is
//!   `f(x, y)` and there are no methods (§4.11). So a misspelled one is
//!   answerable now, without a single type — which is what keeps it from being
//!   reported as a type error about a type nobody wrote. With one limit, and it
//!   is a veto the compiler-engineer earned in panel 015: §4.11's algorithm
//!   looks for a *field* first, and a field can hold a function value (§4.13),
//!   so `h.cb(n)` is legal and unanswerable without the receiver's type. The
//!   error therefore fires only when no record or variant in the whole file
//!   declares a field of that name — a type-free test that never rejects a
//!   legal program, and still catches every misspelling that is not also
//!   somebody's field.
//! - **The left of `@`** is a place, and every place has exactly one root
//!   (there are no references). Writing through a field or an index writes the
//!   root: `l.pos @ l.pos + 1` writes `l`.
//!
//! What is deliberately *not* resolved here: field names (`p.x`), variant case
//! names (`.num`) and argument labels. All three need a type to be answerable,
//! and inventing an answer without one is how a resolver starts guessing.

use crate::source::{Source, Span};
use crate::syntax::{Arg, ArmBody, Ast, DeclKind, ExprId, ExprKind, PatternKind};

use super::builtins::{index_of, BUILTINS};
use super::scope::Binding;
use super::{errors, stmts, Ref, Resolver};

pub(super) fn expr(r: &mut Resolver, ast: &Ast, src: &Source, id: ExprId) {
    match &ast.exprs[id.0 as usize].kind {
        ExprKind::Name => name(r, ast, src, id),
        // The module, not a flag: §4.16's suppression is file-wide, so a hole
        // here must not quiet the unused rule in a module nobody is editing.
        ExprKind::Hole => {
            r.out.holes_in.insert(r.module.clone());
        }
        ExprKind::Int
        | ExprKind::Float
        | ExprKind::Str
        | ExprKind::Char
        | ExprKind::Bool
        | ExprKind::NullPtr
        | ExprKind::Error => {}
        ExprKind::Unary { operand, .. } => expr(r, ast, src, *operand),
        ExprKind::Binary { left, right, .. } => {
            expr(r, ast, src, *left);
            expr(r, ast, src, *right);
        }
        // `p.x` — the base is a value, the field name is not a name in any
        // scope. Which record it belongs to is M-data-declarations's question.
        //
        // **Unless the base is a module**, in which case the three tokens are one
        // qualified name and not a field at all: `grid.WALL` reads a `constant`
        // next door. It is the same question `Method` asks one arm below, and it
        // has to be asked here for the same reason — a module has no type, so
        // anything that resolves the base as a value reports `grid` as a name
        // that is not a value and never reaches the point. Nothing asked it here
        // until a corpus program read a constant across a module (fixedbugs,
        // 2026-08-13): `use` binds *declarations*, and a `constant` is one.
        ExprKind::Field { base, name } => {
            if super::qualified::qualified(r, ast, src, id, *base, *name) {
                return;
            }
            expr(r, ast, src, *base);
        }
        ExprKind::Index { base, index } => {
            expr(r, ast, src, *base);
            expr(r, ast, src, *index);
        }
        ExprKind::Call { callee, args } => {
            expr(r, ast, src, *callee);
            arguments(r, ast, src, args);
        }
        ExprKind::Method { receiver, name: called, args } => {
            // **A module in receiver position is not a value**, so it is decided
            // before the receiver is resolved as one — otherwise `geom.dist2(a)`
            // reports `geom` as an unknown name and never gets to the point.
            // This is the disambiguation panel 031 Q2 asked for, and it is one
            // branch: `use` binds the name, so the resolver already knows.
            if super::qualified::qualified(r, ast, src, id, *receiver, *called) {
                arguments(r, ast, src, args);
                return;
            }
            expr(r, ast, src, *receiver);
            arguments(r, ast, src, args);
            method(r, ast, src, id, *called);
        }
        ExprKind::Case { args, .. } => arguments(r, ast, src, args),
        ExprKind::Array(elements) => {
            for element in elements {
                expr(r, ast, src, *element);
            }
        }
        ExprKind::Map(entries) => {
            for entry in entries {
                expr(r, ast, src, entry.key);
                expr(r, ast, src, entry.value);
            }
        }
        ExprKind::Try(inner) => expr(r, ast, src, *inner),
        ExprKind::If { branches, otherwise } => {
            for branch in branches {
                expr(r, ast, src, branch.cond);
                stmts::block(r, ast, src, &branch.block);
            }
            if let Some(block) = otherwise {
                stmts::block(r, ast, src, block);
            }
        }
        ExprKind::Match { scrutinee, arms } => {
            expr(r, ast, src, *scrutinee);
            for arm in arms {
                // One scope per arm: the payload bindings and the body live
                // together, and two arms may bind the same name.
                r.open_scope();
                for pattern in &arm.patterns {
                    match &pattern.kind {
                        PatternKind::Case { binding, .. } => {
                            if let Some(binding) = binding {
                                bind_payload(r, ast, src, *binding);
                            }
                        }
                        PatternKind::Literal(value) => expr(r, ast, src, *value),
                        PatternKind::Wildcard | PatternKind::Error => {}
                    }
                }
                match &arm.body {
                    ArmBody::Stmt(id) => stmts::statement(r, ast, src, *id),
                    ArmBody::Block(block) => stmts::statements(r, ast, src, &block.stmts),
                }
                r.close_scope();
            }
        }
    }
}

fn arguments(r: &mut Resolver, ast: &Ast, src: &Source, args: &[Arg]) {
    for arg in args {
        expr(r, ast, src, arg.value);
    }
}

/// A payload binding. Two patterns of one arm may not bind the same name:
/// under `|` the two cases carry different payloads, so one name would mean two
/// things depending on which case matched.
fn bind_payload(r: &mut Resolver, ast: &Ast, src: &Source, binding: Span) {
    let text = src.slice(binding);
    if text != "_" && r.bound_in_current_scope(text) {
        let diagnostic = errors::duplicate_pattern_binding(text, binding);
        r.push_diagnostic(diagnostic);
        return;
    }
    r.declare(ast, src, binding, Binding::payload());
}

fn name(r: &mut Resolver, ast: &Ast, src: &Source, id: ExprId) {
    let span = ast.exprs[id.0 as usize].span;
    let text = src.slice(span);
    if let Some(local) = r.lookup_local(text) {
        r.record(id, Ref::Local(local));
        return;
    }
    // **This file's own declarations, then the library's, and no other
    // file's.** One `top_visible` instead of a flat `get` is where "always
    // qualified" is enforced.
    if let Some(decl) = r.top_visible(text) {
        // A variant names a type, never a value: its cases are the values, and
        // they are written `.case` (§4.5's ⇐ mode).
        if matches!(ast.decls[decl as usize].kind, DeclKind::Variant { .. }) {
            let diagnostic = errors::variant_in_value_position(text, span);
            r.push_diagnostic(diagnostic);
            return;
        }
        r.record(id, Ref::Top(decl));
        return;
    }
    if let Some(builtin) = index_of(text) {
        r.record(id, Ref::Builtin(builtin));
        return;
    }
    // A module name standing alone. It is not a value in any position — it can
    // only be followed by a dot — and saying that is better than "unknown".
    if r.out.is_used_module(&r.module, text) {
        r.module_reads.insert((r.module.clone(), text.to_string()));
        let diagnostic = errors::module_is_not_a_value(text, span);
        r.push_diagnostic(diagnostic);
        return;
    }
    if let Some(diagnostic) = super::qualified::elsewhere(r, text, span) {
        r.push_diagnostic(diagnostic);
        return;
    }
    let candidates = value_candidates(r);
    let near = r.near_names(text, &candidates);
    let diagnostic = errors::unknown_name(text, &near, span);
    r.push_diagnostic(diagnostic);
}

/// The name in `x.f(y)`. Locals are deliberately not consulted: a local cannot
/// be reached through a dot, so `f` is a top-level function, a built-in, or —
/// the case that makes this file interesting — a *field* of the receiver holding
/// a function value.
///
/// That third case is why the entry is left `Unresolved` rather than reported
/// when the name is a field somewhere in the file. M-data-declarations finishes the job with the
/// receiver's type in hand, and §4.17 gets the whole answer in one message
/// ("no field `cb` on `Holder`, and no function `cb`") instead of two passes
/// each guessing half of it.
fn method(r: &mut Resolver, ast: &Ast, src: &Source, at: ExprId, called: Span) {
    let text = src.slice(called);
    if let Some(decl) = r.top_visible(text) {
        match &ast.decls[decl as usize].kind {
            DeclKind::Function(_) => r.record(at, Ref::Top(decl)),
            DeclKind::Record { .. } => {
                let diagnostic = errors::not_a_function(text, "a record", called);
                r.push_diagnostic(diagnostic);
            }
            DeclKind::Variant { .. } => {
                let diagnostic = errors::not_a_function(text, "a variant", called);
                r.push_diagnostic(diagnostic);
            }
            DeclKind::Constant { .. } => {
                let diagnostic = errors::not_a_function(text, "a constant", called);
                r.push_diagnostic(diagnostic);
            }
            DeclKind::Test { .. } => {}
        }
        return;
    }
    if let Some(builtin) = index_of(text) {
        r.record(at, Ref::Builtin(builtin));
        return;
    }
    if r.field_in_reach(text) {
        return; // could be a function-valued field: M-data-declarations decides, with the type
    }
    // `p.dist2(o)` where `dist2` is imported. UFCS finds only this file's
    // functions, and this is the message that says so — panel 031 R5 chose it
    // over eleven spec tokens, on the ground that the error carries the repair
    // and a sentence in the prompt carries only the rule.
    if let Some(diagnostic) = super::qualified::elsewhere(r, text, called) {
        r.push_diagnostic(diagnostic);
        return;
    }
    let candidates = function_candidates(r, ast);
    let near = r.near_names(text, &candidates);
    let diagnostic = errors::unknown_function(text, &near, called);
    r.push_diagnostic(diagnostic);
}

/// Innermost first: the locals in scope, then the file's own declarations, then
/// the built-ins. The order is the did-you-mean's ranking, and §4.16's rule
/// applies — deterministic, and capped where it is printed.
pub(super) fn value_candidates(r: &Resolver) -> Vec<String> {
    let mut candidates = r.visible_locals();
    candidates.extend(r.out.names_in(&r.module).map(|(n, _)| n.to_string()));
    candidates.extend(BUILTINS.iter().map(|b| b.name.to_string()));
    candidates
}

fn function_candidates(r: &Resolver, ast: &Ast) -> Vec<String> {
    let mut candidates: Vec<String> = r
        .out
        .names_in(&r.module)
        .filter(|(_, decl)| matches!(ast.decls[*decl as usize].kind, DeclKind::Function(_)))
        .map(|(name, _)| name.to_string())
        .collect();
    candidates.extend(BUILTINS.iter().map(|b| b.name.to_string()));
    candidates
}
