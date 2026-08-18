//! One operator as C — and the guard that keeps it out of undefined behaviour
//! (design.md §4.14; panels 020, 022, 035, 040, 042).
//!
//! Split out of `ops.rs` by the §11 sweep, along the seam the file's own doc had
//! already named. The concern is single and it is the loudest rule in the emitter:
//!
//! - **arithmetic aborts, it never wraps into UB.** `__builtin_*_overflow` for
//!   `+ - *`, an explicit zero test for `/` and `%`, and `%` is guarded exactly like
//!   `/` because `INT64_MIN % -1` does not trap on arm64 — it is UB that happens to
//!   look fine. The shifts are guarded for C's reasons rather than for Heroes'.
//!
//! Everything an operator dispatches on is the operand's *type*, so the aggregate
//! comparisons (`str`, `[T]`, `{K: V}`, a record, a `T?`) live here too: they are
//! the same question — what does `==` mean on this operand — answered by a call
//! instead of by an infix operator.

use crate::ir::{BinOp, UnOp};
use crate::types::{Checked, Ty};

use super::aggregate;
use super::mangle;
use super::writer::Writer;

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
        // **A `nan` on either side of an ordering stops the program** (panel 075).
        // IEEE makes all four false at a `nan`, so `n < a` and `n >= a` are false
        // *together* and the `else` arm runs with nothing reported. That is not a
        // surprising answer, it is no answer wearing one: `smallest([nan, 1.0, 2.0])`
        // gives `nan` and `smallest([1.0, nan, 2.0])` gives `1.0` — same numbers, same
        // program, the result decided by which line the `nan` arrived on. §4.14's own
        // dialect already says this everywhere else (overflow, division by zero,
        // `sort`, a float map key); the bare operator was where it had never reached.
        // `==` and `!=` stay untouched, so equality guards keep working and `x != x`
        // still asks the question — which is why no `is_nan` was added.
        //
        // **`x != x` rather than `isnan()`, and that choice has an expiry.** It is
        // what `runtime/parts/sort.c` and `f64.c` already write, it needs no header,
        // and clang lowers it to arm64's unordered flag (5 instructions against 3 at
        // `-O2`). It is correct at every level in `FLAGS` and **wrong** under
        // `-ffast-math`/`-Ofast`, where the compiler is told no `nan` exists and
        // deletes the test — so the day either flag is adopted this must become
        // `isnan()`, which is panel 075's standing condition.
        //
        // **The same rule lives in `runtime/parts/sort.c:51,57` and cannot move
        // here**: `sort` compares through a function pointer inside the runtime, so no
        // emitted `<` reaches it, and deleting those two on this rule's strength
        // returns an arbitrary permutation at exit 0 — measured by three seats. One
        // rule, two sites, two languages, and `a_nan_is_refused_at_every_ordering_site`
        // is the test that fires when that stops being true.
        BinOp::Lt | BinOp::Le | BinOp::Gt | BinOp::Ge if matches!(operands, Ty::Float(_)) => {
            let operator = match op {
                BinOp::Lt => "<",
                BinOp::Le => "<=",
                BinOp::Gt => ">",
                _ => ">=",
            };
            w.line(&format!(
                "    if ({l} != {l} || {r} != {r}) hero_panic(\"a nan in `{operator}`: it is neither less nor greater, so both arms would be false\");"
            ));
            w.line(&format!("    {name} = {l} {operator} {r};"));
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
