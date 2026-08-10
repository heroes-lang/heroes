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
use crate::types::{Checked, Ty};

use super::aggregate;
use super::ctype::is_unit;
use super::mangle;
use super::writer::Writer;

pub(super) fn emit(
    w: &mut Writer,
    program: &Program,
    types: &aggregate::Types,
    function: &crate::ir::Function,
    inst: &Inst,
    module: &str,
) {
    // Unpacked once: the four references travel together everywhere in the backend
    // (they are what "what is this type called in C" needs), so they arrive as one
    // bundle and are spread here rather than in every signature.
    let (checked, src) = (types.checked, types.src);
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
                w.line(&format!("    {name} = {};", read(types, function, place)));
            }
        }
        Op::Store { place, value } => {
            if !is_unit(checked, function.value_type(value)) {
                w.line(&format!(
                    "    {} = {};",
                    read(types, function, place),
                    mangle::value(value.0)
                ));
            }
        }
        // Housekeeping the author did not write, so it points at the generated file:
        // lldb must not attribute a refcount to a user line.
        //
        // **Dispatched by type**, and the two shapes are different in kind. A `str`
        // owns one block, so the runtime counts it. An aggregate owns nothing of its
        // own and *contains* what is counted, so its generated function walks its
        // fields — which is why this is `retain`/`release` and not `copy`: no bytes
        // move, the references inside them just become owned.
        Op::Incref(value) | Op::Decref(value) => {
            w.at_generated();
            let keep = matches!(inst.op, Op::Incref(_));
            let ty = function.value_type(value);
            let name = mangle::value(value.0);
            let line = match checked.types.get(ty) {
                Ty::Str if keep => format!("    hero_str_incref({name});"),
                Ty::Str => format!("    hero_str_decref({name});"),
                _ => match aggregate::retain(types, ty, value, keep) {
                    Some(call) => format!("    {call};"),
                    None => "    hero_unreachable(); /* the gate refuses this type */".to_string(),
                },
            };
            w.line(&line);
        }
        Op::Unary { op, operand } => unary(w, function, checked, op, operand, target),
        Op::Binary { op, left, right } => {
            binary(w, types, function, checked, op, left, right, target)
        }
        Op::Call { callee, args, .. } => {
            let arguments: Vec<String> = function
                .args_of(args)
                .into_iter()
                .map(|arg| match arg {
                    Arg::Value(value) => mangle::value(value.0),
                    // A place, not a value: the callee gets its address (§4.8).
                    Arg::InOut(place) => format!("&{}", read(types, function, place)),
                })
                .collect();
            call(w, program, function, checked, callee, args, &arguments, target, module);
        }
        Op::Len(value) => {
            if let Some(name) = target {
                w.line(&format!("    {name} = hero_str_len({});", mangle::value(value.0)));
            }
        }
        // `s[i]`: a byte as an `int`, aborting out of range (spec line 142). The
        // array case is the same op and waits for M5c.
        Op::Index { base, index } if checked.types.get(function.value_type(base)) == Ty::Str => {
            if let Some(name) = target {
                w.line(&format!(
                    "    {name} = hero_str_byte({}, {});",
                    mangle::value(base.0),
                    mangle::value(index.0)
                ));
            }
        }
        // Every remaining form is refused by `gate.rs` at this milestone. The arm is
        // here rather than in a catch-all so that M5c and M6 are compile errors
        // until they are written, not silent omissions.
        // `Point(x: 1, y: 2)`. Only a record at this step; the other shapes are
        // still refused, and each stays a named arm so that landing one is a compile
        // error here rather than a silent omission.
        Op::Construct { shape: crate::ir::Shape::Record(decl), args } => {
            if let Some(name) = target {
                let arguments: Vec<String> = function
                    .args_of(args)
                    .into_iter()
                    .map(|arg| match arg {
                        Arg::Value(value) => mangle::value(value.0),
                        Arg::InOut(place) => format!("&{}", read(types, function, place)),
                    })
                    .collect();
                match aggregate::construct(types, inst.ty, decl, &arguments) {
                    Some(literal) => w.line(&format!("    {name} = {literal};")),
                    None => w.line("    hero_unreachable(); /* not a record */"),
                }
            }
        }
        Op::Field { base, index } => {
            if let Some(name) = target {
                match aggregate::read_field(types, function, base, index) {
                    Some(text) => w.line(&format!("    {name} = {text};")),
                    None => w.line("    hero_unreachable(); /* the gate refuses this base */"),
                }
            }
        }
        Op::Cast { .. }
        | Op::Construct { .. }
        | Op::Index { .. }
        | Op::MapGet { .. }
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
fn read(types: &aggregate::Types, function: &crate::ir::Function, place: Place) -> String {
    aggregate::place(types, function, place)
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
        // **A hex float, not a decimal one.** `%a` is round-trip-exact by
        // construction, where `%.17g` is exact only in practice — and design.md §3.1
        // has said "`f64` literals emitted round-trip-exact (`%a`)" since M0. This is
        // the *literal*; how a value **prints** is `hero_print_f64`'s question and a
        // different answer (§4.9).
        Const::Float(x) => hex_float(x),
        // A static block, laid out by clang: refcount -1 means "never freed", so a
        // literal costs no allocation and decrefing one is a no-op.
        Const::Str(id) => format!("HERO_STR_LIT(hero_str_{})", id.0),
    }
}

