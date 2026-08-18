//! Expressions → one temporary holding the value (design.md §4.9, §4.14, §4.16).
//!
//! **Flattening is the whole job.** `x = 2 + 3 * 4` is a tree in the syntax and
//! three instructions here, each naming temporaries and nothing nested. That is
//! what makes the C emitter a printer rather than a second compiler, and what
//! makes the ownership pass (M-strings-ownership) able to point at a single instruction and say
//! "this one produced a value somebody owns".
//!
//! Literals are decoded *here*, and that is a first: nothing before this pass
//! ever needed an `i64` literal's value, so an out-of-range one was invisible
//! until now. It is the one diagnostic this pass owns, and panel 019 recorded it
//! as a new class needing a panel of its own.
//!
//! Two rows of Part 5's table are erased in this file, both by looking at a type
//! the checker already worked out: `m[k]` is a different instruction from `xs[i]`
//! because a map access yields a `V?` and cannot abort (§4.9), and `&&`/`||` are
//! not operators at all — they short-circuit, so they are control flow
//! (`control.rs`).

use crate::lexer::unescape;
use crate::resolve::{Ref, Resolved};
use crate::source::{Source, Span};
use crate::syntax::{Ast, BinaryOp, ExprId, ExprKind, UnaryOp};
use crate::types::{Checked, Ty, TyId};

use super::build::Lowering;
use super::inst::{BinOp, Callee, Const, Op, UnOp};
use super::ids::{Args, ValueId};
use super::{calls, control, fallible, layout};

