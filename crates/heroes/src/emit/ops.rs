//! One operation as C: a literal, an operator, a call, and `print` (design.md §4.14,
//! §4.20; panels 006, 020).
//!
//! Split from `inst.rs` when M-value-aggregates step 3 pushed that file to 450 lines, well past
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
use crate::types::{Checked, IntKind, Ty};

use super::aggregate;
use super::mangle;
use super::writer::Writer;

/// A constant, in the C spelling its own width asks for.
///
/// A bare `-9223372036854775808` warns (`-Wimplicitly-unsigned-literal`) because
/// C parses it as a negation of an out-of-range positive, and a bare decimal
/// above `INT32_MAX` is only `long` on LP64 — so a macro is both the portable and
/// the warning-free spelling, and the macro has to match the width.
///
/// **`UINT64_C` for the unsigned widths is not tidiness.** `18446744073709551615`
/// through `INT64_C` is a constant C cannot represent, and the narrower unsigned
/// widths take it too so that the emitted text says what the Heroes type says
/// rather than relying on the assignment to convert it.
pub(super) fn constant(value: Const, kind: Option<IntKind>) -> String {
    match value {
        Const::Int(n) if n == i128::from(i64::MIN) => "INT64_MIN".to_string(),
        Const::Int(n) => match kind {
            Some(k) if !k.signed() => format!("UINT64_C({n})"),
            _ => format!("INT64_C({n})"),
        },
        Const::Bool(b) => (if b { "true" } else { "false" }).to_string(),
        // `NULL` would need a header; the cast needs none and is the same value.
        Const::NullPtr => "((void *)0)".to_string(),
        // **A hex float, not a decimal one.** `%a` is round-trip-exact by
        // construction, where `%.17g` is exact only in practice — and design.md §3.1
        // has said "`f64` literals emitted round-trip-exact (`%a`)" since M-day-zero. This is
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
            if matches!(checked.types.get(function.value_type(operand)), Ty::Int(_)) {
                // `-INT64_MIN` is overflow, and it is the one negation that is.
                w.line(&format!(
                    "    if (__builtin_sub_overflow(INT64_C(0), {value}, &{name})) hero_panic_overflow();"
                ));
            } else {
                w.line(&format!("    {name} = -{value};"));
            }
        }
        // Every `int64_t` has a complement, `INT64_MIN` included, so unlike `Neg`
        // this one cannot abort and carries no guard.
        UnOp::BitNot => w.line(&format!("    {name} = ~{value};")),
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
            BinOp::Sub
            | BinOp::Mul
            | BinOp::Div
            | BinOp::Rem
            | BinOp::BitAnd
            | BinOp::BitOr
            | BinOp::BitXor
            | BinOp::Shl
            | BinOp::Shr => "0 /* not an operation on str */".to_string(),
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
    // **This line is why the width lives inside the variant** (panel 042). As
    // `operands == Ty::Int` it decided whether to emit `__builtin_*_overflow`,
    // and with one variant per width a `u8` would simply have made it `false`:
    // the guard would be dropped, `t0 = l + r` emitted bare, and "overflow aborts
    // at every width" would be silently untrue at exit 0. Measured as a rustc
    // error here and as nothing at all under the other shape.
    let integral = matches!(operands, Ty::Int(_));
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
            // **`INT64_MIN % -1` and `INT64_MIN / -1` are guarded together and
            // reported apart** (panel 035's owed message). The guard is the same:
            // arm64 does not trap on either, so both are UB that happens to look
            // fine (CLAUDE.md §7). But the *reason* differs, and saying "integer
            // overflow" for `%` was false — the remainder is 0 and overflows
            // nothing; what has no representation is the quotient C computes on
            // the way there. A refusal whose message misleads is worse than the
            // form it refuses.
            let abort = if op == BinOp::Div {
                "hero_panic_overflow()".to_string()
            } else {
                "hero_panic(\"`%` by -1 at the smallest i64: the remainder is 0, but C reaches it through a quotient that has no int64\")".to_string()
            };
            w.line(&format!(
                "    if ({l} == INT64_MIN && {r} == INT64_C(-1)) {abort};"
            ));
            w.line(&format!("    {name} = {l} {operator} {r};"));
        }
        // **The three that are pure bit patterns**: no guard, because every pair of
        // `int64_t`s has an and, an or and an xor. C's `&`, `|` and `^` on two
        // signed values of the same width are fully defined — it is the *shifts*
        // that are not.
        BinOp::BitAnd | BinOp::BitOr | BinOp::BitXor => {
            let operator = match op {
                BinOp::BitAnd => "&",
                BinOp::BitOr => "|",
                _ => "^",
            };
            w.line(&format!("    {name} = {l} {operator} {r};"));
        }
        // **The two that can abort, and they abort for C's reasons rather than for
        // Heroes'** (CLAUDE.md §7: never C UB). A shift count that is negative or
        // ≥ 64 is undefined in C6.5.7p3, and a left shift that moves bits into or
        // past the sign bit of a *signed* operand is undefined too. So: the count is
        // checked, and the shift itself is done on the unsigned bit pattern and cast
        // back, which is defined for every input and is what makes `1 << 63`
        // `INT64_MIN` rather than a trap. Without the sign bit reachable a mask set
        // cannot name its own top flag, which is the whole use.
        //
        // `>>` is **arithmetic**: the operand is signed, so the sign propagates, and
        // C's implementation-defined right shift is replaced by an explicit one.
        BinOp::Shl | BinOp::Shr if integral => {
            w.line(&format!(
                "    if ({r} < 0 || {r} > 63) hero_panic(\"shift count outside 0..63\");"
            ));
            if op == BinOp::Shl {
                w.line(&format!(
                    "    {name} = (int64_t)((uint64_t){l} << (uint64_t){r});"
                ));
            } else {
                // Arithmetic shift, written so no implementation-defined behaviour
                // is relied on: shift the magnitude, then put the sign bits back.
                w.line(&format!(
                    "    {name} = ({l} < 0) ? ~(int64_t)((~(uint64_t){l}) >> (uint64_t){r}) : (int64_t)((uint64_t){l} >> (uint64_t){r});"
                ));
            }
        }
        // `%` on two `f64` is `fmod`, not C's `%`, which takes integers only:
        // `t3 = t1 % t2` on two `double`s is `error: invalid operands to binary
        // expression`, exit 2, on a program spec line 139 explicitly allows
        // (`f64 with f64`). It reached clang because the fallthrough arm below
        // spells every operator the same way (panel 035, compiler-engineer).
        //
        // `fmod` truncates toward zero, which is the rule spec line 135 already
        // states for `/` and `%` — so the C function and the sentence agree
        // without either being changed.
        _ if op == BinOp::Rem => {
            w.line(&format!("    {name} = fmod({l}, {r});"));
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
                // Unreachable: the arms above take every bitwise op on `i64`, and
                // the checker admits them on nothing else (§4.14).
                BinOp::BitAnd
                | BinOp::BitOr
                | BinOp::BitXor
                | BinOp::Shl
                | BinOp::Shr => return w.line("    hero_unreachable();"),
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
                .map(|f| super::signature::instance_name(f, ast, checked, src))
                .unwrap_or_else(|| "hero_unreachable".to_string());
            w.line(&format!("    {assign}{name}({});", arguments.join(", ")));
        }
        Callee::Builtin(index) if BUILTINS[index as usize].name == "print" => {
            print(w, function, checked, args, arguments);
        }
        // `fit_<width>` is not a call, which is why it is here and not in
        // `builtins::entry`: that function answers with the NAME of a C entry
        // point, and this one has to build a `T?` — a tagged union generated for
        // this result type, which no runtime function can return.
        //
        // The range test is written against the SOURCE's C type and the target's
        // bounds, and both halves matter. Comparing an unsigned source against a
        // negative lower bound is a warning and a constant answer, so the low
        // test is omitted where the source cannot be negative; comparing a narrow
        // source against a wider target's top is likewise always true. Emitting
        // `true &&` instead of nothing would be simpler and would turn every such
        // conversion into a `-Wtautological-constant-out-of-range-compare`, which
        // §7 compiles with `-Werror`-adjacent flags.
        Callee::Builtin(index)
            if BUILTINS[index as usize].name.starts_with("fit_") && target.is_some() =>
        {
            let into = target.expect("just matched");
            let result = function.value_type(match function.args_of(args).first() {
                Some(crate::ir::Arg::Value(v)) => *v,
                _ => return,
            });
            let from = match checked.types.get(result) {
                Ty::Int(kind) => kind,
                _ => return,
            };
            let name = BUILTINS[index as usize].name;
            let to = *crate::types::INT_KINDS
                .iter()
                .find(|k| k.name() == &name[4..])
                .expect("the name was checked into existence by `resolve`");
            // A widening cannot fail, so it is an assignment and not an option:
            // no union, no tag, no branch. The C is the cast the type already
            // says, which is also why this arm has to ask before building one.
            if to.contains(from) {
                w.line(&format!("    {into} = ({}){};", to.c_type(), arguments[0]));
                return;
            }
            let (low, high) = to.range();
            let value = &arguments[0];
            let mut tests: Vec<String> = Vec::new();
            if from.signed() && low >= 0 {
                tests.push(format!("{value} >= 0"));
            }
            // The top test is needed only where the source can hold more than the
            // target. `bits` and `signed` together decide that, and asking them is
            // a fact about the two widths rather than a guess about the program.
            let source_top = if from.signed() { from.bits() - 1 } else { from.bits() };
            let target_top = if to.signed() { to.bits() - 1 } else { to.bits() };
            if source_top > target_top {
                tests.push(format!(
                    "{value} <= {}{}",
                    high,
                    if to.signed() { "LL" } else { "ULL" }
                ));
            }
            if from.signed() && !to.signed() && low == 0 && !tests.iter().any(|t| t.ends_with(">= 0"))
            {
                tests.push(format!("{value} >= 0"));
            }
            let condition =
                if tests.is_empty() { "1".to_string() } else { tests.join(" && ") };
            // `lookup`, not `intern`: the checker already made this `T?` when it
            // typed the call, so a miss here would mean the two passes disagree
            // about the result type rather than that a type is missing.
            let union = match checked
                .types
                .lookup(Ty::Int(to))
                .and_then(|inner| checked.types.lookup(Ty::Fallible(inner)))
            {
                Some(id) => types.names.option_of(id),
                None => return,
            };
            w.line(&format!("    if ({condition}) {{"));
            w.line(&format!(
                "        {into} = ({union}){{.tag = INT64_C(0), .as.ok = ({}){value}}};",
                to.c_type()
            ));
            w.line("    } else {");
            w.line(&format!(
                "        {into} = ({union}){{.tag = INT64_C(1), .as.err = hero_failure_does_not_fit()}};"
            ));
            w.line("    }");
        }
        Callee::Builtin(index) if super::EMITTED_BUILTINS.contains(&BUILTINS[index as usize].name) => {
            let entry =
                super::builtins::entry(BUILTINS[index as usize].name, function, checked, args);
            // `push`'s second argument is a *place*, not a value: the runtime copies
            // through the element descriptor, which is the only way one function can
            // append an `i64` and a `Point`.
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
        // **Unmangled, by design** (CLAUDE.md §7). The name in the `.hero` file is
        // the C function's own name, and the `#include` in the prelude is what
        // declares it — so this call is checked against the real header rather
        // than against a prototype this compiler invented. That is the whole of
        // §4.19's guarantee on the argument side, and `decls::extern_assertions`
        // is the other half, on the return side.
        Callee::Extern(decl) => {
            let name = src.slice(ast.decls[decl as usize].name);
            w.line(&format!("    {assign}{name}({});", arguments.join(", ")));
        }
        // Refused by the gate. The arm exists so that adding a callee kind to the
        // IR breaks this file.
        Callee::Builtin(_) => {
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
                // **`u64` is the one width that needs its own printer.** The
                // other seven widen into an `int64_t` without losing a value;
                // 18446744073709551615 does not, and read as signed it is `-1` —
                // which is precisely what `print(SIZE_MAX)` produced when panel
                // 042 measured it, and what this arm exists to stop.
                Ty::Int(IntKind::U64) => "hero_print_uint",
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
