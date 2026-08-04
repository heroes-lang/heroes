//! What this backend cannot emit yet, said in a way that ends the reader's loop
//! (panel 020, R1 and R2).
//!
//! The gate walks the **IR**, not the surface. Part 5's sugar is erased on the way
//! in, so the IR is the smaller closed vocabulary: a table over ops and types
//! cannot drift when a sugar row moves, and one row here covers every spelling
//! that lowers to it. The rows die one milestone at a time — that is the file's
//! whole design, and the reason the milestone names live in the comments here
//! rather than in the message.
//!
//! **The message names the capability, never the milestone.** `(M5b)` resolves
//! only in `docs/ROADMAP.md`, which the reader does not have; the panel's
//! llm-ergonomist read it as an internal tracker id, grepped the repository for
//! it, and then reported to its user that the toolchain was broken — a sentence it
//! recorded as false. §4.17's standard is everything needed without opening another
//! file.
//!
//! Four rows were missing from the proposal and are here because a judge found
//! them:
//!
//! - **built-ins by name.** `xs.len()` lowers to `call builtin len` while `for`
//!   lowers to `Op::Len`. Gating the op alone leaves an undefined symbol at link
//!   time — the exact class the gate exists to prevent.
//! - **`extern`** (the ffi-pragmatist's veto). No header attachment exists yet, so
//!   the emitter would have to invent a C prototype from the Heroes signature — and
//!   an invented prototype is self-consistent by construction, so clang verifies
//!   nothing. Measured: `abs(-2147483649)` returned `2147483647` at exit 0 with one
//!   non-fatal warning, and `sqlite3_open` was accepted in total silence under
//!   `-Weverything -pedantic`; with the real header both are `error: conflicting
//!   types`. §4.19's mechanism is the `#include`, not the declaration.
//! - **`test` functions are skipped, not refused.** `ir/mod.rs` already says
//!   ordinary builds ignore them; refusing them would make a file unbuildable at
//!   M5a *and* after M5b and M5c, for a block nothing emits.
//! - **one diagnostic per capability**, at its first occurrence, sorted by span.
//!   Three unsupported forms should not be three invocations — `cli.rs`'s own rule
//!   about carrying the list, applied to the backend.

use crate::diagnostics::Diagnostic;
use crate::ir::{Abort, Callee, Const, FnKind, Function, Op, Program, Shape, Term};
use crate::resolve::{Resolved, BUILTINS};
use crate::source::{Source, Span};
use crate::syntax::Ast;
use crate::types::{render_ty, Checked, Ty, TyId};

/// What the backend does emit, in the words the spec uses. It goes in a note on
/// every refusal: "what can I do instead" is the reader's next question, and the
/// answer is one line.
pub const SUBSET: &str = "the backend emits `int`, `bool`, `if`, `while`, functions and `print`";

/// Every capability this program needs and this backend lacks, sorted by where it
/// first appears.
pub(super) fn refuse(
    program: &Program,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
) -> Vec<Diagnostic> {
    let mut found: Vec<(String, String, Span)> = Vec::new();
    let _ = resolved;
    for function in &program.functions {
        // §4.18: `heroes test` enters these and ordinary builds ignore them.
        if function.kind == FnKind::Test {
            continue;
        }
        if function.kind == FnKind::Extern {
            note(&mut found, "extern", "an `extern` function".to_string(), function.span);
            continue;
        }
        if !function.generics.is_empty() {
            note(&mut found, "generics", "a generic function".to_string(), function.span);
        }
        for slot in &function.params {
            let ty = function.slots[slot.0 as usize].ty;
            check_type(&mut found, ast, checked, src, function, ty, function.span);
        }
        check_type(&mut found, ast, checked, src, function, function.result, function.span);
        for block in &function.blocks {
            for inst in &block.insts {
                check_type(&mut found, ast, checked, src, function, inst.ty, inst.span);
                check_op(&mut found, ast, src, function, inst.op, inst.span);
            }
            if let Term::Switch { .. } = block.term {
                note(
                    &mut found,
                    "variant",
                    "`match` on a variant".to_string(),
                    block.insts.last().map(|i| i.span).unwrap_or(function.span),
                );
            }
        }
    }
    found.sort_by_key(|(_, _, span)| span.start);
    found
        .into_iter()
        .map(|(code, what, span)| {
            Diagnostic::unsupported(&code, format!("{what} is not emitted yet"), span)
                .with_note(SUBSET.to_string())
        })
        .collect()
}

/// One capability, at its earliest span. The dedup is on the *message*: a program
/// with forty strings in it has one string problem.
fn note(found: &mut Vec<(String, String, Span)>, code: &str, what: String, span: Span) {
    match found.iter_mut().find(|(_, existing, _)| *existing == what) {
        Some((_, _, at)) => {
            if span.start < at.start {
                *at = span;
            }
        }
        None => found.push((code.to_string(), what, span)),
    }
}

