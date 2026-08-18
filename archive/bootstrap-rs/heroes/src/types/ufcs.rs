//! `x.f(y)` — the three tokens with two meanings (design.md §4.11, §4.8; spec
//! § Functions and calls; panel 015 D, panel 031).
//!
//! Split out of `calls.rs` by the §11 sweep. The concern is one question the
//! resolver deliberately **left open**: a dot can be UFCS (`f(x, y)`), a field
//! holding a function value, or module qualification (`geom.dist2(a, b)`) — and
//! only the first two need a type to tell apart, which is why they are answered
//! here and not there.
//!
//! §4.11's order is the order below and it is not an implementation detail: **a
//! field first**, then a free function. And two forms that look like the dot form
//! are not it — a qualified name is an ordinary name that happens to say where it
//! lives, so `geom.Point(x: 3, y: 4)` is a CONSTRUCTION, and routing it as a call
//! is how the emitter ends up asking clang to call a record.

use crate::resolve::{Ref, Resolved};
use crate::source::{Source, Span};
use crate::syntax::{Arg, Ast, DeclKind, ExprId};

use super::apply::user_call;
use super::calls::{builtin_call, indirect_call};
use super::construct::{construct_record, field_of_function_type};
use super::{errors, exprs, Checker, TyId};

/// `x.f(y)` — §4.11's algorithm, in the order §4.11 states it: **a field first**,
/// then a free function. The field half needs the receiver's type, which is why
/// the resolver deliberately left this half unanswered (panel 015 D).
pub(super) fn method(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    at: ExprId,
    receiver: ExprId,
    called: Span,
    args: &[Arg],
) -> TyId {
    let span = ast.exprs[at.0 as usize].span;
    // **`geom.dist2(a, b)` is a call with two arguments, not three.** The
    // resolver marked the receiver as a module, and the check has to come before
    // the receiver is synthesised: a module has no type, and asking for one is
    // how the qualified form turns into an unknown name.
    if resolved.use_at(receiver) == Ref::Module {
        return match resolved.use_at(at) {
            // The same three shapes `call` dispatches on, because a qualified
            // name is an ordinary name that happens to say where it lives.
            // `geom.Point(x: 3, y: 4)` is a CONSTRUCTION (§4.9), not a call, and
            // routing it through `user_call` is how the emitter ends up asking
            // clang to call a record.
            Ref::Top(decl) => match &ast.decls[decl as usize].kind {
                DeclKind::Record { .. } => {
                    construct_record(checker, ast, resolved, src, decl, args, span)
                }
                DeclKind::Function(_) => {
                    user_call(checker, ast, resolved, src, decl, args, None, span)
                }
                _ => checker.error_ty(),
            },
            // The resolver already said what is wrong with it.
            _ => checker.error_ty(),
        };
    }
    let receiver_ty = exprs::synth(checker, ast, resolved, src, receiver);
    let name = src.slice(called);
    if let Some(field) = field_of_function_type(checker, ast, resolved, src, receiver_ty, name) {
        return indirect_call(checker, ast, resolved, src, field, args, None, span);
    }
    match resolved.use_at(at) {
        Ref::Top(decl) => {
            // §4.8: UFCS does not apply when the first parameter is `@`, because
            // `l.advance()` would hide the mutation the marker exists to show.
            if let DeclKind::Function(function) = &ast.decls[decl as usize].kind {
                if function.params.first().is_some_and(|p| p.mutable) {
                    let diagnostic = errors::ufcs_on_mutable(name, called);
                    checker.push_diagnostic(diagnostic);
                    return checker.error_ty();
                }
            }
            user_call(checker, ast, resolved, src, decl, args, Some(receiver_ty), span)
        }
        Ref::Builtin(index) => {
            builtin_call(checker, ast, resolved, src, index, args, Some(receiver_ty), span)
        }
        // The resolver stayed silent because the name *might* have been a field.
        // Now the receiver's type is known, so both halves fit one message —
        // which is what the compiler-engineer's panel-015 veto asked for.
        // A module cannot be reached here: the branch above returns first.
        Ref::Module | Ref::Unresolved | Ref::Local(_) => {
            if !checker.out.types.poisoned(receiver_ty) {
                let holder = checker.show(ast, src, receiver_ty);
                let diagnostic = errors::no_field_and_no_function(&holder, name, called);
                checker.push_diagnostic(diagnostic);
            }
            checker.error_ty()
        }
    }
}