pub(super) fn expr(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    id: ExprId,
) -> ValueId {
    let node = &ast.exprs[id.0 as usize];
    let span = node.span;
    let ty = checked.expr_types[id.0 as usize];
    match &node.kind {
        ExprKind::Int => {
            let value = int_literal(b, src, span);
            b.emit(Op::Const(Const::Int(value)), ty, span)
        }
        ExprKind::Float => {
            // **`_` is a digit separator and Rust's parser does not take one**, so
            // it comes out before the parse — and the sentence that used to stand
            // here (*"the parse cannot fail: the lexer already accepted the
            // shape"*) was a premise written as a justification, falsified by the
            // shortest program that uses the separator the spec grants:
            // `x: f64 @ 1_0.5` printed **0.0** at exit 0, because
            // `unwrap_or(0.0)` swallowed the error (measured 2026-08-18, panel
            // 085 R6's sweep; `lexer/number.rs` accepts `_` between any two
            // digits on both sides of the point).
            //
            // The fallback is now **loud**, which is CLAUDE.md §11's rule and
            // the whole lesson of this defect: after the separators are gone,
            // every shape `lexer/number.rs` can produce is `digits[.digits]`,
            // which Rust parses, and an out-of-range literal is an infinity
            // rather than an error. So a failure here is the compiler's bug and
            // says so instead of inventing a number.
            let text = src.slice(span);
            let separatorless: String = text.chars().filter(|c| *c != '_').collect();
            let value = separatorless.parse::<f64>().unwrap_or_else(|why| {
                unreachable!("the lexer accepted `{text}` as a float literal: {why}")
            });
            b.emit(Op::Const(Const::Float(value)), ty, span)
        }
        ExprKind::Bool => {
            let value = src.slice(span) == "true";
            b.emit(Op::Const(Const::Bool(value)), ty, span)
        }
        ExprKind::NullPtr => b.emit(Op::Const(Const::NullPtr), ty, span),
        ExprKind::Str => {
            let text = unescape(src, span);
            let id = b.intern(text);
            b.emit(Op::Const(Const::Str(id)), ty, span)
        }
        ExprKind::Char => {
            // §4.3: a character literal *is* an `i64`. The six escapes were
            // decided by panel 008 and applied by the same function the lexer uses.
            let text = unescape(src, span);
            let value = text.chars().next().map(|c| c as i128).unwrap_or(0);
            b.emit(Op::Const(Const::Int(value)), ty, span)
        }
        ExprKind::Name => name(b, ast, resolved, id, ty, span),
        ExprKind::Hole => b.emit(Op::Hole, ty, span),
        // **A negative literal is a literal, and it is folded here.** Left as a
        // negation of a positive, `-128` against an `i8` puts `128` in an
        // `int8_t` before the minus reaches it — clang's
        // `-Wconstant-conversion`, and the only way to write that type's lowest
        // value. The frontend already range-checked the value WITH the sign
        // applied, so folding is what makes the two passes agree.
        ExprKind::Unary { op: UnaryOp::Neg, operand }
            if matches!(ast.exprs[operand.0 as usize].kind, ExprKind::Int) =>
        {
            let value = int_literal(b, src, ast.exprs[operand.0 as usize].span);
            b.emit(Op::Const(Const::Int(-value)), ty, span)
        }
        ExprKind::Unary { op, operand } => {
            let value = expr(b, ast, resolved, checked, src, *operand);
            let op = match op {
                UnaryOp::Neg => UnOp::Neg,
                UnaryOp::Not => UnOp::Not,
                UnaryOp::BitNot => UnOp::BitNot,
            };
            b.emit(Op::Unary { op, operand: value }, ty, span)
        }
        ExprKind::Binary { op, left, right } => match op {
            // Short-circuit, so not an instruction: `a && b` evaluates `b` only on
            // one edge, which is a branch (§4.14).
            BinaryOp::And | BinaryOp::Or => {
                control::short_circuit(b, ast, resolved, checked, src, *op, *left, *right, ty, span)
            }
            _ => {
                let left = expr(b, ast, resolved, checked, src, *left);
                let right = expr(b, ast, resolved, checked, src, *right);
                let op = binary(*op);
                b.emit(Op::Binary { op, left, right }, ty, span)
            }
        },
        ExprKind::Field { base, name } => {
            // `grid.WALL` is one qualified name, not a field of a value — the
            // same shape `Method` handles for `grid.f(x)`, decided by the
            // resolver and read back here rather than re-decided.
            if resolved.use_at(*base) == Ref::Module {
                return self::name(b, ast, resolved, id, ty, span);
            }
            let owner = checked.expr_types[base.0 as usize];
            let value = expr(b, ast, resolved, checked, src, *base);
            match layout::field_index(ast, checked, src, owner, *name) {
                Some(index) => b.emit(Op::Field { base: value, index }, ty, span),
                None => b.emit(Op::Missing, ty, span),
            }
        }
        ExprKind::Index { base, index } => {
            let owner = checked.expr_types[base.0 as usize];
            let base = expr(b, ast, resolved, checked, src, *base);
            let key = expr(b, ast, resolved, checked, src, *index);
            match checked.types.get(owner) {
                // A map access yields `V?` and cannot abort; an array index can
                // (§4.9). Same syntax, two instructions.
                Ty::Map(_, _) => b.emit(Op::MapGet { map: base, key }, ty, span),
                _ => b.emit(Op::Index { base, index: key }, ty, span),
            }
        }
        ExprKind::Call { callee, args } => {
            calls::call(b, ast, resolved, checked, src, *callee, args, ty, span)
        }
        ExprKind::Method { receiver, name, args } => {
            super::ufcs::method(b, ast, resolved, checked, src, id, *receiver, *name, args, ty, span)
        }
        ExprKind::Case { name, args } => {
            calls::case(b, ast, resolved, checked, src, *name, args, ty, span)
        }
        ExprKind::Array(elements) => {
            let values: Vec<ValueId> =
                elements.iter().map(|e| expr(b, ast, resolved, checked, src, *e)).collect();
            let args = calls::value_args(b, &values);
            b.emit(Op::Construct { shape: super::Shape::Array, args }, ty, span)
        }
        ExprKind::Map(entries) => {
            // Key and value alternate in the run: a map literal is n-ary, and the
            // pairing is positional rather than a second arena.
            let mut values = Vec::new();
            for entry in entries {
                values.push(expr(b, ast, resolved, checked, src, entry.key));
                values.push(expr(b, ast, resolved, checked, src, entry.value));
            }
            let args = calls::value_args(b, &values);
            b.emit(Op::Construct { shape: super::Shape::Map, args }, ty, span)
        }
        ExprKind::Try(inner) => fallible::try_expr(b, ast, resolved, checked, src, *inner, ty, span),
        ExprKind::If { branches, otherwise } => {
            control::if_expr(b, ast, resolved, checked, src, branches, otherwise, ty, span)
        }
        ExprKind::Match { scrutinee, arms } => {
            control::match_expr(b, ast, resolved, checked, src, *scrutinee, arms, ty, span)
        }
        ExprKind::Error => b.emit(Op::Missing, ty, span),
    }
}