/// A type the runtime has no representation for yet. One row per §4.3 table entry,
/// so M5b and M5c delete rows rather than discovering cases.
fn check_type(
    found: &mut Vec<(String, String, Span)>,
    ast: &Ast,
    checked: &Checked,
    src: &Source,
    function: &Function,
    ty: TyId,
    span: Span,
) {
    let (code, what) = match checked.types.get(ty) {
        Ty::Int | Ty::Bool | Ty::Unit => return,
        // M5b — the runtime's `str` and canonical `f64` rendering land together,
        // because both are about bytes the goldens have to predict.
        Ty::F64 => ("f64", "floating-point arithmetic".to_string()),
        Ty::Str => ("str", "text (`str`)".to_string()),
        // M5c — the descriptor pass, whose ABI spike 04 froze.
        Ty::Array(_) => ("array", "an array".to_string()),
        Ty::Map(_, _) => ("map", "a map".to_string()),
        Ty::Named(_) | Ty::Case(_, _) => ("record", "a record or a variant".to_string()),
        Ty::Fallible(_) | Ty::Failure => ("fallible", "a fallible value (`T?`)".to_string()),
        // M6.
        Ty::Func { .. } => ("function_value", "a function used as a value".to_string()),
        Ty::Generic(_) => ("generics", "a generic type".to_string()),
        // M7 — §4.19's two opaque types arrive with the header that verifies them.
        Ty::Ptr | Ty::Cstr => {
            let name = render_ty(&checked.types, ast, src, ty, &function.generics);
            ("ffi_type", format!("the C type `{name}`"))
        }
        // The checker reported this already; one mistake, one message.
        Ty::Error => return,
    };
    note(found, code, what, span);
}

fn check_op(
    found: &mut Vec<(String, String, Span)>,
    ast: &Ast,
    src: &Source,
    function: &Function,
    op: Op,
    span: Span,
) {
    match op {
        // A place with a path is a field or an element, and both are M5c's.
        Op::Load(place) | Op::Store { place, .. } => {
            if place.path.len > 0 {
                note(found, "record", "a record or a variant".to_string(), span);
            }
        }
        Op::Const(Const::Str(_)) => note(found, "str", "text (`str`)".to_string(), span),
        Op::Const(Const::Float(_)) => {
            note(found, "f64", "floating-point arithmetic".to_string(), span)
        }
        Op::Cast { .. } => note(found, "str", "text (`str`)".to_string(), span),
        Op::Call { callee, args, .. } => {
            callee_note(found, ast, src, callee, span);
            for arg in function.args_of(args) {
                if let crate::ir::Arg::InOut(place) = arg {
                    if place.path.len > 0 {
                        note(found, "record", "a record or a variant".to_string(), span);
                    }
                }
            }
        }
        Op::Construct { shape, .. } => {
            let (code, what) = match shape {
                Shape::Record(_) | Shape::Case(_, _) => {
                    ("record", "a record or a variant".to_string())
                }
                Shape::Array => ("array", "an array".to_string()),
                Shape::Map => ("map", "a map".to_string()),
                Shape::Ok | Shape::Fail | Shape::Err => {
                    ("fallible", "a fallible value (`T?`)".to_string())
                }
            };
            note(found, code, what, span);
        }
        Op::Field { .. } | Op::Tag(_) | Op::Payload { .. } => {
            note(found, "record", "a record or a variant".to_string(), span)
        }
        Op::Index { .. } => note(found, "array", "an array".to_string(), span),
        Op::MapGet { .. } => note(found, "map", "a map".to_string(), span),
        Op::Len(_) => note(found, "builtin", "the built-in `len`".to_string(), span),
        Op::FuncRef(_) => {
            note(found, "function_value", "a function used as a value".to_string(), span)
        }
        Op::Abort { reason, .. } => {
            let (code, what) = match reason {
                Abort::Must => ("fallible", "`.must()`".to_string()),
                Abort::Assert => ("assert", "`assert`".to_string()),
            };
            note(found, code, what, span);
        }
        // §4.16 says a holed file "produces no binary". The build path reports the
        // holes themselves and stops before here, so this row is the belt to that
        // braces: a hole must never be emitted as anything.
        Op::Hole => note(found, "hole", "a hole (`???`)".to_string(), span),
        // The verifier rejects this before the emitter is asked.
        Op::Missing => note(found, "missing", "a form the compiler cannot lower".to_string(), span),
        Op::Const(Const::Int(_))
        | Op::Const(Const::Bool(_))
        | Op::Unary { .. }
        | Op::Binary { .. }
        | Op::CopyOut { .. } => {}
    }
}

fn callee_note(
    found: &mut Vec<(String, String, Span)>,
    ast: &Ast,
    src: &Source,
    callee: Callee,
    span: Span,
) {
    match callee {
        Callee::Heroes(_) => {}
        Callee::Extern(_) => {
            let _ = (ast, src);
            note(found, "extern", "an `extern` function".to_string(), span)
        }
        Callee::Builtin(index) => {
            let name = BUILTINS[index as usize].name;
            if name != "print" {
                note(found, "builtin", format!("the built-in `{name}`"), span);
            }
        }
        Callee::Indirect(_) => {
            note(found, "function_value", "a function used as a value".to_string(), span)
        }
    }
}
