//! The operations this backend can write, one row per IR op (panel 020 R2).
//!
//! Split out of `gate.rs` by the §11 sweep. This is the half that gates by
//! **name as well as by op**, and that pairing is a judge's finding rather than a
//! convenience: `xs.len()` lowers to `call builtin len` while `for` lowers to
//! `Op::Len`, so gating the op alone leaves an undefined symbol at link time —
//! which is the exact class the gate exists to prevent.

use crate::resolve::BUILTINS;
use crate::source::{Source, Span};
use crate::syntax::Ast;
use crate::types::Checked;

use super::builtins::EMITTED as EMITTED_BUILTINS;
use super::gate::note;
use crate::ir::{Abort, Callee, Function, Op};


pub(super) fn check_op(
    found: &mut Vec<(String, String, Span)>,
    ast: &Ast,
    src: &Source,
    op: Op,
    span: Span,
) {
    match op {
        // A place with a path is a field or an element. Reading either emits; *writing*
        // through an element does not yet, because that is the copy-on-write path —
        // one unshare per array step of the place, each writing back at its level
        // (panel 022), and the mutation primitives it needs take `HeroArrayHeader **`.
        //
        // The row is here rather than left out because leaving it out is measured: with
        // arrays emitting and this refused nowhere, `xs[0] @ 7` reached clang as
        // `error: incompatible integer to pointer conversion assigning to
        // 'HeroArrayHeader *'`, which this toolchain reports as an internal error with
        // a path to generated C — the compiler blaming itself for the author's program,
        // which is the one failure the gate exists to prevent.
        // Reading and writing a place both emit now: a field is a member access, and an
        // element write is copy-on-write, one unshare per array step with write-back.
        Op::Load(_) | Op::Store { .. } => {}
        // `str`→`cstr` emits from M-ffi-ladder step 6, which is when it acquired a
        // producer: `s.cstr()`. The row was keyed to the FFI rather than to `str`,
        // which is why landing `str` did not make it emittable.
        Op::Cast { .. } => {}
        Op::Call { callee, args, .. } => {
            callee_note(found, ast, src, callee, span);
            let _ = args;
        }
        // **Every construction emits from M-optional-map**: records, variant cases, both
        // containers, and all three sides of a `T?`. The arm holds no rows rather than
        // being deleted, so adding a shape to the IR is still a compile error here —
        // the gate's own reason for enumerating instead of defaulting.
        Op::Construct { .. } => {}
        // A tag, a payload and a field read are the same three operations on a variant
        // and on a `T?` — §4.6's `ok`/`err` *is* a variant by the time it reaches here —
        // and all of them emit now.
        Op::Field { .. } | Op::Tag(_) | Op::Payload { .. } => {}
        // Both spellings of `[i]` emit now: a byte through `hero_str_byte`, an element
        // through `hero_array_at`.
        Op::Index { .. } => {}
        Op::MapGet { .. } => {}
        // `len` counts bytes on a `str` and elements on an array; a map is the row
        // still standing, and it is refused by its own type before reaching here.
        Op::Len(_) => {}
        // A function designator, from M-generics-library step 5.
        Op::FuncRef(_) => {}
        // **Both aborts emit, and the second row is retired rather than defended.**
        // `.must()` landed at M-generics-library step 1. `assert` was kept at M-optional-map "only because
        // the step that lands `heroes test` needs the row already written" — that
        // step is M-generics-library step 7, and what it needed turned out to be the *emission*,
        // not the refusal. An ordinary build never reaches a `test` block, and a
        // test build emits it, so no program can make this row fire. The arm holds
        // no rows rather than being deleted, which is the gate's own design: adding
        // an `Abort` kind is a compile error here.
        Op::Abort { reason, .. } => match reason {
            Abort::Must | Abort::Assert => {}
        },
        // §4.16 says a holed file "produces no binary". The build path reports the
        // holes themselves and stops before here, so this row is the belt to that
        // braces: a hole must never be emitted as anything.
        Op::Hole => note(found, "hole", "a hole (`???`)".to_string(), span),
        // The verifier rejects this before the emitter is asked.
        Op::Missing => note(found, "missing", "a form the compiler cannot lower".to_string(), span),
        Op::Const(_)
        | Op::Incref(_)
        | Op::Decref(_)
        | Op::Unary { .. }
        | Op::Binary { .. }
        | Op::CopyOut { .. } => {}
    }
}

