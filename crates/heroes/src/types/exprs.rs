//! ⇒ — synthesise an expression's type (design.md §4.5, §4.9, §4.14, §4.16).
//!
//! `synth` is **total**: it always returns a type, because `return`, `break` and
//! `continue` are *statements* in this language, so the checker is never asked
//! "what is the type of `break`". Divergence therefore lives in `stmts.rs`, where
//! it belongs, and reaches expressions only as a `None` branch in `join.rs`.
//!
//! The ⇐ half is next door in `expect.rs`. Four forms have no type of their own
//! and live only there — an empty literal, `ok`/`fail`, a variant case, and `???`
//! — and `contextual` below is the list, in one place, so every rule that needs
//! to know agrees about what it contains.

use crate::resolve::{Ref, Resolved};
use crate::source::{Source, Span};
use crate::syntax::{Ast, DeclKind, ExprId, ExprKind};

use super::access::{field_type, index_type, try_type};
use super::expect::check;
use super::join::{join_for_value, Branch};
use super::stmts::Want;
use super::table::Ty;
use super::{calls, errors, ops, stmts, Checker, Hole, TyId};

pub(super) fn synth(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    id: ExprId,
) -> TyId {
    let span = ast.exprs[id.0 as usize].span;
    let ty = match &ast.exprs[id.0 as usize].kind {
        ExprKind::Int => checker.out.types.int(),
        // §4.3: a character literal *is* an `int`.
        ExprKind::Char => checker.out.types.int(),
        ExprKind::Float => checker.out.types.f64(),
        ExprKind::Str => checker.out.types.str(),
        ExprKind::Bool => checker.out.types.bool(),
        ExprKind::Error => checker.error_ty(),
        ExprKind::Name => name(checker, ast, resolved, src, id, span),
        // In value position a hole has no expected type to report — the ⇐ path
        // is where §4.16 earns its keep.
        ExprKind::Hole => {
            let expected = checker.error_ty();
            checker.out.holes.push(Hole { at: id, span, expected });
            expected
        }
        ExprKind::Unary { op, operand } => {
            let operand_ty = synth(checker, ast, resolved, src, *operand);
            ops::unary(checker, ast, src, *op, operand_ty, span)
        }
        ExprKind::Binary { op, left, right } => {
            // `t == .plus` and `p.here() != .rparen`: a case has no type of its
            // own (§4.5's ⇐ mode), so in an equality it takes the *other* side's.
            // One rule, both directions, and only for the two operators where
            // both operands must have the same type anyway.
            // Every binary operator in this language wants both operands to have
            // the same type — arithmetic and comparison by §4.14, equality by
            // §4.3, `&&`/`||` because both are `bool`. So whenever *one* side has
            // no type of its own, the other side's is the expectation: that is
            // what makes `t == .plus` writable, and what gives the `???` in
            // `"n = " + ???` an expected type to report (§4.16).
            //
            // The operator still decides the *result*: this only settles what the
            // operands are, and `ops::binary` does the rest, so a comparison
            // still yields `bool` and an addition still yields its operands' type.
            let (left_ty, right_ty) = if contextual(ast, *right) && !contextual(ast, *left) {
                let left_ty = synth(checker, ast, resolved, src, *left);
                check(checker, ast, resolved, src, *right, left_ty);
                (left_ty, left_ty)
            } else if contextual(ast, *left) && !contextual(ast, *right) {
                let right_ty = synth(checker, ast, resolved, src, *right);
                check(checker, ast, resolved, src, *left, right_ty);
                (right_ty, right_ty)
            } else {
                let left_ty = synth(checker, ast, resolved, src, *left);
                let right_ty = synth(checker, ast, resolved, src, *right);
                (left_ty, right_ty)
            };
            ops::binary(checker, ast, src, *op, left_ty, right_ty, span)
        }
        ExprKind::Field { base, name: field } => {
            let base_ty = synth(checker, ast, resolved, src, *base);
            field_type(checker, ast, resolved, src, base_ty, *field)
        }
        ExprKind::Index { base, index } => {
            let base_ty = synth(checker, ast, resolved, src, *base);
            let index_ty = synth(checker, ast, resolved, src, *index);
            index_type(checker, ast, src, base_ty, index_ty, span)
        }
        ExprKind::Call { callee, args } => {
            calls::call(checker, ast, resolved, src, id, *callee, args)
        }
        ExprKind::Method { receiver, name: called, args } => {
            calls::method(checker, ast, resolved, src, id, *receiver, *called, args)
        }
        // Which variant a case belongs to comes from context (§4.5's ⇐ mode), so
        // there is nothing to synthesise from.
        ExprKind::Case { name: case, .. } => {
            let diagnostic = errors::cannot_infer_case(src.slice(*case), span);
            checker.push_diagnostic(diagnostic);
            checker.error_ty()
        }
        ExprKind::Array(items) => {
            if items.is_empty() {
                let diagnostic = errors::cannot_infer_empty("array literal", span);
                checker.push_diagnostic(diagnostic);
                return checker.record(id, checker.out.types.error());
            }
            let first = synth(checker, ast, resolved, src, items[0]);
            for item in &items[1..] {
                check(checker, ast, resolved, src, *item, first);
            }
            checker.out.types.intern(Ty::Array(first))
        }
        ExprKind::Map(entries) => {
            if entries.is_empty() {
                let diagnostic = errors::cannot_infer_empty("map literal", span);
                checker.push_diagnostic(diagnostic);
                return checker.record(id, checker.out.types.error());
            }
            let key = synth(checker, ast, resolved, src, entries[0].key);
            let value = synth(checker, ast, resolved, src, entries[0].value);
            for entry in &entries[1..] {
                check(checker, ast, resolved, src, entry.key, key);
                check(checker, ast, resolved, src, entry.value, value);
            }
            checker.out.types.intern(Ty::Map(key, value))
        }
        ExprKind::Try(inner) => {
            let inner_ty = synth(checker, ast, resolved, src, *inner);
            try_type(checker, ast, src, inner_ty, span)
        }
        ExprKind::If { branches, otherwise } => {
            let mut collected: Vec<Branch> = Vec::new();
            for branch in branches {
                let bool_ty = checker.out.types.bool();
                let cond = synth(checker, ast, resolved, src, branch.cond);
                if cond != bool_ty && !checker.out.types.poisoned(cond) {
                    let got = checker.show(ast, src, cond);
                    let at = ast.exprs[branch.cond.0 as usize].span;
                    let diagnostic = errors::not_bool("an `if` condition", &got, at);
                    checker.push_diagnostic(diagnostic);
                }
                let value = stmts::block(checker, ast, resolved, src, &branch.block, Want::Unknown).1;
                collected.push(Branch { value, span: branch.block.span });
            }
            match otherwise {
                Some(block) => {
                    let value = stmts::block(checker, ast, resolved, src, block, Want::Unknown).1;
                    collected.push(Branch { value, span: block.span });
                }
                // Without an `else` the value is missing whenever the condition
                // is false, so an `if` used as a value must have one.
                None => {
                    let diagnostic = errors::if_without_else(span);
                    checker.push_diagnostic(diagnostic);
                    return checker.record(id, checker.out.types.error());
                }
            }
            join_for_value(checker, ast, src, &collected, "if", span)
        }
        ExprKind::Match { scrutinee, arms } => {
            let subject = synth(checker, ast, resolved, src, *scrutinee);
            let collected =
                super::arms::arms(checker, ast, resolved, src, arms, subject, Want::Unknown, span);
            join_for_value(checker, ast, src, &collected, "match", span)
        }
    };
    checker.record(id, ty)
}



