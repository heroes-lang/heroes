//! One operation as C: a literal, an operator, a call, and `print` (design.md §4.14,
//! §4.20; panels 006, 020).
//!
//! Split from `inst.rs` when M5c step 3 pushed that file to 450 lines, well past
//! CLAUDE.md §11's ceiling. The line between them is a real one rather than a
//! line count: `inst.rs` answers **which** operation an instruction is and where its
//! result goes, and this answers **what** one operation is in C. The dispatch is a
//! table; these are the entries.
//!
//! Three rules live here and each has a measured reason:
//!
//! - **arithmetic aborts, it never wraps into UB.** `__builtin_*_overflow` for
//!   `+ - *`, an explicit zero test for `/` and `%`, and `%` is guarded exactly like
//!   `/` because `INT64_MIN % -1` does not trap on arm64 — it is UB that happens to
//!   look fine.
//! - **an `f64` literal is written as hex**, so the decimal round trip that would
//!   otherwise sit between the lexer and clang does not exist.
//! - **`print` is monomorphic**, one runtime entry point per type (panel 006), which
//!   is what lets clang type-check every one of them.

use crate::ir::{Arg, BinOp, Const, Program, UnOp};
use crate::resolve::BUILTINS;
use crate::types::{Checked, Ty};

use super::aggregate;
use super::mangle;
use super::writer::Writer;

/// `INT64_C(n)`, always. A bare `-9223372036854775808` warns
/// (`-Wimplicitly-unsigned-literal`) because C parses it as a negation of an
/// out-of-range positive, and a bare decimal above `INT32_MAX` is only `long` on
/// LP64 — so the macro is both the portable and the warning-free spelling.
pub(super) fn constant(value: Const) -> String {
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

pub(super) fn unary(
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

pub(super) fn binary(
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
    // A map compares order-independently: same length, and every entry found in the
    // other with an equal value. Spec line 58 requires exactly that, and a pairwise
    // walk of two entry arrays would have made `{"a":1,"b":2}` and `{"b":2,"a":1}`
    // unequal — the shape panel 022 ranked first among what a cheap map gets silently
    // wrong.
    if let Ty::Map(_, _) = operands {
        let call = format!("hero_map_eq({l}, {r})");
        let text = match op {
            BinOp::Eq => call,
            BinOp::Ne => format!("!{call}"),
            _ => "(hero_unreachable(), false)".to_string(),
        };
        w.line(&format!("    {name} = {text};"));
        return;
    }
    // An array compares element by element through the descriptor, and a length
    // mismatch is answered before any element is touched.
    if let Ty::Array(_) = operands {
        let call = format!("hero_array_eq({l}, {r})");
        let text = match op {
            BinOp::Eq => call,
            BinOp::Ne => format!("!{call}"),
            // §4.14 gives an array no ordering, so the checker rejected it already.
            _ => "(hero_unreachable(), false)".to_string(),
        };
        w.line(&format!("    {name} = {text};"));
        return;
    }
    // A `T?` and a `Failure` compare through their generated function: different tags
    // are never equal, and §4.6's error side compares its code before its message.
    if matches!(operands, Ty::Fallible(_) | Ty::Failure) {
        let ty = function.value_type(left);
        let call = aggregate::equality(types, ty, left, right);
        let text = match (op, call) {
            (BinOp::Eq, Some(call)) => call,
            (BinOp::Ne, Some(call)) => format!("!{call}"),
            _ => "(hero_unreachable(), false)".to_string(),
        };
        w.line(&format!("    {name} = {text};"));
        return;
    }
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
pub(super) fn call(
    w: &mut Writer,
    program: &Program,
    function: &crate::ir::Function,
    types: &aggregate::Types,
    callee: crate::ir::Callee,
    args: crate::ir::Args,
    arguments: &[String],
    target: Option<String>,
    module: &str,
    instance: Vec<crate::types::TyId>,
) {
    let (checked, ast, src) = (types.checked, types.ast, types.src);
    use crate::ir::Callee;
    let assign = match &target {
        Some(name) => format!("{name} = "),
        None => String::new(),
    };
    match callee {
        Callee::Heroes(decl) => {
            // **The instance, not the template.** Monomorphisation deleted the
            // generic function and left one copy per type tuple, so a call has to
            // name the copy — and it reads which one from the same table the pass
            // read (`Checked::instantiations`, keyed by this call's span). One
            // answer, one place: re-deriving it here is the shape panel 029 R2
            // refused.
            let name = program
                .functions
                .iter()
                .find(|f| f.decl == decl && f.instance == instance)
                .map(|f| super::decls::instance_name(f, ast, checked, src, module))
                .unwrap_or_else(|| "hero_unreachable".to_string());
            w.line(&format!("    {assign}{name}({});", arguments.join(", ")));
        }
        Callee::Builtin(index) if BUILTINS[index as usize].name == "print" => {
            print(w, function, checked, args, arguments);
        }
        Callee::Builtin(index) if super::EMITTED_BUILTINS.contains(&BUILTINS[index as usize].name) => {
            let entry =
                super::builtins::entry(BUILTINS[index as usize].name, function, checked, args);
            // `push`'s second argument is a *place*, not a value: the runtime copies
            // through the element descriptor, which is the only way one function can
            // append an `int` and a `Point`.
            let written: Vec<String> = if matches!(BUILTINS[index as usize].name, "push") {
                arguments
                    .iter()
                    .enumerate()
                    .map(|(at, text)| if at == 1 { format!("&{text}") } else { text.clone() })
                    .collect()
            } else {
                arguments.to_vec()
            };
            w.line(&format!("    {assign}{entry}({});", written.join(", ")));
        }
        // A call through a function value. The temporary already holds the
        // pointer, so this is just C's own indirect call — no dereference, no
        // cast, and clang type-checks the arguments against the typedef's
        // parameter list, which is the property `(void)` on a zero-parameter
        // typedef exists to keep (see `types::functions`).
        Callee::Indirect(value) => {
            w.line(&format!("    {assign}{}({});", mangle::value(value.0), arguments.join(", ")));
        }
        // Refused by the gate. The arm exists so that adding a callee kind to the
        // IR breaks this file.
        Callee::Builtin(_) | Callee::Extern(_) => {
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
pub(super) fn print(
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
