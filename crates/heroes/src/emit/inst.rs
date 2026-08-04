//! One instruction, as C (design.md §3.1, §4.14; panel 020).
//!
//! The sibling of `ir/print_inst.rs`, and deliberately shaped like it: the same
//! `match` over the same `Op`, one arm each, so a new instruction cannot be added
//! to the IR without both files failing to compile.
//!
//! **UB is not a diagnostic.** Every arithmetic operation that §4.3 says aborts is
//! emitted with its check: `__builtin_*_overflow` for `+ - *` and negation, an
//! explicit pair of guards for `/` and `%`. The pair matters — `INT64_MIN % -1` is
//! undefined too, and measured on arm64 it does not trap: it returns 0 at exit 0,
//! while on x86 the same program raises SIGFPE. Same source, two behaviours, no
//! diagnostic anywhere. (The spec says overflow and division by zero abort and is
//! silent on this one; the silence is recorded as a gap rather than read as
//! permission.)
//!
//! One invariant to carry forward, because it will not be visible when it starts
//! mattering: **`__builtin_*_overflow` decides overflow against the destination
//! type, not the operands.** `sub_overflow(false, true)` into a `bool` reports
//! overflow; into an `int64_t` it does not. Heroes never does `bool` arithmetic, so
//! M5a is safe — but §4.19's `c_int` (Part 7) will introduce a 32-bit destination
//! and move the abort threshold from 2⁶³ to 2³¹ with nothing changing at the call
//! site.

use crate::ir::{Arg, BinOp, Const, Inst, Op, Place, Program, UnOp};
use crate::resolve::BUILTINS;
use crate::source::Source;
use crate::syntax::Ast;
use crate::types::{Checked, Ty};

use super::ctype::is_unit;
use super::mangle;
use super::writer::Writer;

pub(super) fn emit(
    w: &mut Writer,
    program: &Program,
    function: &crate::ir::Function,
    ast: &Ast,
    checked: &Checked,
    src: &Source,
    inst: &Inst,
    module: &str,
) {
    let _ = ast;
    let (line, _) = src.line_col(inst.span.start);
    match inst.op {
        // §4.8's second half, and the only instruction that is *about* the calling
        // convention rather than about the program. It is housekeeping, so it is
        // pointed at the generated file: lldb must not blame the author's `return`
        // line for a write the author never wrote.
        Op::CopyOut { param } => {
            w.at_generated();
            let slot = &function.slots[param.0 as usize];
            w.line(&format!(
                "    *{} = {};",
                mangle::out_param(param.0, &slot.name),
                mangle::slot(param.0, &slot.name)
            ));
            return;
        }
        _ => w.at_source(line),
    }
    let dest = inst.dest.filter(|_| !is_unit(checked, inst.ty));
    let target = dest.map(|d| mangle::value(d.0));
    match inst.op {
        Op::Const(value) => {
            if let Some(name) = target {
                w.line(&format!("    {name} = {};", constant(value)));
            }
        }
        Op::Load(place) => {
            if let Some(name) = target {
                w.line(&format!("    {name} = {};", read(function, place)));
            }
        }
        Op::Store { place, value } => {
            if !is_unit(checked, function.value_type(value)) {
                w.line(&format!(
                    "    {} = {};",
                    read(function, place),
                    mangle::value(value.0)
                ));
            }
        }
        Op::Unary { op, operand } => unary(w, function, checked, op, operand, target),
        Op::Binary { op, left, right } => binary(w, function, checked, op, left, right, target),
        Op::Call { callee, args, .. } => {
            let arguments: Vec<String> = function
                .args_of(args)
                .into_iter()
                .map(|arg| match arg {
                    Arg::Value(value) => mangle::value(value.0),
                    // A place, not a value: the callee gets its address (§4.8).
                    Arg::InOut(place) => format!("&{}", read(function, place)),
                })
                .collect();
            call(w, program, function, checked, callee, args, &arguments, target, module);
        }
        // Every remaining form is refused by `gate.rs` at this milestone. The arm is
        // here rather than in a catch-all so that M5b and M5c are compile errors
        // until they are written, not silent omissions.
        Op::Cast { .. }
        | Op::Construct { .. }
        | Op::Field { .. }
        | Op::Index { .. }
        | Op::MapGet { .. }
        | Op::Len(_)
        | Op::Tag(_)
        | Op::Payload { .. }
        | Op::FuncRef(_)
        | Op::Abort { .. }
        | Op::Hole
        | Op::Missing => {
            w.line("    hero_unreachable(); /* the gate refuses this form */");
        }
        Op::CopyOut { .. } => unreachable!("handled above"),
    }
}

/// A place, as a C lvalue. At M5a a place is its root: a path means a field or an
/// element, and `gate.rs` refuses both until the descriptor pass exists.
fn read(function: &crate::ir::Function, place: Place) -> String {
    let slot = &function.slots[place.root.0 as usize];
    mangle::slot(place.root.0, &slot.name)
}

/// `INT64_C(n)`, always. A bare `-9223372036854775808` warns
/// (`-Wimplicitly-unsigned-literal`) because C parses it as a negation of an
/// out-of-range positive, and a bare decimal above `INT32_MAX` is only `long` on
/// LP64 — so the macro is both the portable and the warning-free spelling.
fn constant(value: Const) -> String {
    match value {
        Const::Int(i64::MIN) => "INT64_MIN".to_string(),
        Const::Int(n) => format!("INT64_C({n})"),
        Const::Bool(b) => (if b { "true" } else { "false" }).to_string(),
        // Refused by the gate at M5a; `%a` round-trip-exact rendering is M5b's,
        // with the goldens that pin it.
        Const::Float(_) | Const::Str(_) => "0 /* refused */".to_string(),
    }
}