/// True where an expression has no type of its own and must be checked against
/// one — §4.5's ⇐-only forms, listed in one place so every rule that needs to
/// know agrees about what they are.
fn contextual(ast: &Ast, id: ExprId) -> bool {
    match &ast.exprs[id.0 as usize].kind {
        ExprKind::Case { .. } | ExprKind::Hole => true,
        ExprKind::Array(items) => items.is_empty(),
        ExprKind::Map(entries) => entries.is_empty(),
        _ => false,
    }
}



fn name(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    id: ExprId,
    span: Span,
) -> TyId {
    match resolved.use_at(id) {
        Ref::Local(index) => checker.out.local_types[index as usize],
        Ref::Top(decl) => match &ast.decls[decl as usize].kind {
            // §4.13: a top-level function is a value, and its type is its
            // signature.
            DeclKind::Function(_) => calls::signature_of(checker, ast, resolved, decl),
            DeclKind::Constant { ty, .. } => super::lower::ty(checker, ast, resolved, *ty),
            // A record name is a constructor, not a value on its own: it is only
            // meaningful as the callee of a call (§4.9).
            DeclKind::Record { .. } => {
                let what = src.slice(ast.decls[decl as usize].name).to_string();
                let diagnostic = errors::record_name_alone(&what, span);
                checker.push_diagnostic(diagnostic);
                checker.error_ty()
            }
            _ => checker.error_ty(),
        },
        // **A built-in used as a value.** The comment here used to say this was
        // "only reachable as a callee" and it was false: `f = to_str` and
        // `map(xs, to_str)` both reach it, and an `error_ty()` with no diagnostic
        // is poison — it flowed to the emitter and produced `typedef HeroValue
        // (*h_m_fn0)(int64_t)`, exit 2. Found by M6's closure-list audit.
        Ref::Builtin(index) => {
            let what = crate::resolve::BUILTINS[index as usize].name;
            let diagnostic = errors::builtin_as_value(what, span);
            checker.push_diagnostic(diagnostic);
            checker.error_ty()
        }
        Ref::Unresolved => checker.error_ty(),
    }
}
