//! `to_<width>` as C: the one built-in that builds its own `T?` inline
//! (spec § Types; design.md §4.3; panels 042, 043, 044, 045).
//!
//! Split out of `ops.rs` by the §11 sweep. It sits apart from the other built-ins
//! for the reason its own body states: `builtins::entry` answers with the NAME of a
//! C entry point, and a conversion has to build a **tagged union generated for this
//! result type**, which no runtime function can return. So this is the one built-in
//! whose C is written here rather than called.
//!
//! Two rules live here, and both were paid for by a defect:
//!
//! - **the range tests come from the two RANGES, never from the two shapes.** A
//!   version that derived them from `signed()` and `bits()` by hand let `to_i8(-129)`
//!   through.
//! - **a test that is always true is omitted, never emitted.** `true &&` would turn
//!   every widening into a `-Wtautological-constant-out-of-range-compare`, which §7
//!   compiles as an error.

use crate::types::{Checked, Ty};

use super::aggregate;
use super::writer::Writer;
use crate::types::FloatKind;

/// Emit `into = to_<width>(value)` — a `T?`, always, at every width (the uniform
/// rule restored by the author's ratification of 2026-08-13).
///
/// The range test is written against the SOURCE's C type and the target's bounds,
/// and both halves matter. Comparing an unsigned source against a negative lower
/// bound is a warning and a constant answer, so the low test is omitted where the
/// source cannot be negative; comparing a narrow source against a wider target's
/// top is likewise always true.
pub(super) fn width(
    w: &mut Writer,
    types: &aggregate::Types,
    function: &crate::ir::Function,
    name: &str,
    args: crate::ir::Args,
    arguments: &[String],
    into: &str,
) {
    let checked: &Checked = types.checked;
    // **The loud direction, and the third of this function's silent returns**
    // (CLAUDE.md §11; panel 062's audit). A conversion takes exactly one argument
    // and it is a value: the checker fixed the arity, and `@` marks a parameter, of
    // which a built-in has none — so `None` means the IR lost an argument and
    // `InOut` means it invented one. Returning here emitted **nothing**, and the
    // caller then read a zero-initialised `T?` whose `tag = 0` is `ok` (panel 021
    // zero-initialises every refcounted slot, so `-Werror=uninitialized` cannot see
    // it). That is the `f32` defect this file already records, one cause up.
    let result = function.value_type(match function.args_of(args).first() {
        Some(crate::ir::Arg::Value(v)) => *v,
        other => unreachable!("a conversion takes one value argument, not {other:?}"),
    });
    let to = *crate::types::INT_KINDS
        .iter()
        .find(|k| k.name() == &name[3..])
        .expect("the name was checked into existence by `resolve`");
    // `lookup`, not `intern`: the checker already made this `T?` when it typed the
    // call, so a miss here would mean the two passes disagree about the result type
    // rather than that a type is missing.
    // **A miss is the two passes disagreeing, so it is loud** (CLAUDE.md §11; panel
    // 062's audit). The sentence above already said a miss "would mean the two
    // passes disagree rather than that a type is missing", and then answered it
    // with a silent `return` — which emits nothing and hands the caller the same
    // zeroed `ok` the arm above describes.
    let union = match checked
        .types
        .lookup(Ty::Int(to))
        .and_then(|inner| checked.types.lookup(Ty::Fallible(inner)))
    {
        Some(id) => types.names.option_of(id),
        None => unreachable!("the checker interned `{}?` when it typed this call", to.name()),
    };
    // **The `f64` source is `to_i64`'s alone and takes its own path.** Its range
    // question is not a comparison against two widths; it is `hero_f64_fits_int`,
    // whose predicate is written in the runtime with the reason two obvious
    // spellings of it are wrong. The conversion itself truncates toward zero and
    // cannot fail once the predicate holds.
    let from = match checked.types.get(result) {
        Ty::Int(kind) => kind,
        // **Either float width takes this path**, and an `f32` reaches it through
        // one widening call rather than through a second predicate. `f32` → `f64`
        // is exact, so `hero_f64_fits_int` answers the same question about the
        // same value; writing a `hero_f32_fits_int` beside it would be two
        // spellings of one range test, which is two chances to disagree.
        //
        // **This arm was `Ty::Float(FloatKind::F64)` and the width made it a
        // defect the same hour** (panel 060). The checker had been widened to
        // accept either width and this had not, so an `f32` fell to the `_` below,
        // returned having emitted nothing, and left the option struct at its
        // zero-initialiser — `tag = 0`, which is **`ok`** — so `to_i64` on an
        // `f32` produced `ok(0)` at exit 0 with no diagnostic anywhere.
        // `-Werror=uninitialized` cannot see it, because panel 021's rule
        // deliberately zero-initialises every refcounted slot.
        Ty::Float(kind) => {
            let value = match kind {
                FloatKind::F32 => format!("hero_f32_to_f64({})", arguments[0]),
                FloatKind::F64 => arguments[0].clone(),
            };
            w.line(&format!("    if (hero_f64_fits_int({value})) {{"));
            w.line(&format!(
                "        {into} = ({union}){{.tag = INT64_C(0), .as.ok = hero_f64_to_int({value})}};"
            ));
            w.line("    } else {");
            w.line(&format!(
                "        {into} = ({union}){{.tag = INT64_C(1), .as.err = hero_failure_does_not_fit()}};"
            ));
            w.line("    }");
            return;
        }
        // **The loud direction** (CLAUDE.md §11). A `return` here is what the
        // paragraph above cost: it emits nothing and the caller reads a zeroed
        // slot. The checker refuses every other source type, so reaching this is a
        // compiler bug and must say so rather than produce a value.
        other => unreachable!("a conversion's source is an integer or a float, not {other:?}"),
    };
    // **Both bounds are asked of the two RANGES, not of the two shapes.** A first
    // version derived them from `signed()` and `bits()` by hand and got
    // `to_i8(-129)` wrong: it emitted a low test only when the target's floor was
    // zero, so a *signed narrower* target — whose floor is -128, not 0 — was checked
    // at the top and nowhere else, and -129 walked through. Found by this
    // milestone's own adversarial case, which is what §9's five exist for.
    //
    // Comparing the ranges says exactly what is needed and cannot drift: a bound is
    // tested when the source can reach past it, and omitted when it cannot.
    let (low, high) = to.range();
    let (from_low, from_high) = from.range();
    let value = &arguments[0];
    let mut tests: Vec<String> = Vec::new();
    if from_low < low {
        tests.push(format!("{value} >= {low}{}", if from.signed() { "LL" } else { "ULL" }));
    }
    if from_high > high {
        tests.push(format!("{value} <= {high}{}", if from.signed() { "LL" } else { "ULL" }));
    }
    // **An empty test list is the widening case, and it emits no branch at all.**
    // Every value of the source fits the target, so the option is unconditionally
    // `ok` — no condition, no `else`, and in particular no `if (1)`, which
    // `-Wtautological-constant-out-of-range` would reject anyway.
    //
    // This arm was `hero_unreachable()` between panel 043 and the author's
    // ratification of 2026-08-13, and that was right at the time: the rule then
    // returned a plain `T` for a widening and an early return took every such case
    // before this point. Restoring the uniform `T?` made the branch reachable again,
    // and the golden caught it within a minute — `assigning to … from incompatible
    // type 'void'`. `contains_agrees_with_range` still guards the premise the
    // emptiness rests on: `contains` and `range` must agree, or a narrowing arrives
    // here with no test and truncates in silence.
    if tests.is_empty() {
        w.line(&format!(
            "    {into} = ({union}){{.tag = INT64_C(0), .as.ok = ({}){}}};",
            to.c_type(),
            arguments[0]
        ));
        return;
    }
    let condition = tests.join(" && ");
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
