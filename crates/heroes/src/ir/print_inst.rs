//! One instruction, as one line (panel 019 point 7).
//!
//! Split from `print.rs` because it is a different question: that file decides the
//! *shape* of the dump — signature, slots, blocks, predecessors — and this one
//! decides how a single operation reads. The rules it enforces are the two the
//! llm-ergonomist's session produced:
//!
//! - **the destination is left of `=` (a value) or of `<-` (a place)**, never
//!   after a comma, because LLVM's `store` is value-first and a reader who has seen
//!   LLVM will guess wrong;
//! - **`!` marks an instruction that can abort** — `i64` overflow, division by
//!   zero, an out-of-bounds index (§4.3, §4.9). `f64` arithmetic carries no mark,
//!   which makes the mark a type distinction as well as a warning.
//!
//! Where a name reads better than an index, the name is printed: `field $t3.x`
//! rather than `field $t3.0`, and `call extern sqrt` rather than a number. The
//! index is what the IR *holds* — no pass compares strings again (`layout.rs`) —
//! and recovering the name for the reader is `print_names.rs`'s job.

use crate::source::Source;
use crate::syntax::Ast;
use crate::types::{render_ty, Checked, Ty};

use super::inst::{Abort, Arg, BinOp, CastKind, Const, Inst, Op, Place, Step, UnOp, ValueId};
use super::print_names::{case_name, field_name, field_type, name_of_field, shape_name, target};
use super::Function;

pub(super) fn value_name(value: ValueId) -> String {
    format!("${}", format_args!("t{}", value.0))
}

pub(super) fn instruction(
    function: &Function,
    inst: &Inst,
    ast: &Ast,
    checked: &Checked,
    src: &Source,
) -> String {
    let body = operation(function, inst, ast, checked, src);
    match inst.dest {
        Some(dest) => {
            let ty = render_ty(&checked.types, ast, src, inst.ty, &function.generics);
            format!("{}: {ty} = {body}", value_name(dest))
        }
        None => body,
    }
}

fn operation(
    function: &Function,
    inst: &Inst,
    ast: &Ast,
    checked: &Checked,
    src: &Source,
) -> String {
    let int = checked.types.int();
    match inst.op {
        Op::Const(value) => format!("const {}", constant(value)),
        Op::Load(place) => format!("load {}", written(function, place, ast, checked, src)),
        Op::Store { place, value } => format!(
            "store {} <- {}",
            written(function, place, ast, checked, src),
            value_name(value)
        ),
        Op::Unary { op, operand } => {
            let word = match op {
                UnOp::Neg => "neg",
                UnOp::Not => "not",
                UnOp::BitNot => "bitnot",
            };
            let bang = if op == UnOp::Neg && inst.ty == int { "!" } else { "" };
            format!("{word}{bang} {}", value_name(operand))
        }
        Op::Binary { op, left, right } => {
            let operands = function.value_type(left);
            format!(
                "{}{} {}, {}",
                word_of(op),
                if aborts(op, operands, int) { "!" } else { "" },
                value_name(left),
                value_name(right)
            )
        }
        Op::Cast { kind, operand } => {
            let word = match kind {
                CastKind::StrToCstr => "str_to_cstr",
            };
            format!("cast {word} {}", value_name(operand))
        }
        Op::Call { callee, args, variadic } => {
            let tail = if variadic { " variadic" } else { "" };
            format!(
                "call {}({}){tail}",
                target(callee, ast, src),
                arguments(function, args, ast, checked, src)
            )
        }
        Op::Construct { shape, args } => format!(
            "construct {}({})",
            shape_name(shape, ast, src),
            arguments(function, args, ast, checked, src)
        ),
        Op::Field { base, index } => format!(
            "field {}.{}",
            value_name(base),
            field_name(function, ast, checked, src, base, index)
        ),
        // §4.9: an out-of-bounds index aborts, which is the whole reason for `!`.
        Op::Index { base, index } => {
            format!("index! {}, {}", value_name(base), value_name(index))
        }
        Op::MapGet { map, key } => format!("mapget {}, {}", value_name(map), value_name(key)),
        Op::Len(value) => format!("len {}", value_name(value)),
        Op::Tag(value) => format!("tag {}", value_name(value)),
        Op::Payload { base, case } => format!(
            "payload {} {}",
            value_name(base),
            case_name(function, ast, checked, src, base, case)
        ),
        Op::FuncRef(callee) => format!("funcref {}", target(callee, ast, src)),
        Op::CopyOut { param } => {
            format!("copyout {}", function.slots[param.0 as usize].name)
        }
        Op::Abort { reason, args } => {
            let word = match reason {
                Abort::Must => "must",
                Abort::Assert => "assert",
            };
            let operands = arguments(function, args, ast, checked, src);
            if operands.is_empty() {
                format!("abort {word}")
            } else {
                format!("abort {word}({operands})")
            }
        }
        Op::Incref(value) => format!("incref {}", value_name(value)),
        Op::Decref(value) => format!("decref {}", value_name(value)),
        Op::Hole => "???".to_string(),
        Op::Missing => "MISSING".to_string(),
    }
}

