//! The left of `@` — where resolving a name is not the same as reading it
//! (design.md §4.4, §4.8; panel 015 B).
//!
//! Split out of `exprs.rs` by the §11 sweep. Everywhere else in the resolver an
//! occurrence of a name is a **read**, and that is what the unused rule counts.
//! Here it is not: in `v @ v + 1` the root on the left is *written*, and any index
//! on the way — the `i` of `xs[i] @ …` — is *read*. Keeping the two rules in one
//! file is what makes "a read is a use and a write is not" implementable at all,
//! and spec line 87 states exactly that, with the `@` parameter as its one
//! exception.

use crate::source::Source;
use crate::syntax::{Ast, ExprId, ExprKind};

use super::builtins::index_of;
use super::errors;
use super::exprs::{expr, value_candidates};
use super::{Ref, Resolver};

/// The left of `@`. Resolving it is not the same as reading it: the root is
/// *written*, and any index on the way (`xs[i] @ …`) is read.
pub(super) fn place(r: &mut Resolver, ast: &Ast, src: &Source, id: ExprId) {
    match &ast.exprs[id.0 as usize].kind {
        ExprKind::Name => write_root(r, ast, src, id),
        ExprKind::Field { base, .. } => place(r, ast, src, *base),
        ExprKind::Index { base, index } => {
            expr(r, ast, src, *index);
            place(r, ast, src, *base);
        }
        // Not a place at all — the parser already said so (`not_a_place`).
        _ => expr(r, ast, src, id),
    }
}

fn write_root(r: &mut Resolver, ast: &Ast, src: &Source, id: ExprId) {
    let span = ast.exprs[id.0 as usize].span;
    let text = src.slice(span);
    if let Some(local) = r.lookup_local(text) {
        if !r.out.locals[local as usize].mutable {
            let kind = r.out.locals[local as usize].kind;
            let diagnostic = errors::not_mutable(text, kind, span);
            r.push_diagnostic(diagnostic);
            return;
        }
        // A write is recorded, and it is *not* a read: `v @ 1` alone leaves `v`
        // unused. Writing through a field or an index (`l.pos @ …`) is a write
        // to `l` for the same reason — with no aliasing anywhere (§4.10), a
        // write nobody reads back is unobservable. Go left this hole open
        // (golang/go#20802); Heroes closes it because it has no aliases.
        r.out.uses[id.0 as usize] = Ref::Local(local);
        r.out.locals[local as usize].writes += 1;
        return;
    }
    if r.top_visible(text).is_some() {
        let diagnostic = errors::no_mutable_globals(text, "top-level declaration", span);
        r.push_diagnostic(diagnostic);
        return;
    }
    if index_of(text).is_some() {
        let diagnostic = errors::no_mutable_globals(text, "built-in", span);
        r.push_diagnostic(diagnostic);
        return;
    }
    let candidates = value_candidates(r);
    let near = r.near_names(text, &candidates);
    let diagnostic = errors::unknown_name(text, &near, span);
    r.push_diagnostic(diagnostic);
}
