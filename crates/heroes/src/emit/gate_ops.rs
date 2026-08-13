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

/// A built-in whose *name* emits but whose **operand type** has no entry point.
/// The rule itself lives in `builtins.rs`, beside the entry-point table it is the
/// complement of; this is the walk that applies it.
pub(super) fn check_builtin_operand(
    found: &mut Vec<(String, String, Span)>,
    checked: &Checked,
    function: &Function,
    op: Op,
    span: Span,
) {
    let Op::Call { callee: Callee::Builtin(index), args, .. } = op else { return };
    let name = BUILTINS[index as usize].name;
    if let Some(what) = super::builtins::unsupported_operand(name, function, checked, args) {
        note(found, "builtin", what, span);
    }
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
