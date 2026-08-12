//! Calls, construction, and UFCS erased (design.md §4.9, §4.11, §4.13, §4.19;
//! Part 5's rows for UFCS, named arguments and record construction).
//!
//! **Two of Part 5's rows cost nothing here, and the reason is a surface rule.**
//! A label in Heroes does not *reorder* an argument — `types/calls.rs` checks that
//! the label written at a position matches the parameter *at that position*
//! (§4.9). So "named arguments → positional, after checking labels" is erased by
//! simply not carrying the label: the arguments were already in declaration
//! order. UFCS costs almost as little: `x.f(y)` becomes `f(x, y)` by putting the
//! receiver first, and the resolver already recorded which `f` that is.
//!
//! **`Callee` is where this file earns its keep**, and it is panel 019's most
//! expensive finding. The ffi-pragmatist compiled two translation units in which
//! a Heroes `function open` reaches C unmangled: clang says nothing and the
//! program prints `7` instead of a file descriptor. `open` is not among the 23
//! reserved built-in names, so that program is legal Heroes today. Recording the
//! linkage here — Heroes, `extern`, built-in, or a pointer in a temporary — is
//! what stops the emitter from re-deriving it and getting it wrong.

use crate::resolve::{Ref, Resolved, BUILTINS};
use crate::source::{Source, Span};
use crate::syntax::{Arg as AstArg, Ast, DeclKind, ExprId, ExprKind};
use crate::types::{Checked, Ty, TyId};

use super::build::Lowering;
use super::inst::{Arg, Args, Callee, Op, Shape, ValueId};
use super::{exprs, fallible, layout, places};

/// A declaration as a call target. The `extern` split is not a detail: those
/// names pass through the mangler untouched, by design (CLAUDE.md §7).
pub(super) fn callee_of(ast: &Ast, decl: u32) -> Callee {
    match &ast.decls[decl as usize].kind {
        DeclKind::Function(function) if function.is_extern => Callee::Extern(decl),
        _ => Callee::Heroes(decl),
    }
}

pub(super) fn value_args(b: &mut Lowering, values: &[ValueId]) -> Args {
    let args: Vec<Arg> = values.iter().map(|v| Arg::Value(*v)).collect();
    b.args(&args)
}

/// `f(x)`, `Point(x: 3, y: 4)`, `ok(v)`, `fail(c, m)`, `h.cb(n)` — one syntax,
/// and what it *is* depends on what the callee resolved to.
#[allow(clippy::too_many_arguments)]
pub(super) fn call(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    callee: ExprId,
    args: &[AstArg],
    ty: TyId,
    span: Span,
) -> ValueId {
    if matches!(ast.exprs[callee.0 as usize].kind, ExprKind::Name) {
        match resolved.use_at(callee) {
            Ref::Top(decl) => {
                // Record construction *is* a call with named arguments (§4.9), so
                // the surface needs no node of its own — but the IR does, because
                // the layout decision belongs to M-value-aggregates's descriptor pass.
                if matches!(ast.decls[decl as usize].kind, DeclKind::Record { .. }) {
                    let run = lower_args(b, ast, resolved, checked, src, args);
                    return b.emit(Op::Construct { shape: Shape::Record(decl), args: run }, ty, span);
                }
                let run = lower_args(b, ast, resolved, checked, src, args);
                return emit_call(b, callee_of(ast, decl), run, false, ty, span);
            }
            Ref::Builtin(index) => {
                let name = BUILTINS[index as usize].name;
                // `ok`/`fail` build the two sides of a `T?` (§4.6, panel 002).
                // They are built-ins so that a name resolves, and constructions
                // here because that is what they do.
                if let Some(shape) = fallible::constructor(name) {
                    let run = lower_args(b, ast, resolved, checked, src, args);
                    return b.emit(Op::Construct { shape, args: run }, ty, span);
                }
                let run = lower_args(b, ast, resolved, checked, src, args);
                return emit_call(b, Callee::Builtin(index), run, is_variadic(name), ty, span);
            }
            // A local holding a function value (§4.13) — and `geom(...)`, which
            // the checker refused, since lowering only runs on a clean program.
            Ref::Module | Ref::Local(_) | Ref::Unresolved => {}
        }
    }
    let target = exprs::expr(b, ast, resolved, checked, src, callee);
    let run = lower_args(b, ast, resolved, checked, src, args);
    emit_call(b, Callee::Indirect(target), run, false, ty, span)
}