/// A `double` as a C11 hexadecimal floating literal. Exact, warning-free, and
/// independent of every decimal-rendering question.
fn hex_float(x: f64) -> String {
    if x.is_nan() {
        return "(0.0 / 0.0)".to_string();
    }
    if x.is_infinite() {
        return if x > 0.0 { "HUGE_VAL".to_string() } else { "(-HUGE_VAL)".to_string() };
    }
    // Rust has no `{:a}`, so the digits are produced by the same route C reads them:
    // sign, mantissa in hex, binary exponent.
    let bits = x.to_bits();
    let negative = bits >> 63 == 1;
    let exponent = ((bits >> 52) & 0x7ff) as i64;
    let mantissa = bits & 0x000f_ffff_ffff_ffff;
    let sign = if negative { "-" } else { "" };
    if exponent == 0 && mantissa == 0 {
        return format!("{sign}0x0p+0");
    }
    let (lead, unbiased) = if exponent == 0 {
        (0, -1022) // subnormal
    } else {
        (1, exponent - 1023)
    };
    // 13 hex digits hold all 52 mantissa bits exactly.
    let digits = format!("{mantissa:013x}");
    let trimmed = digits.trim_end_matches('0');
    let fraction = if trimmed.is_empty() { String::new() } else { format!(".{trimmed}") };
    format!("{sign}0x{lead}{fraction}p{}{}", if unbiased < 0 { "-" } else { "+" }, unbiased.abs())
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
    types: &aggregate::Types,
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
    let operands = checked.types.get(function.value_type(left));
    // `str` has its own arithmetic: `+` is concatenation (§4.14's operator table) and
    // the comparisons are byte-wise, which is what makes `==` structural on a string
    // the same way it is on a record.
    if operands == Ty::Str {
        let call = match op {
            BinOp::Add => format!("hero_str_concat({l}, {r})"),
            BinOp::Eq => format!("hero_str_eq({l}, {r})"),
            BinOp::Ne => format!("!hero_str_eq({l}, {r})"),
            BinOp::Lt => format!("hero_str_cmp({l}, {r}) < 0"),
            BinOp::Le => format!("hero_str_cmp({l}, {r}) <= 0"),
            BinOp::Gt => format!("hero_str_cmp({l}, {r}) > 0"),
            BinOp::Ge => format!("hero_str_cmp({l}, {r}) >= 0"),
            // `- * / %` on `str` do not type-check (§4.14), so this is unreachable
            // by construction rather than by hope.
            BinOp::Sub | BinOp::Mul | BinOp::Div | BinOp::Rem => {
                "0 /* not an operation on str */".to_string()
            }
        };
        w.line(&format!("    {name} = {call};"));
        return;
    }
    // Structural `==` on a record is one call to its generated `eq`, which walks
    // fields — never bytes, because padding makes two equal records differ under
    // `memcmp` with no warning and no sanitiser report (panel 022).
    if matches!(operands, Ty::Named(_) | Ty::Case(_, _)) {
        let ty = function.value_type(left);
        let call = aggregate::equality(types, ty, left, right);
        let text = match (op, call) {
            (BinOp::Eq, Some(call)) => call,
            (BinOp::Ne, Some(call)) => format!("!{call}"),
            // §4.14 gives a record no ordering, so the checker rejected it already.
            _ => "(hero_unreachable(), false)".to_string(),
        };
        w.line(&format!("    {name} = {text};"));
        return;
    }
    let integral = operands == Ty::Int;
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
        Callee::Builtin(index) if super::EMITTED_BUILTINS.contains(&BUILTINS[index as usize].name) => {
            let entry = match BUILTINS[index as usize].name {
                "len" => "hero_str_len",
                "slice" => "hero_str_slice",
                // `to_str` is one Heroes name over three C entry points, chosen by
                // the argument's type — the same shape as `print`, for the same
                // reason: the runtime is monomorphic and the emitter composes it.
                _ => match function.args_of(args).first() {
                    Some(Arg::Value(value)) => match checked.types.get(function.value_type(*value)) {
                        Ty::F64 => "hero_f64_to_str",
                        Ty::Bool => "hero_bool_to_str",
                        Ty::Str => "hero_str_identity",
                        _ => "hero_int_to_str",
                    },
                    _ => "hero_int_to_str",
                },
            };
            w.line(&format!("    {assign}{entry}({});", arguments.join(", ")));
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
                Ty::Str => "hero_print_str",
                Ty::F64 => "hero_print_f64",
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