fn unary(
    w: &mut Writer,
    function: &crate::ir::Function,
    checked: &Checked,
    op: UnOp,
    operand: crate::ir::ValueId,
    target: Option<String>,
) {
    let Some(name) = target else { return };
    let value = mangle::value(operand.0);
    match op {
        UnOp::Not => w.line(&format!("    {name} = !{value};")),
        UnOp::Neg => {
            if checked.types.get(function.value_type(operand)) == Ty::Int {
                // `-INT64_MIN` is overflow, and it is the one negation that is.
                w.line(&format!(
                    "    if (__builtin_sub_overflow(INT64_C(0), {value}, &{name})) hero_panic_overflow();"
                ));
            } else {
                w.line(&format!("    {name} = -{value};"));
            }
        }
    }
}

fn binary(
    w: &mut Writer,
    function: &crate::ir::Function,
    checked: &Checked,
    op: BinOp,
    left: crate::ir::ValueId,
    right: crate::ir::ValueId,
    target: Option<String>,
) {
    let Some(name) = target else { return };
    let l = mangle::value(left.0);
    let r = mangle::value(right.0);
    let integral = checked.types.get(function.value_type(left)) == Ty::Int;
    match op {
        BinOp::Add | BinOp::Sub | BinOp::Mul if integral => {
            let builtin = match op {
                BinOp::Add => "add",
                BinOp::Sub => "sub",
                _ => "mul",
            };
            w.line(&format!(
                "    if (__builtin_{builtin}_overflow({l}, {r}, &{name})) hero_panic_overflow();"
            ));
        }
        BinOp::Div | BinOp::Rem if integral => {
            let operator = if op == BinOp::Div { "/" } else { "%" };
            w.line(&format!("    if ({r} == 0) hero_panic(\"division by zero\");"));
            w.line(&format!(
                "    if ({l} == INT64_MIN && {r} == INT64_C(-1)) hero_panic_overflow();"
            ));
            w.line(&format!("    {name} = {l} {operator} {r};"));
        }
        _ => {
            let operator = match op {
                BinOp::Add => "+",
                BinOp::Sub => "-",
                BinOp::Mul => "*",
                BinOp::Div => "/",
                BinOp::Rem => "%",
                BinOp::Eq => "==",
                BinOp::Ne => "!=",
                BinOp::Lt => "<",
                BinOp::Le => "<=",
                BinOp::Gt => ">",
                BinOp::Ge => ">=",
            };
            w.line(&format!("    {name} = {l} {operator} {r};"));
        }
    }
}

#[allow(clippy::too_many_arguments)] // PORT-DEBT: one emitter call site, six facts
fn call(
    w: &mut Writer,
    program: &Program,
    function: &crate::ir::Function,
    checked: &Checked,
    callee: crate::ir::Callee,
    args: crate::ir::Args,
    arguments: &[String],
    target: Option<String>,
    module: &str,
) {
    use crate::ir::Callee;
    let assign = match &target {
        Some(name) => format!("{name} = "),
        None => String::new(),
    };
    match callee {
        Callee::Heroes(decl) => {
            let name = program
                .functions
                .iter()
                .find(|f| f.decl == decl)
                .map(|f| mangle::function(module, &f.name))
                .unwrap_or_else(|| "hero_unreachable".to_string());
            w.line(&format!("    {assign}{name}({});", arguments.join(", ")));
        }
        Callee::Builtin(index) if BUILTINS[index as usize].name == "print" => {
            print(w, function, checked, args, arguments);
        }
        // Refused by the gate. The arm exists so that adding a callee kind to the
        // IR breaks this file.
        Callee::Builtin(_) | Callee::Extern(_) | Callee::Indirect(_) => {
            w.line("    hero_unreachable(); /* the gate refuses this callee */");
        }
    }
}

/// `print` is a compiler form, not a function value (§4.20, panel 006): the runtime
/// exposes one monomorphic printer per type and the emitter composes them.
///
/// The contract is **no separator, exactly one trailing newline** — Pascal's
/// `WriteLn`, which the ISO standard defines as `write(f,e1); write(f,e2,…)` with
/// nothing inserted between. `hero_print_end` owns the newline rather than the
/// emitter, and that is measured rather than chosen: the emitter owning it needs
/// `putchar`, which is `error: call to undeclared function 'putchar'` without
/// `#include <stdio.h>` in every generated unit — a header-collision surface on
/// every FFI program, against CLAUDE.md §7.
fn print(
    w: &mut Writer,
    function: &crate::ir::Function,
    checked: &Checked,
    args: crate::ir::Args,
    arguments: &[String],
) {
    // The order is the IR's, which is the source's. The printer per argument comes
    // from the *value's* type, because `print` has no signature to check against —
    // it is the one call clang verifies nothing about (§4.19's variadic note).
    for (arg, name) in function.args_of(args).into_iter().zip(arguments) {
        let printer = match arg {
            Arg::Value(value) => match checked.types.get(function.value_type(value)) {
                Ty::Bool => "hero_print_bool",
                _ => "hero_print_int",
            },
            // `print(@x)` cannot be written: §4.8's marker is for parameters
            // declared `@`, and `print` declares none.
            Arg::InOut(_) => "hero_print_int",
        };
        w.line(&format!("    {printer}({name});"));
    }
    w.line("    hero_print_end();");
}