/// A place, in the surface's own notation: `l.pos`, `xs[$t2]`, `total`.
fn written(
    function: &Function,
    place: Place,
    ast: &Ast,
    checked: &Checked,
    src: &Source,
) -> String {
    let mut out = function.slots[place.root.0 as usize].name.clone();
    let mut ty = function.slots[place.root.0 as usize].ty;
    for step in function.steps_of(place.path) {
        match step {
            Step::Field(index) => {
                out.push('.');
                out.push_str(&name_of_field(ast, checked, src, ty, index));
                // Carry the field's own type, or the next step in the path has
                // nothing to look a name up in.
                ty = field_type(ast, checked, ty, index);
            }
            Step::Index(value) => {
                out.push('[');
                out.push_str(&value_name(value));
                out.push(']');
                ty = match checked.types.get(ty) {
                    Ty::Array(element) => element,
                    other => {
                        let _ = other;
                        checked.types.error()
                    }
                };
            }
        }
    }
    out
}

fn arguments(
    function: &Function,
    args: super::inst::Args,
    ast: &Ast,
    checked: &Checked,
    src: &Source,
) -> String {
    let rendered: Vec<String> = function
        .args_of(args)
        .into_iter()
        .map(|arg| match arg {
            Arg::Value(value) => value_name(value),
            // §4.8's marker, repeated here for the same reason it is repeated at
            // the call site: the mutation is visible on the line.
            Arg::InOut(place) => format!("@{}", written(function, place, ast, checked, src)),
        })
        .collect();
    rendered.join(", ")
}

fn constant(value: Const) -> String {
    match value {
        Const::Int(n) => format!("{n}"),
        // Canonical `f64` rendering is M-strings-ownership's (panel 006), and the goldens here will
        // move with it — pre-registered rather than discovered.
        Const::Float(x) => format!("{x:?}"),
        Const::Bool(b) => format!("{b}"),
        Const::NullPtr => "nullptr".to_string(),
        Const::Str(id) => format!("str {}", id.0),
    }
}

fn word_of(op: BinOp) -> &'static str {
    match op {
        BinOp::Add => "add",
        BinOp::Sub => "sub",
        BinOp::Mul => "mul",
        BinOp::Div => "div",
        BinOp::Rem => "rem",
        BinOp::Eq => "eq",
        BinOp::Ne => "ne",
        BinOp::Lt => "lt",
        BinOp::Le => "le",
        BinOp::Gt => "gt",
        BinOp::Ge => "ge",
        BinOp::BitAnd => "and",
        BinOp::BitOr => "or",
        BinOp::BitXor => "xor",
        BinOp::Shl => "shl",
        BinOp::Shr => "shr",
    }
}

/// Whether this operation can stop the program. `i64` arithmetic can (overflow
/// aborts, §4.3; division by zero aborts, §4.6); `f64` arithmetic cannot; a
/// comparison never can.
fn aborts(op: BinOp, operands: crate::types::TyId, int: crate::types::TyId) -> bool {
    if operands != int {
        return false;
    }
    matches!(op, BinOp::Add | BinOp::Sub | BinOp::Mul | BinOp::Div | BinOp::Rem)
}
