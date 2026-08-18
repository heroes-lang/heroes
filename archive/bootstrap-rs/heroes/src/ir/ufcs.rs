//! `x.f(y)` — the three tokens with two meanings, lowered (design.md §4.11, §4.13).
//!
//! Split from `ir/calls.rs` by CLAUDE.md §11, on the seam `types/calls.rs` was cut
//! on one pass earlier: the dot form is its own question, because **the same three
//! tokens are either a call to a free function with the receiver first, or a read
//! of a field that holds a function value** — and only the receiver's type settles
//! which.
//!
//! **Both halves live here on purpose, and panel 079 is why.** They used to be a
//! branch of one `match` on what the resolver said, and the resolver answers
//! `Ref::Top` whenever a free function of that name exists — whether or not the
//! receiver has the field. `types/ufcs.rs` asks the field first, as design.md:1534
//! states; this pass did not, so the two disagreed and the disagreement had two
//! faces, both measured: a printed answer from the free function while the checker
//! had typed the field's (**50007 against 100, exit 0, no diagnostic**), and, where
//! the two result types differed, a type confusion the type system did not catch
//! and clang did — *"assigning to 'int64_t' from incompatible type 'HeroStr'"*, at
//! **exit 2**, the compiler blaming itself.
//!
//! One rule, one file, asked once: the field first, then whatever the resolver
//! said.

use crate::resolve::{Ref, Resolved, BUILTINS};
use crate::source::{Source, Span};
use crate::syntax::{Arg as AstArg, Ast, DeclKind, ExprId};
use crate::types::{Checked, Ty, TyId};

use super::build::Lowering;
use super::calls::{callee_of, emit_call, is_variadic, lower_args, with_receiver};
use super::ids::ValueId;
use super::inst::{Callee, CastKind, Op, Shape};
use super::{exprs, fallible, layout};

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
    // **§4.11's order, asked HERE too, because this pass used to answer it
    // differently from the checker** (panel 079). `types/ufcs.rs` tries the field
    // first and falls back to a free function; this `match` asked the *resolver*,
    // which answers `Ref::Top` whenever a free function of that name exists —
    // whether or not the receiver has the field. So the two passes disagreed, and
    // the disagreement had two faces, both measured:
    //
    // - with matching result types, `s.g(50)` **printed the free function's
    //   answer** while the checker had typed the field's — 50007 against 100, at
    //   exit 0, with no diagnostic anywhere;
    // - with differing result types the checker typed the expression from the
    //   field and this pass called the function, and the only thing that noticed
    //   was clang: *"assigning to 'int64_t' from incompatible type 'HeroStr'"*,
    //   **exit 2**, the compiler blaming itself for a program the author is
    //   entitled to write. A type confusion the type system did not catch.
    //
    // design.md:1534 states the order — *"on seeing a dot, first looks for a
    // field; failing that, looks for a free function"* — and `ufcs.rs`'s own doc
    // says it "is not an implementation detail". Under CLAUDE.md §12 the compiler
    // had the bug, in this pass. Measured over 268 corpus files: **zero** programs
    // change meaning, because none declares a function-typed field whose name is
    // also a top-level function.
    if let Some(field_ty) =
        layout::field_type(ast, checked, src, checked.expr_types[receiver.0 as usize], name)
    {
        if matches!(checked.types.get(field_ty), Ty::Func { .. }) {
    return field_call(b, ast, resolved, checked, src, receiver, name, args, ty, span);
        }
    }
    match target {
        Ref::Builtin(index) => {
    let builtin = BUILTINS[index as usize].name;
    // `s.cstr()` — the same cast the plain-call form makes, and the shape
    // every program actually writes (§4.11's UFCS). Taken before the
    // arguments are lowered, because a cast has exactly one operand and
    // it is the receiver.
    if builtin == "cstr" {
        let operand = exprs::expr(b, ast, resolved, checked, src, receiver);
        return b.emit(Op::Cast { kind: CastKind::StrToCstr, operand }, ty, span);
    }
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
    field_call(b, ast, resolved, checked, src, receiver, name, args, ty, span)
        }
    }
}

/// `h.cb(n)` where `cb` is a field holding a function value: the dot is a **field
/// read**, not a receiver, so the arguments are only the written ones (§4.11's
/// first lookup, §4.13).
///
/// Split out by panel 079 so the two callers cannot drift: the resolver's
/// fallback reaches it, and so does §4.11's field-first order above.
#[allow(clippy::too_many_arguments)]
fn field_call(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    receiver: ExprId,
    name: Span,
    args: &[AstArg],
    ty: TyId,
    span: Span,
) -> ValueId {
    let owner = checked.expr_types[receiver.0 as usize];
    let base = exprs::expr(b, ast, resolved, checked, src, receiver);
    let field = layout::field_index(ast, checked, src, owner, name);
    // **The field's own type, read from the declaration that wrote it.**
    // This used to be `ty` — the *call's result* — under a comment saying
    // nothing here could intern a function type, which was true and
    // beside the point: the type is already interned, in the record's
    // declaration, and `field_type` reads it. The comment's real premise
    // was *"function values land at M-generics-library, which is where
    // this path is first exercised"*, and that expired when the milestone
    // landed. Measured: `h.f(21)` on `record Holder { f: (function(i64)
    // -> i64) }` emitted `int64_t t4 = t3.f_f;` and then called it,
    // giving `incompatible pointer to integer conversion` at **exit 2** —
    // the compiler blaming itself for a correct program (panel 068 F8).
    //
    // The fallback is the old behaviour and stays deliberately: a field
    // the checker could not resolve has already been reported, and
    // lowering must stay quiet rather than invent a second message.
    let function_ty =
        layout::field_type(ast, checked, src, owner, name).unwrap_or(ty);
    let target = match field {
        Some(index) => b.emit(Op::Field { base, index }, function_ty, span),
        None => b.emit(Op::Missing, function_ty, span),
    };
    let run = lower_args(b, ast, resolved, checked, src, args);
    emit_call(b, Callee::Indirect(target), run, false, ty, span)
}
