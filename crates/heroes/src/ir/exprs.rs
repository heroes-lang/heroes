//! Expressions → one temporary holding the value (design.md §4.9, §4.14, §4.16).
//!
//! **Flattening is the whole job.** `x = 2 + 3 * 4` is a tree in the syntax and
//! three instructions here, each naming temporaries and nothing nested. That is
//! what makes the C emitter a printer rather than a second compiler, and what
//! makes the ownership pass (M5b) able to point at a single instruction and say
//! "this one produced a value somebody owns".
//!
//! Literals are decoded *here*, and that is a first: nothing before this pass
//! ever needed an `int` literal's value, so an out-of-range one was invisible
//! until now. It is the one diagnostic this pass owns, and panel 019 recorded it
//! as a new class needing a panel of its own.
//!
//! Two rows of Part 5's table are erased in this file, both by looking at a type
//! the checker already worked out: `m[k]` is a different instruction from `xs[i]`
//! because a map access yields a `V?` and cannot abort (§4.9), and `&&`/`||` are
//! not operators at all — they short-circuit, so they are control flow
//! (`control.rs`).

use crate::diagnostics::Diagnostic;
use crate::lexer::unescape;
use crate::resolve::{Ref, Resolved};
use crate::source::{Source, Span};
use crate::syntax::{Ast, BinaryOp, ExprId, ExprKind, UnaryOp};
use crate::types::{Checked, Ty, TyId};

use super::build::Lowering;
use super::inst::{Args, BinOp, Callee, Const, Op, UnOp, ValueId};
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
            // The parse cannot fail: the lexer already accepted the shape, and an
            // out-of-range `f64` literal is an infinity rather than an error.
            let value = src.slice(span).parse::<f64>().unwrap_or(0.0);
            b.emit(Op::Const(Const::Float(value)), ty, span)
        }
        ExprKind::Bool => {
            let value = src.slice(span) == "true";
            b.emit(Op::Const(Const::Bool(value)), ty, span)
        }
        ExprKind::Str => {
            let text = unescape(src, span);
            let id = b.intern(text);
            b.emit(Op::Const(Const::Str(id)), ty, span)
        }
        ExprKind::Char => {
            // §4.3: a character literal *is* an `int`. The five escapes were
            // decided by panel 008 and applied by the same function the lexer uses.
            let text = unescape(src, span);
            let value = text.chars().next().map(|c| c as i64).unwrap_or(0);
            b.emit(Op::Const(Const::Int(value)), ty, span)
        }
        ExprKind::Name => name(b, ast, resolved, id, ty, span),
        ExprKind::Hole => b.emit(Op::Hole, ty, span),
        ExprKind::Unary { op, operand } => {
            let value = expr(b, ast, resolved, checked, src, *operand);
            let op = match op {
                UnaryOp::Neg => UnOp::Neg,
                UnaryOp::Not => UnOp::Not,
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
            calls::method(b, ast, resolved, checked, src, id, *receiver, *name, args, ty, span)
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

/// Decodes an `int` literal, and owns the one diagnostic this pass produces.
///
/// The message speaks in the syntax the author wrote and names the range, because
/// `int` is the only integer type (§4.3): there is no wider one to suggest, so the
/// fix is a different number and the compiler should not pretend otherwise.
fn int_literal(b: &mut Lowering, src: &Source, span: Span) -> i64 {
    let text = src.slice(span);
    match text.parse::<i64>() {
        Ok(value) => value,
        Err(_) => {
            let diagnostic = Diagnostic::new(
                "int-literal-out-of-range",
                format!("`{text}` does not fit in an `int`"),
                span,
            )
            .with_note(
                "`int` is a 64-bit signed integer and the only integer type, so it holds -9223372036854775808 through 9223372036854775807"
                    .to_string(),
            );
            b.push_diagnostic(diagnostic);
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
        // Handled one level up: they branch.
        BinaryOp::And | BinaryOp::Or => BinOp::Eq,
    }
}