/// **Where a fixed array is allowed to flow** — panel 081 R3, and the rule is
/// C's rather than this language's: `float t[4]; t = …;` does not compile at any
/// optimisation level, so a `T[N]` has no storage of its own and
/// `emit/storageless.rs` renders it *where it is used* instead of assigning it.
///
/// That works in exactly **two** positions, which are the only two that consume
/// `fixed_text`: as the base of a subscript (`container.rs`) and as an argument to
/// a construction (`construct.rs`). Every other position went to clang unrefused,
/// and clang blamed the compiler — four shapes, every one of them `heroes check`
/// **exit 0**, measured 2026-08-16:
///
/// - `r = a.reserved` — `use of undeclared identifier 'h1_r'`, exit 2. The
///   instruction that would define the local emits nothing (`inst.rs:86`), and the
///   store that follows it still names it.
/// - `w.reserved @ [9, …]` — `use of undeclared identifier 't21'`, exit 2.
/// - `b = Widget(…, reserved: a.reserved)` — `incompatible pointer to integer
///   conversion`, exit 2. Here `fixed_text` *did* render, as `t15.reserved`, and
///   that spelling is right as a subscript base and wrong as a struct initialiser.
/// - `[a.reserved]` — exit **134**, `hero_unreachable`, refused by
///   `check_element`'s own new row rather than here.
///
/// **The refusal is a `Ty::Fixed` row and not a repair of the lowering**, because
/// Principle 0 puts the widening behind a need that does not exist: `selfhost/`
/// declares zero fixed-array fields, measured by two panel seats. What it buys
/// today is CLAUDE.md §7 — a program the author wrote stops being the compiler's
/// fault — and the widening stays available at a price nobody has taken.
pub(super) fn check_fixed_flow(
    found: &mut Vec<(String, String, Span)>,
    checked: &Checked,
    function: &Function,
    op: Op,
    span: Span,
) {
    let is_fixed = |value: crate::ir::ValueId| {
        matches!(checked.types.get(function.values[value.0 as usize]), crate::types::Ty::Fixed(_, _))
    };
    match op {
        // Covers the binding and the field store alike: both are one `Op::Store`,
        // and neither has a C spelling.
        Op::Store { value, .. } if is_fixed(value) => note(
            found,
            "fixed_flow",
            "a fixed array given a name or stored into a place"
                .to_string(),
            span,
        ),
        // An argument that is a fixed array is fine when it **is** the literal, and
        // is the copy C refuses when it came from somewhere else. The array and map
        // shapes are left to `check_element`, which names the container and would
        // otherwise report the same mistake twice.
        Op::Construct { shape, args }
            if !matches!(shape, crate::ir::Shape::Array | crate::ir::Shape::Map) =>
        {
            for arg in function.args_of(args) {
                let crate::ir::Arg::Value(value) = arg else { continue };
                if is_fixed(value) && !is_a_written_literal(function, value) {
                    note(
                        found,
                        "fixed_flow",
                        "a fixed array copied from another value"
                            .to_string(),
                        span,
                    );
                }
            }
        }
        _ => {}
    }
}

/// Whether this value is the `[a, b, c]` an author wrote, as opposed to a field
/// read that happens to have the same type. The distinction is the whole of the
/// `Construct` row above: a literal renders as `{a, b, c}`, which C accepts as an
/// initialiser, and a field read renders as `v.member`, which it does not.
fn is_a_written_literal(function: &Function, value: crate::ir::ValueId) -> bool {
    function
        .blocks
        .iter()
        .flat_map(|block| &block.insts)
        .find(|inst| inst.dest == Some(value))
        .is_some_and(|inst| {
            matches!(inst.op, Op::Construct { shape: crate::ir::Shape::Array, .. })
        })
}


pub(super) fn callee_note(
    found: &mut Vec<(String, String, Span)>,
    ast: &Ast,
    src: &Source,
    callee: Callee,
    span: Span,
) {
    match callee {
        // A call to C emits from M-ffi-ladder step 5: the group's header is in the
        // prelude, so the name this call writes is declared by the real header
        // rather than by a prototype this compiler invented.
        Callee::Heroes(_) | Callee::Extern(_) => {
            let _ = (ast, src);
        }
        Callee::Builtin(index) => {
            let name = BUILTINS[index as usize].name;
            if !EMITTED_BUILTINS.contains(&name) {
                note(found, "builtin", format!("the built-in `{name}`"), span);
            }
        }
        // A call through a function value emits from M-generics-library step 5.
        Callee::Indirect(_) => {}
    }
}