/// A bare name: a local, a top-level declaration, or a built-in.
///
/// The three cases lower to three different things, and the resolver decided
/// which one this is — `resolve/mod.rs`'s rule, and the reason it is a rule is
/// that after UFCS erasure a re-resolution of `f(x, y)` could legitimately give
/// the *other* answer.
#[allow(clippy::too_many_arguments)]
fn name(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    id: ExprId,
    ty: TyId,
    span: Span,
) -> ValueId {
    match resolved.use_at(id) {
        // A module name standing alone is a resolver error, so a clean program
        // never lowers one. It is a real `Ref`, though, so the arm is written
        // rather than folded into `_`.
        Ref::Module => b.emit(Op::Missing, ty, span),
        Ref::Local(local) => match b.slot_of_local(local) {
            Some(slot) => b.load(slot, span),
            None => b.emit(Op::Missing, ty, span),
        },
        Ref::Top(decl) => {
            if is_constant(ast, decl) {
                // A `constant` is a zero-argument function, so reading its name is
                // a call (§4.2: there are no mutable globals to read instead).
                let args = Args { start: 0, len: 0 };
                let callee = Callee::Heroes(decl);
                b.emit(Op::Call { callee, args, variadic: false }, ty, span)
            } else {
                // A function used as a value (§4.13): a pointer, no environment.
                b.emit(Op::FuncRef(calls::callee_of(ast, decl)), ty, span)
            }
        }
        Ref::Builtin(index) => b.emit(Op::FuncRef(Callee::Builtin(index)), ty, span),
        Ref::Unresolved => b.emit(Op::Missing, ty, span),
    }
}

fn is_constant(ast: &Ast, decl: u32) -> bool {
    matches!(ast.decls[decl as usize].kind, crate::syntax::DeclKind::Constant { .. })
}

/// Decodes an `i64` literal.
///
/// **The checker reports the range first, so this is the net rather than the
/// gate** (M-ffi-ladder). Lowering only runs on a program the frontend accepted,
/// so a literal reaching here out of range is a frontend bug — the diagnostic
/// stays, spelled identically, because a net that has never fired is
/// indistinguishable from no net.
///
/// The message speaks in the syntax the author wrote and names the range, because
/// `i64` is the only integer type (§4.3): there is no wider one to suggest, so the
/// fix is a different number and the compiler should not pretend otherwise.
fn int_literal(b: &mut Lowering, src: &Source, span: Span) -> i128 {
    let text = src.slice(span);
    // `decode_wide`, not `decode_int`: after M-sized-integers the frontend accepts
    // `0xffffffffffffffff` against a `u64`, and the narrower decoder would refuse
    // here what the checker has already allowed — a program that checks clean and
    // fails to lower, which is the exact asymmetry the range check was moved to
    // the frontend to remove.
    match crate::lexer::decode_wide(text) {
        Some(value) => value,
        None => {
            b.push_diagnostic(crate::lexer::int_out_of_range(text, span));
            0
        }
    }
}

fn binary(op: BinaryOp) -> BinOp {
    match op {
        BinaryOp::Add => BinOp::Add,
        BinaryOp::Sub => BinOp::Sub,
        BinaryOp::Mul => BinOp::Mul,
        BinaryOp::Div => BinOp::Div,
        BinaryOp::Rem => BinOp::Rem,
        BinaryOp::Eq => BinOp::Eq,
        BinaryOp::Ne => BinOp::Ne,
        BinaryOp::Lt => BinOp::Lt,
        BinaryOp::Le => BinOp::Le,
        BinaryOp::Gt => BinOp::Gt,
        BinaryOp::Ge => BinOp::Ge,
        BinaryOp::BitAnd => BinOp::BitAnd,
        BinaryOp::BitOr => BinOp::BitOr,
        BinaryOp::BitXor => BinOp::BitXor,
        BinaryOp::Shl => BinOp::Shl,
        BinaryOp::Shr => BinOp::Shr,
        // Handled one level up: they branch.
        BinaryOp::And | BinaryOp::Or => BinOp::Eq,
    }
}