/// `x.f(y)` — UFCS, erased (§4.11, Part 5). Three things it can be, and the
/// resolver already decided which: a built-in, a top-level function, or a *field*
/// of the receiver holding a function value.
#[allow(clippy::too_many_arguments)]
pub(super) fn method(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    at: ExprId,
    receiver: ExprId,
    name: Span,
    args: &[AstArg],
    ty: TyId,
    span: Span,
) -> ValueId {
    // The resolver filed the method name's meaning at the `Method` node itself:
    // the name after the dot has no `ExprId` of its own (`resolve::Ref`).
    let target = resolved.use_at(at);
    // The qualified form, lowered as the plain call it is: the receiver names a
    // module, which is erased here exactly as it is erased in the C.
    if resolved.use_at(receiver) == Ref::Module {
        if let Ref::Top(decl) = target {
            let run = lower_args(b, ast, resolved, checked, src, args);
            // A record is constructed, not called — the same split `call` makes,
            // and the reason the module receiver is erased *here* rather than by
            // pretending it was never written.
            if matches!(ast.decls[decl as usize].kind, DeclKind::Record { .. }) {
                return b.emit(Op::Construct { shape: Shape::Record(decl), args: run }, ty, span);
            }
            return emit_call(b, callee_of(ast, decl), run, false, ty, span);
        }
    }
    if let Ref::Builtin(index) = target {
        let builtin = BUILTINS[index as usize].name;
        // `.must()`, `.default(v)` and `.is_err()` are branches, not calls
        // (Part 5): they read a tag and choose an edge.
        if fallible::is_operation(builtin) {
            return fallible::operation(
                b, ast, resolved, checked, src, builtin, receiver, args, ty, span,
            );
        }
    }
    match target {
        Ref::Builtin(index) => {
            let builtin = BUILTINS[index as usize].name;
            let run = with_receiver(b, ast, resolved, checked, src, receiver, args);
            emit_call(b, Callee::Builtin(index), run, is_variadic(builtin), ty, span)
        }
        Ref::Top(decl) => {
            let run = with_receiver(b, ast, resolved, checked, src, receiver, args);
            emit_call(b, callee_of(ast, decl), run, false, ty, span)
        }
        // `h.cb(n)` where `cb` is a field holding a function value: the dot is a
        // *field read*, not a receiver, so the arguments are only the written ones
        // (§4.11's first lookup, §4.13).
        // A module receiver never reaches here — the branch above returns.
        Ref::Module | Ref::Local(_) | Ref::Unresolved => {
            let owner = checked.expr_types[receiver.0 as usize];
            let base = exprs::expr(b, ast, resolved, checked, src, receiver);
            let field = layout::field_index(ast, checked, src, owner, name);
            // The field's own type is a function type, and nothing here can intern
            // one: `Checked` is read-only by design. It is typed with the call's
            // result for now — function values land at M-generics-library, which is where this
            // path is first exercised (§4.13).
            let function_ty = ty;
            let target = match field {
                Some(index) => b.emit(Op::Field { base, index }, function_ty, span),
                None => b.emit(Op::Missing, function_ty, span),
            };
            let run = lower_args(b, ast, resolved, checked, src, args);
            emit_call(b, Callee::Indirect(target), run, false, ty, span)
        }
    }
}

/// `.plus`, `.num(v: 12)` — a variant case, its variant coming from the type the
/// checker recorded (§4.5's ⇐ mode).
#[allow(clippy::too_many_arguments)]
pub(super) fn case(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    name: Span,
    args: &[AstArg],
    ty: TyId,
    span: Span,
) -> ValueId {
    let Ty::Named(decl) = checked.types.get(ty) else {
        return b.emit(Op::Missing, ty, span);
    };
    let Some(index) = layout::case_index(ast, src, decl, name) else {
        return b.emit(Op::Missing, ty, span);
    };
    let run = lower_args(b, ast, resolved, checked, src, args);
    b.emit(Op::Construct { shape: Shape::Case(decl, index), args: run }, ty, span)
}

/// A call, with the one thing the C proved: **at most one destination.** `dst =
/// call print(x)` is a hard clang error and raylib is almost entirely
/// void-returning, so a `()` result produces no destination at all — the value
/// handed back is the function-wide unit, which no instruction defines.
fn emit_call(
    b: &mut Lowering,
    callee: Callee,
    args: Args,
    variadic: bool,
    ty: TyId,
    span: Span,
) -> ValueId {
    let op = Op::Call { callee, args, variadic };
    if ty == b.unit_ty() {
        b.emit_void(op, ty, span);
        return b.unit_value();
    }
    b.emit(op, ty, span)
}

/// Lowers the written arguments, left to right.
///
/// `@x` at the call site becomes a **place**, not a value: the callee copies in
/// and copies out, so it needs to know where to write back (§4.8). Everything
/// else is an ordinary value, and the label is dropped here — it was checked
/// against the parameter at this position and carries no further meaning.
fn lower_args(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    args: &[AstArg],
) -> Args {
    let mut lowered = Vec::new();
    for arg in args {
        if arg.mutable {
            let place = places::place(b, ast, resolved, checked, src, arg.value);
            lowered.push(Arg::InOut(place));
        } else {
            let value = exprs::expr(b, ast, resolved, checked, src, arg.value);
            lowered.push(Arg::Value(value));
        }
    }
    b.args(&lowered)
}

/// The UFCS shape: the receiver becomes argument zero. §4.11 forbids this where
/// the first parameter is `@`, so the receiver is always a value.
fn with_receiver(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    receiver: ExprId,
    args: &[AstArg],
) -> Args {
    let first = exprs::expr(b, ast, resolved, checked, src, receiver);
    let mut lowered = vec![Arg::Value(first)];
    for arg in args {
        if arg.mutable {
            let place = places::place(b, ast, resolved, checked, src, arg.value);
            lowered.push(Arg::InOut(place));
        } else {
            let value = exprs::expr(b, ast, resolved, checked, src, arg.value);
            lowered.push(Arg::Value(value));
        }
    }
    b.args(&lowered)
}

/// The one call shape clang verifies nothing about (§4.19). `print` today; at M-ffi-ladder
/// it is `TextFormat` and `sqlite3_mprintf`, where the ffi-pragmatist measured a
/// wrong argument count compiling, running, and printing garbage.
fn is_variadic(name: &str) -> bool {
    name == "print"
}
